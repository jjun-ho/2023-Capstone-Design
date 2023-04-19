#ifndef FEATURECOORD_H
#define FEATURECOORD_H

#include "kfc.h"

class KFeatureCoord
{
    int _nAxis;

public:
    KFeatureCoord(int axis = _X_AXIS) : _nAxis(axis){ } //_X_AXIS=0, _Y_AXIS=1
    double  operator()(const KVector& vX){  return vX[_nAxis]; }
    KVector operator()(const KList<KVector>& lX)
    {
        KVector vValue(lX.Count());
        for(int i=0, ii=lX.Count(); ii; i++, ii--)
            vValue[i]   = lX[i][_nAxis];
        return vValue;
    }
};


#endif // FEATURECOORD_H

