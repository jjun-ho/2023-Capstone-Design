#ifndef QVIEWSURROUND_H
#define QVIEWSURROUND_H

#include <QObject>
#include <QStringList>
#include <QPolygonF>
#include "kfc.h"

#define     _COMPOSITE_VIEW360      0
#define     _NEW_VIEW360            1
#define     _MAX_COMPOSITE_VIEW360  6

/**
 * @brief KPOINTBLENDED struct.
 * 기본 영상과 이것의 영상면에 투영된 다른 영상이 합성될 때 대응하는 픽셀의 합성정보를 저장하는
 * 구조체이다.
 */
struct KPIXELBLENDED
{
    int         nUc;             /**< 기본 영상점 X 좌표 */
    int         nVc;             /**< 기본 영상점 Y 좌표 */
    float       fUn;             /**< 기본 영상점 좌표에 대응되는 투영 영상의 원래 X 좌표 */
    float       fVn;             /**< 기본 영상점 좌표에 대응되는 투영 영상의 원래 Y 좌표 */
    int         npDistFrom[2];  /**< 중첩영역의 기본 영상점 좌표에서 기본 영상의 경계로부터의 최단거리 및 투영 영상의 경계로부터의 최단거리 */
    KCOLOR24    oColorNew;      /**< 대응되는 투영 영상점의 컬러 값 */
    KCOLOR32    oColorComposite;/**< 대응되는 기본 영상점의 컬러 값 */

};

/**
 * @brief KBLENDINFO struct.
 * 기본 영상 각 픽셀에서의 합성이력을 저장하는 구조체. 이 정보를 활용하면 고속 영상합성이 가능하고, GPU 기반 병렬계산에 직접적으로 적용할 수 있다.
 */
struct KBLENDINGLOG
{
    unsigned char   cpImg[_MAX_COMPOSITE_VIEW360];     /**< 합성된 영상의 ID를 저장하는 배열.
                                                          * 배열의 첫 요소는 상위 4비트에 합성된 영상의 개수를 저장함
                                                       */
    unsigned short  spUn[_MAX_COMPOSITE_VIEW360];      /**< 해당 픽셀에서 순차적으로 합성될 때마다의 투영영상 X 좌표(10배율)를 저장하는 배열 */
    unsigned short  spVn[_MAX_COMPOSITE_VIEW360];      /**< 해당 픽셀에서 순차적으로 합성될 때마다의 투영영상 X 좌표(10배율)를 저장하는 배열 */
    unsigned short  spRc[_MAX_COMPOSITE_VIEW360];      /**< 해당 픽셀에서 순차적으로 합성될 때마다의 기본영상의 합성비율(10배율) */
    unsigned short  spRn[_MAX_COMPOSITE_VIEW360];      /**< 해당 픽셀에서 순차적으로 합성될 때마다의 투영영상의 합성비율(10배율) */
};

/**
 * @brief QVIEWSURROUNDINFO struct.
 * QViewSurround 클래스의 초기화 정보 구조체로서 Init(...) 함수의 인수로 사용된다.
 */
struct QVIEWSURROUND_INFO
{
    QStringList q_stlHomography;            /**< 호모그래피 행렬의 파일이름 리스트  */
    QString     q_stBlendingLog;            /**< 픽셀당 영상합성 이력을 저장한 파일의 이름  */
    double      dCropRatio;                 /**< 합성영상에서 출력영상을 위해  중심점 기준하여 Cropping 비율 */
    int         nWidthView;                 /**< 출력 영상의 폭  */
    int         nHeightView;                /**< 출력 영상의 높이 */
    void*       pShared;                    /**< 출력 영상(Run(..) 함수의 반환값)과 공유되는 메모리 포인터 */    
    int         nOffsetS;                   /**< 출력 영상의 첫 행 번호(기본값은 0) */
    int         nOffsetF;                   /**< 출력 영상의 마지막 행 번호(기본값은 0) */
};


/**
 * @brief QViewSurround class.
 * 여러 영상을 기본 영상면에 투영하여 합성하는 객체로서 각 영상을 투영하기 위한 호모그래피 행렬을
 * 구하는 부분을 포함하고 있고, Run(...) 함수를 통해 기본 영상면에서의 최종 합성영상과 각 픽셀의 합성이력을 이진파일로 출력한다.
 */
class QViewSurround
{
    QVIEWSURROUND_INFO          _oInfo;            /**< 객체 초기화 정보 구조체 */
    KMatrices                   _msHomography;     /**< 호모그래피 행렬 리스트 */
    KMatrices                   _msInvHomography;  /**< 호모그래피 역행렬 리스트 */
    KImageColor                 _icComposite;      /**< 출력영상 */
    QPolygonF                   _q_plgView;        /**< 출력영상의 폴리곤 */
    KArrayObject<KBLENDINGLOG>  _arBlendingLog;    /**< 출력영상의 합성정보를 저장한 2차원 배열 */

    //Stiching & Blending
public:
    /** 클래스 생성자 함수. */
    QViewSurround();
    /** 초기화 정보 구조체를 인수로 하는 클래스 생성자 함수. */
    QViewSurround(const QVIEWSURROUND_INFO& oInfo){ Init(oInfo); }
    /** 클래스 소멸자 함수. */
    ~QViewSurround();

    /**초기화 정보 구조체 반환 함수. */
    QVIEWSURROUND_INFO& Info(){ return _oInfo; }

    /** 클래스 초기화 함수. */
    void                Init(const QVIEWSURROUND_INFO& oInfo /**< 클래스 초기화 정보 구조체 */);


    /**
     * Stitching & Blending을 통한 복수 영상의 합성함수.
     * @return 최종 합성된 영상
     */
    KImageColor&        Run(const QStringList& q_stImages   /**< 합성될 영상의 파일이름 리스트 */);
    /**
     * Stitching & Blending을 통한 복수 영상의 고속 합성함수.
     * 기본 영상면에서 미리 구한 각 픽셀의 합성정보를 이용하여 고속으로 Stitching & Blending을 수행한다.
     * @return 최종 합성된 영상
     */
    KImageColor&        Run(const KImageColor* icpImages    /**< 합성될 영상객체의 배열 포인터 */);

    /**
     * Homography 행렬을 계산하는 Static 함수.
     * 합성될 각 영상의 Homography 행렬을 구하고 "*.mtx" 파일들을 출력한다.
     * 이 파일 리스트는 QVIEWSURROUNDINFO 구조체의 맴버가 된다.
     */
    static void         ConstructHomographies(const QStringList& q_stlModel,    /**< 기본 영상면에서의 특징점 파일 리스트 */
                                              const QStringList& q_stlFeature   /**< 기본 영상면의 특징점과 대응되는 영상의 특징점 파일 리스트 */
                                              );
};

#endif // QVIEWSURROUND_H
