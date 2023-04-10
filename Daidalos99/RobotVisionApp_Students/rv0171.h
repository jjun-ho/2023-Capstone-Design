#ifndef RV0171_H
#define RV0171_H

#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <fstream>

#include "kfc.h"
#include "QDebug"

#define __X 0
#define __Y 1

#define __PI 3.141592
#define __2PI 6.283184

#define __MonitorWidth 1920
#define __MonitorHeight 1080

using namespace std;

namespace rv0171
{
    // Read List of Points(x, y) from .txt file
    vector<pair<double, double>> PointList(string location);

    // Zhang's Method (HW1)
    pair<double, double> Normalization(double dX, double dY, KMatrix mat);
    KMatrix NormalizeCoordinates(KMatrix& M, KMatrix& normalized_M);
    KVector MakevV(KMatrix mH, int i, int j);
    KVector ZhangsCalibration(vector<vector<pair<double, double>>> pointzip, int nImg, int nFeature, int iter = 500);

    // Stereo Camera Calibration (HW2)
    KVector StereoCalibration(int nImg, int nFeature, KVector vXl, KVector vXr, KPoint**& psFl, KPoint**& psFr, KPoint*& psM);
    KMatrix ImageProjection(KMatrix mA, KVector vK, KHomogeneous hP, KMatrix mM);

    // AdaBoost


}

// =========================================================================

// Camera Calibration(HW1)
class KCalibrationZhang : public KOptima
{
public:
    KMatrix mM;         // model world coordinate
    KMatrix mF[15];      // feature image coordinate

    int nImg;
    int nFeature;

    int cnt = 0;
    double dError = 0;

protected:
    virtual double Erf(const KVector& vX);
};

// =========================================================================

// Stereo Camera Calibration(HW2)
class KCalibrationStereo : public KOptima
{
public:
    KMatrix         mAl, mAr;
    // Homogeneous Transform(Right Cam to World, Left Cam to World,  Left Cam to Right)
    KHomogeneous    hLr;   // Xr = hRw * Xw, xl Xl = hLr * Xl
    KHomogeneous*   _hLw;
    KHomogeneous*   _hRw;
    KVector         vKl, vKr;

    // temepral
    KList<KMatrix>*     _lpFl;  // Left Cam Feature Points
    KList<KMatrix>*     _lpFr;  // Right Cam Feature Points
    KMatrix*            _mpM;   // Model points

    double dError = 0;
    int cnt = 0;

    KMatrix ImageProjection(KMatrix mA, KVector vK, KHomogeneous hP, KMatrix mM);

public:
    KCalibrationStereo(){}
    //bool Run(const KList<KMatrix>* lpFl, const KList<KMatrix>* lpFr, const KMatrix* mpM) { }

protected:
    // Powell
    void UpdateParameters(const KVector& vX);
    virtual double Erf(const KVector& vX);
};

// =========================================================================

// AdaBoost(HW3)
// Return '_nAxis'th Col Vector of FeatureCoord
class KFeatureCoord
{
    int _nAxis;

public:
    KFeatureCoord(int axis = _X_AXIS) : _nAxis(axis){ }
    double operator()(const KVector& vX){ return vX[_nAxis]; }
    KVector operator()(vector<KVector>& lX)
    {
        KVector vValue(lX.size());
        for(unsigned int i = 0; i < lX.size(); i++){
            vValue[i] = lX[i][_nAxis];
        }

        return vValue;
    }
};

class KWeakClassifierCoord   // function object
{
    KFeatureCoord   _F;
    double          _dThresh;
    int             _nPolar;
    double          _dAlpha;

public:
    KWeakClassifierCoord(){ };
    int     operator()(const KVector& vX){ return (_F(vX)*_nPolar > _dThresh*_nPolar ? +1 : -1); }
    KVector operator()(vector<KVector>& lX);

    double  OptimalThreshold(const KVector &vGap, const KVector &vY, const KVector &vW);
    double  Train(vector<KVector> &lX, const KVector &vY, const KVector &vW, vector<KFeatureCoord> &lF);
};

class KStrongClassifierCoord : public vector<KWeakClassifierCoord>
{
    KVector _vAlpha;

public:
    KStrongClassifierCoord() { };
    int     operator()(const KVector& vX){ return Classify(vX); };
    KVector operator()(vector<KVector>& lX);

    double  Train(vector<KVector> &lX, const KVector &vY, vector<KFeatureCoord> &lF, int nWeak=10, double dMinError=0.05);
    int     Classify(const KVector& vX);
    KVector Classify(vector<KVector>& lX);
};

#endif // RV0171_H
