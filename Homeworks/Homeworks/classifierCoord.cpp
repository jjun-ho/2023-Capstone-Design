#include <iostream>

#include "classifierCoord.h"


using namespace std;

double KStrongClassifierCoord::Train(const KList<KVector>& lX, const KVector& vY, const KList<KFeatureCoord>& lF,
                                   int nTmax, double dMinError)
{
    static int           nID;
    double               dError = 1000.0, dAlpha;
    KVector              vError;
    int                  nNum = lX.Count();
    KVector              vH, vW = KMatrix::Ones(nNum,1);



    KWeakClassifierCoord oClassifier;
/*
    //
    int     nSum    = vY.Sum();
    int     nPlus   = (vY.Dim() - nSum)/2 + nSum; //number of positive samples
    int     nMinus  = (vY.Dim() - nSum)/2;        //number of negative samples
    double  dFPR    = 1000.0;                     //False Positive Ration
    KVector vFPR;
*/

    _vAlpha.Release();
    vW.Normalized(_UNITSUM_NORMALIZE);


    for(int i=0; i<nTmax && (dError > dMinError); i++)
    {
        dError = oClassifier.Train(lX, vY, vW, lF);

        //cout << i << ": " << dError << endl;

        //the weight of the classifier
        dAlpha = 0.5*log((1.0-dError) / dError);
        _vAlpha.Tailed(dAlpha);

        //add the weak classifier
        KList<KWeakClassifierCoord>::Add(oClassifier);

        //update the weight of each misclassified sample
        for(int n=0; n<nNum; n++)
            vW[n] *= exp(-dAlpha*vY[n]*oClassifier(lX[n]));
        vW.Normalized(_UNITSUM_NORMALIZE);
        //validation
        vH  = Classify(lX); //recommend to use validation samples instead of the training samples lX
        dError = 0.5 - 0.5 * (vY & vH) / (double)(vY.Dim());
        vError.Tailed(dError);
/*
        dFPR= ((vH.Dim() - vH.Sum()) / 2 + vH.Sum() - nPlus) / (double)(nMinus);
        vFPR.Tail(dFPR);
*/
    }

    //vError.WriteText(KString::Format("./output/error_in_strongclassifier_train_%02d.txt",nID).Address());
//    for (unsigned int i = 0; i < vError.Size(); i++) {
//        qDebug() << vError[i];
//    }
    return vError.Min();
}


KVector KWeakClassifierCoord::operator()(const KList<KVector>& lX)
{
    KVector vYp(lX.Count());

    for(int i=0, ii=lX.Count(); ii; i++, ii--)
        vYp[i] = (*this)(lX[i]);

    return vYp;
}

int KStrongClassifierCoord::Classify(const KVector& vX)
{
    int     nNum = KList<KWeakClassifierCoord>::Count();
    double  dH   = 0.0;

    for(int i=0, ii=nNum; ii; i++, ii--)
        dH += _vAlpha[i] * (*this)[i](vX);

    return _SIGN(dH);
}

KVector KStrongClassifierCoord::Classify(const KList<KVector>& lX)
{
    KVector vOut(lX.Count());

    for(int n=0, nn=lX.Count(); nn; n++, nn--)
        vOut[n] = (*this)( lX[n] );

    return vOut;
}

double KWeakClassifierCoord::Train(const KList<KVector>& lX, const KVector& vY,
                                   const KVector& vW, const KList<KFeatureCoord>& lF)
{
    KVector vGap;
    double  dThresh, dError, dMin = 1000.0;
    int     nPolar;


    for(int i=0, ii=lF.Count(); ii; i++, ii--)
    {
        vGap    = lF[i](lX);    // 첫번째 for문: x1열, 2번째 for문: x2열
        dThresh = OptimalThreshold(vGap, vY, vW);
        //cout << dThresh << endl;


        //오차 계산
        dError  = 0.0;
        nPolar  = (dThresh > 0 ? +1 : -1);
        dThresh = (dThresh > 0 ? dThresh : -dThresh);

        for(int i=0, ii=lX.Count(); ii; i++, ii--)
        {         
            dError += vW[i] * vY[i] * (vGap[i]*nPolar > dThresh*nPolar ? +1 : -1);
        }
        dError = 0.5 - 0.5*dError;

        //최소 오차의 Feature와 파라미터 설정
        if(dError < dMin)
        {
            dMin     = dError;
            _dThresh = dThresh;
            _nPolar  = nPolar;
            _F       = lF[i];
        }
    }

    return dMin;
}


double KWeakClassifierCoord::OptimalThreshold(const KVector& vGap, const KVector& vY, const KVector& vW)
{
    //특징값에 ㄸㅏ라 오름차순 정렬
    KVector     vGs(vGap), vYs(vY), vWs(vW);
    vector<int> lIdx;

    vGs.QuickSorted(&lIdx);
    vYs.SortedBy(lIdx);
    vWs.SortedBy(lIdx);


    //인덱스에 ㄸㅏ른 누적합 계산
    int     nNum = vYs.Dim();
    KVector vCp(nNum), vCm(nNum);
    double  dTp  = 0.0, dTm  = 0.0;

    for(int i=0, ii=nNum; ii; i++, ii--)
    {
        if(vYs[i] > 0) dTp += vWs[i];
        else           dTm += vWs[i];

        vCp[i] = dTp;
        vCm[i] = dTm;
    }

    //최적 threshold 값
    double dThresh, dEp, dEm, dMin = 1000.0;

    for(int i=0, ii=nNum-1; ii; i++, ii--)
    {
        dEp = dTm - vCm[i] + vCp[i];
        dEm = vCm[i] + dTp - vCp[i];   

        if(dEp < dMin)
        {
            dMin = dEp;
            dThresh = (vGs[i] + vGs[i+1]) * 0.5;
        }
        if(dEm < dMin)
        {
            dMin = dEm;
            dThresh = -(vGs[i] + vGs[i+1]) * 0.5; // 극성을 부호로 포함시킴
        }
    }


    return dThresh;
}

