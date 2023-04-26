//---------------------------------------------------------------------------

#ifndef linearTransformH
#define linearTransformH

#include "kfc.h"

#define _OPTIMA_HOMOGRAPHY      1
#define _OPTIMA_PLANETOIMAGE	2

/**
 * @brief KLinearTransform class.
 * 대응되는 2차원 혹은 3차원 정보로부터 변환행렬을 구하는 여러가지
 * 방법을 구현하다. KOptima 클래스를 상속했기 때문에 변환행렬의 비선형 최적화가 가능하다.
 */
class KLinearTransform : public KOptima
{
    int 		_nOptima;   /*< 비선형 최적화를 하는 대상을 나타냄 */
    KMatrix		_mK;        /*< 카메라 내부행렬 */

	//계산용
    KMatrix		_mA;        /*< 계산을 위한 임시 버퍼 */
    KMatrix  	_mB;        /*< 계산을 위한 임시 버퍼 */

public:
    /** 클래스 생성자 함수. */
	KLinearTransform(){}
    /** 클래스 소멸자 함수. */
	virtual ~KLinearTransform(){}

    KVector                LinearEquation(const KMatrix& mA, const KVector& vB);

    /*
     * @brief AX = XB에 대한 Tsai 기법 구현함수.
     * @see 관련논문: R.Y. Tsai and R.K. Lenz, "a new technique for fully autonomous and efficient 3d robotics hand/eye calibration
     * @return Tsai 방법으로 구한 Homogeneous Transform(4X4 행렬), Tc_b (Xc = Tc_b * Xb)
     */
    KMatrix                 ByTsai(const KMatrices& msA,   /*< 로봇운동으로 인한 카메라 프레임 사이의 변환행렬 리스트 */
                                   const KMatrices& msB    /*< 로봇운동으로 인한 End-Effector 프레임 사이의 변환행렬 리스트 */
                                   );
    KMatrix                 ByAndreff(const KMatrices& msA,/*< 로봇운동으로 인한 카메라 프레임 사이의 변환행렬 리스트 */
                                      const KMatrices& msB /*< 로봇운동으로 인한 End-Effector 프레임 사이의 변환행렬 리스트 */
                                      );
    KMatrix                 ByLiang(const KMatrices& msA,  /*< 로봇운동으로 인한 카메라 프레임 사이의 변환행렬 리스트 */
                                    const KMatrices& msB   /*< 로봇운동으로 인한 End-Effector 프레임 사이의 변환행렬 리스트 */
                                    );
    void                    ByLinearClosedForm(int nNum, KMatrix* mpA, KMatrix* mpB, KMatrix& mX){ }
    void                    ByDualQuaternion(int nNum, KMatrix* mpA, KMatrix* mpB, KMatrix& mX){ }

    /*
     * @brief 이차원 영상의 대응점으로부터 Homography 행렬을 구하는 함수.
     * @return A = H * B를 만족하는 Homography 행렬 H
     */
    KMatrix                 Homography(const KPoints& psA,  /*< 대응되는 특징점 리스트 A*/
                                       const KPoints& psB,  /*< 대응되는 특징점 리스트 B*/
                                       bool bOptima         /*< 비선형 최적화 수행여부 */
                                       );
    KMatrix                 Homography(const KMatrix& mF, const KMatrix& mM, bool bOptima);
    /*
     * @brief 3차원 데이터 기반 변환행렬 계산 함수.
     * 두 좌표계(A, B)에서 대응되는 3차원 점으로부터 두 좌표계 사이의 변환행렬 계산. 즉, B = H * A를 만족하는 H(4X4) 행렬을 구한다.
     * @see 관련논문: B.K.P. Horn, "closed-form solution of absolute orientation using unit quaternions, J. the Optical Society of America A, Vol.4, 1987
     * @return Homogeneous Transform(4X4 행렬)     
     */
    KHomogeneous            TransformFrom3D_3D(int nNumPoints,          /*< 대응하는 3차원 점의 개수 */
                                               KVector* vpA,            /*< 좌표계 A에서의 3차원 점의 백터 배열포인터 */
                                               KVector* vpB,            /*< 좌표계 B에서의 3차원 점의 백터 배열포인터 */
                                               double*  dpError=0,      /*< 변환행렬로 인한 최종 오차 */
                                               bool     bScale=false    /*< 스케일 계수를 구할지 여부 */
                                               );
    KHomogeneous            TransformFrom3D_3D(const KMatrix&  mA,      /*< 좌표계 A에서의 3차원 점의 행렬. 열백터가 3D 좌표 */
                                               const KMatrix&  mB,      /*< 좌표계 B에서의 3차원 점의 행렬. 열백터가 3D 좌표 */
                                               double*  dpError=0,      /*< 변환행렬로 인한 최종 오차 */
                                               bool     bScale=false    /*< 스케일 계수를 구할지 여부 */
                                               );
    KMatrix                 TransformFrom2D_2D(double dFocal,int nNumPoints,KPoint* ptpModel,KPoint* ptpImage);
    static	KHomogeneous    TransformFromPlanes_Planes(const KMatrices& mlTo,const KMatrices& mlT1);    // return mT1_o ( Xo = To_1 * X1)
    static	KVector         TransformFromPlanes_Planes(const KVectors& vsTo,const KVectors& vsT1);

	//
    KMatrix 			    CameraToPlane(const KMatrix& mK,const KPoints& psImage,const KPoints& psModel,bool bOptima);
    virtual KMatrix         NormalizeCoordinates(const KMatrix& mImgO,KMatrix& mImgN);
    virtual double          Erf(const KVector& vX);

protected:
	

};
//---------------------------------------------------------------------------
#endif
 
