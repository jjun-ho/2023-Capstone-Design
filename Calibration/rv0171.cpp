#include <iostream>
#include <cmath>
#include <math.h>
#include <random>
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "rv0171.h"

using namespace std;

// Read List of Points(x, y) from .txt file
vector<pair<double, double>> rv0171::PointList(string location)
{
    vector<pair<double, double>> pt;
    double x, y;

    ifstream fin;
    fin.open(location);
    while(!fin.eof())
    {
        fin >> x >> y;
        pt.push_back(make_pair(x, y));
    }
    if(fin.is_open()) fin.close();
    return pt;
}



//////////////////////////////
//// Zhang's Method (HW1) ////
//////////////////////////////
pair<double, double> rv0171::Normalization(double dX, double dY, KMatrix mat)
{
    //  dX, dX is x, y coordinate(double) of point, mat size is 2 x n
    // xmin, ymin, xmax, ymax(to make rectangle)
    double xmin = mat[0][0];
    double ymin = mat[1][0];
    double xmax = mat[0][0];
    double ymax = mat[1][0];
    for(unsigned int i = 1; i < mat.Col(); i++)
    {
        if(mat[0][i] < xmin) xmin = mat[0][i];
        if(mat[1][i] < ymin) ymin = mat[1][i];
        if(mat[0][i] > xmax) xmax = mat[0][i];
        if(mat[1][i] > ymax) ymax = mat[1][i];
    }

    // 1. shift-operation
    double xshift = dX - xmin;
    double yshift = dY - ymin;

    // 2. scale-operation
    double xlength = xmax - xmin;
    double ylength = ymax - ymin;

    double normalized_x = xshift / xlength;
    double normalized_y = yshift / ylength;

    return make_pair(normalized_x, normalized_y);
}

KMatrix rv0171::NormalizeCoordinates(KMatrix& M, KMatrix& normalized_M)
{
    KMatrix TM(3, 3);

    // 1. x_bar, y_bar (평균)
    double x_bar = 0;
    double y_bar = 0;
    for(unsigned int i = 0; i < M.Col(); i++)
    {
        x_bar += M[0][i];
        y_bar += M[1][i];
    }
    x_bar /= M.Col();
    y_bar /= M.Col();

    // 2. x', y'
    KMatrix temp_mM;
    for(unsigned int i=0; i < M.Col(); i++)
        temp_mM |= KVector(M[0][i] - x_bar, M[1][i] - y_bar); // 2 x 156

    // 3. s
    double s=0;
    double sum=0;
    for(unsigned int  i=0 ; i < temp_mM.Col(); i++)
        sum += sqrt(_SQR(temp_mM[0][i]) + _SQR(temp_mM[1][i]));
    s = sqrt(2) * temp_mM.Col() / sum;

    // 4. get T
    TM[0][0] =   s; TM[0][1] = 0.0; TM[0][2] = -s * x_bar;
    TM[1][0] = 0.0; TM[1][1] =   s; TM[1][2] = -s * y_bar;
    TM[2][0] = 0.0; TM[2][1] = 0.0; TM[2][2] =        1.0;

    normalized_M = TM * M;
    return TM;
}


KVector rv0171::MakevV(KMatrix mH, int i, int j)
{
    KVector vV(6);

    i -= 1;
    j -= 1;

    vV[0] = mH[0][i] * mH[0][j];
    vV[1] = mH[0][i] * mH[1][j] + mH[1][i] * mH[0][j];
    vV[2] = mH[1][i] * mH[1][j];
    vV[3] = mH[2][i] * mH[0][j] + mH[0][i] * mH[2][j];
    vV[4] = mH[2][i] * mH[1][j] + mH[1][i] * mH[2][j];
    vV[5] = mH[2][i] * mH[2][j];

    return vV;
}

KVector rv0171::ZhangsCalibration(vector<vector<pair<double, double>>> pointzip, const int nImg, const int nFeature, const int nItr)
{
    // 1. Estimate the Homography H
    KList<KMatrix> lmF;
    KMatrix mFn, mMn;
    KMatrix mTf, mTm;

    for(int i = 0; i < nImg; i++)
    {
        KMatrix mF;
        for(int j = 0; j < nFeature; j++)
            mF |= KVector(pointzip[i][j].first, pointzip[i][j].second, 1.0);    // Feature matrix
        lmF.Add(mF);
    }

    KMatrix mM;
    for(int i = 0; i < nFeature; i++)
        mM |= KVector(pointzip[nImg][i].first, pointzip[nImg][i].second, 1.0);  // Model matrix

    double mFn_x[nFeature];
    double mFn_y[nFeature];
    double mMn_x[nFeature];
    double mMn_y[nFeature];

    // Mn
    for(int i = 0; i < nFeature; i++)
    {
        mMn_x[i] = rv0171::Normalization(mM[0][i], mM[1][i], mM).first;
        mMn_y[i] = rv0171::Normalization(mM[0][i], mM[1][i], mM).second;
    }

    for(int i = 0; i < nFeature; i++)
    {
        mMn |= KVector(mMn_x[i], mMn_y[i], 1.0);    // Normalized model matrix
    }

    // Tm
    mTm = rv0171::NormalizeCoordinates(mM, mMn);    // Normalized model transform matrix

    // F(iterative)
    KList<KMatrix> lmH;
    KMatrix mV;

    for(int i = 0; i < nImg; i++)
    {
        for(int j = 0; j < nFeature; j++)
        {
            // refresh mFn_x, mFn_y for every image
            mFn_x[j] = rv0171::Normalization(lmF[i][0][j], lmF[i][1][j], lmF[i]).first;
            mFn_y[j] = rv0171::Normalization(lmF[i][0][j], lmF[i][1][j], lmF[i]).second;
        }

        for(int j = 0; j < nFeature; j++)
            mFn |= KVector(mFn_x[j], mFn_y[j], 1.0);        // Normalized feature matrix

        // Tf
        mTf = rv0171::NormalizeCoordinates(lmF[i], mFn);    // Normalized feature transform matrix

        // Form the matrix A such that ||Ah||=0, ||h||=1, (A is not intrinsic)
        KMatrix mA; // 3x9 matrix
        for(int j = 0; j < nFeature; j++)
        {
            KVector vM;
            // mMn: 3 x 156
            vM = KVector(mMn[0][j], mMn[1][j], 1.0); // Transpose in Advance
            // ^= 행단위로 추가
            mA ^= KVector(0.0, 0.0, 0.0).Tail(-vM).Tail(vM * mFn[1][j]).Tr();                  // 1 x 9, ^= 열단위 추가: (0^T, -mi^T, vi * mi^T)
            mA ^= vM.Tail(KVector(0.0, 0.0, 0.0)).Tail(vM * (-mFn[0][j])).Tr();                // ^= 열단위 추가: (mi^T, 0^T, -ui * mi^T)
            mA ^= (vM * (-mFn[1][j])).Tail(vM * mFn[0][j]).Tail(KVector(0.0, 0.0, 0.0)).Tr();  // ^= 열단위 추가: (-vi * mi^T, ui * mi^T, 0^T)
        }

        KMatrix _mU, _mV, mHn, mH;
        KVector _vD, _vH;

        // Apply SVD
        mA.SVD(_mU, _vD, _mV);

        // last col composes H
        _vH = _mV.Column(_mV.Col() - 1);

        // make H(normalized)
        mHn = (_vH.Cut(0, 2).Tr() ^ _vH.Cut(3, 5).Tr() ^ _vH.Cut(6, 8).Tr());

        // undo the normalization
        mH = ~mTf * mHn * mTm;
        // scale to make mH[2][2]=1.0
        mH /= mH[2][2];

        lmH.Add(mH);

        // 2. Form the matrix V as shown in the previous lecture
        KVector vV11, vV12, vV22;
        vV11 = rv0171::MakevV(mH, 1, 1);
        vV12 = rv0171::MakevV(mH, 1, 2);
        vV22 = rv0171::MakevV(mH, 2, 2);

        mV ^= vV12.Tr();                        // 1x6
        mV ^= (vV11-vV22).Tr();                 // 1x6
    }

    // 3. Solve Vb=0, then form the matrix B
    KMatrix mU, mW;
    KVector vD, vB;

    mV.SVD(mU, vD, mW);
    vB = mW.Column(mW.Col() - 1);

    KMatrix mB(3, 3);
    mB[0][0] = vB[0]; mB[0][1] = vB[1]; mB[0][2] = vB[3];
    mB[1][0] = vB[1]; mB[1][1] = vB[2]; mB[1][2] = vB[4];
    mB[2][0] = vB[3]; mB[2][1] = vB[4]; mB[2][2] = vB[5];

    // 4. Obtain the intrinsic parameters from B=r(A^-T)(A^-1)
    KMatrix _mA(3, 3);   // 3x3 calibration matrix
    double dLambda;
    _mA[1][2] = (mB[0][1]*mB[0][2] -  mB[0][0]*mB[1][2]) / (mB[0][0]*mB[1][1] - mB[0][1]*mB[0][1]);             // v0
    dLambda = mB[2][2] - (_SQR(mB[0][2]) + _mA[1][2] * (mB[0][1]*mB[0][2] - mB[0][0]*mB[1][2])) / mB[0][0];     // Lambda
    _mA[0][0] = sqrt(dLambda / mB[0][0]);                                                                       // alpha
    _mA[1][1] = sqrt(dLambda * mB[0][0] / (mB[0][0]*mB[1][1] - _SQR(mB[0][1])));                                // beta
    _mA[0][2] = -mB[0][2] * _SQR(_mA[0][0]) / dLambda;                                                          // u0
    _mA[2][2] = 1.0;

    // 5. Obtain the extrinsic parameters from H=[h1 h2 h3]=n*A[r1 r2 t]
    KList<KMatrix> lmR;
    KList<KVector> lvT;
    for(int i= 0; i < nImg; i++)
    {
        double dSize;
        KVector vR1, vR2, vR3, _vT;

        vR1 = _mA.Iv() * lmH[i].Column(0); vR1.Normalized(_SIZE_NORMALIZE, &dSize);
        vR2 = _mA.Iv() * lmH[i].Column(1); vR2.Normalized(_SIZE_NORMALIZE);
        vR3 = vR1.Skew() * vR2;
        _vT = 1 / dSize * _mA.Iv() * lmH[i].Column(2);

        //orthogonalization
        KMatrix _mR = (vR1 | vR2 | vR3);
        _mR.SVD(mU, vD, mW);
        _mR = mU * mW.Tr();
        //cout << "mRSize: " << _mR.Size() << endl;

        lmR.Add(_mR);
        lvT.Add(_vT);
    }

    // 6. Perform the nonlinear optimization for fine-tuning of the parameters including the lens distortion parameters
    // vX: 6x1 vector (alpha, beta, u0, v0, k1, k2)
    KVector vX;

    vX.Tailed(_mA[0][0]); vX.Tailed(_mA[1][1]); // alpha, beta
    vX.Tailed(_mA[0][2]); vX.Tailed(_mA[1][2]); // u0, v0

    vX.Tailed(KVector(0.0, 0.0));   // k1, k2

    for (int i = 0; i < nImg; i++)
    {
        KRotation rR(lmR[i]);
        KHomogeneous hP(rR, lvT[i]);
        //vX.Tailed(hP.EulerTranslation());
        vX.Tailed(hP.R().Rodrigues());              // R1, R2, R3
        vX.Tailed(lvT[i]);                          // Tx, Ty, Tz
    }
    // vX: 48x1

    // optimization
    KCalibrationZhang zhang;
    zhang.mM = mM;
    for(int i = 0; i < nImg; i++) zhang.mF[i] = lmF[i];
    zhang.nImg = nImg;
    zhang.nFeature = nFeature;
    zhang.Powell(vX, 0.05, nItr);

    vX.Tailed(zhang.dError);
    // vX: (alpha, beta, u0, v0, k1, k2) + (R1, R2, R3, Tx, Ty, Tz)*nImg + dError => 6*nImgx1

    return vX;
}

double KCalibrationZhang::Erf(const KVector& vX)
{
    double dError = 0;

    KMatrix mA(3, 3);   // intrinsic matrix
    mA[0][0] = vX[0];   // alpha
    mA[1][1] = vX[1];   // beta
    mA[0][2] = vX[2];   // u0
    mA[1][2] = vX[3];   // v0
    mA[2][2] = 1.0;     // scale factor
    double dK1 = vX[4]; // distortion parameter k1
    double dK2 = vX[5]; // distortion parameter k2

    KHomogeneous lP[nImg];
    for (int i = 0; i < nImg; i++)
    {
        KRotation rR;
        rR.FromRodrigues(vX[6*(i+1)], vX[6*(i+1)+1], vX[6*(i+1)+2]);
        KVector vT(vX[6*(i+1)+3], vX[6*(i+1)+4], vX[6*(i+1)+5]);
        lP[i] = KHomogeneous(rR, vT);
    }

    double dR2;
    KVector vXc, vXn, vU, vDr;
    // double sigma
    for(int i = 0; i < nImg; i++)
    {
        for(int j = 0; j < nFeature; j++)
        {
            vXc = (lP[i].R() * mM.Column(j) + lP[i].t());       // 3x1 matrix
            vXn = (vXc / vXc[2]).To(1);                         // 2x1 matrix

            // Radial distortion
            dR2 = _SQR(vXn[0]) + _SQR(vXn[1]);                  // double
            vDr = vXn*(1.0 + dK1 * dR2 + dK2 * dR2 * dR2);      // 2x1 matrix

            // Final distortion
            vXc = vDr.Tailed(1.0);                              // 3x1 matrix

            // image projection
            vU = mA * vXc; vU /= vU[2];                         // 3x1 matrix

            //error computation
            dError += (_SQR(mF[i][0][j] - vU[0]) + _SQR(mF[i][1][j] - vU[1]));
        }
    }
    //dError /= nImg * nFeature;
    this->dError = dError;

    cout << "Epoch:" << cnt++ \
         << " | Alpha:" << vX[0] << " Beta:" << vX[1] \
         << " u0:" << vX[2] << " v0:" << vX[3] \
         << " k1:" << vX[4] << " k2:" << vX[5] \
         << " Error:" << dError << endl;
    return dError;
}


//////////////////////////////
// Stereo Calibration (HW2) //
//////////////////////////////
KVector rv0171::PointProjection(KMatrix mA, KVector vK, KHomogeneous hP, KVector vM)
{
    // mA: Intrinsic parameter, vK: Distortion parameter, hP: Homogeneous matrix, vM: Model feature
    // Projection
    double dR2;
    KVector vXc, vXn, vU, vDr;
    vXc = hP.R() * vM + hP.t();     // 3x1 matrix
    vXn = (vXc / vXc[2]).To(1);     // 2x1 matrix
    // Radial distortion
    dR2 = _SQR(vXn[0]) + _SQR(vXn[1]);
    vDr = vXn * (1.0 + vK[0] * dR2 + vK[1] * pow(dR2, 2));  // 2x1 matrix
    // Final distortion
    vXc = vDr.Tailed(1.0);                              // 3x1 matrix
    // Image projection
    vU = mA * vXc;      vU /= vU[2];                    // 3x1 matrix, {ud, vd, 1.0}

    return vU;
}

KMatrix rv0171::ImageProjection(KMatrix mA, KVector vK, KHomogeneous hP, KMatrix mM)
{
    // mA: Intrinsic parameter, vK: Distortion parameter, hP: homogeneous matrix, mM:Model features
    int nFeature = mM.Size() / 3;
    KMatrix mF; // Projected points

    for(int i = 0; i < nFeature; i++)
    {
        // Projection
        double dR2;
        KVector vXc, vXn, vU, vDr;
        vXc = hP.R() * mM.Column(i) + hP.t();     // 3x1 matrix
        vXn = (vXc / vXc[2]).To(1);     // 2x1 matrix

        // Radial distortion
        dR2 = _SQR(vXn[0]) + _SQR(vXn[1]);
        vDr = vXn*(1.0 + vK[0] * dR2 + vK[1] * pow(dR2, 2));    // 2x1 matrix

        // Final distortion
        vXc = vDr.Tailed(1.0);          // 3x1 matrix

        // Image Projection
        vU = mA * vXc; vU /= vU[2];     // 3x1 matrix(scaled)

        mF |= vU;   // 3xnFeature matrix, add col
    }

    return mF;
}

KVector rv0171::StereoCalibration(KVector vXl, KVector vXr, vector<vector<pair<double, double>>> psFl, vector<vector<pair<double, double>>> psFr, const int nImg, const int nFeature, const int nItr)
{
    // Left image points
    KList<KMatrix> lpFl;    // Feature matrix for each image
    for (int i = 0; i < nImg; i++)
    {
        // For each image
        KMatrix mF;         // 3 x nFeature matrix
        for (int j = 0; j < nFeature; j++)
            mF |= KVector(psFl[i][j].first, psFl[i][j].second, 1.0); // Add a column
        lpFl.Add(mF);
    }

    // Right image points
    KList<KMatrix> lpFr;    // Feature matrix for each image
    for (int i = 0; i < nImg; i++)
    {
        // For each image
        KMatrix mF;         // 3 x nFeature matrix
        for (int j = 0; j < nFeature; j++)
            mF |= KVector(psFr[i][j].first, psFr[i][j].second, 1.0); // Add a column
        lpFr.Add(mF);
    }

    // Model points
    KMatrix mpM;             // 3 x mFeature matrix
    for (int i = 0; i < nFeature; i++)
        mpM |= KVector(psFr[nImg][i].first, psFr[nImg][i].second, 1.0);             // Add a column

    KCalibrationStereo stereo_calib;
    stereo_calib.vXl = vXl;
    stereo_calib.vXr = vXr;

    stereo_calib.nImg = nImg;
    stereo_calib.nFeature = nFeature;

    // update mAl, mAr, hRw, hLw, hLr, vKl, vKr

    KMatrix mAl(3, 3), mAr(3, 3);
    KVector vKl, vKr;

    // _mAl, _mAr, _vKl, _vKr
    mAl[0][0] = vXl[0];    // alpha
    mAl[1][1] = vXl[1];    // beta
    mAl[0][2] = vXl[2];    // u0
    mAl[1][2] = vXl[3];    // v0
    mAl[2][2] = 1.0;       // scale factor

    mAr[0][0] = vXr[0];    // alpha
    mAr[1][1] = vXr[1];    // beta
    mAr[0][2] = vXr[2];    // u0
    mAr[1][2] = vXr[3];    // v0
    mAr[2][2] = 1.0;       // scale factor

    stereo_calib._mAl = mAl;
    stereo_calib._mAr = mAr;

    // k1, k2
    stereo_calib._vKl = KVector(vXl[4], vXl[5]);
    stereo_calib._vKr = KVector(vXr[4], vXr[5]);

    KHomogeneous* hLw; // hLw
    hLw = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vXl[6*(i+1)], vXl[6*(i+1)+1], vXl[6*(i+1)+2]);
        KVector   vT(vXl[6*(i+1)+3], vXl[6*(i+1)+4], vXl[6*(i+1)+5]);
        hLw[i] = KHomogeneous(rR, vT);      // 4x4 matrix, W->TL
    }

    KHomogeneous* hRw; // hRw
    hRw = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vXr[6*(i+1)], vXr[6*(i+1)+1], vXr[6*(i+1)+2]);
        KVector   vT(vXr[6*(i+1)+3], vXr[6*(i+1)+4], vXr[6*(i+1)+5]);
        hRw[i] = KHomogeneous(rR, vT);      // 4x4 matrix, W->TR
    }

    KHomogeneous hLr;
    hLr = hLw[0] * ~hRw[0];

    stereo_calib._lpFl = &lpFl;
    stereo_calib._lpFr = &lpFr;
    stereo_calib._mpM = &mpM;

    stereo_calib._hLw = hLw;
    stereo_calib._hRw = hRw;
    stereo_calib._hLr = hLr;

    KVector vH;
    vH.Tailed(stereo_calib._hLr.R().Rodrigues());   // 3x1
    vH.Tailed(stereo_calib._hLr.t());               // 3x1

    stereo_calib.Powell(vH, 0.05, nItr);
    vH.Tailed(stereo_calib.dError);

    return vH;
}

double KCalibrationStereo::Erf(const KVector& vH)
{
    const KList<KMatrix>& lFl   = *_lpFl;   // list point Feature left
    const KList<KMatrix>& lFr   = *_lpFr;   // list point Feature right
    const KMatrix& mM           = *_mpM;    // matrix point Model

    KRotation rR;
    rR.FromRodrigues(vH[0], vH[1], vH[2]);
    KVector   vT(vH[3], vH[4], vH[5]);
    _hLr = KHomogeneous(rR, vT);
    _hRl = ~_hLr;

    double dError = 0.0;

    for (unsigned int i = 0; i < lFr.Count(); i++)  // // lFr.Count() = 15
    {
        // For each images
        // Right image projection
        KMatrix mFl = rv0171::ImageProjection(_mAl, _vKl, _hLr * _hRw[i], mM);     // 3 x nFeature
        KMatrix mFr = rv0171::ImageProjection(_mAr, _vKr, _hRl * _hLw[i], mM);     // 3 x nFeature
        // (--, --, 1.0)
        mFl /= KMatrix::RepMat(mFl.RowVec(2), 3, 1);
        mFr /= KMatrix::RepMat(mFr.RowVec(2), 3, 1);
        // Error
        KMatrix mEl = lFl[i] - mFl;
        KMatrix mEr = lFr[i] - mFr;
        // 3 x nFeature
        // Element-wise square
        mEl ^= 2.0;
        mEr ^= 2.0;
        // Norm
        dError += (mEl.Sum().Sum() + mEr.Sum().Sum());
    }
    cout << "Epoch:" << cnt++ \
         << " R1:" << vH[0] << " R2:" << vH[1] \
         << " R3:" << vH[2] << " t1:" << vH[3] \
         << " t2:" << vH[4] << " t3:" << vH[5] \
         << " Error:" << dError << endl;

    return dError;
}

Mat makePanorama(Mat matLeftImage, Mat matRightImage) {
    //imshow("L", matLeftImage);
    //imshow("R", matRightImage);
    waitKey(1);

    Mat matGrayLImage;
    Mat matGrayRImage;

    //Gray 이미지로 변환
    cvtColor(matLeftImage, matGrayLImage, COLOR_RGB2GRAY);
    cvtColor(matRightImage, matGrayRImage, COLOR_RGB2GRAY);

    //step 1 SURF이용해서 특징점 추출
    int nMinHessian =1500; // threshold (한계점)
    Ptr<SurfFeatureDetector> Detector = SURF::create(nMinHessian);

    vector <KeyPoint> vtKeypointsObject, vtKeypointsScene;

    Detector->detect(matGrayLImage, vtKeypointsObject);
    Detector->detect(matGrayRImage, vtKeypointsScene);

    Mat matLImageKeypoints;
    Mat matRImageKeypoints;
    drawKeypoints(matGrayLImage, vtKeypointsObject, matLImageKeypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    drawKeypoints(matGrayRImage, vtKeypointsScene, matRImageKeypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    //imshow("LK", matLImageKeypoints);
    //imshow("RK", matRImageKeypoints);
    waitKey(1);

    //step 2 기술자
    Ptr<SurfDescriptorExtractor> Extractor = SURF::create();
    //Ptr<SurfDescriptorExtractor> Extractor = SURF::create(100, 4, 3, false,true);

    Mat matDescriptorsObject, matDescriptorsScene;

    Extractor->compute(matGrayLImage, vtKeypointsObject, matDescriptorsObject);
    Extractor->compute(matGrayRImage, vtKeypointsScene, matDescriptorsScene);
    //descriptor(기술자)들 사이의 매칭 결과를 matches에 저장한다.
    FlannBasedMatcher Matcher; //kd트리를 사용하여 매칭을 빠르게 수행
    vector <DMatch> matches;
    Matcher.match(matDescriptorsObject, matDescriptorsScene, matches);

    Mat matAllMatches;
    drawMatches(matGrayLImage, vtKeypointsObject, matGrayRImage, vtKeypointsScene, matches, matAllMatches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    //imshow("allmatches", matAllMatches);
    waitKey(1);
    double dMaxDist = matches[0].distance;
    double dMinDist = matches[0].distance;
    double dDistance;

    // 두 개의 keypoint 사이에서 min-max를 계산한다 (min값만 사용)
    for (int i = 0; i < matDescriptorsObject.rows; i++) {
        dDistance = matches[i].distance;

        if (dDistance < dMinDist) dMinDist = dDistance;
        if (dDistance > dMaxDist) dMaxDist = dDistance;
    }
    printf("max_dist : %f \n", dMaxDist);
    printf("min_dist : %f \n", dMinDist);

    //match의 distance 값이 작을수록 matching이 잘 된 것
    //min의 값의 3배 또는 good_matches.size() > 60 까지만 goodmatch로 인정해준다.
    vector<DMatch>good_matches;
    int distance = 10;
    do {
        vector<DMatch>good_matches2;
        for (int i = 0; i < matDescriptorsObject.rows; i++) {
            if (matches[i].distance < distance * dMinDist)
                good_matches2.push_back(matches[i]);
        }
        good_matches = good_matches2;
        distance -= 1;
    } while (distance != 2 && good_matches.size() > 60);

    //keypoint들과 matching 결과 ("good" matched point)를 선으로 연결하여 이미지에 그려 표시
    Mat matGoodMatches;
    drawMatches(matGrayLImage, vtKeypointsObject, matGrayRImage, vtKeypointsScene, good_matches, matGoodMatches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    //imshow("good-matches", matGoodMatches);
    waitKey(3000);

    //Point2f형으로 변환
    vector <Point2f> obj;
    vector <Point2f> scene;

    // goodmatch에서의 keypoint를 저장
    for (int i = 0; i < good_matches.size();i++) {
        obj.push_back(vtKeypointsObject[good_matches[i].queryIdx].pt);
        scene.push_back(vtKeypointsScene[good_matches[i].trainIdx].pt);
    }
    Mat HomoMatrix = findHomography(scene, obj, FM_RANSAC);
    //RANSAC기법을 이용하여 첫 번째 매개변수와 두번째 매개변수 사이의 3*3 크기의 투영행렬변환 H를 구한다

//    KRotation rT;
//    rT.FromRodrigues(0.115867, 0.468524, -0.0275208); // 3x3
//    Mat HomoMatrix(3, 3, CV_64F);

//    for(int i = 0; i<3; i++)
//    {
//        for(int j = 0; j<3; j++)
//        {
//            HomoMatrix.at<double>(Point(j, i)) = rT[i][j];
//        }
//    }

//    cout << HomoMatrix << endl;

    //Homograpy matrix를 사용하여 이미지를 삐뚤게
    Mat matResult;
    warpPerspective(matRightImage, matResult, HomoMatrix, Size(matLeftImage.cols*2, matLeftImage.rows*1.2), INTER_CUBIC);

    Mat matPanorama;
    matPanorama = matResult.clone(); //복사본 대입

    imshow("wrap", matResult);
    waitKey(3000);

    Mat matROI(matPanorama, Rect(0, 0, matLeftImage.cols, matLeftImage.rows));
    matLeftImage.copyTo(matROI);

    //imshow("Panorama", matPanorama);
    //검은 여백 잘라내기

    int colorx = 0, colory = 0;
    for (int y = 0; y < matPanorama.rows; y++) {
        for (int x = 0; x < matPanorama.cols; x++) {
            if (matPanorama.at<Vec3b>(y, x)[0] == 0 && matPanorama.at<Vec3b>(y, x)[1] == 0 && matPanorama.at<Vec3b>(y, x)[2] == 0) {
                continue;
            }
            if (colorx < x) {
                colorx = x;
            }
            if (colory < y){
                colory = y;
            }
        }
    }

    Mat blackCutPanorama;
    blackCutPanorama = matPanorama(Range(0, colory), Range(0, colorx));
    //imshow("cutblack", blackCutPanorama);
    return blackCutPanorama;
}

//Cylinderical_Warping//
//Import_mAi
KMatrix rv0171::Import_mAi(int left_cam_num, int right_cam_num, int relative_cam_num)
{
    string mAi_path = "./data/Caltxt/Cal_";
    mAi_path += to_string(left_cam_num);
    mAi_path += to_string(right_cam_num);
    mAi_path += to_string(relative_cam_num);
    mAi_path += ".txt";

    string str;
    double buf[4] = {0,};
    ifstream mAi_fname(mAi_path);
    if (mAi_fname.is_open()) {
        int i =0;
        while (i<4)
        {
            getline(mAi_fname, str);
            buf[i] = stod(str);
            i++;
        }
    }
    else {
        qDebug() << "File open error!\n";
    }
    mAi_fname.close();

    KMatrix mAi(3,3);
    mAi[0][0] = buf[0];   // alpha
    mAi[1][1] = buf[1];   // beta
    mAi[0][2] = buf[2];   // u0
    mAi[1][2] = buf[3];   // v0
    mAi[2][2] = 1.0;     // scale

    return mAi;
}

//Import_rRi
KRotation rv0171::Import_rRi(int left_cam_num, int right_cam_num, int relative_cam_num)
{
    string RT_path = "./data/RTtxt/RT_";
    RT_path += to_string(left_cam_num);
    RT_path += to_string(right_cam_num);
    RT_path += to_string(relative_cam_num);
    RT_path += ".txt";

    string str;
    double buf[6] = {0,};
    ifstream RT_fname(RT_path);
    if (RT_fname.is_open()) {
        int i =0;
        while (getline(RT_fname, str))
        {
            buf[i] = stod(str);
            i++;
        }
    }
    else {
        qDebug() << "File open error!\n";
    }
    RT_fname.close();

    KRotation rRi;
    rRi.FromRodrigues(buf[0], buf[1], buf[2]); // 3x3

    return rRi;
}

////Import_vTi
KVector rv0171::Import_vTi(int left_cam_num, int right_cam_num, int relative_cam_num)
{
    string RT_path = "./data/RTtxt/RT_";
    RT_path += to_string(left_cam_num);
    RT_path += to_string(right_cam_num);
    RT_path += to_string(relative_cam_num);
    RT_path += ".txt";

    string str;
    double buf[6] = {0,};
    ifstream RT_fname(RT_path);
    if (RT_fname.is_open()) {
        int i =0;
        while (getline(RT_fname, str))
        {
            buf[i] = stod(str);
            i++;
        }
    }
    else {
        qDebug() << "File open error!\n";
    }
    RT_fname.close();

    KVector vTi;
    vTi.Tailed(buf[3]).Tailed(buf[4]).Tailed(buf[5]); // 아래로붙임 3x1

    return vTi;
}


vector<vector<KVector>> rv0171::make_3DCameraCoord(KImageColor Img)
{
    KVector vXi(3);
    vector<vector<KVector>> vvXi;
    vector<KVector> subvvXi;

    for(int i=0; i<Img.Row(); i++) //1024
    {
        for(int j=0; j<Img.Col(); j++)//1280
        {
            vXi[0] = j; // u - ui
            vXi[1] = i; // v - vi
            vXi[2] = 1;

            subvvXi.push_back(vXi); // 1x1280
        }
        vvXi.push_back(subvvXi); //1024x1280
        subvvXi.clear();
    }
    return vvXi;
}



void rv0171::make_Surround_View_Stitching(KMatrix mA, KMatrix mAi, KRotation rRi, KVector vTi, vector<vector<KVector>> &ui)
{
    KMatrix temp(3,1);
    KMatrix temp2(3,1);

    for(int i =0; i<ui.size();i++) //1024
    {
        for(int j =0; j<ui.at(0).size();j++) // 1280
        {
            temp = mA*rRi.Iv()*(mAi.Iv()*ui[i][j]);
            //내꺼
//            temp = mA*(rRi.Iv()*mAi.Iv())*ui[i][j];
            ui[i][j] = temp/temp[2][0];
        }
    }

    for(int i =0; i<ui.size();i++) //1024
    {
        for(int j =0; j<ui.at(0).size();j++) // 1280
        {
            temp2 = -mA*rRi.Iv()*vTi;
            //내꺼
//            temp2 = mA*(rRi.Iv()*mAi.Iv())*(-mAi*vTi);
            ui[i][j] += temp2/temp2[2][0];
        }
    }
}


void rv0171::make_Cylinderical_Warp(KMatrix mA, vector<vector<KVector>> &ui)
{
    // V0(uv,vv) // 가상 좌표계를 만들고  (가상의 방향) 이것으로 virtual camera 를 만든다  virtual camera의  중심을 optical center (V0)로 했다
    //N개의 카메라가 있다면 각각의 optical center가 있는데 그것의  중점이 world coordinate 의 중심으로   (uv,vv) 로 둿다.

    double fv = mA[0][0];

    for(int i =0; i<ui.size();i++) //1024
    {
        for(int j =0; j<ui.at(0).size();j++) // 1280
        {
            ui.at(i).at(j)._ppA[0][0] = fv * atan2( ui.at(i).at(j)._ppA[0][0],fv)+mA[0][2];
            ui.at(i).at(j)._ppA[1][0] = fv * ( ui.at(i).at(j)._ppA[1][0] / sqrt(ui.at(i).at(j)._ppA[0][0]*ui.at(i).at(j)._ppA[0][0] + fv*fv))+mA[1][2];
        }
    }

}


KMatrices rv0171::make_Surround_View_Stitching_ver2(KMatrix mA, KMatrix mAi, KRotation rRi, KVector vTi, vector<vector<KVector>> &ui, int n)
{
    KMatrix mppU;
    KMatrix mppUv,mppUv2;

    KMatrices mppUs(1024);
    mppUs.RemoveAll();

    KHomogeneous hpTvo_c , hpTc_vo;

    hpTvo_c.t() = vTi;
    hpTvo_c.R() = rRi;

    hpTc_vo = hpTvo_c.Iv();

    for(int i =0; i<ui.size();i++) //1024
    {
        KMatrix mppU;
        for(int j =0; j<ui.at(0).size();j++) // 1280
        {
            mppU |= KVector(ui.at(i).at(j)._ppA[0][0],ui.at(i).at(j)._ppA[1][0],1.0); // 3x1280

        }

        mppUv = mA * KRotation(_Y_AXIS, -_RADIAN(22.5*(2*n-1)))* hpTvo_c.R() * (mAi.Iv()*mppU);  // mppu -> 이미지 좌표

        mppUv2 = mA  * KRotation(_Y_AXIS, -_RADIAN(22.5*(2*n-1))) *hpTvo_c.R() * (- KMatrix::RepMat(hpTc_vo.t(), 1, mppUv.Col() ));

        mppUv += mppUv2;

        mppUv /= KMatrix::RepMat(mppUv.Cut(2,_ROW),3,1);

        mppUs.Add(mppUv);

    }
    return mppUs; // 1024x1280
}


void rv0171::make_Cylinderical_Warp_ver2(KMatrix mA, KMatrices &ui)
{
    // V0(uv,vv) // 가상 좌표계를 만들고  (가상의 방향) 이것으로 virtual camera 를 만든다  virtual camera의  중심을 optical center (V0)로 했다
    //N개의 카메라가 있다면 각각의 optical center가 있는데 그것의  중점이 world coordinate 의 중심으로   (uv,vv) 로 둿다.

    double fv = mA[0][0];

    for(int i =0; i<1024;i++) //1024
    {
        for(int j =0; j<1280;j++) // 1280
        {
            ui[i]._ppA[0][j] = fv * atan2( ui[i]._ppA[0][j],fv)+mA[0][2];
            ui[i]._ppA[1][j] = fv * ( ui[i]._ppA[1][j] / sqrt(ui[i]._ppA[0][j]*ui[i]._ppA[0][j] + fv*fv))+mA[1][2];
        }
    }

}
