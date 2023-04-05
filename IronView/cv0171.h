#ifndef CV0171_H
#define CV0171_H

#include "kfc.h"
#include "QDebug"
#include "vector"

#define PI 3.1415926353

using namespace std;

struct EdgeData{
    double dx;
    double dy;
    double magn;
    int    dir;     // 8개의 방향이 존재할 수 있다. 0,1,2,3 중 하나로 표현
    unsigned char val;  // for optical flow
};

typedef vector<vector<EdgeData>*> KEdgeImg;
typedef vector<pair<int, int>> KPOINTS;

class cv0171{
private:
    KEdgeImg    edgeData;   // edge 정보를 담은 이미지 데이터이다.
    KImageGray      resultIgImg;
    KImageColor     resultIcImg;
public:
    //static void Apply_Sepia(KImageGray&);
    static KImageColor Histogram_Equalization(KImageColor& Input_Img, int row, int col);
    static KImageColor Histogram_Matching(KImageColor& Input_Img, KImageColor& Target_Img, int row, int col);
    static KImageColor Gaussian_Noise(KImageColor& Input_Img, double dSigma, int row, int col);
    static KImageColor Salt_and_Pepper(KImageColor& Input_Img, double dTau, int row, int col);
    static KImageColor Box_Filter(KImageColor& Input_Img, QString BoxFilter_Size, int row, int col);
    static KImageColor Gaussian_Filter(KImageColor& Input_Img, double dSigma, int row, int col);
    static KImageColor Median_Filter(KImageColor& Input_Img, QString MedianFilter_Size, int row, int col);
    static KImageGray CannyEdge(KImageGray& gray_img, double sigma, int high_threshold, int low_threshold, int row, int col);
    static KImageColor GeneralizedHT(KImageGray& Input_Img, int row, int co);
    KEdgeImg OpticalFlow(KImageGray& igImg1, KImageGray& igImg2, int iter = 100);
    vector<vector<double>>  GetValMatrix(const KEdgeImg& matrix);
    KImageColor GetResultIcImg(){return resultIcImg;}

};










// For CV08
class ImgPyramid{
private:
    KImageGray _igImg;
    int _rowSize;
    int _colSize;
    vector<KEdgeImg> pyImg;


public:

    ImgPyramid(){};
    ImgPyramid(KImageGray & igImg) : _igImg(igImg){_rowSize = _igImg.Row(); _colSize = _igImg.Col();}

    void Create(int num);
    vector<KEdgeImg>* getPyImg(){return &pyImg;}

};

#endif // CV0171_H
