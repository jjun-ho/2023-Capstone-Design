//---------------------------------------------------------------------------
#include "linearTransform.h"


//////////////////////////////////////////////////////////////////////////////////
/// reference : R.Y. Tsai and R.K. Lenz, "a new technique for fully autonomous and
///            efficient 3d robotics hand/eye calibration
///
/// - this function solves AX = XB
/////////////////////////////////////////////////////////////////////////////////
///
KHomogeneous KLinearTransform::ByTsai(const KMatrices& msA, const KMatrices& msB)
{
    //obtain scaled eigen vector(=rotation axis) of each rotation matrix
    int         nNum = msA.Count();
    double      dTheta;
    KVectors    vsPg(nNum), vsPc(nNum);

    for(int i=0; i<nNum; i++)
    {
        //interframe(from i frame to j frame) rotation of gripper,
        KRotation(msA[i].To(2,2)).AxisRotation(vsPg[i],dTheta);
        vsPg[i] *= 2.0*sin(dTheta/2.0);

        //interframe(from i frame to j frame) rotation of camera,
        KRotation(msB[i].To(2,2)).AxisRotation(vsPc[i],dTheta);
        vsPc[i] *= 2.0*sin(dTheta/2.0);
    }

    //apply least squars method to compute P'cn
    KVector vX(3);
    KMatrix mP(3*(nNum),3);
    KVector vQ(3*(nNum));

    for(int i=0,ii=0; i<nNum; i++,ii+=3){
        //set mA
        mP.Place(ii,0,(vsPg[i]+vsPc[i]).Skew());
        //set vB
        vQ.Place(ii, vsPg[i] - vsPc[i]);
    }

    vX = ~(mP.Tr()*mP)*mP.Tr()*vQ;

    //rotation matrix from Camera coord. to Gripper coord.
    KRotation   rX;
    double      dNorm2;

    vX     *= 2.0/sqrt(1.0+vX.Norm2());
    dNorm2  = vX.Norm2();
    rX   = KMatrix::Diag(1-dNorm2*0.5, 1-dNorm2*0.5, 1-dNorm2*0.5) +
           (vX*vX.Tr() + sqrt(4 - dNorm2)*vX.Skew());
    /*
    vX.Normalized(_SIZE_NORMALIZE,&dTmp);
    dTmp = 2.0*asin(dTmp/2.0);
    rX.FromAxisRotation(vX,dTmp);
    */

    //apply least squares method to compute translation)
    KVector vT;
    KMatrix mI(3,3,_IDENTITY);

    for(int i=0,ii=0; i<nNum; i++,ii+=3){
        mP.Place(ii,0, msA[i].To(2,2) - mI);
        vQ.Place(ii, rX*msB[i].Column(3).To(2) - msA[i].Column(3).To(2));
    }
    vT =  (~(mP.Tr()*mP))*mP.Tr()*vQ;

    //output    
    return KHomogeneous(rX, vT);
}


//////////////////////////////////////////////////////////////////////////////////
/// reference : N. Andreff, R. Horaud, B. Espiau, On-line Hand-Eye Calibration,
///             In Second International Conference on 3-D Digital Imaging and Modeling (3DIM'99), pages 430-436, 1999
///             http://math.loyola.edu/~mili/Calibration/index.html
/// this function solves AX = XB
/////////////////////////////////////////////////////////////////////////////////
///
KMatrix KLinearTransform::ByAndreff(const KMatrices& msA, const KMatrices& msB)
{
    int     nNum = msA.Count();
    KMatrix mA(12*nNum,12);
    KVector vB(12*nNum), vTa, vTb;
    KRotation rA, rB;
    KMatrix   mI9(9,9,KMatrix::Ones(9,1).Address());
    KMatrix   mI3(3,3,KMatrix::Ones(3,1).Address());

    for(int i=0; i<nNum; i++)
    {
        rA  = msA[i].To(2,2);
        rB  = msB[i].To(2,2);
        vTa = msA[i].Cut(0,3,2,3);
        vTb = msB[i].Cut(0,3,2,3);
        mA.Place(12*i,0, mI9 - KMatrix::Kron(rB,rA));
        mA.Place(12*i+9,0, KMatrix::Kron(vTb.Tr(),mI3) | (mI3-rA));
        vB.Place(12*i+9, vTa);
    }

    KMatrix mU,mV;
    KVector vW;
    KVector vX = LinearEquation(mA,vB);
    KMatrix mX(3,3,vX.Address());

    mX *= _SIGN(mX.Det()) / pow(abs(mX.Det()),1.0/3.0);
    mX.SVD(mU,vW,mV);
    mX = mU*mV.Tr();

    if(mX.Det()<0)
        mX = mU*KMatrix::Diag(1.0,1.0,-1.0)*mV.Tr();

    mX = (mX.Tr() | vX.From(9)) ^ KVector(0.0, 0.0, 0.0, 1.0).Tr();


    return mX;
}

//////////////////////////////////////////////////////////////////////////////////
/// reference : R. Liang, J. Mao, Hand-Eye Calibration with a New Linear Decomposition Algorithm,
///              In Journal of Zhejiang University, 9(10):1363-1368, 2008
///             http://math.loyola.edu/~mili/Calibration/index.html
/// this function solves AX = XB
/////////////////////////////////////////////////////////////////////////////////
///
KMatrix KLinearTransform::ByLiang(const KMatrices& msA, const KMatrices& msB)
{
    int       nNum = msA.Count();
    KMatrix   mA(9*nNum,9);
    KVector   vB(9*nNum);
    KRotation rA, rB;
    KMatrix   mI3(3,3,KMatrix::Ones(3,1).Address());


    for(int i=0; i<nNum; i++)
    {
        rA = msA[i].To(2,2);
        rB = msB[i].To(2,2);
        mA.Place(9*i,0, KMatrix::Kron(rA,mI3) + KMatrix::Kron(-mI3,rB.Tr()));
    }

    KMatrix mU,mV,mR,mC(3*nNum,3);
    KVector vW, vX,vD(3*nNum);

    mA.SVD(mU,vW,mV);
    vX  = mV.Column(mV.Col()-1);
    mR  = KMatrix(3,3,vX.Address());
    mR *= _SIGN(mR.Det()) / pow(abs(mR.Det()), 1.0/3.0);
    mR.SVD(mU,vW,mV);
    mR  = mU*mV.Tr();

    if(mR.Det() <0.0)
        mR = mU * KMatrix::Diag(1.0,1.0,-1.0) * mV.Tr();

    for(int i=0; i<nNum; i++)
    {
        mC.Place(3*i,0,mI3-msA[i].To(2,2));
        vD.Place(3*i,msA[i].Cut(0,3,2,3)-mR*msB[i].Cut(0,3,2,3));
    }

    vX = LinearEquation(mC, vD);

    return (mR | vX) ^ KVector(0.0,0.0,0.0,1.0).Tr();

}
//---------------------------------------------------------------------------


//reference : B.K.P. Horn, "closed-form solution of absolute orientation using unit quaternions,
//            J. the Optical Society of America A, Vol.4, 1987
// - compute transformation coordinate frame A to coordinate frame B
// - A points : 3-D points w.r.t. A coordinate frame
// - B points : 3-D points w.r.t. B coordinate frame
// - return value : homogeneous transformation H from A coordinates to B coordinates, B = H * A
//
KHomogeneous KLinearTransform::TransformFrom3D_3D(int nNumPoints,KVector* vpA,KVector* vpB, bool bOptima,double* dpError, bool bScale)
{
    //compute centroids
    KVector vCa(3),vCb(3);
    for(int i=0; i<nNumPoints; i++)
    {
        vCa += vpA[i];
        vCb += vpB[i];
    }
    vCa *= (1.0/(double)(nNumPoints));
    vCb *= (1.0/(double)(nNumPoints));

    //compute N matrix
	KMatrix mN(4,4);
    KMatrix mRa(4,4),mRb(4,4);

    for(int i=0; i<nNumPoints; i++)
    {
        mRa[0][1] = vpA[i][0] - vCa[0];
        mRa[0][2] = vpA[i][1] - vCa[1];
        mRa[0][3] = vpA[i][2] - vCa[2];
        mRa[1][0] = -mRa[0][1];
        mRa[2][0] = -mRa[0][2];
        mRa[3][0] = -mRa[0][3];
        mRa[1][2] = -(vpA[i][2]-vCa[2]);
        mRa[1][3] =  (vpA[i][1]-vCa[1]);
        mRa[2][1] = -mRa[1][2];
        mRa[3][1] = -mRa[1][3];
        mRa[2][3] = -(vpA[i][0] - vCa[0]);
        mRa[3][2] = -mRa[2][3];

        mRb[0][1] = -(vpB[i][0] - vCb[0]);
        mRb[0][2] = -(vpB[i][1] - vCb[1]);
        mRb[0][3] = -(vpB[i][2] - vCb[2]);
        mRb[1][0] = -mRb[0][1];
        mRb[2][0] = -mRb[0][2];
        mRb[3][0] = -mRb[0][3];
        mRb[1][2] = -(vpB[i][2]-vCb[2]);
        mRb[1][3] =  (vpB[i][1]-vCb[1]);
        mRb[2][1] = -mRb[1][2];
        mRb[3][1] = -mRb[1][3];
        mRb[2][3] = -(vpB[i][0] - vCb[0]);
        mRb[3][2] = -mRb[2][3];

        mN += mRa*mRb;
    }

    //compute the eigen vector with the maximum eigen value
	KVector     vEigen,mEigen,mV;
    KQuaternion qOut;

    mN.SVD(mEigen,vEigen,mV);
    qOut = mEigen.Column(0);

    //obtain rotation
    KRotation rOut = qOut.Rotation();

    //scale factor
    double dScale = 1.0;
    if(bScale == true)
    {
        double dSa = 0.0;
        double dSb = 0.0;

        for(int i=0; i<nNumPoints; i++)
        {
            dSa += (vpB[i] - vCb) & KVector(rOut*(vpA[i] - vCa));
            dSb += (vpB[i] - vCb).Norm2();
        }

        dScale = dSb/dSa;
    }

	//compute translation
    KVector vT = vCb - dScale*rOut*vCa;

    //residual error
    if(dpError)
    {
        *dpError = 0.0;
        for(int i=0; i<nNumPoints; i++)
            *dpError += sqrt((vpB[i] - (dScale*rOut*vpA[i] + vT)).Norm2());
        *dpError /= (double)(nNumPoints);
    }  

    //비선형 최적화
    if(bOptima == false)
        return KHomogeneous(rOut,vT);

    _mA.Release();
    _mB.Release();

    for(int i=0; i<nNumPoints; i++) //used in Erf()
    {
        _mB |= vpB[i];
        _mA |= vpA[i];
    }

    _nOptima = _OPTIMA_RT4X4;

    KVector vX = rOut.Euler(_RAD);
    vX.Tailed(vT);

    KOptima::Powell(vX,0.001);

    rOut.FromEuler(vX[0],vX[1],vX[2],_RAD);
    vT = vX.From(3);


    //homogeneous transformation
    return KHomogeneous(rOut,vT);
}

KHomogeneous KLinearTransform::TransformFrom3D_3D(const KMatrix& mA, const KMatrix& mB,bool bOptima,double* dpError, bool bScale)
{
    //compute centroids
    KVector vCa(3),vCb(3);
    int     nNumPoints = mA.Col();

    for(int i=0; i<nNumPoints; i++)
    {
        vCa[0] += mA[0][i]; vCa[1] += mA[1][i]; vCa[2] += mA[2][i];
        vCb[0] += mB[0][i]; vCb[1] += mB[1][i]; vCb[2] += mB[2][i];
    }
    vCa *= (1.0/(double)(nNumPoints));
    vCb *= (1.0/(double)(nNumPoints));

    //compute N matrix
    KMatrix mN(4,4);
    KMatrix mRa(4,4),mRb(4,4);

    for(int i=0; i<nNumPoints; i++)
    {
        mRa[0][1] = mA[0][i] - vCa[0];
        mRa[0][2] = mA[1][i] - vCa[1];
        mRa[0][3] = mA[2][i] - vCa[2];
        mRa[1][0] = -mRa[0][1];
        mRa[2][0] = -mRa[0][2];
        mRa[3][0] = -mRa[0][3];
        mRa[1][2] = -(mA[2][i]-vCa[2]);
        mRa[1][3] =  (mA[1][i]-vCa[1]);
        mRa[2][1] = -mRa[1][2];
        mRa[3][1] = -mRa[1][3];
        mRa[2][3] = -(mA[0][i] - vCa[0]);
        mRa[3][2] = -mRa[2][3];

        mRb[0][1] = -(mB[0][i] - vCb[0]);
        mRb[0][2] = -(mB[1][i] - vCb[1]);
        mRb[0][3] = -(mB[2][i] - vCb[2]);
        mRb[1][0] = -mRb[0][1];
        mRb[2][0] = -mRb[0][2];
        mRb[3][0] = -mRb[0][3];
        mRb[1][2] = -(mB[2][i]-vCb[2]);
        mRb[1][3] =  (mB[1][i]-vCb[1]);
        mRb[2][1] = -mRb[1][2];
        mRb[3][1] = -mRb[1][3];
        mRb[2][3] = -(mB[0][i] - vCb[0]);
        mRb[3][2] = -mRb[2][3];

        mN += mRa*mRb;
    }

    //compute the eigen vector with the maximum eigen value
    KVector     vEigen,mEigen,mV;
    KQuaternion qOut;

    mN.SVD(mEigen,vEigen,mV);
    qOut = mEigen.Column(0);

    //obtain rotation
    KRotation rOut = qOut.Rotation();

    //scale factor
    double dScale = 1.0;
    if(bScale == true)
    {
        double dSa = 0.0;
        double dSb = 0.0;

        for(int i=0; i<nNumPoints; i++)
        {
            dSa += (mB.Column(i) - vCb) & KVector(rOut*(mA.Column(i) - vCa));
            dSb += (mB.Column(i) - vCb).Norm2();
        }

        dScale = dSb/dSa;
    }

    //compute translation
    KVector vT = vCb - dScale*rOut*vCa;

    //residual error
    if(dpError)
    {
        *dpError = 0.0;
        for(int i=0; i<nNumPoints; i++)
            *dpError += sqrt((mB.Column(i) - (dScale*rOut*mA.Column(i) + vT)).Norm2());
        *dpError /= (double)(nNumPoints);
    }

    //비선형 최적화
    if(bOptima == false)
        return KHomogeneous(rOut,vT);

    _mA = mA;
    _mB = mB;

    _nOptima = _OPTIMA_RT4X4;
    KVector vX = rOut.Euler(_RAD);
    vX.Tailed(vT);

    KOptima::Powell(vX,0.001);

    rOut.FromEuler(vX[0],vX[1],vX[2], _RAD);
    vT = vX.From(3);


    //homogeneous transformation
    return KHomogeneous(rOut,vT);
}

//2004-Linear Least-Squares Optimization for Point-to-Plane ICP Surface Registration
//https://www.researchgate.net/publication/228571031_Linear_Least-Squares_Optimization_for_Point-to-Plane_ICP_Surface_Registration
//
KHomogeneous KLinearTransform::TransformFrom3D_3D(const KArray<KVector>& avA,
                                                  const KArray<KVector>& avB,
                                                  const KArray<KVector>& avNormalB,
                                                  double dThDist,
                                                  double dThDeg,
                                                  double* dpError)
{
    //계산에 사용될 점의 개수
    int     nNum = 0;
    double  dThreshDist2 = dThDist*dThDist;
    double  dThreshCos   = cos(_RADIAN(dThDeg));
    bool*   bpMatched     = new bool[avNormalB.Size()];


    for(int i=0, ii=avNormalB.Row(),k=0; ii; i++, ii--)
        for(int j=0, jj=avNormalB.Col(); jj; j++, jj--, k++)
        {
            if(avNormalB[i][j].Address() == 0 || avA[i][j].Address() == 0)// KVector::Distance2(avA[i][j],avB[i][j]) < dThreshDist2 && (avNormalA[i][j] & avNormalB[i][j]) < dThreshCos)
                bpMatched[k] = false;
            else
            {
                bpMatched[k] = true;
                nNum ++;              
            }

        }

    assert(nNum > 5);

    //L 행렬 및 M 행렬 생성 ( mL * vX = mM )
    KMatrix mL(nNum, 6);
    KVector vM(nNum);

    for(int i=0, ii=avNormalB.Row(), k=0,kk=0; ii; i++, ii--)
        for(int j=0, jj=avNormalB.Col(); jj; j++, jj--, k++)
        {
            if(bpMatched[k] == false) continue;

            mL[kk][0] = avNormalB[i][j][2]*avA[i][j][1] - avNormalB[i][j][1]*avA[i][j][2];
            mL[kk][1] = avNormalB[i][j][0]*avA[i][j][2] - avNormalB[i][j][2]*avA[i][j][0];
            mL[kk][2] = avNormalB[i][j][1]*avA[i][j][0] - avNormalB[i][j][0]*avA[i][j][1];
            mL[kk][3] = avNormalB[i][j][0];
            mL[kk][4] = avNormalB[i][j][1];
            mL[kk][5] = avNormalB[i][j][2];

            vM[kk++]  = avNormalB[i][j][0]*avB[i][j][0] + avNormalB[i][j][1]*avB[i][j][1] + avNormalB[i][j][2]*avB[i][j][2] -
                        avNormalB[i][j][0]*avA[i][j][0] - avNormalB[i][j][1]*avA[i][j][1] - avNormalB[i][j][2]*avA[i][j][2];
        }
    delete[] bpMatched;

    //SVD
    KMatrix mU,mV;
    KVector vW;

    mL.SVD(mU,vW,mV);

    //solve mL * vX = mM
    KVector vX;
    for(int i=0; i<vW.Dim(); i++)
        if(vW[i] != 0.0)
            vW[i] = 1.0/vW[i];
    vX = mV * vW * mU.Tr() * vM;       

    //residual error
    if(dpError)
        *dpError = (mL*vX - vM).SumAbs() / (double)(nNum);

    //회전행렬
    KRotation rR;
    rR[0][1] =  vX[0]*vX[1]-vX[2];
    rR[0][2] =  vX[0]*vX[2]+vX[1];
    rR[1][0] =  vX[2];
    rR[1][1] =  vX[0]*vX[1]*vX[2]+1.0;
    rR[1][2] =  vX[1]*vX[2] - vX[0];
    rR[2][0] = -vX[1];
    rR[2][1] =  vX[0];

//        rR[0][1] =  -vX[2];
//        rR[0][2] =  vX[1];
//        rR[1][0] =  vX[2];
//        rR[1][2] = -vX[0];
//        rR[2][0] = -vX[1];
//        rR[2][1] =  vX[0];

    return KHomogeneous(rR.Orthogonalized(), vX.From(3));
}

//---------------------------------------------------------------------------


//reference : R.Y. Tsai, "a versatile camera calibration technique for high-accuracy 3D machine vision
//            metrology using off-the-shelf TV cameras and lenses, IEEE J. R&A, Vol.RA-3,No.4, 1987
// - compute transformation model coordinate frame to camera coordinate frame
// - model points : 2-D points on the X-Y plane
// - image points : 2-D image points relative to image center, pixel unit
// - dFocal       : pixel unit
// - return value : homogeneous transformation H from Model coord. to Camera coord., C_coord = H * M_coord
//
KMatrix KLinearTransform::TransformFrom2D_2D(double dFocal,int nNumPoints,KPoint* ptpModel,KPoint* ptpImage)
{
	//set the equation : mA' * vX = vB
	int     i;
	KMatrix mA;
	KVector vB;

	mA.Create(5,nNumPoints);
	vB.Create(nNumPoints);
	for(i=0; i<nNumPoints; i++){
		mA[0][i] =  ptpImage[i]._dY * ptpModel[i]._dX;
		mA[1][i] =  ptpImage[i]._dY * ptpModel[i]._dY;
		mA[2][i] =  ptpImage[i]._dY;
		mA[3][i] = -ptpImage[i]._dX * ptpModel[i]._dX;
		mA[4][i] = -ptpImage[i]._dX * ptpModel[i]._dY;

		vB[i]    =  ptpImage[i]._dX;
	}

	//obtain vX by least squares
	KVector vX = mA.Cor().Iv()*mA*vB;

	//dTy
	double dSr  = _SQR(vX[0])+_SQR(vX[1]) + _SQR(vX[3])+_SQR(vX[4]);
	double dTmp = _SQR(vX[0]*vX[4]-vX[3]*vX[1]);
	double dTy  = sqrt( (dSr - sqrt(_SQR(dSr)-4.0*dTmp)) / (2.0*dTmp) );

	//selection of a point away from the image origin
	int     nIdx;
	double  dMax = 0.0, dDist;

	for(i=0; i<nNumPoints; i++){
		if((dDist=_ABS(ptpImage[i]._dX) + _ABS(ptpImage[i]._dY)) > dMax){
			dMax = dDist;
			nIdx = i;
		}
	}

	//sign of dTy
	vX *= dTy;

	if(_SIGN(vX[0]*ptpModel[nIdx]._dX + vX[1]*ptpModel[nIdx]._dY + vX[2]) != _SIGN(ptpImage[nIdx]._dX) ||
			_SIGN(vX[3]*ptpModel[nIdx]._dX + vX[4]*ptpModel[nIdx]._dY + dTy) != _SIGN(ptpImage[nIdx]._dY)){
		dTy  = -dTy;
		vX  *= -1.0;
	}

	//set transform matrix from Model to Camera
	KMatrix mT(4,4,_IDENTITY);
	mT[0][0] = vX[0];
	mT[0][1] = vX[1];
	mT[0][2] = sqrt(1.0-_SQR(vX[0])-_SQR(vX[1])); //still not sure

	mT[1][0] = vX[3];
	mT[1][1] = vX[4];
	mT[1][2] = -_SIGN(vX[0]*vX[3]+vX[1]*vX[4])*sqrt(1.0-_SQR(vX[3])-_SQR(vX[4])); //still not sure

	mT[2][0] = mT[0][1]*mT[1][2] - mT[0][2]*mT[1][1];
	mT[2][1] = mT[0][2]*mT[1][0] - mT[0][0]*mT[1][2];
	mT[2][2] = mT[0][0]*mT[1][1] - mT[0][1]*mT[1][0];

	//tx
	mT[0][3] = vX[2];
	//ty
	mT[1][3] = dTy;

/////
	//determinateion 'f' and to 'Tz' to verify rotation matrix
	mA.Create(2,nNumPoints);
	for(i=0; i<nNumPoints; i++){
		mA[0][i] = mT[1][0]*ptpModel[i]._dX + mT[1][1]*ptpModel[i]._dY + mT[1][3];
		mA[1][i] = -ptpImage[i]._dY;
		vB[i]    = (mT[2][0]*ptpModel[i]._dX + mT[2][1]*ptpModel[i]._dY) * ptpImage[i]._dY;
	}
	vX = mA.Cor().Iv()*mA*vB;

	//modification of rotation
	if(vX[0] < 0.0){ //if focal length < 0
		mT[0][2] = -mT[0][2];
		mT[1][2] = -mT[1][2];
		mT[2][0] = -mT[2][0];
		mT[2][1] = -mT[2][1];
	}
////

	//tz;
	KVector vV(nNumPoints);
	KVector vP(nNumPoints);

	for(int i=0; i<nNumPoints; i++){
		vV[i] = ptpImage[i]._dY;
		vP[i] = dFocal * (mT[1][0]*ptpModel[i]._dX + mT[1][1]*ptpModel[i]._dY +  mT[1][3])
				 - vV[i]*(mT[2][0]*ptpModel[i]._dX + mT[2][1]*ptpModel[i]._dY);
	}
	mT[2][3] = (vV & vP) / vV.Norm2();

	return mT;
}
//---------------------------------------------------------------------------

// - F_3x1 = H_3x3 * M_3x1에서 호모그래피 행렬 H_3x3을 구함
// - psF: Image Point 리스트
// - psM: Model Point 리스트
// - bOptima: 비선형 최적화 수행여부
// - 반환값: 호모그래피 행렬 H_3x3
//
KMatrix KLinearTransform::Homography(const KPoints& psF,const KPoints& psM,bool bOptima)
{
    //Normalize image coordinates
    KVector vB;
    KMatrix mF,mFn,mTf,mM,mMn,mTm;

    for(int i=0,ii=psM.size(); ii; i++,ii--)
    {
        mF |= KVector(psF[i]._dX,psF[i]._dY,1.0);
        mM |= KVector(psM[i]._dX,psM[i]._dY,1.0);
    }

    mTf	= NormalizeCoordinates(mF,mFn); //normalizaing transform
    mTm	= NormalizeCoordinates(mM,mMn); //normalizaing transform

    //Set A matrix    
    KMatrix mA;
    KVector vM;
    for(int i=0,ii=psM.size(); ii; i++,ii--)
    {
        vM   = KVector(mMn[0][i],mMn[1][i],1.0);
        mA 	^= KVector(0.0,0.0,0.0).Tail(-vM).Tail(vM*mFn[1][i]).Tr();
        mA 	^= vM.Tail(KVector(0.0,0.0,0.0)).Tail(vM*(-mFn[0][i])).Tr();
        mA 	^= (vM*(-mFn[1][i])).Tail(vM*mFn[0][i]).Tail(KVector(0.0,0.0,0.0)).Tr();

    }

    //Closed-form solution
    KMatrix mU,mV,mHn, mH;
    KVector vD, vH;
    //SVD 적용
    mA.SVD(mU,vD,mV);
    //마지막 열 벡터가 호모그래피 행렬 요소를 구성함
    vH  = mV.Column(mV.Col()-1);
    //호모그래피 행렬 구성
    mHn = (vH.Cut(0,2).Tr() ^ vH.Cut(3,5).Tr() ^ vH.Cut(6,8).Tr());
    //undo the normalization
    mH  = ~mTf * mHn* mTm;
    //mH[2][2]=1.0 이 되도록 스케일링
    mH /= mH[2][2];


    //비선형 최적화
	if(bOptima == false)
		return mH;

    _mA.Release();
    _mB.Release();

    for(unsigned int i=0; i<psM.size(); i++) //used in Erf()
	{
        _mB |= KVector(psM[i]._dX,psM[i]._dY,1.0);
        _mA |= KVector(psF[i]);
	}

	_nOptima = _OPTIMA_HOMOGRAPHY;
    KVector vX = mH.Vector().To(7);

	KOptima::Powell(vX,0.001);
	vX.Tailed(1.0);

	mH.Create(3,3,vX.Address());

	return mH;
}

//mF, mM의 열벡터는 3x1 homogeneous coordinate (--, --, 1)
//
KMatrix KLinearTransform::Homography(const KMatrix& mF, const KMatrix& mM, bool bOptima)
{
    //Normalize image coordinates
    KVector vB;
    KMatrix mFn,mTf,mMn,mTm;

    mTf	= NormalizeCoordinates(mF,mFn); //normalizaing transform
    mTm	= NormalizeCoordinates(mM,mMn); //normalizaing transform

    //Set A matrix
    KMatrix mA;
    KVector vM;
    for(int i=0,ii=mM.Col(); ii; i++,ii--)
    {
        vM   = KVector(mMn[0][i],mMn[1][i],1.0);
        mA 	^= KVector(0.0,0.0,0.0).Tail(-vM).Tail(vM*mFn[1][i]).Tr();
        mA 	^= vM.Tail(KVector(0.0,0.0,0.0)).Tail(vM*(-mFn[0][i])).Tr();
        mA 	^= (vM*(-mFn[1][i])).Tail(vM*mFn[0][i]).Tail(KVector(0.0,0.0,0.0)).Tr();
    }

    //Closed-form solution
    KMatrix mU,mV,mHn, mH;
    KVector vD, vH;
    //SVD 적용
    mA.SVD(mU,vD,mV);
    //마지막 열 벡터가 호모그래피 행렬 요소를 구성함
    vH  = mV.Column(mV.Col()-1);
    //호모그래피 행렬 구성
    mHn = (vH.Cut(0,2).Tr() ^ vH.Cut(3,5).Tr() ^ vH.Cut(6,8).Tr());
    //undo the normalization
    mH  = ~mTf * mHn* mTm;
    //mH[2][2]=1.0 이 되도록 스케일링
    mH /= mH[2][2];


    //비선형 최적화
    if(bOptima == false)
        return mH;

    _mA.Release();
    _mB.Release();

    for(int i=0; i<mM.Col(); i++) //used in Erf()
    {
        _mB |= mM.Column(i);
        _mA |= mF.Column(i);
    }

    _nOptima = _OPTIMA_HOMOGRAPHY;
    KVector vX = mH.Vector().To(7);

    KOptima::Powell(vX,0.001);
    vX.Tailed(1.0);

    mH.Create(3,3,vX.Address());

    return mH;
}

//reference :  Z. Zhang, "A Flexible New Technique for Camera Calibration", IEEE Transactions on
//						  Pattern Analysis and Machine Intelligence, 22(11):1330?1334, 2000.
// - compute homogeneous transform from the camera frame to the plane model frame
// - psModel : 2-D points on the model plane
// - psImage : 2-D image points
// - mK      : camera matrix
// - return value : homogeneous transformation mT from Camera coord. to Plane Model coord., C_coord = mT * M_coord
//
KMatrix KLinearTransform::CameraToPlane(const KMatrix& mK,const KPoints& psImage,const KPoints& psModel,bool bOptima)
{
    // Homography
    KMatrix mH  = Homography(psImage,psModel,false);
	

    ////////////////////////////////////
    /// Extrinsic Parameter

	//apply the known camera matrix
	mH  = ~mK * mH;

	//rescaling  mH = (r1 r2 t)
    double  dS1,dS2;
	mH.Place(0,0,mH.Column(0).Normalized(_SIZE_NORMALIZE,&dS1));
	mH.Place(0,1,mH.Column(1).Normalized(_SIZE_NORMALIZE,&dS2));
    mH.Place(0,2,mH.Column(2)*(1.0/((dS1+dS2)*0.5)));
	mH *= _SIGN(mH[2][2]); //tz > 0

	//rotation
	KRotation   rRot;

    rRot = (mH.Column(0) | mH.Column(1) | (mH.Column(0).Skew() * mH.Column(1)));
	rRot.Orthogonalized();

	//translation
	KVector vT = mH.Column(2);

	//Homogeneous Transform
	KMatrix mT = (rRot | vT) ^ KVector(0.0,0.0,0.0,1.0).Tr();
	if(bOptima == false)
		return mT;

	//init. parameters for nonlinear optimization
	_mA.Release();
	_mB.Release();

    for(int i=0,ii=psModel.size(); ii; i++,ii--) //used in Erf()
	{
        _mA |= KVector(psImage[i]);
		_mB |= KVector(psModel[i]._dX,psModel[i]._dY,1.0);
	}
	_nOptima = _OPTIMA_PLANETOIMAGE;
	_mK 	 = mK;

	//nonlinear optimization
    KVector vX = rRot.Euler(_RAD).Tailed(vT);
	KOptima::Powell(vX,0.001);

	//return the homegeneous transform
	mT = (KRotation(vX[0],vX[1],vX[2],_RAD) | vX.Cut(3,5)) ^ KVector(0.0,0.0,0.0,1.0).Tr();

	return mT;
}

double KLinearTransform::Erf(const KVector& vX)
{
    double dError = 0.0;

	if(_nOptima == _OPTIMA_HOMOGRAPHY)
	{
		KMatrix mH(3,3,vX.Tail(1.0).Address());
		KMatrix mTmp = mH * _mB;

        mTmp /= KMatrix::RepMat(mTmp.Cut(2,_ROW),3,1);
		mTmp  = _mA - mTmp.To(1,mTmp.Col()-1);
		mTmp ^= 2.0;

		dError = mTmp.Sum().Sum();
	}
	else if(_nOptima == _OPTIMA_PLANETOIMAGE)
	{
		KMatrix mH   = (KRotation(vX[0],vX[1],vX[2],_RAD).To(2,1) | vX.Cut(3,5));
		KMatrix mTmp = _mK * mH * _mB;

        mTmp /= KMatrix::RepMat(mTmp.Cut(2,_ROW),3,1);
		mTmp  = _mA - mTmp.To(1,mTmp.Col()-1);
		mTmp ^= 2.0;

		dError = mTmp.Sum().Sum();
	}
    else if(_nOptima == _OPTIMA_RT4X4)
    {
        KRotation rOut;
        KVector   vT;

        rOut.FromEuler(vX[0],vX[1],vX[2],_RAD);
        vT = vX.From(3);

        KMatrix mTmp = _mB - (rOut*_mA + KMatrix::RepMat(vT,1,_mA.Col()));

        mTmp ^= 2.0;
        dError = mTmp.Sum().Sum();
    }


	return dError;
}

//////////////////////////////////////////////////////////////////////////////////
//mImg0 : 3 X N
//mImgN : 3 X N
//mTn   : 3 X 3
//
KMatrix KLinearTransform::NormalizeCoordinates(const KMatrix& mImgO,KMatrix& mImgN)
{
    double  dScale;
    KVector vMean(2), vUn;
	int     nCol = mImgO.Col();

	mImgN.Create(mImgO.Row(),nCol);

    vMean[0] = KVector(mImgO.Cut(0,_ROW)).Average(); //mean X coordinate
    vMean[1] = KVector(mImgO.Cut(1,_ROW)).Average(); //mean Y coordinate

    //calculate the scale parameter
    //평균점으로 원점을 이동하고 원점과의 거리평균이 2의 제곱근이 되도록 함
	dScale = 0.0;
	for(int j=0; j<nCol; j++)
		dScale += sqrt((mImgO.Column(j).To(1) - vMean).Norm2());
    dScale = nCol*1.41421356 / dScale;

	//set the image normalization transform
	KMatrix mTn(3,3);
	mTn[0][0] = mTn[1][1] = dScale;
	mTn[0][2] = -dScale*vMean[0];
	mTn[1][2] = -dScale*vMean[1];
	mTn[2][2] = 1.0;

	//image normalization
	mImgN = mTn * mImgO;

	return mTn;
}

// return mTo_1 ( Xo = h1_0 * X1)
//
KHomogeneous KLinearTransform::TransformFromPlanes_Planes(const KMatrices& msTo,const KMatrices& msT1)
{
#ifdef _DEBUG
	assert(msTo.Count() == msT1.Count());
#endif

	int			   nNum = msTo.Count();
	KMatrix		mMo(3,nNum), mM1(3,nNum);
	KVector		vBo(nNum), vB1(nNum),vNormal;
	double		    dDist;
		
	for(int i=0; i<nNum; i++)
	{		
		vNormal = msTo[i].Column(2).To(2);          
	
		mMo.Place(0,i,vNormal);						//plane normal vector(z-axis)
		vBo[i] = vNormal & msTo[i].Column(3).To(2); //distance to origin

		vNormal = msT1[i].Column(2).To(2);          
	
		mM1.Place(0,i,vNormal);						//plane normal vector(z-axis)
		vB1[i] = vNormal & msT1[i].Column(3).To(2); //distance to origin
	}

	//rotation and translation
	KRotation r1_0;
	KVector   v1_0;

	r1_0 = mM1 * mMo.Tr();
	r1_0.Orthogonalized();
	
	v1_0 = (mM1*mM1.Tr()).Iv() * mM1 * vB1 - r1_0*(mMo*mMo.Tr()).Iv()*mMo*vBo;
//	v1_0 = (mMo*mMo.Tr()).Iv() * mMo * (vBo-vB1);

	return KHomogeneous(r1_0, v1_0.To(2));
}

KVector KLinearTransform::TransformFromPlanes_Planes(const KVectors& vsTo,const KVectors& vsT1)
{
#ifdef _DEBUG
	assert(vsTo.Count() == vsT1.Count());
#endif

    int			 nNum = vsTo.size();
	KMatrix		 mMo(3,nNum), mM1(3,nNum);
	KHomogeneous hMo, hM1;
	KVector		 vBo(nNum), vB1(nNum),vNormal;

	for(int i=0; i<nNum; i++)
	{		
        hMo.Create(vsTo[i], _EULER);
		vNormal = hMo.R().Column(2);   	
		mMo.Place(0,i,vNormal);		 	//plane normal vector(z-axis)
		vBo[i] = vNormal & hMo.t();     //distance to origin

        hM1.Create(vsT1[i], _EULER);
		vNormal = hM1.R().Column(2);	
		mM1.Place(0,i,vNormal);						//plane normal vector(z-axis)
		vB1[i] = vNormal & hM1.t(); 	//distance to origin
	}

	//rotation and translation
	KRotation r1_0;
	KVector   v1_0;

	r1_0 = mM1 * mMo.Tr();
	r1_0.Orthogonalized();
	
	v1_0 = (mM1*mM1.Tr()).Iv() * mM1 * vB1 - r1_0*(mMo*mMo.Tr()).Iv()*mMo*vBo;
//	v1_0 = (mMo*mMo.Tr()).Iv() * mMo * (vBo-vB1);
	
	return	r1_0.Euler(_RAD).Tailed(v1_0);
}

KVector KLinearTransform::LinearEquation(const KMatrix& mA,const KVector& vB)
{
    KMatrix mAA(mA);
    KMatrix mU,mV,mWiv(mA.Col(),mA.Col());
    KVector vW;

    mAA.SVD(mU,vW,mV);

    for(int i=0; i<vW.Dim(); i++)
        mWiv[i][i] = (vW[i] < vW[0]*1.0e-6 ? 0.0 : 1.0/vW[i]);

    KVector vX = mV * mWiv * mU.Tr() * vB;

    return vX;
}

KMatrix KLinearTransform::TransformSimilarity(vector<KPoint> lFrom, const vector<KPoint>& lTo)
{
    int     nNum = lFrom.size();
    double  dNumIv = 1.0 / (double)nNum;
    KPoint  oMeanFrom, oMeanTo;
    KVector vDistFrom(2), vDistTo(2);
    KMatrix mCov(2, 2);
    double  dSigmaFrom = 0.0, dSigmaTo = 0.0;


    for(int i = 0; i<nNum; i++)
    {
        oMeanFrom._dX += lFrom[i]._dX;
        oMeanFrom._dY += lFrom[i]._dY;
        oMeanTo._dX   += lTo[i]._dX;
        oMeanTo._dY   += lTo[i]._dY;
    }
    oMeanFrom *= dNumIv;
    oMeanTo   *= dNumIv;

    for(int i = 0; i<nNum; i++)
    {
        vDistFrom[0] = lFrom[i]._dX - oMeanFrom._dX;
        vDistFrom[1] = lFrom[i]._dY - oMeanFrom._dY;
        vDistTo[0]   = lTo[i]._dX   - oMeanTo._dX;
        vDistTo[1]   = lTo[i]._dY   - oMeanTo._dY;

        dSigmaFrom += vDistFrom.Norm2();
        dSigmaTo += vDistTo.Norm2();
        mCov += vDistTo * vDistFrom.Tr();
    }
    dSigmaFrom *= dNumIv;
    dSigmaTo *= dNumIv;
    mCov *= dNumIv;

    KMatrix mU, mV, mS(KMatrix::Diag(1.0, 1.0));
    KVector vD;

    mCov.SVD(mU, vD, mV);

    if (mCov.Det() < 0.0 || (mCov.Det() == 0.0 && mU.Det()*mV.Det()<0.0))
        mS[1][1] = -1.0;

    KMatrix mOut(2, 3);
    double  dC = (dSigmaFrom != 0.0 ? 1.0 / dSigmaFrom * (mS*vD).Sum() : 1.0);

    mOut.Place(0, 0, mU*mS*mV.Tr());
    mOut.Place(0, 2, KVector(oMeanTo) - mOut.To(1, 1)*KVector(oMeanFrom)*dC);

    return mOut;
}

KMatrix KLinearTransform::TransformSimilarity(const KPoints& psFrom, const KPoints& psTo)
{
    int     nNum   = psFrom.size();
    double  dNumIv = 1.0/(double)nNum;
    KPoint  ptMeanFrom(0.0,0.0), ptMeanTo(0.0,0.0);
    KVector vDistFrom(2), vDistTo(2);
    KMatrix mCov(2,2);
    double  dSigmaFrom = 0.0, dSigmaTo = 0.0;


    for(int i=0; i<nNum; i++)
    {
        ptMeanFrom += psFrom[i];
        ptMeanTo   += psTo[i];
    }
    ptMeanFrom *= dNumIv;
    ptMeanTo   *= dNumIv;

    for(int i=0; i<nNum; i++)
    {
        vDistFrom[0]=  psFrom[i]._dX - ptMeanFrom._dX;
        vDistFrom[1]=  psFrom[i]._dY - ptMeanFrom._dY;
        vDistTo[0]  =  psTo[i]._dX - ptMeanTo._dX;
        vDistTo[1]  =  psTo[i]._dY - ptMeanTo._dY;

        dSigmaFrom += vDistFrom.Norm2();
        dSigmaTo   += vDistTo.Norm2();
        mCov       += vDistTo * vDistFrom.Tr();
    }
    dSigmaFrom *= dNumIv;
    dSigmaTo   *= dNumIv;
    mCov       *= dNumIv;

    KMatrix mU,mV,mS(KMatrix::Diag(1.0,1.0));
    KVector vD;

    mCov.SVD(mU,vD,mV);

    if(mCov.Det() < 0.0 || (mCov.Det() == 0.0 && mU.Det()*mV.Det()<0.0))
        mS[1][1] = -1.0;

    KMatrix mOut(2,3);
    double  dC = (dSigmaFrom != 0.0 ? 1.0/dSigmaFrom * (mS*vD).Sum() : 1.0);

    mOut.Place(0,0,mU*mS*mV.Tr());
    mOut.Place(0,2, KVector(ptMeanTo)-mOut.To(1,1)*KVector(ptMeanFrom)*dC);

    return mOut;
}
