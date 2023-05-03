#include "msjrv.h"

namespace msjrv {
//////////
// HW01 //
//////////
KVector ZhangMethod(int nImg, int nFeature, KPoint**& psF, KPoint*& psM) {
    // Debug
//    qDebug() << nImg << nFeature;
//    for (int i = 0; i < nImg; i++) {
//        for (int j = 0; j < nFeature; j++) {
//            qDebug() << "image" << i + 1 << psF[i][j]._dX << psF[i][j]._dY;
//        }
//    }
//    for (int i = 0; i < nFeature; i++) {
//        qDebug() << "model" << psM[i]._dX << psM[i]._dY;
//    }

    // Gather pairs of a target point and its corresponding point
    // Image points
    KList<KMatrix> lmF; // Feature matrix for each image
    for (int i = 0; i < nImg; i++) {
        // For each image
        KMatrix mF; // 3 x nFeature matrix
        for (int j = 0; j < nFeature; j++) {
            mF |= KVector(psF[i][j]._dX, psF[i][j]._dY, 1.0);   // Add a column
        }
        lmF.Add(mF);
    }

    // lmF input debug
//    for (int i = 0; i < nImg; i++) {
//        for (int j = 0; j < nFeature; j++) {
//            qDebug() << i << lmF[i][__X][j] << lmF[i][__Y][j];
//        }
//    }

    // Model points
    KMatrix mM;     // 3 x mFeature matrix
    for (int i = 0; i < nFeature; i++) {
        mM |= KVector(psM[i]._dX, psM[i]._dY, 1.0);             // Add a column
    }

    // mM input debug
//    for (int i = 0; i < nFeature; i++) {
//        qDebug() << mM.Column(i)[__X] << mM.Column(i)[__Y];
//    }

    // Coordinate Normalization
    KLinearTransform KLT;
    KMatrix mMn, mTm;                           // 3 x nFeature normalized matrix, 3x3 transformation matrix
    mMn = Normalization(mM);                    // Model coordinate normalization
    mTm = KLT.NormalizeCoordinates(mM, mMn);    // Model coordinate normalization transform

    KMatrix mV;
    KList<KMatrix> lmH;                         // Homogeneous matrix for each image
    for (int i = 0; i < nImg; i++) {
        // Coordinate Normalization
        KMatrix mFn, mTf;                       // 3 x nFeature normalized matrix, 3x3 transformation matrix
        mFn = Normalization(lmF[i]);            // Images coordinate Normalization
        mTf = KLT.NormalizeCoordinates(lmF[i], mFn);// Images coordinate normalization transform

        // Form the matrix A such that ||Ah||=0, ||h||=1
        KMatrix mA;     // 3x9 matrix
        for (int j = 0; j < nFeature; j++) {
            KVector vM;
            vM = KVector(mMn[__X][j], mMn[__Y][j], 1.0);
            mA ^= KVector(0.0, 0.0, 0.0).Tail(-vM).Tail(vM * mFn[1][j]).Tr();                   // 1x9, Add a row
            mA ^= vM.Tail(KVector(0.0, 0.0, 0.0)).Tail(vM * (-mFn[0][j])).Tr();                 // Add a row
            mA ^= (vM * (-mFn[1][j])).Tail(vM * mFn[0][j]).Tail(KVector(0.0, 0.0, 0.0)).Tr();   // Add a row
        }

        // Apply SVD to the matrix A
        KMatrix mU_, mV_, mHn, mH;
        KVector vD_, vH_;
        // Apply SVD
        mA.SVD(mU_, vD_, mV_);
        // 마지막 열 벡터가 호모그래피 행렬 요소를 구성함
        vH_ = mV_.Column(mV_.Col() - 1);
        // 호모그래피 행렬 구성
        mHn = (vH_.Cut(0, 2).Tr() ^ vH_.Cut(3, 5).Tr() ^ vH_.Cut(6, 8).Tr());
        // Undo the normalization
        mH = ~mTf * mHn * mTm;
        // mH[2][2] = 1.0 이 되도록 스케일링
        // Homogeneous matrix mH, 3x3 matrix
        mH /= mH[2][2];
        lmH.Add(mH);

        // Form the matrix V
        KVector vV11, vV12, vV22;               // 6x1 vector
        vV11 = MakeVVector(1, 1, mH);
        vV12 = MakeVVector(1, 2, mH);
        vV22 = MakeVVector(2, 2, mH);

        //mV.Place(2 * i + 0, 0, vV12.Tr());
        //mV.Place(2 * i + 1, 0, (vV11 - vV22).Tr());
        mV ^= vV12.Tr();                        // 1x6
        mV ^= (vV11-vV22).Tr();                 // 1x6
    }   //mV: (2*num_img) x 6 matrix

    // Solve Vb=0, then form the matrix B
    // B = A^(-T)A^(-1)
    KMatrix mU_, mW_;
    KVector vD_, vB_;

    mV.SVD(mU_, vD_, mW_);
    vB_ = mW_.Column(mW_.Col() - 1);

    KMatrix mB(3, 3);
    mB[0][0] = vB_[0];       mB[0][1] = vB_[1];       mB[0][2] = vB_[3];
    mB[1][0] = vB_[1];       mB[1][1] = vB_[2];       mB[1][2] = vB_[4];
    mB[2][0] = vB_[3];       mB[2][1] = vB_[4];       mB[2][2] = vB_[5];

    // Obtain the intrinsic parameters from B
    // B=lambda*A^(-T)A^(-1)
    KMatrix mA(3, 3);       // 3x3 calibration matrix
    double dLambda;
    mA[1][2] = (mB[0][1] * mB[0][2] - mB[0][0] * mB[1][2]) / (mB[0][0] * mB[1][1] - pow(mB[0][1], 2));          // v0
    dLambda = mB[2][2] - (_SQR(mB[0][2]) + mA[1][2] * (mB[0][1] * mB[0][2] - mB[0][0] * mB[1][2])) / mB[0][0];
    mA[0][0] = sqrt(dLambda / mB[0][0]);                                                                        // alpha
    mA[1][1] = sqrt(dLambda * mB[0][0] / (mB[0][0] * mB[1][1] - _SQR(mB[0][1])));                               // beta
    mA[0][2] = -mB[0][2] * _SQR(mA[0][0]) / dLambda;                                                            // u0
    mA[2][2] = 1.0;

    KList<KMatrix> lmR;
    KList<KVector> lvT;
    for (int i = 0; i < nImg; i++) {
        // Obtain the extrinsic parameters from H=[h1 h2 h3]=\etaA[r1 r2 t]
        double dSize;
        KVector vR1, vR2, vR3, vT;
        vR1 = mA.Iv() * lmH[i].Column(0);      vR1.Normalized(_SIZE_NORMALIZE, &dSize);
        vR2 = mA.Iv() * lmH[i].Column(1);      vR2.Normalized(_SIZE_NORMALIZE);
        vR3 = vR1.Skew() * vR2;
        vT = 1/dSize * mA.Iv() * lmH[i].Column(2);

        // Orthogonalization
        KMatrix mR = (vR1 | vR2 | vR3);
        KMatrix mU_, mW_;
        KVector vD_;
        mR.SVD(mU_, vD_, mW_);
        mR = mU_ * mW_.Tr();

        lmR.Add(mR);        // Rotation
        lvT.Add(vT);        // Translation
    }

    // Perform the nonlinear optimization for fine-tuning of the
    // parameters including the lens distortion parameters
    // Set the state vector
    KVector vX;
    vX.Tailed(mA[0][0]);   vX.Tailed(mA[1][1]);     // alpha, beta
    vX.Tailed(mA[0][2]);   vX.Tailed(mA[1][2]);     // u0, v0
    vX.Tailed(KVector(0.0, 0.0));                   // k1, k2
    // vX: 6x1 vector {alpha, beta, u0, v0, k1, k2}

    // Use Rodrigues
    for (int i = 0; i < nImg; i++) {
        KRotation rR(lmR[i]);
        KHomogeneous hP(rR, lvT[i]);
        //vX.Tailed(hP.EulerTranslation());
        vX.Tailed(hP.R().Rodrigues());              // 3x1
        vX.Tailed(lvT[i]);                          // 3x1
    }
    // vX: 48x1

    // Optimization
    KCalibrationZhang calib_zhang;
    calib_zhang.mM = mM;
    calib_zhang.lmF = lmF;
    calib_zhang.nImg = nImg;
    calib_zhang.nFeature = nFeature;

    int nItr = 100;
    calib_zhang.Powell(vX, 0.05, nItr);

    // Save error value to last row of vX
    vX.Tailed(calib_zhang.dError);
    return vX;
}

KMatrix Normalization(KMatrix coor_mat) {
    // coor_mat: 3x156 matrix (x, y, 1.0)
    double x_sum = 0, y_sum = 0;
    double x_avg = 0, y_avg = 0;
    for (int i = 0; i < (int)coor_mat.Size() / 3; i++) {
        x_sum += coor_mat[__X][i];
        y_sum += coor_mat[__Y][i];
    }
    x_avg = x_sum / (coor_mat.Size() / 3);
    y_avg = y_sum / (coor_mat.Size() / 3);

    for (int i = 0; i < (int)coor_mat.Size() / 3; i++) {
        coor_mat[__X][i] -= x_avg;
        coor_mat[__Y][i] -= y_avg;
    }

    return coor_mat;
}

KVector MakeVVector(int i, int j, KMatrix mH) {
    KVector vV(6);

    vV[0] = mH[0][i - 1] * mH[0][j - 1];
    vV[1] = mH[0][i - 1] * mH[1][j - 1] + mH[1][i - 1] * mH[0][j - 1];
    vV[2] = mH[1][i - 1] * mH[1][j - 1];
    vV[3] = mH[2][i - 1] * mH[0][j - 1] + mH[0][i - 1] * mH[2][j - 1];
    vV[4] = mH[2][i - 1] * mH[1][j - 1] + mH[1][i - 1] * mH[2][j - 1];
    vV[5] = mH[2][i - 1] * mH[2][j - 1];

    return vV;
}

//////////
// HW02 //
//////////
KVector StereoCalibration(int nImg, int nFeature, KVector vXl, KVector vXr, KPoint**& psFl, KPoint**& psFr, KPoint*& psM) {
    // Gather pairs of a target point and its corresponding point
    // Left image points
    KList<KMatrix> lmFl;    // Feature matrix for each image
    for (int i = 0; i < nImg; i++) {
        // For each image
        KMatrix mF;         // 3 x nFeature matrix
        for (int j = 0; j < nFeature; j++) {
            mF |= KVector(psFl[i][j]._dX, psFl[i][j]._dY, 1.0); // Add a column
        }
        lmFl.Add(mF);
    }
//    // lmFl input debug
//    for (int i = 0; i < nImg; i++) {
//        for (int j = 0; j < nFeature; j++) {
//            cout << i << lmFl[i][__X][j] << lmFl[i][__Y][j];
//            cout << "AA" << endl;
//        }
//    }

    // Right image points
    KList<KMatrix> lmFr;    // Feature matrix for each image
    for (int i = 0; i < nImg; i++) {
        // For each image
        KMatrix mF;         // 3 x nFeature matrix
        for (int j = 0; j < nFeature; j++) {
            mF |= KVector(psFr[i][j]._dX, psFr[i][j]._dY, 1.0); // Add a column
        }
        lmFr.Add(mF);
    }
    // lmFr input debug
//    for (int i = 0; i < nImg; i++) {
//        for (int j = 0; j < nFeature; j++) {
//            qDebug() << i << lmFr[i][__X][j] << lmFr[i][__Y][j];
//        }
//    }

    // Model points
    KMatrix mM;             // 3 x mFeature matrix
    for (int i = 0; i < nFeature; i++) {
        mM |= KVector(psM[i]._dX, psM[i]._dY, 1.0);             // Add a column
    }
    // mM input debug
//    for (int i = 0; i < nFeature; i++) {
//        qDebug() << mM.Column(i)[__X] << mM.Column(i)[__Y];
//    }

    // For left images
    KMatrix mAl(3, 3);
    mAl[0][0] = vXl[0];     // alpha
    mAl[1][1] = vXl[1];     // beta
    mAl[0][2] = vXl[2];     // u0
    mAl[1][2] = vXl[3];     // v0
    mAl[2][2] = 1.0;        //
    double dK1l = vXl[4];   // distortion parameter k1
    double dK2l = vXl[5];   // distortion parameter k2
    // mAl debug
//    qDebug() << mAl[0][0] << mAl[0][1] << mAl[0][2];
//    qDebug() << mAl[1][0] << mAl[1][1] << mAl[1][2];
//    qDebug() << mAl[2][0] << mAl[2][1] << mAl[2][2];
//    qDebug() << dK1l << dK2l;

//    cout << "vXl: " << vXl[6*(2+1)+0] << " " << vXl[6*(2+1)+1] << " " << vXl[6*(2+1)+2] << endl;


    KHomogeneous* hPl;
    hPl = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vXl[6 * (i + 1)], vXl[6 * (i + 1) + 1], vXl[6 * (i + 1) + 2]);
        KVector   vT(vXl[6*(i + 1) + 3], vXl[6 * (i + 1) + 4], vXl[6 * (i + 1) + 5]);
        hPl[i] = KHomogeneous(rR, vT);      // 4x4 matrix, TL->W
    }

    // For right images
    KMatrix mAr(3, 3);
    mAr[0][0] = vXr[0];     // alpha
    mAr[1][1] = vXr[1];     // beta
    mAr[0][2] = vXr[2];     // u0
    mAr[1][2] = vXr[3];     // v0
    mAr[2][2] = 1.0;        //
    double dK1r = vXr[4];   // distortion parameter k1
    double dK2r = vXr[5];   // distortion parameter k2
    // mAr debug
//    qDebug() << mAr[0][0] << mAr[0][1] << mAr[0][2];
//    qDebug() << mAr[1][0] << mAr[1][1] << mAr[1][2];
//    qDebug() << mAr[2][0] << mAr[2][1] << mAr[2][2];
//    qDebug() << dK1r << dK2r;

    KHomogeneous* hPr;
    hPr = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vXr[6 * (i + 1)], vXr[6 * (i + 1) + 1], vXr[6 * (i + 1) + 2]);
        KVector   vT(vXr[6*(i + 1) + 3], vXr[6 * (i + 1) + 4], vXr[6 * (i + 1) + 5]);
        hPr[i] = KHomogeneous(rR, vT);      // 4x4 matrix, TR->W
    }

    cout << endl << endl;
//    for(int i = 0; i < nImg; i++)
//    {
//        cout << i << ": ";
//        for(int j = 0; j < 6; j++)
//        {
//            cout << vXl[6*(i+1)+j] << " ";
//        }
//        cout << endl;
//    }

//    cout << "vXr: " << vXr[6*(2+1)+0] << " " << vXr[6*(2+1)+1] << " " << vXr[6*(2+1)+2] << endl;

    // Stereo Calibration
    KCalibrationStereo calib_stereo;
    calib_stereo.mAl = mAl;
    calib_stereo.mAr = mAr;


    calib_stereo._hRw = hPr;
    calib_stereo._hLw = hPl;

    calib_stereo.hLr = hPl[0] * ~hPr[0];




//    qDebug() << calib_stereo.hLr.Size();
//    for (int i = 0; i < 4; i++) {
//        for (int j = 0; j < 4; j++) {
//            qDebug() << calib_stereo.hLr[i][j];
//        }
//        qDebug() << "---";
//    }

    KVector vLw;
    vLw.Tailed(calib_stereo._hLw[0].R().Rodrigues());   // 3x1
    vLw.Tailed(calib_stereo._hLw[0].t());               // 3x1

    KVector vRw;
    vRw.Tailed(calib_stereo._hRw[0].R().Rodrigues());   // 3x1
    vRw.Tailed(calib_stereo._hRw[0].t());               // 3x1

    KVector vX;
    vX.Tailed(calib_stereo.hLr.R().Rodrigues());    // 3x1
    vX.Tailed(calib_stereo.hLr.t());                // 3x1

//    cout << "vLw" << endl;
//    for(unsigned int i = 0; i < vLw.Size(); i++)
//    {
//        cout << vLw[i] << " ";
//    }
//    cout << endl;
//    cout << "vRw" << endl;
//    for(unsigned int i = 0; i < vRw.Size(); i++)
//    {
//        cout << vRw[i] << " ";
//    }
//    cout << endl;
//    cout << "vX" << endl;
//    for(int i = 0; i < vX.Size(); i++)
//    {
//        cout << vX[i] << " ";
//    }
//    cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;

    calib_stereo.vKl = KVector(dK1l, dK2l);
    calib_stereo.vKr = KVector(dK1r, dK2r);

    calib_stereo._lmFl = &lmFl;
    calib_stereo._lmFr = &lmFr;
    calib_stereo._mM  = &mM;

    int nItr = 100;
    calib_stereo.Powell(vX, 0.05, nItr);
    vX.Tailed(calib_stereo.dError);

    cout << "vX" << endl;
    for(unsigned int i = 0; i < vX.Size(); i++)
    {
        cout << vX[i] << " ";
    }

    delete[] hPl;
    delete[] hPr;
    return vX;
}

KVector PointProjection(KMatrix mA, KVector vK, KHomogeneous hP, KVector vM) {
    // mA: Intrinsic parameter, vK: Distortion parameter, hP: Homogeneous matrix, vM: Model feature
    // Projection
    double dR2;
    KVector vXc, vXn, vU, vDr;
    vXc = hP.R() * vM + hP.t();         // 3x1 matrix
    vXn = (vXc / vXc[2]).To(1);                         // 2x1 matrix
    // Radial distortion
    dR2 = _SQR(vXn[0]) + _SQR(vXn[1]);                  // double
    vDr = vXn * (1.0 + vK[0] * dR2 + vK[1] * pow(dR2, 2));  // 2x1 matrix
    // Final distortion
    vXc = vDr.Tailed(1.0);                              // 3x1 matrix
    // Image projection
    vU = mA * vXc;      vU /= vU[2];                    // 3x1 matrix, {ud, vd, 1.0}

    return vU;
}

KMatrix ImageProjection(KMatrix mA, KVector vK, KHomogeneous hP, KMatrix mM) {
    // mA: Intrinsic parameter, vK: Distortion parameter, hP: Homogeneous matrix, mM: Model features
    int nFeature = mM.Size() / 3;
    KMatrix mF;     // Return value: projected points

    for (int i = 0; i < nFeature; i++) {
        // Projection
        double dR2;
        KVector vXc, vXn, vU, vDr;
        vXc = hP.R() * mM.Column(i) + hP.t();               // 3x1 matrix
        vXn = (vXc / vXc[2]).To(1);                         // 2x1 matrix
        // Radial distortion
        dR2 = _SQR(vXn[0]) + _SQR(vXn[1]);                  // double
        vDr = vXn * (1.0 + vK[0] * dR2 + vK[1] * pow(dR2, 2));              // 2x1 matrix
        // Final distortion
        vXc = vDr.Tailed(1.0);                              // 3x1 matrix
        // Image projection
        vU = mA * vXc;      vU /= vU[2];                    // 3x1 matrix, {ud, vd, 1.0}

        mF |= vU;                                           // 3 x nFeature matrix, add a column
    }

    return mF;
}


}

double KCalibrationZhang::Erf(const KVector& vX) {
    int idx = 0;
    double dError = 0;

    KMatrix mA(3, 3);
    mA[0][0] = vX[idx];     // alpha
    mA[1][1] = vX[++idx];   // beta
    mA[0][2] = vX[++idx];   // u0
    mA[1][2] = vX[++idx];   // v0
    mA[2][2] = 1.0;         //
    double dK1 = vX[++idx]; // distortion parameter k1
    double dK2 = vX[++idx]; // distortion parameter k2

    KHomogeneous lP[nImg];
    for (int i = 0; i < nImg; i++) {
//        lP[i] = KHomogeneous(vX.Cut(idx, idx + 5));
//        idx += 6;
        KRotation rR;
        rR.FromRodrigues(vX[6 * (i + 1)], vX[6 * (i + 1) + 1], vX[6 * (i + 1) + 2]);
        KVector   vT(vX[6*(i + 1) + 3], vX[6 * (i + 1) + 4], vX[6 * (i + 1) + 5]);
        lP[i] = KHomogeneous(rR, vT);
    }

    double dR2;
    KVector vXc, vXn, vU, vDr;
    for (int i = 0; i < nImg; i++) {
        for (int j = 0; j < nFeature; j++) {
//            vXc = lP[i].R() * mM.Column(j) + lP[i].t();         // 3x1 matrix
//            vXn = (vXc / vXc[2]).To(1);                         // 2x1 matrix
//            // Image projection
//            vU = mA * vXc;      vU /= vU[2];                    // 3x1 matrix
//            // Radial distortion
//            dR2 = _SQR(vXn[0]) + _SQR(vXn[1]);                  // double
//            //vDr = (1.0 + dK1 * dR2 + dK2 * pow(dR2, 2));
//            // Image distortion
//            vU[0] -= mA[0][2];  vU[1] -= mA[1][2];              // 3x1 matrix
//            vU *= (1.0 + dK1 * dR2 + dK2* pow(dR2, 2));         // 3x1 matrix
//            vU[0] += mA[0][2];  vU[1] += mA[1][2];              // 3x1 matrix
//            // Error computation;
//            dError += _SQR(vU[0] - lmF[i][__X][j]) + _SQR(vU[1] - lmF[i][__Y][j]);
//            dError = sqrt(dError);

            vXc = lP[i].R() * mM.Column(j) + lP[i].t();         // 3x1 matrix
            vXn = (vXc / vXc[2]).To(1);                         // 2x1 matrix
            // Radial distortion
            dR2 = _SQR(vXn[0]) + _SQR(vXn[1]);                  // double
            vDr = vXn * (1.0 + dK1 * dR2 + dK2 * dR2 * dR2);  // 2x1 matrix
            // Final distortion
            vXc = vDr.Tailed(1.0);                              // 3x1 matrix
            // Image projection
            vU = mA * vXc;      vU /= vU[2];                    // 3x1 matrix
            // Error computation
            dError += (_SQR(vU[0] - lmF[i][__X][j]) + _SQR(vU[1] - lmF[i][__Y][j]));
        }
    }

    this->dError = dError;
    qDebug() << cnt++ << dError << "|" << vX[0] << vX[1] << vX[2] << vX[3] << vX[4] << vX[5];
    return dError;
}

double KCalibrationStereo::Erf(const KVector& vX) {
    const KList<KMatrix>& lFl = *_lmFl;
    const KList<KMatrix>& lFr = *_lmFr;
    const KMatrix& mM         = *_mM;

    KRotation rR;
    rR.FromRodrigues(vX[0], vX[1], vX[2]);
    KVector   vT(vX[3], vX[4], vX[5]);
    hLr = KHomogeneous(rR, vT);
    KHomogeneous hRl = ~hLr;

    double dError = 0.0;
    for (unsigned int i = 0; i < lFr.Count(); i++) {
        // For each images
        // Right image projection
        KMatrix mFr = msjrv::ImageProjection(mAr, vKr, hRl*_hLw[i], mM);     // 3 x nFeature
        KMatrix mFl = msjrv::ImageProjection(mAl, vKl, hLr*_hRw[i], mM);     // 3 x nFeature
        // (--, --, 1.0)
        mFr /= KMatrix::RepMat(mFr.RowVec(2), 3, 1);
        mFl /= KMatrix::RepMat(mFl.RowVec(2), 3, 1);
        // Error
        KMatrix mEr = lFr[i] - mFr;
        KMatrix mEl = lFl[i] - mFl;
        // 3 x nFeature
        // Element-wise square
        mEr ^= 2.0;
        mEl ^= 2.0;
        // Norm
        dError += (mEr.Sum().Sum() + mEl.Sum().Sum());
    }

    this->dError = dError;
    //qDebug() << cnt++ << dError << "|" << vX[0] << vX[1] << vX[2] << vX[3] << vX[4] << vX[5];
    return dError;
}

KMatrix make_3DCameraCoord (KVector& vX, KVector Img)
{
//    mA[0][0] = vX[0];   // alpha
//    mA[1][1] = vX[1];   // beta
//    mA[0][2] = vX[2];   // u0
//    mA[1][2] = vX[3];   // v0

    KMatrix Xi;
    for (int i = 0; i < Img.Size(); i++){

    Xi[0][i] = Img[i].dx - vX[2]; // u - ui
    Xi[1][i] = Img[i].dy - vX[3]; // v - vi
    Xi[2][i] = vX[0];
    Xi[3][i] = 1;

    }
    return Xi;

}
