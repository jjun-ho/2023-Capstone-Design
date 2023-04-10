#include "qViewsurround.h"
#include "linearTransform.h"
#include <QHash>

QViewSurround::QViewSurround()
{

}

QViewSurround::~QViewSurround()
{

}

void QViewSurround::Init(const QVIEWSURROUND_INFO& oInfo)
{
    //초기화 정보 저장    
    _oInfo.nHeightView     = oInfo.nHeightView;
    _oInfo.nWidthView      = oInfo.nWidthView;
    _oInfo.dCropRatio      = oInfo.dCropRatio;
    _oInfo.q_stBlendingLog = oInfo.q_stBlendingLog;
    _oInfo.q_stlHomography = oInfo.q_stlHomography;
    _oInfo.pShared         = oInfo.pShared;
    _oInfo.nOffsetS        = oInfo.nOffsetS;
    _oInfo.nOffsetF        = oInfo.nOffsetF;


    //Homography 파일 읽기
    if(_oInfo.q_stlHomography.size() != 0)
    {
        FILE*       fp;
        KMatrix     mTmp;

        _msHomography.RemoveAll();
        _msInvHomography.RemoveAll();

        for(int n=0; n<oInfo.q_stlHomography.size(); n++)
        {
            if((fp = fopen(oInfo.q_stlHomography.value(n).toStdString().c_str(),"rb")) == 0)  return;

            if(mTmp.Read(fp) == false){ fclose(fp);  return; }
            fclose(fp);

            _msHomography.Add(mTmp);
            _msInvHomography.Add(~mTmp);
        }
    }

    //블렌딩 정보 읽어오기        
    if(_oInfo.q_stBlendingLog.size() != 0)
    {        
        FILE* fb;
        if((fb=fopen(_oInfo.q_stBlendingLog.toStdString().c_str(),"rb")) == 0)   return;

        fread(&_oInfo.nWidthView,sizeof(int),1,fb);
        fread(&_oInfo.nHeightView,sizeof(int),1,fb);                

        if(_oInfo.nOffsetF != 0)
        {
             _oInfo.nHeightView = _oInfo.nOffsetF - _oInfo.nOffsetS + 1;

             if(_oInfo.pShared != 0)
                 _oInfo.pShared     = (void*)((KCOLOR32*)(_oInfo.pShared) + _oInfo.nWidthView * _oInfo.nOffsetS);
        }

        _arBlendingLog.Create(_oInfo.nHeightView, _oInfo.nWidthView);
        fseek(fb, sizeof(KBLENDINGLOG)*_oInfo.nOffsetS*_oInfo.nWidthView, SEEK_CUR);
        fread(_arBlendingLog.Address(),sizeof(KBLENDINGLOG),_arBlendingLog.Size(),fb);
        fclose(fb);

        //블렌딩 정보에서 비율 미리 계산해둠
        int         nIdx;
        KTRIPLET    oColor;
        double      dRatio;

        for(int r=0,rr=_oInfo.nHeightView; rr; r++,rr--)
            for(int c=0,cc=_oInfo.nWidthView; cc; c++,cc--)
            {
                KBLENDINGLOG& oLog = _arBlendingLog[r][c];
                int           nNum = (oLog.cpImg[0] >> 4);

                for(int i=0; i<nNum; i++)
                {
                    nIdx   = (oLog.cpImg[i] & 0x0F);
                    dRatio = (double)(oLog.spRn[i])*0.01;

                    for(int j=i+1; j<nNum; j++)
                        dRatio *= (double)(oLog.spRc[j])*0.01;

                    oLog.spRc[i] = (unsigned short)(dRatio*100);
                }
            }
    }
    else
        _arBlendingLog.Create(_oInfo.nHeightView, _oInfo.nWidthView); //블랜딩로그

    //출력영상 생성
    if(oInfo.pShared) //외부 메모리와 공유되는 경우
        _icComposite.Create(_oInfo.nHeightView, _oInfo.nWidthView, (KCOLOR32*)_oInfo.pShared,_LOCK);
    else
        _icComposite.Create(_oInfo.nHeightView, _oInfo.nWidthView);

    //출력영상의 폴리곤 설정
    _q_plgView << QPointF(0.0, 0.0) << QPointF(_oInfo.nWidthView-1.0, 0.0) << QPointF(_oInfo.nWidthView-1.0, _oInfo.nHeightView-1.0)
               << QPointF(0.0, _oInfo.nHeightView-1.0) << QPointF(0.0, 0.0);

}

KImageColor& QViewSurround::Run(const KImageColor* icpImgs)
{
    //KTRIPLET        oColor;
    KCOLOR32        oColor;
    double          dRatio;
    int             nNum,nIdx;

    //출력영상 초기화
    memset(_icComposite.Address(),0,sizeof(KCOLOR32)*_icComposite.Size());

    //Warping & Blending    
    for(int r=0,rr=_icComposite.Row(); rr; r++,rr--)
        for(int c=0,cc=_icComposite.Col(); cc; c++,cc--)
        {
            KBLENDINGLOG& oLog = _arBlendingLog[r][c];

            nNum = (oLog.cpImg[0] >> 4);

            for(int i=0; i<nNum; i++)
            {
                nIdx   = (oLog.cpImg[i] & 0x0F);
                oColor = icpImgs[nIdx][(int)(oLog.spVn[i]*0.1)][(int)(oLog.spUn[i]*0.1)];
                //oColor = icpImgs[nIdx].BilinearInterpolation(oLog.spUn[i]*0.1,oLog.spVn[i]*0.1);
                dRatio = oLog.spRc[i]*0.01;

                _icComposite[r][c].r += (unsigned char)(oColor.r * dRatio);
                _icComposite[r][c].g += (unsigned char)(oColor.g * dRatio);
                _icComposite[r][c].b += (unsigned char)(oColor.b * dRatio);
            }
        }

    return _icComposite;
}

KImageColor& QViewSurround::Run(const QStringList& q_stlImage)
{
    int             nNumImages = q_stlImage.size(), nBlendNum;
    QPolygonF       q_plgComposite, q_plgNew, q_plgOverap, q_plgTotal;
    QRectF          q_oRect;
    KMatrix         mProject;
    KVector         vUV;
    KCOLOR32        oColor32;
    KCOLOR24        oColor24;
    KTRIPLET        oColor;

    //블렌딩정보 배열 초기화
    KBLENDINGLOG* opBlendingLog = _arBlendingLog.Address();
    for(int i=0; i<_arBlendingLog.Size(); i++)
        opBlendingLog[i].cpImg[0] = 0; //해당 픽셀에서의 합성된 영상의 개수, 상위 4비트에 저장함

    //투영된 영상의 전체 영역 계산
    KImageColor24 	icImg = KDib(q_stlImage.value(0).toStdString().c_str()).ImageColor();
    for(int i=0; i<nNumImages; i++)
    {
        //영상 끝점의 투영점으로 구성된 폴리곤을 구함
        mProject  = _msInvHomography[i] * ( KVector(0.0,0.0,1.0) | KVector(icImg.Col()-1.0, 0.0, 1.0) |
                                           KVector(icImg.Col()-1.0, icImg.Row()-1.0, 1.0) | KVector(0.0, icImg.Row()-1.0, 1.0) |  KVector(0.0, 0.0, 1.0) ) ;
        q_plgNew.clear();
        for(int k=0; k<mProject.Col(); k++)
            q_plgNew << QPointF(mProject[0][k]/mProject[2][k], mProject[1][k]/mProject[2][k]);

        q_plgTotal = q_plgTotal.united(q_plgNew);
    }

    //출력영상의 변환설정
    QRect  q_Rect = q_plgTotal.boundingRect().toRect();
    KRect  rcWin  = KRect(q_Rect.left(),q_Rect.top(),q_Rect.right(),q_Rect.bottom()).Transform(_oInfo.dCropRatio,_CENTER);
    double dScale = _MAX((double)(rcWin.Height())/(double)(_icComposite.Row()), (double)(rcWin.Width())/(double)(_icComposite.Col()));


    //입력 영상의 순서대로 영상합성
    _icComposite.Create(_icComposite.Row(),_icComposite.Col());

    for(int i=0; i<nNumImages; i++)
    {
        //신규 영상을 읽어들임        
        KImageColor24  icNew = KDib(q_stlImage.value(i).toStdString().c_str()).ImageColor();

        //신규 영상 끝점의 투영점으로 구성된 폴리곤을 구함
        mProject  = _msInvHomography[i] * ( KVector(0.0,0.0,1.0) | KVector(icNew.Col()-1.0, 0.0, 1.0) |
                                           KVector(icNew.Col()-1.0, icNew.Row()-1.0, 1.0) | KVector(0.0, icNew.Row()-1.0, 1.0) |  KVector(0.0, 0.0, 1.0) ) ;
        q_plgNew.clear();
        for(int k=0; k<mProject.Col(); k++)
        {
            //2차원 영상좌표 계산
            QPointF q_ptImg(mProject[0][k]/mProject[2][k], mProject[1][k]/mProject[2][k]);

            //출력영상의 좌표로 변환하여 폴리곤 생성
            q_ptImg.rx() = (q_ptImg.rx() - rcWin._nLeft)/dScale;
            q_ptImg.ry() = (q_ptImg.ry() - rcWin._nTop)/dScale;
            q_plgNew << q_ptImg;
        }       
        q_plgNew = q_plgNew.intersected(_q_plgView); //출력영상 영역에 해당하는 영역만 구함

        //기존 composite polygon과 겹친 영역을 구함
        q_plgOverap = q_plgComposite.intersected(q_plgNew);

        assert(q_plgComposite.isEmpty() || !q_plgOverap.isEmpty( ));

        //겹치지 않은 영역에서의 합성영상        
        q_oRect = q_plgNew.subtracted(q_plgOverap).boundingRect();
        for(int r=q_oRect.top(); r<=q_oRect.bottom(); r++)
            for(int c=q_oRect.left(); c<=q_oRect.right(); c++)
            {
                QPointF pt((double)c, (double)r);
                if(!q_plgNew.containsPoint(pt, Qt::OddEvenFill) || q_plgOverap.containsPoint(pt, Qt::OddEvenFill))
                    continue;

                vUV = _msHomography[i] * KVector(c*dScale+rcWin._nLeft, r*dScale+rcWin._nTop, 1.0);
                if(vUV[2] == 0.0 || (_arBlendingLog[r][c].cpImg[0] >> 4))
                    continue;
                vUV /= vUV[2];                
                if(vUV[0] < 0.0 || vUV[1] < 0.0 || vUV[0] > icNew.Col()-1 || vUV[1] > icNew.Row()-1)
                    continue;

                oColor              = icNew.BilinearInterpolation(vUV[0],vUV[1]);
                oColor32.r          = (unsigned char)oColor.r; oColor32.g = (unsigned char)oColor.g; oColor32.b = (unsigned char)oColor.b;
                _icComposite[r][c]  = oColor32;

                //블렌딩 정보
                _arBlendingLog[r][c].cpImg[0]   = (unsigned char)i;
                _arBlendingLog[r][c].spUn[0]    = (unsigned short)(vUV[0]*10.);
                _arBlendingLog[r][c].spVn[0]    = (unsigned short)(vUV[1]*10.);
                _arBlendingLog[r][c].spRc[0]    = 0;
                _arBlendingLog[r][c].spRn[0]    = (unsigned short)(1.0*100);
                _arBlendingLog[r][c].cpImg[0]  += 0x10;
            }

        //폴리곤 합성 및 블렌딩
        QHash<void*, KPIXELBLENDED*>   q_Hash;
        KPtrList<KPIXELBLENDED*>       lpBound[2], lTemp(10000,false);
        int                            nIdx;
        KPIXELBLENDED*                 opPtBlended;
        KVector                        vD, vN, vA, vB, vMid;

        //경계점을 구함
        lpBound[_COMPOSITE_VIEW360].Create(10000,false); //for composite area
        lpBound[_NEW_VIEW360].Create(10000,false);       //for new area

        for(int v=0; v<q_plgOverap.size()-1; v++)
        {
            vD = KVector( q_plgOverap[v+1].x()-q_plgOverap[v].x(), q_plgOverap[v+1].y()-q_plgOverap[v].y()).Normalized(_SIZE_NORMALIZE);
            vN.Create(-vD[1], vD[0]);
            vMid.Create((q_plgOverap[v+1].x()+q_plgOverap[v].x())*0.5, (q_plgOverap[v+1].y()+q_plgOverap[v].y())*0.5);

            vA          = vMid + vN*0.3;
            vB          = vMid - vN*0.3;
            nIdx        = (q_plgComposite.containsPoint(QPointF(vA[0],vA[1]),Qt::OddEvenFill) &&
                           q_plgComposite.containsPoint(QPointF(vB[0],vB[1]),Qt::OddEvenFill) ? _COMPOSITE_VIEW360 : _NEW_VIEW360);

            //겹침영역 경계점 정보를 hash 테이블에 넣음
            double  dLength = sqrt(_SQR(q_plgOverap[v].x() - q_plgOverap[v+1].x()) +  _SQR(q_plgOverap[v].y() - q_plgOverap[v+1].y()));
            KVector vStart(q_plgOverap[v].x(), q_plgOverap[v].y()), vProbe;
            KPOINT  oProbe;

            for(double d=0.0; d<=dLength; d+= 0.5)
            {
                vProbe      = vStart + vD*d;
                oProbe.nX   = floor(vProbe[0]);
                oProbe.nY   = floor(vProbe[1]);                
                vUV         = _msHomography[i] * KVector(oProbe.nX*dScale+rcWin._nLeft, oProbe.nY*dScale+rcWin._nTop, 1.0);

                if(oProbe.nX < 0 || oProbe.nY < 0)
                    continue;

                assert(vUV[2] != 0.0);

                vUV     /= vUV[2];
                vUV[0]  = (vUV[0] < 0.0 ? 0.0 : (vUV[0]>icNew.Col()-1.0 ? icNew.Col()-1.0 : vUV[0]));
                vUV[1]  = (vUV[1] < 0.0 ? 0.0 : (vUV[1]>icNew.Row()-1.0 ? icNew.Row()-1.0 : vUV[1]));

                oColor      = icNew.BilinearInterpolation(vUV[0],vUV[1]);
                oColor24.r  = (unsigned char)oColor.r;
                oColor24.g  = (unsigned char)oColor.g;
                oColor24.b  = (unsigned char)oColor.b;

                opPtBlended                                 = new KPIXELBLENDED;
                opPtBlended->oColorNew                      = oColor24;
                opPtBlended->oColorComposite                = _icComposite[oProbe.nY][oProbe.nX];
                opPtBlended->nUc                            = oProbe.nX;
                opPtBlended->nVc                            = oProbe.nY;
                opPtBlended->fUn                            = vUV[0];
                opPtBlended->fVn                            = vUV[1];
                opPtBlended->npDistFrom[_COMPOSITE_VIEW360] = (nIdx ? 100000 : 0);
                opPtBlended->npDistFrom[_NEW_VIEW360]       = (nIdx ? 0 : 100000);

                if(!q_Hash.contains(&_icComposite[oProbe.nY][oProbe.nX]))
                {
                    q_Hash.insert(&_icComposite[oProbe.nY][oProbe.nX], opPtBlended);
                    lpBound[nIdx].Add(opPtBlended);
                }
            }
        }

        //Grass Fire Algorithm 적용

        //composite 경계
        while(lpBound[_COMPOSITE_VIEW360].Count())
        {
            for(int n=0, nn=lpBound[_COMPOSITE_VIEW360].Count(); nn; n++, nn--)
            {
                for(int r=lpBound[_COMPOSITE_VIEW360][n]->nVc-1; r<=lpBound[_COMPOSITE_VIEW360][n]->nVc+1; r++)
                    for(int c=lpBound[_COMPOSITE_VIEW360][n]->nUc-1; c<=lpBound[_COMPOSITE_VIEW360][n]->nUc+1; c++)
                    {                        
                        //겹침영역에 있는지 확인
                        if(!q_plgOverap.containsPoint(QPointF((double)c, (double)r), Qt::OddEvenFill)) continue;


                        if(!q_Hash.contains(&_icComposite[r][c]))
                        {
                            vUV = _msHomography[i] * KVector(c*dScale+rcWin._nLeft, r*dScale+rcWin._nTop,1.0);
                            vUV /= vUV[2];

                            vUV[0]  = (vUV[0] < 0.0 ? 0.0 : (vUV[0]>icNew.Col()-1.0 ? icNew.Col()-1.0 : vUV[0]));
                            vUV[1]  = (vUV[1] < 0.0 ? 0.0 : (vUV[1]>icNew.Row()-1.0 ? icNew.Row()-1.0 : vUV[1]));

                            oColor      = icNew.BilinearInterpolation(vUV[0],vUV[1]);
                            oColor24.r  = (unsigned char)oColor.r;
                            oColor24.g  = (unsigned char)oColor.g;
                            oColor24.b  = (unsigned char)oColor.b;

                            opPtBlended                     = new KPIXELBLENDED;
                            opPtBlended->oColorNew          = oColor24;
                            opPtBlended->oColorComposite    = _icComposite[r][c];
                            opPtBlended->nUc                = c;
                            opPtBlended->nVc                = r;
                            opPtBlended->fUn                = vUV[0];
                            opPtBlended->fVn                = vUV[1];
                            opPtBlended->npDistFrom[_COMPOSITE_VIEW360] = lpBound[_COMPOSITE_VIEW360][n]->npDistFrom[_COMPOSITE_VIEW360] + 1;
                            opPtBlended->npDistFrom[_NEW_VIEW360]       = 100000;

                            q_Hash.insert(&_icComposite[r][c], opPtBlended);
                            lTemp.Add(opPtBlended);
                        }
                    }
                }
                lpBound[_COMPOSITE_VIEW360] = lTemp;
                lTemp.RemoveAll();
        }

        //new 경계
        while(lpBound[_NEW_VIEW360].Count())
        {
            for(int n=0, nn=lpBound[_NEW_VIEW360].Count(); nn; n++, nn--)
            {
                for(int r=lpBound[_NEW_VIEW360][n]->nVc-1; r<=lpBound[_NEW_VIEW360][n]->nVc+1; r++)
                    for(int c=lpBound[_NEW_VIEW360][n]->nUc-1; c<=lpBound[_NEW_VIEW360][n]->nUc+1; c++)
                    {
                        //겹침영역에 있는지 확인
                        if(!q_plgOverap.containsPoint(QPointF((double)c, (double)r), Qt::OddEvenFill)) continue;
                        if(!q_Hash.contains(&_icComposite[r][c])) continue;

                        opPtBlended = q_Hash.value(&_icComposite[r][c]);
                        if(opPtBlended->npDistFrom[_NEW_VIEW360] > lpBound[_NEW_VIEW360][n]->npDistFrom[_NEW_VIEW360] + 1)
                        {
                            opPtBlended->npDistFrom[_NEW_VIEW360] = lpBound[_NEW_VIEW360][n]->npDistFrom[_NEW_VIEW360] + 1;
                            lTemp.Add(opPtBlended);
                        }
                    }
            }

            lpBound[_NEW_VIEW360] = lTemp;
            lTemp.RemoveAll();
        }


        QHash<void*, KPIXELBLENDED*>::iterator itr;
        for(itr = q_Hash.begin(); itr != q_Hash.end(); itr++)
        {
            KPIXELBLENDED*  p       = itr.value();
            double          dRatioC = (double)(p->npDistFrom[1]) / (double)(p->npDistFrom[0] + p->npDistFrom[1]);
            double          dRatioN = (double)(p->npDistFrom[0] )/ (double)(p->npDistFrom[0] + p->npDistFrom[1]);

            _icComposite[p->nVc][p->nUc].b  = (unsigned char)(dRatioC * p->oColorComposite.b + dRatioN * p->oColorNew.b);
            _icComposite[p->nVc][p->nUc].g  = (unsigned char)(dRatioC * p->oColorComposite.g + dRatioN * p->oColorNew.g);
            _icComposite[p->nVc][p->nUc].r  = (unsigned char)(dRatioC * p->oColorComposite.r + dRatioN * p->oColorNew.r);

            //블렌딩 정보
            nBlendNum = (_arBlendingLog[p->nVc][p->nUc].cpImg[0] >> 4);
            if(nBlendNum >= _MAX_COMPOSITE_VIEW360)
                int kk=0;

            _arBlendingLog[p->nVc][p->nUc].cpImg[nBlendNum]  &= 0xF0;
            _arBlendingLog[p->nVc][p->nUc].cpImg[nBlendNum]  |= (unsigned char)(i);
            _arBlendingLog[p->nVc][p->nUc].spUn[nBlendNum]    = (unsigned short)(p->fUn*10.);
            _arBlendingLog[p->nVc][p->nUc].spVn[nBlendNum]    = (unsigned short)(p->fVn*10.);
            _arBlendingLog[p->nVc][p->nUc].spRc[nBlendNum]    = (unsigned short)(dRatioC*100.0);
            _arBlendingLog[p->nVc][p->nUc].spRn[nBlendNum]    = 100 - (unsigned short)(dRatioC*100.0);
            _arBlendingLog[p->nVc][p->nUc].cpImg[0]          += 0x10;
        }

        q_plgComposite = q_plgComposite.united(q_plgNew);

    }


    //블렌딩 정보 저장
    FILE*   fb;
    KString stPath = KString(q_stlImage.value(0).toStdString().c_str()).FilePathOnly();

    if((fb=fopen(stPath + "/blendingLog.bin","wb")) != 0)
    {
        fwrite(&_oInfo.nWidthView,sizeof(int),1,fb);
        fwrite(&_oInfo.nHeightView,sizeof(int),1,fb);
        fwrite(_arBlendingLog.Address(), sizeof(KBLENDINGLOG), _arBlendingLog.Size(),fb);
        fclose(fb);
    }

    //합성영상 반환
    return _icComposite;
}


void QViewSurround::ConstructHomographies(const QStringList& q_stlModel, const QStringList& q_stlFeature)
{
    //homography 계산
    FILE* 			    fp;
    int     			nX,nY;
    KPoint  			ptTmp;
    KPoints 			ptsModel,ptsFeature;
    KLinearTransform    oLT;
    KString             stPath = KString(q_stlModel.value(0).toStdString().c_str()).FilePathOnly();

    for(int n=0; n<q_stlModel.size(); n++)
    {
        //모델 데이터일 읽어들임
        if((fp = fopen(q_stlModel.value(n).toStdString().c_str(),"rt")) == 0)
            return;

        ptsModel.clear();
        while(fscanf(fp,"%d %d\n", &nX, &nY) == 2)
            ptsModel.push_back(KPoint((double)nX,(double)nY));
        fclose(fp);

        //모델에 대응하는 영상의 특징점 데이터 읽어들임
        if((fp = fopen(q_stlFeature.value(n).toStdString().c_str(),"rt")) == 0)
            return;
        ptsFeature.clear();
        while(fscanf(fp,"%lf %lf\n", &ptTmp._dX,&ptTmp._dY) == 2)
            ptsFeature.push_back(ptTmp);
        fclose(fp);

        //Homography 계산
        KMatrix mH = oLT.Homography(ptsFeature,ptsModel,true);
        fp = fopen(stPath + KString().Format("/homography%0d.mtx",n),"wb");
        mH.Write(fp);
        fclose(fp);
    }
}
