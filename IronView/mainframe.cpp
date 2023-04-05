#include <QFileDialog>
#include <QPainter>
#include <iostream>
#include <cmath>
#include <math.h>
#include <random>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <vector>

#include "opencv2/calib3d.hpp"
#include "opencv2/core/types.hpp"

#include "mainframe.h"
#include "ui_mainframe.h"
#include "imageform.h"
#include "cv0171.h"
#include "HikCamera.cpp"

#define __PI 3.141592
#define __2PI 6.283185307179586477

using namespace std;

MainFrame::MainFrame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainFrame)
{
    ui->setupUi(this);

    _plpImageForm       = new KPtrList<ImageForm*>(10,false,false);
    _q_pFormFocused     = 0;

    //객체 맴버의 초기화


    //get a current directory
    char st[100];
    GetCurrentDirectoryA(100,st);

    //리스트 출력창을 안보이게
    ui->listWidget->setVisible(false);
    this->adjustSize();

    //UI 활성화 갱신
    UpdateUI();
}

MainFrame::~MainFrame()
{ 
    delete ui;         
    delete _plpImageForm;   
}

void MainFrame::Detach(ImageForm *pForm)
{    
    //ImageForm 포인터 삭제
    _plpImageForm->Remove(pForm);

    //활성화 ImageForm 초기화
    _q_pFormFocused     = 0;

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::UpdateUI()
{
    if(ui->tabWidget->currentIndex() == 0) // hw 01-02
    {
        //ui->buttonSepiaTone->setEnabled( _q_pFormFocused &&  _q_pFormFocused->ID() == "OPEN" );
    }
}

void MainFrame::OnMousePos(const int &nX, const int &nY, ImageForm* q_pForm)
{
    UpdateUI();
}

void MainFrame::closeEvent(QCloseEvent* event)
{
    //생성된 ImageForm을 닫는다.
    for(int i=_plpImageForm->Count()-1; i>=0; i--)
        _plpImageForm->Item(i)->close();
}


void MainFrame::on_buttonOpen_clicked()
{
    //이미지 파일 선택
    QFileDialog::Options    q_Options   =  QFileDialog::DontResolveSymlinks  | QFileDialog::DontUseNativeDialog; // | QFileDialog::ShowDirsOnly
    QString                 q_stFile    =  QFileDialog::getOpenFileName(this, tr("Select a Image File"),  "./data", "Image Files(*.bmp *.ppm *.pgm *.png)",0, q_Options);

    if(q_stFile.length() == 0)
        return;

    //이미지 출력을 위한 ImageForm 생성    
    ImageForm*              q_pForm   = new ImageForm(q_stFile, "OPEN", this);

    _plpImageForm->Add(q_pForm);
    q_pForm->show();

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::on_buttonDeleteContents_clicked()
{
    //생성된 ImageForm을 닫는다.
    for(int i=_plpImageForm->Count()-1; i>=0; i--)
        _plpImageForm->Item(i)->close();

    //리스트에서 삭제한다.
    _plpImageForm->RemoveAll();
}

void MainFrame::on_buttonShowList_clicked()
{
    static int nWidthOld = ui->tabWidget->width();

    if(ui->listWidget->isVisible())
    {
        nWidthOld = ui->listWidget->width();
        ui->listWidget->hide();
        this->adjustSize();
    }
    else
    {        
        ui->listWidget->show();
        QRect q_rcWin = this->geometry();

        this->setGeometry(q_rcWin.left(), q_rcWin.top(), q_rcWin.width()+nWidthOld, q_rcWin.height());
    }
}

void MainFrame::on_ApplySepia_clicked()
{
    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageColor   icMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() &&  _q_pFormFocused->ID() == "OPEN")
        icMain = _q_pFormFocused->ImageColor();
    else
        return;

    int row = icMain.Row();
    int col = icMain.Col();

    // 출력 영상
    KImageGray Hue_Image(row, col);
    KImageGray Sat_Image(row, col);
    KImageGray Val_Image(row, col);
    KImageColor icSepia(row, col);

    KArray<KHSV> icHSV;
    icHSV.Create(row, col);

    // 입력받은 값
    double  dHue = ui->SpinHue->value();
    double  dSat = ui->SpinSaturation->value();

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            //cout << (int)icMain[i][j].r << "\t" << (int)icMain[i][j].g << "\t" << (int)icMain[i][j].b << endl;
            double dMin, dMax,dDiff;
            dMin  = _MIN(_MIN(icMain[i][j].r,icMain[i][j].g),icMain[i][j].b);
            dMax  = _MAX(_MAX(icMain[i][j].r,icMain[i][j].g),icMain[i][j].b);
            dDiff = dMax - dMin;

            //hue
            if(dMax == (double)(icMain[i][j].r))
                icHSV[i][j].h = 60.0 * (double)(icMain[i][j].g-icMain[i][j].b)/dDiff;
            else if(dMax == (double)(icMain[i][j].g))
                icHSV[i][j].h = 60.0 * (double)(icMain[i][j].b-icMain[i][j].r)/dDiff + 120.0;
            else
                icHSV[i][j].h = 60.0 * (double)(icMain[i][j].r-icMain[i][j].g)/dDiff + 240.0;

            icHSV[i][j].h += (double)(icHSV[i][j].h < 0.0 ? 360.0 : 0.0); // 0 ~ 360

            //saturation
            icHSV[i][j].s = dDiff / dMax; // 0 ~ 1

            //value
            icHSV[i][j].v = dMax / 255; // 0 ~ 1
        }
    }

    for(int i=0; i < row; i++){
        for(int j = 0; j < col; j++){
            Hue_Image[i][j] = (icHSV[i][j].h / 360 * 255); // 0 ~ 255
            Sat_Image[i][j] = (icHSV[i][j].s * 255); // 0 ~ 255
            Val_Image[i][j] = (icHSV[i][j].v * 255); // 0 ~ 255
        }
    }

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            float v,h,c,x,m;
            double r, g, b;

            v = icHSV[i][j].v; // 0 ~ 1
            h = dHue / 60.0;
            c = v * dSat;
            x = c * (1 - abs(fmodf(h, 2) -1));
            m = v - c;

            if(h == 6.0) h = 0.0;

            switch((int)h){
                case 0: r=c; g=x; b=0; break;
                case 1: r=x; g=c; b=0; break;
                case 2: r=0; g=c; b=x; break;
                case 3: r=0; g=x; b=c; break;
                case 4: r=x; g=0; b=c; break;
                case 5: r=c; g=0; b=x; break;
            }

            icSepia[i][j].r = (r + m) * 255;
            icSepia[i][j].g = (g + m) * 255;
            icSepia[i][j].b = (b + m) * 255;
       }
    }

    //출력을 위한 ImageForm 생성
    ImageForm*  H_Form = new ImageForm(Hue_Image, "H", this);
    ImageForm*  S_Form = new ImageForm(Sat_Image, "S", this);
    ImageForm*  V_Form = new ImageForm(Val_Image, "V", this);
    ImageForm*  q_pForm = new ImageForm(icSepia, "Sepia Applied", this);
    _plpImageForm->Add(q_pForm);
    _plpImageForm->Add(H_Form);
    _plpImageForm->Add(S_Form);
    _plpImageForm->Add(V_Form);
    q_pForm->show();
    H_Form->show();
    S_Form->show();
    V_Form->show();
    UpdateUI();
}


void MainFrame::on_Apply_Otsu_Threshold_clicked()
{
    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageGray igMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() &&  _q_pFormFocused->ID() == "OPEN")
        igMain = _q_pFormFocused->ImageGray();
    else
        return;

    int row = igMain.Row();
    int col = igMain.Col();
    KImageGray igThreshold(row, col);

    int TotalNum = row * col;
    int q1 = 0;
    int q2 = 1;
    int mu1 = 0;
    int mu2 = 1;
    int maxthres = 0;

    double Histo[256] = {0};
    double Histo_P[256] = {0};
    double sum_P1 = 0;
    double sum_P2 = 0;
    double sum_iP1 = 0;
    double sum_iP2 = 0;
    double q[256][2] = {};
    double mu[256][2] = {};
    double b_var[256] = {};
    double max = b_var[0];

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            Histo[(int)igMain[i][j]]++;
        }
    }

    for(int i = 0; i < 256; i++){
        Histo_P[i] = Histo[i]/(TotalNum);
    }

    for(int thres = 0; thres < 256; thres++){
        for(int i = 0; i < thres; i++){
            sum_P1 += Histo_P[i];
        }
        for(int j = thres; j < 256; j++){
            sum_P2 += Histo_P[j];
        }
        q[thres][q1] = sum_P1;
        q[thres][q2] = sum_P2;
        sum_P1 = 0;
        sum_P2 = 0;
    }

    for(int thres = 0; thres < 256; thres++){
        for(int i = 0; i < thres; i++){
            sum_iP1 += i * Histo_P[i];
        }
        for(int j = thres; j < 256; j++){
            sum_iP2 += j * Histo_P[j];
        }
        mu[thres][mu1] = sum_iP1 / q[thres][q1];
        mu[thres][mu2] = sum_iP2 / q[thres][q2];
        sum_iP1 = 0;
        sum_iP2 = 0;
    }

    for(int i = 0; i < 256; i++){
        for(int j = 0; j < 2; j++){
            if(isnan(mu[i][j]) == 1){
                mu[i][j] = 0;
            }
        }
    }

    for(int thres = 0; thres < 256; thres++){
        b_var[thres] = q[thres][q1] * q[thres][q2] * pow((mu[thres][mu1] - mu[thres][mu2]), 2);
    }

    for(int thres = 0; thres < 256; thres++){
        if(b_var[thres] > max){
            max = b_var[thres];
            maxthres = thres;
        }
    }

    igThreshold = igMain.Thresholded_output(maxthres, igThreshold);

    ImageForm*  q_pForm = new ImageForm(igThreshold, "Otsu_Thresholded", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();
    UpdateUI();
}


void MainFrame::on_Apply_Threshold_clicked()
{
    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageGray igMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() &&  _q_pFormFocused->ID() == "OPEN")
        igMain = _q_pFormFocused->ImageGray();
    else
        return;

    int row = igMain.Row();
    int col = igMain.Col();

    KImageGray igThreshold(row, col);

    int dThreshold = ui->SpinThreshold->value();

    igThreshold = igMain.Thresholded_output(dThreshold, igThreshold);

    ImageForm*  q_pForm = new ImageForm(igThreshold, "manually_Thresholded", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();
    UpdateUI();
}


void MainFrame::on_Apply_Labeling_clicked()
{
    KImageGray igMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() && \
            (_q_pFormFocused->ID() == "Otsu_Thresholded" ||\
             _q_pFormFocused->ID() == "manually_Thresholded" ||\
             _q_pFormFocused->ID() == "Dilated" ||\
             _q_pFormFocused->ID() == "Erosed"))
        igMain = _q_pFormFocused->ImageGray();
    else
        return;

    int row = igMain.Row();
    int col = igMain.Col();

    //KImageGray igLabel(row, col);   // 라벨 번호를 부여할 이미지
    int igLabel[256][256] = {};  // (색깔)라벨 번호를 부여할 배열
    KImageColor Labelled_image(row, col);   // 실제 라벨링 결과를 담을 이미지
    int Label = 0;  // Labeling_Num 0은 아무것도 없는 영역

    vector<vector <int>> Eqt; // Equivalance table(2차원 벡터)
    vector<int> pixel; //픽셀의 위치 (x, y)좌표

    // 이미지의 0행, 0열 값을 0으로
    for(int j = 0; j < col ;j++){
        igMain[0][j] = 0;
    }
    for(int i = 0; i <row; i++){
       igMain[i][0] = 0;
    }

    // 결과를 담을 이미지 0으로 초기화
    for(int i = 0; i < row; i++){
        for(int j = 0; j<col; j++){
            Labelled_image[i][j].r = 0;
            Labelled_image[i][j].g = 0;
            Labelled_image[i][j].b = 0;
        }
    }

    // 8-neighbor 방식:
    for(int i = 1; i < row; i++){
        for(int j = 1; j<col; j++){
            if(igMain[i][j] == 255){ // i, j픽셀이 1일 때
                // 0 0
                // 0
                if((igMain[i-1][j-1] == 0)&&(igMain[i][j-1] == 0)&&(igMain[i-1][j] == 0)){
                    Label++;
                    igLabel[i][j] = Label;//new label
                }
                // 0 0
                // 1
                else if((igMain[i-1][j-1] == 0)&&(igMain[i][j-1] == 255)&&(igMain[i-1][j] == 0)){
                    igLabel[i][j] = igLabel[i][j-1];//same label with left
                }
                // 0 1
                // 0
                else if((igMain[i-1][j-1] == 0)&&(igMain[i][j-1] == 0)&&(igMain[i-1][j] == 255)){
                    igLabel[i][j] = igLabel[i-1][j];//same label with upper
                }
                // 1 0
                // 0
                else if((igMain[i-1][j-1] == 255)&&(igMain[i][j-1] == 0)&&(igMain[i-1][j] == 0)){
                    igLabel[i][j] = igLabel[i-1][j-1];//same label with upper left
                }
                // 0 1
                // 1
                else if((igMain[i-1][j-1] == 0)&&(igMain[i][j-1] == 255)&&(igMain[i-1][j] == 255)){
                    pixel.push_back(i);
                    pixel.push_back(j); //Eqivalance table
                    Eqt.push_back(pixel);
                    pixel = {};
                }
            }
        }
    }

    // equivalence table 후처리
    for(size_t n = 0; n < Eqt.size(); n++){ // eqt점들을 1개씩 돌면서
        igLabel[Eqt[n][0]][Eqt[n][1]] = igLabel[Eqt[n][0]-1][Eqt[n][1]];
        int left = igLabel[Eqt[n][0]][Eqt[n][1]-1]; // eqt점의 왼쪽 점 라벨 left
        int up = igLabel[Eqt[n][0]-1][Eqt[n][1]]; // eqt점의 위쪽 점 라벨 up
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                if(igLabel[i][j] == left){ // 이미지상에서 라벨이 left 인 점들을 up으로 교체
                    igLabel[i][j] = up;
                }
            }
        }
    }

    Eqt = {};

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(igMain[i][j] == 255){
                if(((igMain[i-1][j-1] == 255)&&(igMain[i][j-1] == 255)&&(igMain[i-1][j] == 255)) ||\
                        ((igMain[i-1][j-1] == 255)&&(igMain[i][j-1] == 255)&&(igMain[i-1][j] == 0)) ||\
                        ((igMain[i-1][j-1] == 255)&&(igMain[i][j-1] == 0)&&(igMain[i-1][j] == 255))){
                    igLabel[i][j] = igLabel[i-1][j-1]; //same label with upper left
                }
                // 0 0
                // 1
                if((igMain[i-1][j-1] == 0)&&(igMain[i][j-1] == 255)&&(igMain[i-1][j] == 0)){
                    igLabel[i][j] = igLabel[i][j-1];//same label with left
                }
                // 0 1
                // 0
                else if((igMain[i-1][j-1] == 0)&&(igMain[i][j-1] == 0)&&(igMain[i-1][j] == 255)){
                    igLabel[i][j] = igLabel[i-1][j];//same label with upper
                }
                // 1 0
                // 0
                else if((igMain[i-1][j-1] == 255)&&(igMain[i][j-1] == 0)&&(igMain[i-1][j] == 0)){
                    igLabel[i][j] = igLabel[i-1][j-1];//same label with upper left
                }
                // 0 1
                // 1
                else if((igMain[i-1][j-1] == 0)&&(igMain[i][j-1] == 255)&&(igMain[i-1][j] == 255)){
                    //igLabel[i][j] = 0;
                    pixel.push_back(i);
                    pixel.push_back(j); //Eqivalance table
                    Eqt.push_back(pixel);
                    pixel = {};
                }
            }
        }
    }

    for(size_t n = 0; n < Eqt.size(); n++){ // eqt점들을 1개씩 돌면서
        igLabel[Eqt[n][0]][Eqt[n][1]] = igLabel[Eqt[n][0]-1][Eqt[n][1]];
        int left = igLabel[Eqt[n][0]][Eqt[n][1]-1]; // eqt점의 왼쪽 점 라벨 left
        int up = igLabel[Eqt[n][0]-1][Eqt[n][1]]; // eqt점의 위쪽 점 라벨 up
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                if(igLabel[i][j] == left){ // 이미지상에서 라벨이 left 인 점들을 up으로 교체
                    igLabel[i][j] = up;
                }
            }
        }
    }

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(igMain[i][j] == 255){
                if(((igMain[i-1][j-1] == 255)&&(igMain[i][j-1] == 255)&&(igMain[i-1][j] == 255)) ||\
                        ((igMain[i-1][j-1] == 255)&&(igMain[i][j-1] == 255)&&(igMain[i-1][j] == 0)) ||\
                        ((igMain[i-1][j-1] == 255)&&(igMain[i][j-1] == 0)&&(igMain[i-1][j] == 255))){
                    igLabel[i][j] = igLabel[i-1][j-1]; //same label with upper left
                }
                if((igMain[i-1][j-1] == 0)&&(igMain[i][j-1] == 0)&&(igMain[i-1][j] == 255)){
                    igLabel[i][j] = igLabel[i-1][j];//same label with upper
                }
            }
        }
    }

    srand(time(NULL));
    int num1 = rand() % 100;
    int num2 = rand() % 100;
    int num3 = rand() % 100;

    // 라벨링 이미지를 같은 라벨 픽셀별로 같은 색깔로 칠함
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(igMain[i][j] != 0){

                Labelled_image[i][j].r = igLabel[i][j] * num1 % 256;
                Labelled_image[i][j].g = igLabel[i][j] * num2 % 256;
                Labelled_image[i][j].b = igLabel[i][j] * num3 % 256;
            }
        }
    }

    ImageForm*  q_pForm = new ImageForm(Labelled_image, "Labelled", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();
    UpdateUI();
}


void MainFrame::on_Apply_Dilation_clicked()
{
    KImageGray igMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() &&\
            (_q_pFormFocused->ID() == "Otsu_Thresholded" ||\
             _q_pFormFocused->ID() == "manually_Thresholded" ||\
             _q_pFormFocused->ID() == "Dilated" ||\
             _q_pFormFocused->ID() == "Erosed"))
        igMain = _q_pFormFocused->ImageGray();
    else
        return;

    int row = igMain.Row();
    int col = igMain.Col();

    KImageGray igDilate(row, col);

    for(int i = 0; i< row; i++){
        for(int j = 0; j < col; j++){
            if(igMain[i][j] == 255){
                igDilate[i][j] = 255;
            }
            else igDilate[i][j] = 0;
        }
    }

    int ksize = ui->spinKernel->value();

    if(ksize % 2 != 1){
        cout << "Kernel Size have to be the odd number!" << endl;
        return;
    }

    for(int i = ksize/2; i < row - ksize/2; i++){
        for(int j = ksize/2; j < col - ksize/2; j++){
            for(int k = i - ksize/2; k <= i + ksize/2; k++){
                for(int l = j - ksize/2; l <= j + ksize/2; l++){
                    if(igMain[k][l] == 255){
                        igDilate[i][j] = 255;
                    }
                }
            }
        }
    }

    ImageForm*  q_pForm = new ImageForm(igDilate, "Dilated", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();
    UpdateUI();
}


void MainFrame::on_Apply_Erosion_clicked()
{
    KImageGray igMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() &&\
            (_q_pFormFocused->ID() == "Otsu_Thresholded" ||\
             _q_pFormFocused->ID() == "manually_Thresholded" ||\
             _q_pFormFocused->ID() == "Dilated" ||\
             _q_pFormFocused->ID() == "Erosed"))
        igMain = _q_pFormFocused->ImageGray();
    else
        return;

    int row = igMain.Row();
    int col = igMain.Col();

    KImageGray igErose(row, col);

    for(int i = 0; i< row; i++){
        for(int j = 0; j < col; j++){
            if(igMain[i][j] == 255){
                igErose[i][j] = 255;
            }
            else igErose[i][j] = 0;
        }
    }

    int ksize = ui->spinKernel->value();

    if(ksize % 2 != 1){
        cout << "Kernel Size have to be the odd number!" << endl;
        return;
    }

    for(int i = ksize/2; i < row - ksize/2; i++){
        for(int j = ksize/2; j < col - ksize/2; j++){
            for(int k = i - ksize/2; k <= i + ksize/2; k++){
                for(int l = j - ksize/2; l <= j + ksize/2; l++){
                    if(igMain[k][l] == 0){
                        igErose[i][j] = 0;
                    }
                }
            }
        }
    }

    ImageForm*  q_pForm = new ImageForm(igErose, "Erosed", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();
    UpdateUI();
}

//    for(int i = 1; i < row - 1; i++){
//        for(int j = 1; j < col - 1; j++){
//            if(igMain[i][j] == 0){
//                for(int k = i - ksize/2; k <= i + ksize/2; k++){
//                    for(int l = j - ksize/2; l <= j + ksize/2; l++){
//                        if(igMain[k][l] == 255){
//                            igDilate[i][j] = 255;
//                        }
//                    }
//                }
//            }
//        }
//    } 테두리 따짐 ㅋㅋ

void MainFrame::on_Histogram_Equalization_clicked()
{
    KImageColor icMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() && (_q_pFormFocused->ID() == "OPEN"))
        icMain = _q_pFormFocused->ImageColor();
    else
        return;

    int row = icMain.Row();
    int col = icMain.Col();

    KImageColor icEqualized(row, col);

    icEqualized = cv0171::Histogram_Equalization(icMain, row, col);

    ImageForm*  q_pForm = new ImageForm(icEqualized, "Histogram Equalized", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();
    UpdateUI();
}

void MainFrame::on_Register_Reference_clicked()
{
    KImageColor icMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() && (_q_pFormFocused->ID() == "OPEN"))
        icMain = _q_pFormFocused->ImageColor();
    else
        return;

    ImageForm*  q_pForm = new ImageForm(icMain, "Target", this);
    _plpImageForm->Add(q_pForm);
}


void MainFrame::on_Histogram_Matching_clicked()
{
    KImageColor icSource;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() && (_q_pFormFocused->ID() == "OPEN"))
        icSource = _q_pFormFocused->ImageColor();
    else
        return;

    int row = icSource.Row();
    int col = icSource.Col();

    KImageColor icMatched(row, col);
    KImageColor icTarget;

    bool return_flag = true;
    if(!_plpImageForm->IsEmpty()) {
        for (unsigned int i = 0; i < _plpImageForm->Count(); i++) {
            if ((*_plpImageForm)[i]->ID() == "Target") {
                ImageForm* q_pForm = (*_plpImageForm)[i];
                icTarget = q_pForm->ImageColor();
                return_flag = false;
                break;
            }
        }
    }
    else
        return;
    if (return_flag)
        return;

    icMatched = cv0171::Histogram_Matching(icSource, icTarget, row, col);

    ImageForm*  q_pForm = new ImageForm(icMatched, "Histogram Matched", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();
    UpdateUI();
}

void MainFrame::on_Apply_Gaussian_clicked()
{
    KImageColor icMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() && (_q_pFormFocused->ID() == "OPEN"))
        icMain = _q_pFormFocused->ImageColor();
    else
        return;

    int row = icMain.Row();
    int col = icMain.Col();

    KGaussian gaussian_noise;
    KImageColor icGaussian = icMain;

    double sigma = ui->Gaussian_sigma->value();

    icGaussian = cv0171::Gaussian_Noise(icMain, sigma, row, col);

    ImageForm*  q_pForm = new ImageForm(icGaussian, "Gaussian Noised", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();
    UpdateUI();
}


void MainFrame::on_Apply_SNP_clicked()
{
    KImageColor icMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() && (_q_pFormFocused->ID() == "OPEN"))
        icMain = _q_pFormFocused->ImageColor();
    else
        return;

    int row = icMain.Row();
    int col = icMain.Col();

    KImageColor icSNP(row, col);
    double tau = ui->SNP_tau->value();
    icSNP = cv0171::Salt_and_Pepper(icMain, tau, row, col);

    ImageForm*  q_pForm = new ImageForm(icSNP, "S&P", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();
    UpdateUI();
}


void MainFrame::on_Apply_BoxFilter_clicked()
{
    KImageColor icMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() && (_q_pFormFocused->ID() == "Gaussian Noised" || _q_pFormFocused->ID() == "S&P"))
        icMain = _q_pFormFocused->ImageColor();
    else
        return;

    int row = icMain.Row();
    int col = icMain.Col();

    KImageColor icBox = icMain;
    QString BoxFilter_Size = ui->BoxFilter_Size->currentText();
    icBox = cv0171::Box_Filter(icMain, BoxFilter_Size, row, col);

    ImageForm*  q_pForm = new ImageForm(icBox, "Box Filtered", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();
    UpdateUI();
}


void MainFrame::on_Apply_GaussianFilter_clicked()
{
    KImageColor icMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() && (_q_pFormFocused->ID() == "Gaussian Noised" || _q_pFormFocused->ID() == "S&P"))
        icMain = _q_pFormFocused->ImageColor();
    else
        return;

    int row = icMain.Row();
    int col = icMain.Col();

    double sigma = ui->Gaussian_Filter_Sigma->value();
    //KGaussian gaussian_filter;
    KImageColor icGaussian(row, col);
    icGaussian = cv0171::Gaussian_Filter(icMain, sigma, row, col);

    ImageForm*  q_pForm = new ImageForm(icGaussian, "Gaussian Filtered", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();
    UpdateUI();
}


void MainFrame::on_Apply_MedianFilter_clicked()
{
    KImageColor icMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() && (_q_pFormFocused->ID() == "Gaussian Noised" || _q_pFormFocused->ID() == "S&P"))
        icMain = _q_pFormFocused->ImageColor();
    else
        return;

    int row = icMain.Row();
    int col = icMain.Col();


    KImageColor icMedian = icMain;
    QString MedianFilter_Size = ui->MedianFilter_Size->currentText();

    icMedian = cv0171::Median_Filter(icMain, MedianFilter_Size, row, col);

    ImageForm*  q_pForm = new ImageForm(icMedian, "Median Filtered", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();
    UpdateUI();
}


void MainFrame::on_Apply_Canny_edge_clicked()
{
    KImageGray igMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() && (_q_pFormFocused->ID() == "OPEN"))
        igMain = _q_pFormFocused->ImageGray();
    else
        return;

    int row = igMain.Row();
    int col = igMain.Col();

    KImageGray igCanny(row, col);

    double dSigma = ui->spin_Sigma->value();
    int high_thres = ui->spin_High_Threshold->value();
    int low_thres = ui->spin_Low_Threshold->value();

    igCanny = cv0171::CannyEdge(igMain, dSigma, high_thres, low_thres, row, col);

    ImageForm*  q_pForm_Canny = new ImageForm(igCanny, "Canny Edge Applied", this);
    _plpImageForm->Add(q_pForm_Canny);
    q_pForm_Canny->show();
    UpdateUI();
}


void MainFrame::on_Apply_HoughCircle_clicked()
{
    KImageGray igMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() && (_q_pFormFocused->ID() == "OPEN"))
        igMain = _q_pFormFocused->ImageGray();
    else
        return;

    int row = igMain.Row();
    int col = igMain.Col();
    int a,b  = 0;
    int cnt = 0;
    int Voting_Threshold = 100;

    KImageGray igCanny(row,col);
    KImageGray igCircleDraw(row, col);
    KImageGray igHoughCircle = igMain;

    vector<int> coordinate; // 엣지점을 두른 원 각 점의 좌표
    vector<vector<int>> coordinates;    // 원 좌표들의 모음 벡터

    int radius = 51;    // 주어진 원 반지름

    igCanny = cv0171::CannyEdge(igMain, 0.8, 45, 20, row, col); // 캐니 엣지 적용

    // 각 엣지픽셀 주위로 반지름이 51인 원을 두르고, 그 원 픽셀들을 전부 2차원 벡터로 저장
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(igCanny[i][j] == 255){
                for(int angle = 0; angle < 360; angle++){
                    a = i - radius*cos(angle);
                    b = j - radius*sin(angle);
                    coordinate.push_back(a);
                    coordinate.push_back(b);
                    coordinates.push_back(coordinate);
                    // 한번 사용한 원 위 점 픽셀 좌표쌍 벡터 초기화
                    coordinate.clear();
                    cnt++;
                }
            }
        }
    }
    // 755640 개의 점(겹치는 점 포함) 생성됨

//    // 모든 원을 그리는 코드
//    for(int i = 0; i < cnt; i++){
//        if((coordinates[i][0] >= 0 && coordinates[i][0] <= row) && (coordinates[i][1] >= 0 && coordinates[i][1] <= col)){
//            igCircleDraw[coordinates[i][0]][coordinates[i][1]] = 255;
//        }
//    }

    // coordinates을 정렬(겹치는 점벡터들을 서로 모음)
    sort(coordinates.begin(), coordinates.end());

    // coordinates에서 겹치는 점들을 생략할 2차원 벡터 생성
    int duplicate_number = 1;
    int count_element = 0;
    vector<int> deduplicated_coordinate;
    vector<vector<int>> deduplicated_coordinates;

    for(unsigned int i = 1; i < coordinates.size(); i++){
        // coordinates에서 이전 점과 지금 점이 같다면, 카운트++
        if(coordinates[i - 1] == coordinates[i]){
            duplicate_number++;
        }
        // deduplicated_coornate: 중첩 개수, x, y좌표
        else if(coordinates[i - 1] != coordinates[i]){
            deduplicated_coordinate.push_back(duplicate_number);
            deduplicated_coordinate.push_back(coordinates[i][0]);
            deduplicated_coordinate.push_back(coordinates[i-1][1]);
            deduplicated_coordinates.push_back(deduplicated_coordinate);

            deduplicated_coordinate.clear();
            count_element++;
            // 중첩 개수 초기화
            duplicate_number = 1;
        }
    }
    // 0번째 원소 기준(중첩개수)으로 정렬(점별 중첩 횟수 파악용)
    sort(deduplicated_coordinates.begin(), deduplicated_coordinates.end());

//    for(int i = 0; i < att.size(); i++){
//        cout << att[i][0] << " " << att[i][1] << " " << att[i][2] << endl;
//    }

    // 가장 많이 중첩된 점(벡터의 마지막 원소) 좌표만 따서 소스 이미지 위에 원 그리기
    int x_center = deduplicated_coordinates[deduplicated_coordinates.size()-1][1];
    int y_center = deduplicated_coordinates[deduplicated_coordinates.size()-1][2];

    for(int angle = 0; angle < 360; angle++){
        a = x_center - radius*cos(angle);
        b = y_center - radius*sin(angle);
        if((a >= 0 && a < row) && (b >= 0 && b < col)){
            igHoughCircle[a][b] = 255;
        }
    }
    igHoughCircle[x_center][y_center] = 255;

    ImageForm*  q_pForm_Canny = new ImageForm(igCanny, "Canny Edge Detection", this);
    ImageForm*  q_pForm_Circle_Draw = new ImageForm(igCircleDraw, "Hough Space Circles", this);
    ImageForm*  q_pForm_Hough_Circle = new ImageForm(igHoughCircle, "Hough Circle Detection", this);
    _plpImageForm->Add(q_pForm_Canny);
    _plpImageForm->Add(q_pForm_Circle_Draw);
    _plpImageForm->Add(q_pForm_Hough_Circle);
    q_pForm_Canny->show();
    q_pForm_Circle_Draw->show();
    q_pForm_Hough_Circle->show();
    UpdateUI();
}


void MainFrame::on_Apply_Generalized_Hough_Transform_clicked()
{
    KImageGray igMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() && (_q_pFormFocused->ID() == "OPEN"))
        igMain = _q_pFormFocused->ImageGray();
    else
        return;
    KImageColor output_img;
    int row = igMain.Row();
    int col = igMain.Col();

    output_img = cv0171::GeneralizedHT(igMain, row, col);

    ImageForm*  q_pForm_Canny = new ImageForm(output_img, "Canny Edge Detection", this);
    _plpImageForm->Add(q_pForm_Canny);
    q_pForm_Canny->show();
    UpdateUI();
}


//void MainFrame::on_Apply_Optical_Flow_clicked()
//{
//    KImageGray Image1;
//    KImageGray Image2;

//    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() && (_q_pFormFocused->ID() == "OPEN"))
//        Image1 = _q_pFormFocused->ImageGray();
//    else
//        return;

//    cout << "A" << endl;

////    bool return_flag = true;
////    if(!_plpImageForm->IsEmpty()) {
////        for (unsigned int i = 0; i < _plpImageForm->Count(); i++) {
////            if ((*_plpImageForm)[i]->ID() == "Compare") {
////                cout << "A" << endl;
////                ImageForm* q_pForm = (*_plpImageForm)[i];
////                Image2 = q_pForm->ImageGray();
////                return_flag = false;
////                break;
////            }
////        }
////    }
////    else
////        return;
////    if (return_flag)
////        return;

////    KImageColor icMain;
////    KEdgeImg    edgeImg;

////    vector<vector<double>> a;
////    vector<double> tmp;
////    tmp.assign(2, 0.);
////    a.assign(2, tmp);

////    vector<vector<double>> b;
////    b.assign(2, tmp);

////    for(int nRow=0; nRow < 2; nRow++){
////        for(int nCol=0; nCol < 2; nCol++){
////           a[nRow][nCol] = nCol + nRow*2;
////           b[nRow][nCol] = nCol + nRow*2;
////        }
////    }

////    edgeImg = cv0171::OpticalFlow(Image1, Image2);

////    KImageColor     resultIcImg;
////    icMain = resultIcImg;

////    ImageForm*  q_pFormRes = new ImageForm(icMain, "Optical Flow Image", this);

////    int nScale = 8;

////    for(unsigned nRow = 0; nRow < icMain.Row()/nScale; nRow++){
////        for(unsigned int nCol=0; nCol < icMain.Col()/nScale; nCol++){
////            int nDx = edgeImg[nRow*nScale]->at(nCol*nScale).dx;
////            int nDy = edgeImg[nRow*nScale]->at(nCol*nScale).dy;

////            q_pFormRes->DrawLine(nCol*nScale, nRow*nScale, nCol*nScale+nDx, nRow*nScale+nDy,
////                                 QColor(255, 255, 0), 1);
////        }
////    }
//    ImageForm*  q_pForm = new ImageForm(Image1, "Image1", this);
//    _plpImageForm->Add(q_pForm);
//    q_pForm->show();


//    //UI 활성화 갱신
//    UpdateUI();
//}




void MainFrame::on_Apply_Optical_Flow_clicked()
{
    KImageGray Image1;
    KImageGray Image2;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() && (_q_pFormFocused->ID() == "OPEN"))
        Image1 = _q_pFormFocused->ImageGray();
    else
        return;

    bool return_flag = true;
    if(!_plpImageForm->IsEmpty()) {
        for (unsigned int i = 0; i < _plpImageForm->Count(); i++) {
            if ((*_plpImageForm)[i]->ID() == "Compare") {

                ImageForm* q_pForm = (*_plpImageForm)[i];
                Image2 = q_pForm->ImageGray();
                return_flag = false;
                break;
            }
        }
    }
    else
        return;
    if (return_flag)
        return;



    KImageColor icMain;
    KEdgeImg    edgeImg;

    vector<vector<double>> a;
    vector<double> tmp;
    tmp.assign(2, 0.);
    a.assign(2, tmp);

    vector<vector<double>> b;
    b.assign(2, tmp);

    for(int i=0; i < 2; i++){
        for(int j=0; j < 2; j++){
           a[i][j] = i*2 + j;
           b[i][j] = i*2 + j;
        }
    }
    cv0171 Lee_Ki;
    edgeImg = Lee_Ki.OpticalFlow(Image1, Image2, 100);

    icMain = Lee_Ki.GetResultIcImg();

    // 문제 있는 부분
    ImageForm*  q_pFormRes = new ImageForm(icMain, "Optical Flow Applied", this);
    int scale = 7;

    for(unsigned i = 0; i < icMain.Row()/scale; i++){
        for(unsigned int j=0; j < icMain.Col()/scale; j++){
            int dx = edgeImg[i*scale]->at(j*scale).dx;
            int dy = edgeImg[i*scale]->at(j*scale).dy;

            q_pFormRes->DrawLine(j*scale, i*scale, j*scale+dx, i*scale+dy, QColor(255, 0, 0), 1);
        }
    }

    _plpImageForm->Add(q_pFormRes);
    q_pFormRes->show();


    //UI 활성화 갱신
    UpdateUI();

    qDebug() << b.size();
}

void MainFrame::on_Apply_Compare_Img_clicked()
{
    KImageGray igMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() && (_q_pFormFocused->ID() == "OPEN"))
        igMain = _q_pFormFocused->ImageGray();
    else
        return;

    ImageForm*  q_pForm = new ImageForm(igMain, "Compare", this);
    _plpImageForm->Add(q_pForm);
    q_pForm->show();


    //UI 활성화 갱신
    UpdateUI();
}


void MainFrame::on_CameraBtn_clicked()
{
    void* handle1;
    void* handle2;
    void* handle3;
    void* handle4;
    void* handle5;

    //handle = CreateCamera("MV-CA013-20GC");            // Create Handle and Open
    handle1 = CreateCamera("Cam01");            // Create Handle and Open
    handle2 = CreateCamera("Cam02");            // Create Handle and Open
    handle3 = CreateCamera("Cam03");            // Create Handle and Open
    handle4 = CreateCamera("Cam04");            // Create Handle and Open
    handle5 = CreateCamera("Cam05");            // Create Handle and Open

    SetExposureAuto(handle1, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle1, 15.0f);                    // set Frame rate

    SetExposureAuto(handle2, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle2, 15.0f);                    // set Frame rate

    SetExposureAuto(handle3, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle3, 15.0f);                    // set Frame rate

    SetExposureAuto(handle4, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle4, 15.0f);                    // set Frame rate

    SetExposureAuto(handle5, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle5, 15.0f);                    // set Frame rate

    // data type : BayerRG8
    MV_CC_SetEnumValue(handle1, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle2, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle3, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle4, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle5, "PixelFormat", 0x01080009);

    GrabCamera(handle1);
    GrabCamera(handle2);
    GrabCamera(handle3);
    GrabCamera(handle4);
    GrabCamera(handle5);

    Mat img1, img2, img3, img4, img5;

    while (true) {
        img1 = GetMatFrame(handle1);                                // get frame
        img2 = GetMatFrame(handle2);
        img3 = GetMatFrame(handle3);
        img4 = GetMatFrame(handle4);
        img5 = GetMatFrame(handle5);
        cv::resize(img1, img1, Size(img1.cols/2, img1.rows/2));        // resize
        cv::resize(img2, img2, Size(img2.cols/2, img2.rows/2));        // resize
        cv::resize(img3, img3, Size(img3.cols/2, img3.rows/2));        // resize
        cv::resize(img4, img4, Size(img4.cols/2, img4.rows/2));        // resize
        cv::resize(img5, img5, Size(img5.cols/2, img5.rows/2));        // resize

        imshow("camera1", img1);
        imshow("camera2", img2);
        imshow("camera3", img3);
        imshow("camera4", img4);
        imshow("camera5", img5);

        char c = (char)waitKey(10);
        if (c == 27) { // ESC
            StopCamera(handle1);                        // stop camera
            StopCamera(handle2);                        // stop camera
            StopCamera(handle3);                        // stop camera
            StopCamera(handle4);                        // stop camera
            StopCamera(handle5);                        // stop camera
            CloseCamera(handle1);                    // close camera
            CloseCamera(handle2);                    // close camera
            CloseCamera(handle3);                    // close camera
            CloseCamera(handle4);                    // close camera
            CloseCamera(handle5);                    // close camera
            break;
        }
    }
}


void MainFrame::on_Checker_Corner_clicked()
{
    int numCornerHor = 9;
    int numCornerVer = 6;
    //int oneSqureLen = 40;
    int numBoards = 3;

    int numSquares = numCornerHor *  numCornerVer;
    Size board_sz = Size(numCornerHor, numCornerVer);

    int success = 0;

    for(int i=0; i<numBoards; i++)
    {
        vector<vector<Point3f>> object_Points;
        vector<vector<Point2f>> image_Points;

        vector<Point2f> corners;
        Mat image;
        Mat gray_image;

        //string iname =  "C:/Users/ironview/Desktop/Base/data/CheckerBoard/";
        string iname =  "C:/Users/gkswn/Desktop/Base/data/CheckerBoard/";
        iname += to_string(success+1);
        iname += ".bmp";

        image = imread(iname);

        vector<Point3d> obj;
        for(int j=0; j<numSquares; j++)
            obj.push_back(Point3d(j/numCornerHor, j%numCornerHor, 0.0f));

        cvtColor(image, gray_image, COLOR_BGR2GRAY);

        bool found = findChessboardCorners(image, board_sz, corners,CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS);

        if(found)
        {
            cornerSubPix(gray_image, corners, Size(11,11), Size(-1,-1), TermCriteria(TermCriteria::MAX_ITER |TermCriteria::EPS, 30, 0.1 ));
            drawChessboardCorners(gray_image, board_sz, corners, found);

            success++;
        }

        cv::resize(gray_image, gray_image, Size(gray_image.cols/2, gray_image.rows/2));

        string winname = "win_";
        winname += to_string(success);
        imshow(winname, gray_image);

        ofstream ofs;
        //string fname = "C:/Users/ironview/Desktop/Base/data/Points/points_";
        string fname = "C:/Users/gkswn/Desktop/Base/data/Points/points_";
        fname += to_string(success);
        fname += ".txt";

        ofs.open(fname);
        if(ofs.fail())
        {
            cout<<"File Error"<<endl;
        }
        //ofs << corners;
        for(int i=0;i<corners.size();i++)
        {
            ofs<<corners.at(i).x << " " << corners.at(i).y << endl;
        }
        ofs.close();
    }
}


