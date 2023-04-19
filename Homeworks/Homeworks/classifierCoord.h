#ifndef KCLASSIFIERCOORD_H
#define KCLASSIFIERCOORD_H

#include "featureCoord.h"
#include "QDebug"

class KWeakClassifierCoord
{
    KFeatureCoord   _F;         //feature
    double          _dThresh;   //threshold    +1 if F(input)*nPolar > dThresh*nPolar
    int             _nPolar;    //weight       -1 otherwise
    double          _dAlpha;

public:
    KWeakClassifierCoord(){ };
    int     operator()(const KVector& vX){ return (_F(vX)*_nPolar > _dThresh*_nPolar ? +1 : -1); }
    KVector operator()(const KList<KVector>& lX);

    double  Train(const KList<KVector>& lX, const KVector& vY, const KVector& vW, const KList<KFeatureCoord>& lF);
    double  OptimalThreshold(const KVector& vGap, const KVector& vY, const KVector& vW);

};


class KStrongClassifierCoord : public KList<KWeakClassifierCoord>
{
    KVector     _vAlpha;

public:
    KStrongClassifierCoord(){ };
    int     operator()(const KVector& vX){ return Classify(vX); };
    KVector operator()(const KList<KVector>& lX){ return Classify(lX); }

    double  Train(const KList<KVector>& lX, const KVector& vY, const KList<KFeatureCoord>& lF, int nTmax=10, double dMinError=0.05);
    int     Classify(const KVector& vX);
    KVector Classify(const KList<KVector>& lX);

};

#endif // KWEAKCLASSIFIERCOORD_H
