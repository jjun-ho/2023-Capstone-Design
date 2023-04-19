#ifndef MSJRV_H
#define MSJRV_H

#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <fstream>

#include "kfc.h"
#include "classifierCoord.h"
#include "linearTransform.h"
#include "time.h"
#include "QDebug"
#include <QPainter>

#define __X 0
#define __Y 1

#define __L 0
#define __R 1

#define __PI 3.141592
#define __2PI 6.283185307179586477

#define __MonitorWidth 1920
#define __MonitorHeight 1080

using namespace std;

namespace msjrv {
    KVector ZhangMethod(int, int, KPoint**&, KPoint*&);
    KMatrix Normalization(KMatrix);
    KVector MakeVVector(int, int, KMatrix);

    KVector StereoCalibration(int, int, KVector, KVector, KPoint**&, KPoint**&, KPoint*&);
    KVector PointProjection(KMatrix, KVector, KHomogeneous, KVector);
    KMatrix ImageProjection(KMatrix, KVector, KHomogeneous, KMatrix);

    KVector OpticalCenter(KMatrix);
    KVector BilinearInterpolation(QImage*, int, int, KMatrix, KVector);

    KVector AdaBoost(int, int, double**&, int);
};

class KCalibrationZhang: public KOptima {
public:
    KMatrix mM;         // model world coordinate
    KList<KMatrix> lmF; // feature image coordinate

    int nImg;
    int nFeature;

    int cnt = 0;
    double dError = 0;

protected:
    virtual double Erf(const KVector& vX);
};

class KCalibrationStereo: public KOptima {
public:
    KMatrix mAl, mAr;
    KHomogeneous hLr;
    KHomogeneous* _hLw;
    KHomogeneous* _hRw;
    KVector vKl, vKr;

    KList<KMatrix>* _lmFl;
    KList<KMatrix>* _lmFr;
    KMatrix*        _mM;

    int cnt = 0;
    double dError;

protected:
    virtual double Erf(const KVector& vX);
};

#endif // MSJRV_H
