#include <QFileDialog>
#include <QmessageBox>
#include <QPainter>
#include <sstream>
#include <fstream>
#include <QString>
#include <QImageReader>
#include <random>
#include <Windows.h>
#include <iostream>
#include <string>
#include <cmath>

#include "mainframe.h"
#include "ui_mainframe.h"
#include "imageform.h"
#include "rv0171.h"
#include "HikCamera.cpp"
#include "serial.cpp"

#define MY_SERIALPORT  3  // 연결된 시리얼 포트번호
#define SBUF_SIZE 2000

#define __PI 3.141592
#define __2PI 6.283185307179586477

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;
namespace fs = std::filesystem;

string rvdir = "./data/";

using namespace std;

char sbuf[SBUF_SIZE];
signed int sbuf_cnt = 0;

struct Quaternion {
    double w, x, y, z;
};

struct EulerAngles {
    double roll, pitch, yaw;
};

// this implementation assumes normalized quaternion
// converts to Euler angles in 3-2-1 sequence
EulerAngles ToEulerAngles(Quaternion q) {
    EulerAngles angles;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.roll = std::atan2(sinr_cosp, cosr_cosp) * (180.0/_PI); // rad to deg

    // pitch (y-axis rotation)
    double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
    double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
    angles.pitch = 2 * std::atan2(sinp, cosp) - M_PI / 2 * (180.0/_PI); // rad to deg

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.yaw = std::atan2(siny_cosp, cosy_cosp) * (180.0/_PI); // rad to deg

    return angles;
}

char* my_strtok(char * str, char dm, int *result)
{
    int n;

    *result = 0;

    for (n = 0; n<100; n++)
    {
        if (str[n] == dm) { *result = 1;  break; }
        if (str[n] == NULL) break;
    }

    return &str[n+1];
}

int EBimuAsciiParser(int *id, float *item, unsigned int number_of_item)
{
    SERIALREADDATA srd;
    unsigned int n, i;
    char *addr;
    int result = 0;
    int ret;

    //	char *context;

    if (ReadSerialPort(MY_SERIALPORT, &srd) == ERR_OK)	// -7 == 0
    {
        if (srd.nSize) // 100
        {
            for (n = 0; n<srd.nSize; n++)
            {
                //printf("%d ", sbuf_cnt);
                //////////////////////////////////////////////////////////////////////
                sbuf[sbuf_cnt] = srd.szData[n];	//  initial sbuf_cnt = 0
                //printf("%c", srd.szData[n]);
                if (sbuf[sbuf_cnt] == '\r')  // 1줄 수신완료
                {
                    {
                        addr = my_strtok(sbuf, '-' ,&ret);
                        if (ret)
                        {
                            *id = (float)atoi(addr);

                            addr = my_strtok(sbuf, ',', &ret);
                            for (i = 0; i<number_of_item; i++)
                            {
                                item[i] = (float)atof(addr);
                                addr = my_strtok(addr, ',', &ret);
                            }

                            result = 1;
                        }
                    }
                }
                else if (sbuf[sbuf_cnt] == '\n')
                {
                    sbuf_cnt = -1;
                }

                sbuf_cnt++;
                if (sbuf_cnt >= SBUF_SIZE) sbuf_cnt = 0;
                ///////////////////////////////////////////////////////////////////////
            }
        }
    }

    return result;
}

MainFrame::MainFrame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainFrame)
{
    ui->setupUi(this);

    _q_pFormFocused     = 0;

    //객체 맴버의 초기화


    //리스트 출력창을 안보이게
    ui->listWidget->setVisible(false);
    this->adjustSize();

    //UI 활성화 갱신
    UpdateUI();
}

MainFrame::~MainFrame()
{
    delete ui;

    for(auto& item : _lImageForm)
        delete item;

}

void MainFrame::CloseImageForm(ImageForm *pForm)
{
    //ImageForm 포인터 삭제
    unsigned int idx = std::find(_lImageForm.begin(), _lImageForm.end(), pForm) - _lImageForm.begin();
    if(idx != _lImageForm.size())
    {
        delete _lImageForm[idx];
        _lImageForm.erase(_lImageForm.begin() + idx);
    }

    //활성화 ImageForm 초기화
    _q_pFormFocused     = 0;

    //관련 객체 삭제

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::UpdateUI()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {
    }
    else if(ui->tabWidget->currentIndex() == 2)
    {
    }
    else if(ui->tabWidget->currentIndex() == 3)
    {
    }
}

void MainFrame::OnMousePos(const int &nX, const int &nY, ImageForm* q_pForm)
{
}

void MainFrame::focusInEvent(QFocusEvent * event)
{
    Q_UNUSED(event) ;

    UpdateUI();
}

void MainFrame::closeEvent(QCloseEvent* event)
{
    //생성된 ImageForm을 닫는다.
    for(unsigned int i=0; i< _lImageForm.size(); i++)
        delete _lImageForm[i];

    //리스트에서 삭제한다.
    _lImageForm.clear();
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

    _lImageForm.push_back(q_pForm);
    q_pForm->show();

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::on_buttonDeleteContents_clicked()
{
    //생성된 ImageForm을 닫는다.
    for(int i=_lImageForm.size()-1; i>=0; i--)
        delete _lImageForm[i];

    //리스트에서 삭제한다.
    _lImageForm.clear();

    ui->listWidget->clear();
}

void MainFrame::on_tabWidget_currentChanged(int index)
{
    static int nOld = -1;

    if(nOld == 0)
    {

    }
    else if(nOld == 1)
    {

    }
    nOld = index;

    //UI 활성화 갱신
    UpdateUI();
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

int col = 1280;
int row = 1024;

void MainFrame::on_CameraBtn_clicked()
{
    int show_flag = 1;
    int cam_num  = 8;

    void* handle1;
    void* handle2;
    void* handle3;
    void* handle4;
    void* handle5;
    void* handle6;
    void* handle7;
    void* handle8;

    double setFrame = 30.0f;

    handle1 = CreateCamera("Cam01");            // Create Handle and Open
    handle2 = CreateCamera("Cam02");            // Create Handle and Open
    handle3 = CreateCamera("Cam03");            // Create Handle and Open
    handle4 = CreateCamera("Cam04");            // Create Handle and Open
    handle5 = CreateCamera("Cam05");            // Create Handle and Open
    handle6 = CreateCamera("Cam06");            // Create Handle and Open
//    handle7 = CreateCamera("Cam07");            // Create Handle and Open
    handle8 = CreateCamera("Cam08");            // Create Handle and Open

    SetExposureAuto(handle1, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle1, setFrame);                    // set Frame rate

    SetExposureAuto(handle2, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle2, setFrame);                    // set Frame rate

    SetExposureAuto(handle3, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle3, setFrame);                    // set Frame rate

    SetExposureAuto(handle4, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle4, setFrame);                    // set Frame rate

    SetExposureAuto(handle5, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle5, setFrame);                    // set Frame rate

    SetExposureAuto(handle6, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle6, setFrame);                    // set Frame rate

//    SetExposureAuto(handle7, true);                    // set Exposure Auto off
//    //SetExposure(handle, 30000.0f);                    // set Exposure
//    SetFramerate(handle7, setFrame);                    // set Frame rate

    SetExposureAuto(handle8, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle8, setFrame);                    // set Frame rate

    // data type : BayerRG8
    MV_CC_SetEnumValue(handle1, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle2, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle3, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle4, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle5, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle6, "PixelFormat", 0x01080009);
//    MV_CC_SetEnumValue(handle7, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle8, "PixelFormat", 0x01080009);

    GrabCamera(handle1);
    GrabCamera(handle2);
    GrabCamera(handle3);
    GrabCamera(handle4);
    GrabCamera(handle5);
    GrabCamera(handle6);
//    GrabCamera(handle7);
    GrabCamera(handle8);

    Mat img1, img2, img3, img4, img5, img6, img7, img8;
    Mat show_img1, show_img2, show_img3, show_img4, show_img5, show_img6, show_img7, show_img8;

    ImageForm* q_pForm[8] = {0,};
    while (1) {
        img1 = GetMatFrame(handle1);                                // get frame
        img2 = GetMatFrame(handle2);
        img3 = GetMatFrame(handle3);
        img4 = GetMatFrame(handle4);
        img5 = GetMatFrame(handle5);
        img6 = GetMatFrame(handle6);
//        img7 = GetMatFrame(handle7);
        img8 = GetMatFrame(handle8);

        if(show_flag == 1)
        {
            cv::resize(img1, show_img1, Size(img1.cols/2, img1.rows/2));        // resize
            cv::resize(img2, show_img2, Size(img2.cols/2, img2.rows/2));        // resize
            cv::resize(img3, show_img3, Size(img3.cols/2, img3.rows/2));        // resize
            cv::resize(img4, show_img4, Size(img4.cols/2, img4.rows/2));        // resize
            cv::resize(img5, show_img5, Size(img5.cols/2, img5.rows/2));        // resize
            cv::resize(img6, show_img6, Size(img6.cols/2, img6.rows/2));        // resize
//            cv::resize(img7, show_img7, Size(img7.cols/2, img7.rows/2));        // resize
            cv::resize(img8, show_img8, Size(img8.cols/2, img8.rows/2));        // resize

            imshow("camera1", show_img1);
            imshow("camera2", show_img2);
            imshow("camera3", show_img3);
            imshow("camera4", show_img4);
            imshow("camera5", show_img5);
            imshow("camera6", show_img6);
//            imshow("camera7", show_img7);
            imshow("camera8", show_img8);
        }

        char c = (char)waitKey(10);
        if (c == 27)
        { // ESC
            StopCamera(handle1);                        // stop camera
            StopCamera(handle2);                        // stop camera
            StopCamera(handle3);                        // stop camera
            StopCamera(handle4);                        // stop camera
            StopCamera(handle5);                        // stop camera
            StopCamera(handle6);                        // stop camera
//            StopCamera(handle7);                        // stop camera
            StopCamera(handle8);                        // stop camera

            CloseCamera(handle1);                    // close camera
            CloseCamera(handle2);                    // close camera
            CloseCamera(handle3);                    // close camera
            CloseCamera(handle4);                    // close camera
            CloseCamera(handle5);                    // close camera
            CloseCamera(handle6);                    // close camera
//            CloseCamera(handle7);                    // close camera
            CloseCamera(handle8);                    // close camera
            break;
        }
        else if (c ==  32)
        { // SPASE
            imwrite("CamImage/1.bmp", img1);
            imwrite("CamImage/2.bmp", img2);
            imwrite("CamImage/3.bmp", img3);
            imwrite("CamImage/4.bmp", img4);
            imwrite("CamImage/5.bmp", img5);
            imwrite("CamImage/6.bmp", img6);
//            imwrite("CamImage/7.bmp", img7);
            imwrite("CamImage/8.bmp", img8);
        }
    }

}

void MainFrame::on_Checker_Corner_clicked()
{
    int cam_num = 3;
    string direction = "left";

    int numCornerHor = 9;
    int numCornerVer = 6;
    //int oneSqureLen = 40;
    int numBoards = 10;

    int numSquares = numCornerHor *  numCornerVer;
    Size board_sz = Size(numCornerHor, numCornerVer);

    for(int i=0; i<numBoards; i++)
    {
        vector<vector<Point3f>> object_Points;
        vector<vector<Point2f>> image_Points;

        vector<Point2f> corners;
        Mat image;
        Mat gray_image;

        string iname = rvdir + "Streo/cam" + to_string(cam_num) + "/" + direction + "/";
        //string iname = rvdir + "Zhang/cam" + to_string(cam_num) + "/";
        iname += to_string(i+1);
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
        }

        cv::resize(gray_image, gray_image, Size(gray_image.cols/2, gray_image.rows/2));

        string winname = "win_";
        winname += to_string(i+1);
        imshow(winname, gray_image);

        ofstream ofs;
        string fname = rvdir + "Streo/cam" + to_string(cam_num) +  "/" + direction + "_txt/";
//        string fname = rvdir + "Zhang/cam" + to_string(cam_num) + "_txt/";
        fname += to_string(i+1);
        fname += ".txt";

        ofs.open(fname);
        if(ofs.fail())
        {
            cout<<"File Error"<<endl;
        }
        //ofs << corners;
        for(int i=0;i<corners.size();i++)
        {
            ofs<<double(corners.at(i).x)<< " " << corners.at(i).y << endl;
        }
        ofs.close();
    }
}

//////////////////////////////
//// Zhang's Method (HW1) ////
//////////////////////////////
int cal_cam_num = 5;
//string cal_direction = "left";
void MainFrame::on_zhang_clicked()
{
    int nImg = 15;
    int nFeature = 54;
    int nItr = 1000;

    // Load Image & Model Points to vector
    vector<vector<pair<double, double>>> pointzip;
    for(int i = 1; i <= nImg + 1; i++){
        string dir = rvdir;
        if(i != nImg + 1) {
            dir += "Zhang/cam" + to_string(cal_cam_num) + "_txt/";
            dir += to_string(i);
            dir += ".txt";
            pointzip.push_back(rv0171::PointList(dir));
        }
        else {
            dir += "model.txt";
            pointzip.push_back(rv0171::PointList(dir));
        }
    }

    KVector vX;
    vX = rv0171::ZhangsCalibration(pointzip, nImg, nFeature, nItr);

    // Reprojection
    KMatrix mA(3, 3);
    mA[0][0] = vX[0];   // alpha
    mA[0][1] = 0.0;
    mA[1][1] = vX[1];   // beta
    mA[0][2] = vX[2];   // u0
    mA[1][2] = vX[3];   // v0
    mA[2][2] = 1.0;     // scale
    double dK1 = vX[4]; // k1
    double dK2 = vX[5]; // k2
    double dError = vX[vX.Size() - 1];  // vX's last element is Err

    //Calibration Matrix Save
    ofstream ofs;
    string Cal_fname = rvdir + "Caltxt/ZhangSelf/Cal_";
    Cal_fname += to_string(cal_cam_num);
    Cal_fname += ".txt";

    ofs.open(Cal_fname);
    if(ofs.fail())
    {
        cout<<"File Error"<<endl;
    }
    //ofs << corners;
    for(int i=0;i<6;i++)
    {
        ofs<< vX[i] << endl;
    }
    ofs.close();

    //  Print calibration parameters
    QString temp_str;
    temp_str = QString::fromStdString(std::to_string(mA[0][0]));
    ui->textAlpha->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA[0][1]));
    ui->textSkew->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA[0][2]));
    ui->textU0->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA[1][1]));
    ui->textBeta->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA[1][2]));
    ui->textV0->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK1));
    ui->textK1->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK2));
    ui->textK2->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dError));
    ui->textError->setText(temp_str);

    //UI 활성화 갱신
    UpdateUI();
}

// 체커보드 점을 띄운 검은 이미지 출력
void MainFrame::on_plot_point_clicked()
{
    int nImg = 1;
    int nFeature = 54;
    int row = 1024;
    int col = 1280;

    // Load Image & Model Points to vector
    vector<vector<pair<double, double>>> pointzip;
    for(int i = 1; i <= nImg + 1; i++){
        string dir = rvdir;
        if(i != nImg + 1) {
            dir += "Zhang/cam" + to_string(cal_cam_num) + "_txt/";
            dir += to_string(i);
            dir += ".txt";
            pointzip.push_back(rv0171::PointList(dir));
        }
        else {
            dir += "model.txt";
            pointzip.push_back(rv0171::PointList(dir));
        }
    }

    KImageColor Image[nImg + 1];
    ImageForm* q_pForm[nImg + 1];
    for(int i = 0; i < nImg + 1; i++){
        string f_name = "";
        Image[i].Create(row, col);
        if(i != nImg) {
            f_name += "Image";
            f_name += to_string(i + 1);
            q_pForm[i] = new ImageForm(Image[i], QString::fromStdString(f_name), this);
        }
        else{
            f_name += "Model";
            q_pForm[i] = new ImageForm(Image[i], QString::fromStdString(f_name), this);
        }
    }

    for(int i = 0; i < nImg + 1; i++){
        for(int j = 0; j < nFeature; j++){
            q_pForm[i]->DrawLine(pointzip[i][j].first, pointzip[i][j].second, pointzip[i][j].first, pointzip[i][j].second, QColor(255, 0, 0), 2);
        }
    }

    for(int i = 0; i < nImg + 1; i++){
        _lImageForm.push_back(q_pForm[i]);
        q_pForm[i]->show();
    }

    //UI 활성화 갱신
    UpdateUI();
}

//////////////////////////////
// Stereo Calibration (HW2) //
//////////////////////////////
void MainFrame::on_stereo_calibration_clicked()
{
    int left_cam_num = 3;
    int right_cam_num = 38;

    int row = 1024;
    int col = 1280;

    int nImg = 10;
    int nFeature = 54; // Feature점 개수: 100
    int nItr = 100;

    // 1. Capture stereo pairs
    vector<vector<pair<double, double>>> left_pointzip;
    for(int i = 1; i <= nImg + 1; i++){
        string dir = rvdir;
        if(i != nImg + 1) {
            dir += "Streo/cam"+ to_string(left_cam_num) + "/left_txt/";
            dir += to_string(i);
            dir += ".txt";
            left_pointzip.push_back(rv0171::PointList(dir));
        }
        else {
            dir += "model.txt";
            left_pointzip.push_back(rv0171::PointList(dir));
        }
    }

    vector<vector<pair<double, double>>> right_pointzip;
    for(int i = 1; i <= nImg + 1; i++){
        string dir = rvdir;
        if(i != nImg + 1) {
            dir += "Streo/cam" + to_string(right_cam_num) + "/right_txt/";
            dir += to_string(i);
            dir += ".txt";
            right_pointzip.push_back(rv0171::PointList(dir));
        }
        else {
            dir += "model.txt";
            right_pointzip.push_back(rv0171::PointList(dir));
        }
    }

    KVector vX_l, vX_r;
    // 2. Calibrate separately the two cameras using Zhang's method
    vX_l = rv0171::ZhangsCalibration(left_pointzip, nImg, nFeature, nItr);
    vX_r = rv0171::ZhangsCalibration(right_pointzip, nImg, nFeature, nItr);

    //Calibration Matrix Save
    ofstream ofs_l;
    string Cal_fname_l = rvdir + "Caltxt/Cal_";
    Cal_fname_l += to_string(left_cam_num);
    Cal_fname_l += to_string(right_cam_num);
    Cal_fname_l += ".txt";

    ofs_l.open(Cal_fname_l);
    if(ofs_l.fail())
    {
        cout<<"File Error"<<endl;
    }
    //ofs << corners;
    for(int i=0;i<6;i++)
    {
        ofs_l<< vX_l[i] << endl;
    }
    ofs_l.close();

//    //Calibration Matrix Save
//    ofstream ofs_r;
//    string Cal_fname_r = rvdir + "Caltxt/Cal_";
//    Cal_fname_r += to_string(right_cam_num);
//    Cal_fname_r += "_right.txt";

//    ofs_r.open(Cal_fname_r);
//    if(ofs_r.fail())
//    {
//        cout<<"File Error"<<endl;
//    }
//    //ofs << corners;
//    for(int i=0;i<6;i++)
//    {
//        ofs_r<< vX_r[i] << endl;
//    }
//    ofs_r.close();

    KMatrix mA_l(3, 3); // left camera intrinsic matrix
    //mA_l = rv0171::Import_mAi(left_cam_num);
    mA_l[0][0] = vX_l[0];
    mA_l[1][1] = vX_l[1];
    mA_l[0][2] = vX_l[2];
    mA_l[1][2] = vX_l[3];
    mA_l[2][2] = 1.0;
    double dK1_l = vX_l[4];
    double dK2_l = vX_l[5];
    double dError_l = vX_l[vX_l.Size() - 1];  // vX's last element is Err

    //  Print left calibration parameters
    QString temp_str;
    temp_str = QString::fromStdString(std::to_string(mA_l[0][0]));
    ui->textAlphaCam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_l[0][1]));
    ui->textSkewCam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_l[0][2]));
    ui->textU0Cam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_l[1][1]));
    ui->textBetaCam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_l[1][2]));
    ui->textV0Cam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK1_l));
    ui->textK1Cam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK2_l));
    ui->textK2Cam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dError_l));
    ui->textErrorCam01->setText(temp_str);

    KMatrix mA_r(3, 3);// right camera intrinsic matrix
    //mA_r = rv0171::Import_mAi(right_cam_num);
    mA_r[0][0] = vX_r[0];
    mA_r[1][1] = vX_r[1];
    mA_r[0][2] = vX_r[2];
    mA_r[1][2] = vX_r[3];
    mA_r[2][2] = 1.0;
    double dK1_r = vX_r[4];
    double dK2_r = vX_r[5];
    double dError_r = vX_r[vX_r.Size() - 1];  // vX's last element is Err

    KHomogeneous* hRw; // hRw
    hRw = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vX_r[6*(i+1)], vX_r[6*(i+1)+1], vX_r[6*(i+1)+2]);
        KVector   vT(vX_r[6*(i+1)+3], vX_r[6*(i+1)+4], vX_r[6*(i+1)+5]);
        hRw[i] = KHomogeneous(rR, vT);      // 4x4 matrix, W->TR
    }

    // Print right calibration parameters
    temp_str = QString::fromStdString(std::to_string(mA_r[0][0]));
    ui->textAlphaCam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_r[0][1]));
    ui->textSkewCam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_r[0][2]));
    ui->textU0Cam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_r[1][1]));
    ui->textBetaCam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_r[1][2]));
    ui->textV0Cam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK1_r));
    ui->textK1Cam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK2_r));
    ui->textK2Cam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dError_r));
    ui->textErrorCam02->setText(temp_str);

    // 3. Stereo Calibration
    // vX: 6*1 matrix, {r1, r2, r3, t1, t2, t3}: 호모그래피를 평균 낸 것 사용
    KVector vX = rv0171::StereoCalibration(vX_l, vX_r, left_pointzip, right_pointzip, nImg, nFeature, nItr);    // hLr

    KRotation rR;
    rR.FromRodrigues(vX[0], vX[1], vX[2]);
    KVector vT(vX[3], vX[4], vX[5]);
    KHomogeneous hLr = KHomogeneous(rR, vT);

    //RT save
    ofstream ofs;
    string RT_fname = rvdir + "RTtxt/RT_";
    RT_fname += to_string(left_cam_num);
    RT_fname += to_string(right_cam_num);
    RT_fname += ".txt";

    ofs.open(RT_fname);
    if(ofs.fail())
    {
        cout<<"File Error"<<endl;
    }
    //ofs << corners;
    for(int i=0;i<6;i++)
    {
        ofs<< vX[i] << endl;
    }
    ofs.close();

    // Print right calibration parameters
    temp_str = QString::fromStdString(std::to_string(vX[0]));
    ui->textR1->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(vX[1]));
    ui->textR2->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(vX[2]));
    ui->textR3->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(vX[3]));
    ui->textT1->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(vX[4]));
    ui->textT2->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(vX[5]));
    ui->textT3->setText(temp_str);

    // 4. Project Right points to Left points (for verification)
    // Model points
    KMatrix mM;             // 3 x mFeature matrix
    for (int i = 0; i < nFeature; i++) {
        mM |= KVector(left_pointzip[nImg][i].first, left_pointzip[nImg][i].second, 1.0);             // Add a column
    }

    // 출력을 위한 ImageForm 생성
    ImageForm* q_pForm[nImg] = {0,};
    string wName[] = {"left1", "left2", "left3", "left4", "left5", "left6", "left7", "left8", "left9", "left10"};
    for (int i = 0; i < nImg; i++) {
        string sImgName = rvdir;
        sImgName += "Streo/cam" + to_string(left_cam_num) +"/left/";
        sImgName += to_string(i + 1);
        sImgName += ".bmp";

        KImageColor icMain(row, col);
        QImage* _img = new QImage();        //이미지를 로드하기 위한 QImage 선언
        if (_img->load(QString::fromStdString(sImgName))) {
            for (int ii = 0; ii < row; ii++) {
                for (int jj = 0; jj < col; jj++) {
                    QColor color = _img->pixelColor(jj,ii);
                    icMain[ii][jj].r = color.red();
                    icMain[ii][jj].g = color.green();
                    icMain[ii][jj].b = color.blue();
                }
            }
        }
        else {
            qDebug() << "Image load Error";
        }

        KHomogeneous hP = hLr * hRw[i];

        // Display window
        if (q_pForm[i]) {
            // 이미지 창이 이미 존재하면 아무것도 하지 않음
        }
        else {
            q_pForm[i] = new ImageForm(icMain, QString::fromStdString(wName[i]), this);

            for(int j = 0; j < nFeature; j++) {
                // Real point
                QPoint point;
                point.setX((int)(left_pointzip[i][j].first));
                point.setY((int)(left_pointzip[i][j].second));
                QColor color;
                color.setRed(255);
                color.setGreen(0);
                color.setBlue(0);
                q_pForm[i]->DrawEllipse(point, 3, 3, color);

                // Projection
                KVector vU = rv0171::PointProjection(mA_l, KVector(dK1_l, dK2_l), hP, mM.Column(j));

                // Reprojected point
                QPoint rePoint;
                rePoint.setX((int)(vU[__X]));
                rePoint.setY((int)(vU[__Y]));
                QColor reColor;
                reColor.setRed(0);
                reColor.setGreen(0);
                reColor.setBlue(255);
                q_pForm[i]->DrawEllipse(rePoint, 3, 3, reColor);
            }
            _lImageForm.push_back(q_pForm[i]);
            q_pForm[i]->show();
        }
    }

    cout << "\nStereo Calibration Process Finished!" << endl;

    // UI 활성화 갱신
    UpdateUI();

    delete[] hRw;
}

void MainFrame::on_pushRtMatrix_clicked()
{
    int left_cam_num = 1;
    int right_cam_num = 18;

    string RT_path = rvdir + "RTtxt/RT_";
    RT_path += to_string(left_cam_num);
    RT_path += to_string(right_cam_num);
    RT_path += ".txt";

    string str;
    double buf[6] = {0,};
    ifstream RT_fname(RT_path);
    if (RT_fname.is_open()) {
        int i =0;
        while (getline(RT_fname, str))
        {
            buf[i] = stod(str);
            i++;
        }
    }
    else {
        qDebug() << "File open error!\n";
    }
    RT_fname.close();

    KRotation Rt;
    Rt.FromRodrigues(buf[0], buf[1], buf[2]);
    cout <<  "RT_" << left_cam_num << right_cam_num << "_Matrix =" <<endl;
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            cout<< Rt[i][j] << " ";
        }
        cout << endl;
    }

    cout << "t1:" << buf[3] << " t2:" << buf[4] << " t3:" << buf[5] << endl;
}

void MainFrame::on_RT_product_clicked()
{
    int left_cam_num = 3;
    int main_cam_num = 1;
    int right_cam_num = 18;

    //T12
    string RT_path12 = rvdir + "RTtxt/RT_";
    RT_path12 += to_string(left_cam_num);
    RT_path12 += to_string(main_cam_num);
    RT_path12 += ".txt";

    string str12;
    double buf12[6] = {0,};
    ifstream RT_fname12(RT_path12);
    if (RT_fname12.is_open()) {
        int i =0;
        while (getline(RT_fname12, str12))
        {
            buf12[i] = stod(str12);
            i++;
        }
    }
    else {
        qDebug() << "File open error!\n";
    }
    RT_fname12.close();

    KRotation R12;
    R12.FromRodrigues(buf12[0], buf12[1], buf12[2]);

    // T23
    string RT_path23 = rvdir + "RTtxt/RT_";
    RT_path23 += to_string(main_cam_num);
    RT_path23 += to_string(right_cam_num);
    RT_path23 += ".txt";

    string str23;
    double buf23[6] = {0,};
    ifstream RT_fname23(RT_path23);
    if (RT_fname23.is_open()) {
        int i =0;
        while (getline(RT_fname23, str23))
        {
            buf23[i] = stod(str23);
            i++;
        }
    }
    else {
        qDebug() << "File open error!\n";
    }
    RT_fname23.close();

    KRotation R23;
    R23.FromRodrigues(buf23[0], buf23[1], buf23[2]);

    // T13
    KRotation R13;
    R13 = R12 * R23;

    KMatrix RT12(3,3);
    KMatrix RT23(3,3);
    KMatrix RT13(4,4);

    RT12 = KMatrix(R12);
    RT23 = KMatrix(R23);

    RT12 |= KVector(buf12[3],buf12[4],buf12[5]);
    RT12 ^= KVector(0,0,0,1).Tr();
    RT23 |= KVector(buf23[3],buf23[4],buf23[5]);
    RT23 ^= KVector(0,0,0,1).Tr();

    RT13 = RT12 * RT23; // 4x4

    KVector vT13;
    vT13.Tailed(RT13[0][3]); vT13.Tailed(RT13[1][3]); vT13.Tailed(RT13[2][3]); // 아래로붙임 3x1

    KRotation rR13(R13); //3x3
    KHomogeneous hP(rR13, vT13); //4x4

    KVector vX;
    vX.Tailed(hP.RT(_RODRIGUES));

    for(int i = 0; i < vX.Size(); i++)
    {
        cout << "i: " << vX[i] << " ";
    }
    cout << endl;

    //RT save
    ofstream ofs;
    string RT_fname = rvdir + "RTtxt/RT_";
    RT_fname += to_string(left_cam_num);
    RT_fname += to_string(right_cam_num);
    RT_fname += ".txt";

    ofs.open(RT_fname);
    if(ofs.fail())
    {
        cout<<"File Error"<<endl;
    }
    //ofs << corners;
    for(int i=0;i<6;i++)
    {
        ofs<< vX[i] << endl;
    }

    ofs.close();

    // 만들어진 T확인하기
    cout << "RT:" << endl;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            cout<< RT13[i][j] << " ";
        }
        cout << endl;
    }

}

void MainFrame::on_pushPanorama_clicked()
{
    Mat matImage1;
    Mat matImage2;
    Mat matImage3;
    Mat matImage4;

    Mat result1;
    Mat result2;
    Mat result3;

    //가운데 image가 중심이므로 가운데 image를 기준으로 좌/우에 image stitching
    matImage1 = imread("./data/realtime/1.bmp", IMREAD_COLOR);
    matImage2 = imread("./data/realtime/3.bmp", IMREAD_COLOR);
//    matImage3 = imread("./data/Cam123/main3.bmp", IMREAD_COLOR);
//    matImage4 = imread("./data/Cam123/right3.bmp", IMREAD_COLOR);

    //if (matImage1.empty() || matImage2.empty() || matImage3.empty()) return -1;

    //flip(matImage1, result2, 1);
    //flip(matImage2, result3, 1);
    result1 = makePanorama(matImage1, matImage2);
    //flip(result1, result, 1);
    //result2 = makePanorama(matImage4, matImage3);
    //result3 = makePanorama(result1, result2);

    imshow("Result", result1);
    //imshow("Result2", result2);
    //imshow("Result3", result3);
}

void MainFrame::on_Cylinderical_Warp_clicked()
{
    //Image
    KImageColor Img(1024,1280);

    Mat img1 = imread("./data/realtime/1.bmp");
    Mat img2 = imread("./data/realtime/2.bmp");
    Mat img3 = imread("./data/realtime/3.bmp");
    Mat img4 = imread("./data/realtime/4.bmp");
    Mat img5 = imread("./data/realtime/5.bmp");
    Mat img6 = imread("./data/realtime/6.bmp");
    ///////////////////////////////R,t 불러오기//////////////////////////////////
    KRotation rR4_458;
    rR4_458 = rv0171::Import_rRi(4, 48);
    KVector vT4_458;
    vT4_458 = rv0171::Import_vTi(4, 48);

    KRotation rR5_458;
    rR5_458 = rv0171::Import_rRi(5, 58);
    KVector vT5_458;
    vT5_458 = rv0171::Import_vTi(5, 58);

    KRotation rR6_618;
    rR6_618 = rv0171::Import_rRi(6, 68);
    KVector vT6_618;
    vT6_618 = rv0171::Import_vTi(6, 68);

    KRotation rR1_618;
    rR1_618 = rv0171::Import_rRi(1, 18);
    KVector vT1_618;
    vT1_618 = rv0171::Import_vTi(1, 18);

    KRotation rR2_238;
    rR2_238 = rv0171::Import_rRi(2, 28);
    KVector vT2_238;
    vT2_238 = rv0171::Import_vTi(2, 28);

    KRotation rR3_238;
    rR3_238 = rv0171::Import_rRi(3, 38);
    KVector vT3_238;
    vT3_238 = rv0171::Import_vTi(3, 38);
    ///////////////////////////////Surround View Stitching//////////////////////////////////
    KMatrix mA(3,3);
    mA[0][0] = 950; //fi
    mA[1][1] = 950; //fi
    mA[2][2] = 1.0;
    mA[0][2] = 0;
    mA[1][2] = 0;

    //Image 4_458
    vector<vector<KVector>> vvXi_tilt4_458;
    vvXi_tilt4_458 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA4_458(3,3);
    mA4_458 = rv0171::Import_mAi(4, 48);

    rv0171::make_Surround_View_Stitching(mA, mA4_458, rR4_458, vT4_458, vvXi_tilt4_458);

    //cylinderical warping
    rv0171::make_Cylinderical_Warp(mA, vvXi_tilt4_458);

    cout << vvXi_tilt4_458.at(0).at(0)._ppA[0][0] << " " << vvXi_tilt4_458.at(0).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt4_458.at(0).at(1279)._ppA[0][0] << " " << vvXi_tilt4_458.at(0).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt4_458.at(1023).at(0)._ppA[0][0] << " " << vvXi_tilt4_458.at(1023).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt4_458.at(1023).at(1279)._ppA[0][0] << " " << vvXi_tilt4_458.at(1023).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << "Image 4_458 End"<<endl;
    cout << endl;

    //Image 5_458
    vector<vector<KVector>> vvXi_tilt5_458;
    vvXi_tilt5_458 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA5_458(3,3);
    mA5_458 = rv0171::Import_mAi(5,58);

    rv0171::make_Surround_View_Stitching(mA, mA5_458, rR5_458, vT5_458, vvXi_tilt5_458);

    //cylinderical warping
    rv0171::make_Cylinderical_Warp(mA, vvXi_tilt5_458);

    cout << vvXi_tilt5_458.at(0).at(0)._ppA[0][0] << " " << vvXi_tilt5_458.at(0).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt5_458.at(0).at(1279)._ppA[0][0] << " " << vvXi_tilt5_458.at(0).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt5_458.at(1023).at(0)._ppA[0][0] << " " << vvXi_tilt5_458.at(1023).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt5_458.at(1023).at(1279)._ppA[0][0] << " " << vvXi_tilt5_458.at(1023).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << "Image 5_458 End" << endl;
    cout << endl;

    //Image 6_618
    vector<vector<KVector>> vvXi_tilt6_618;
    vvXi_tilt6_618 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA6_618(3,3);
    mA6_618 = rv0171::Import_mAi(6, 68);

    rv0171::make_Surround_View_Stitching(mA, mA6_618, rR6_618, vT6_618, vvXi_tilt6_618);

    //cylinderical warping
    rv0171::make_Cylinderical_Warp(mA, vvXi_tilt6_618);

    cout << vvXi_tilt6_618.at(0).at(0)._ppA[0][0] << " " << vvXi_tilt6_618.at(0).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt6_618.at(0).at(1279)._ppA[0][0] << " " << vvXi_tilt6_618.at(0).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt6_618.at(1023).at(0)._ppA[0][0] << " " << vvXi_tilt6_618.at(1023).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt6_618.at(1023).at(1279)._ppA[0][0] << " " << vvXi_tilt6_618.at(1023).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << "Image 6_618 End" << endl;
    cout << endl;

    //Image 1_618
    vector<vector<KVector>> vvXi_tilt1_618;
    vvXi_tilt1_618 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA1_618(3,3);
    mA1_618 = rv0171::Import_mAi(1, 18);

    rv0171::make_Surround_View_Stitching(mA, mA1_618, rR1_618, vT1_618, vvXi_tilt1_618);

    //cylinderical warping
    rv0171::make_Cylinderical_Warp(mA, vvXi_tilt1_618);

    cout << vvXi_tilt1_618.at(0).at(0)._ppA[0][0] << " " << vvXi_tilt1_618.at(0).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt1_618.at(0).at(1279)._ppA[0][0] << " " << vvXi_tilt1_618.at(0).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt1_618.at(1023).at(0)._ppA[0][0] << " " << vvXi_tilt1_618.at(1023).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt1_618.at(1023).at(1279)._ppA[0][0] << " " << vvXi_tilt1_618.at(1023).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << "Image 1_618 End" <<endl;
    cout << endl;

    //Image 2_238
    vector<vector<KVector>> vvXi_tilt2_238;
    vvXi_tilt2_238 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA2_238(3,3);
    mA2_238 = rv0171::Import_mAi(2, 28);

    rv0171::make_Surround_View_Stitching(mA, mA2_238, rR2_238, vT2_238, vvXi_tilt2_238);

    // cylinderical warping
    rv0171::make_Cylinderical_Warp(mA, vvXi_tilt2_238);

    cout << vvXi_tilt2_238.at(0).at(0)._ppA[0][0]<< " " << vvXi_tilt2_238.at(0).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt2_238.at(0).at(1279)._ppA[0][0] << " " << vvXi_tilt2_238.at(0).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt2_238.at(1023).at(0)._ppA[0][0] << " " << vvXi_tilt2_238.at(1023).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt2_238.at(1023).at(1279)._ppA[0][0] << " " << vvXi_tilt2_238.at(1023).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << "Image 2_238 End" << endl;
    cout << endl;

    //Image 3_238
    vector<vector<KVector>> vvXi_tilt3_238;
    vvXi_tilt3_238 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA3_238(3,3);
    mA3_238 = rv0171::Import_mAi(3, 38);

    rv0171::make_Surround_View_Stitching(mA, mA3_238, rR3_238, vT3_238, vvXi_tilt3_238);

    //cylinderical warping
    rv0171::make_Cylinderical_Warp(mA, vvXi_tilt3_238);

    cout << vvXi_tilt3_238.at(0).at(0)._ppA[0][0] << " " << vvXi_tilt3_238.at(0).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt3_238.at(0).at(1279)._ppA[0][0] << " " << vvXi_tilt3_238.at(0).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt3_238.at(1023).at(0)._ppA[0][0] << " " << vvXi_tilt3_238.at(1023).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt3_238.at(1023).at(1279)._ppA[0][0] << " " << vvXi_tilt3_238.at(1023).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << "Image 3_238 End" <<endl;
    cout << endl;

    Mat icMain(1024*2, 1280*5, CV_8UC3);
    Mat icMain_show;

    //IMU
    Mat icKernel(850, 1280, CV_8UC3);

    int id;
    float item[100];

    if (OpenSerialPort(MY_SERIALPORT, 115200, NOPARITY, 8, ONESTOPBIT) != ERR_OK)
    {
        printf("\n\rSerialport Error...");
        Sleep(2000);
    }

    Quaternion q;
    EulerAngles e;

    QPoint mp4_458;
    QPoint mp5_458;

    QPoint mp6_618;
    QPoint mp1_618;

    QPoint mp2_238;
    QPoint mp3_238;

    for(int i=0; i<1024; i++)
    {
        for(int j=0; j<1280; j++)
        {
            mp4_458.setX((int)(vvXi_tilt4_458.at(i).at(j)._ppA[0][0])+1000);
            mp4_458.setY((int)(vvXi_tilt4_458.at(i).at(j)._ppA[1][0])+1000);
            icMain.at<Vec3b>(mp4_458.y(), mp4_458.x()) = img4.at<Vec3b>(i,j);

            mp5_458.setX((int)(vvXi_tilt5_458.at(i).at(j)._ppA[0][0])+1000);
            mp5_458.setY((int)(vvXi_tilt5_458.at(i).at(j)._ppA[1][0])+1000);
            icMain.at<Vec3b>(mp5_458.y(), mp5_458.x()) = img5.at<Vec3b>(i,j);

            mp6_618.setX((int)(vvXi_tilt6_618.at(i).at(j)._ppA[0][0])+1000+mA[0][0]*_RADIAN(90));
            mp6_618.setY((int)(vvXi_tilt6_618.at(i).at(j)._ppA[1][0])+1000);
            icMain.at<Vec3b>(mp6_618.y(), mp6_618.x()) = img6.at<Vec3b>(i,j);

            mp1_618.setX((int)(vvXi_tilt1_618.at(i).at(j)._ppA[0][0])+1000+mA[0][0]*_RADIAN(90));
            mp1_618.setY((int)(vvXi_tilt1_618.at(i).at(j)._ppA[1][0])+1000);
            icMain.at<Vec3b>(mp1_618.y(), mp1_618.x()) = img1.at<Vec3b>(i,j);

            mp2_238.setX((int)(vvXi_tilt2_238.at(i).at(j)._ppA[0][0])+1000+mA[0][0]*_RADIAN(90*2));
            mp2_238.setY((int)(vvXi_tilt2_238.at(i).at(j)._ppA[1][0])+1000);
            icMain.at<Vec3b>(mp2_238.y(), mp2_238.x()) = img2.at<Vec3b>(i,j);

            mp3_238.setX((int)(vvXi_tilt3_238.at(i).at(j)._ppA[0][0])+1000+mA[0][0]*_RADIAN(90*2));
            mp3_238.setY((int)(vvXi_tilt3_238.at(i).at(j)._ppA[1][0])+1000);
            icMain.at<Vec3b>(mp3_238.y(), mp3_238.x()) = img3.at<Vec3b>(i,j);
        }
    }

    //UI 활성화 갱신
    cout << "cylinderical Process Finished!" << endl;

    cv::resize(icMain, icMain_show, Size(icMain.cols/2, icMain.rows/2));        // resize
    imshow("icMain", icMain_show);

//    //IMU
//    while(1)
//    {
//        if (EBimuAsciiParser(&id,item, 5))
//        {
//            q.z = item[0];
//            q.y = item[1];
//            q.x = item[2];
//            q.w = item[3];

//            e = ToEulerAngles(q);
//            cout << "Yaw: " << e.yaw <<endl;

//            int data_row = (int)((e.yaw + 180)*(icMain.cols/360));
//            //cout << "data_row: " << data_row << endl;

//            for(int c = 0; c < icKernel.cols; c++)
//            {
//                for(int r = 0; r < icKernel.rows; r++)
//                {
//                    if(data_row > icMain.cols)
//                    {
//                        icKernel.at<Vec3b>(r, c) = icMain.at<Vec3b>(r+600, data_row + c - icMain.cols);
//                    }
//                    else
//                    {
//                        icKernel.at<Vec3b>(r, c) = icMain.at<Vec3b>(r+600, data_row + c);
//                    }
//                }
//            }
//            imshow("Kernel Image", icKernel);

//            char ch = waitKey(10);
//            if(ch == 27)
//                break;       // 27 == ESC key
//        }
//    }
}

void MainFrame::on_IMUButton_clicked()
{
    int id;
    float item[100];

    if (OpenSerialPort(MY_SERIALPORT, 115200, NOPARITY, 8, ONESTOPBIT) != ERR_OK)
    {
        printf("\n\rSerialport Error...");
        Sleep(2000);
    }

    Quaternion q;
    EulerAngles e;

    while (1)
    {
        if (EBimuAsciiParser(&id,item, 5))
        {
            q.z = item[0];
            q.y = item[1];
            q.x = item[2];
            q.w = item[3];
            printf("Quaternion: z:%f y:%f x:%f w:%f,  ", q.z, q.y, q.x, q.w);

            e = ToEulerAngles(q);
            printf("Yaw: %f\n", e.yaw);
        }
    }
}


void MainFrame::on_pushRealTime_clicked()
{
    int show_flag = 0;

    void* handle1;
    void* handle2;
    void* handle3;
    void* handle4;
    void* handle5;
    void* handle6;
//    void* handle7;
//    void* handle8;

    double setFrame = 30.0f;

    handle1 = CreateCamera("Cam01");            // Create Handle and Open
    handle2 = CreateCamera("Cam02");            // Create Handle and Open
    handle3 = CreateCamera("Cam03");            // Create Handle and Open
    handle4 = CreateCamera("Cam04");            // Create Handle and Open
    handle5 = CreateCamera("Cam05");            // Create Handle and Open
    handle6 = CreateCamera("Cam06");            // Create Handle and Open
//    handle7 = CreateCamera("Cam07");            // Create Handle and Open
//    handle8 = CreateCamera("Cam08");            // Create Handle and Open

    SetExposureAuto(handle1, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle1, setFrame);                    // set Frame rate

    SetExposureAuto(handle2, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle2, setFrame);                    // set Frame rate

    SetExposureAuto(handle3, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle3, setFrame);                    // set Frame rate

    SetExposureAuto(handle4, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle4, setFrame);                    // set Frame rate

    SetExposureAuto(handle5, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle5, setFrame);                    // set Frame rate

    SetExposureAuto(handle6, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle6, setFrame);                    // set Frame rate

//    SetExposureAuto(handle7, true);                    // set Exposure Auto off
//    //SetExposure(handle, 30000.0f);                    // set Exposure
//    SetFramerate(handle7, setFrame);                    // set Frame rate

//    SetExposureAuto(handle8, true);                    // set Exposure Auto off
//    //SetExposure(handle, 30000.0f);                    // set Exposure
//    SetFramerate(handle8, setFrame);                    // set Frame rate

    // data type : BayerRG8
    MV_CC_SetEnumValue(handle1, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle2, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle3, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle4, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle5, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle6, "PixelFormat", 0x01080009);
//    MV_CC_SetEnumValue(handle7, "PixelFormat", 0x01080009);
//    MV_CC_SetEnumValue(handle8, "PixelFormat", 0x01080009);

    GrabCamera(handle1);
    GrabCamera(handle2);
    GrabCamera(handle3);
    GrabCamera(handle4);
    GrabCamera(handle5);
    GrabCamera(handle6);
//    GrabCamera(handle7);
//    GrabCamera(handle8);

    Mat img1, img2, img3, img4, img5, img6, img7, img8;
    Mat show_img1, show_img2, show_img3, show_img4, show_img5, show_img6, show_img7, show_img8;

    //Cylindrical Warp
    KImageColor Img(1024,1280);

    ///////////////////////////////R,t 불러오기//////////////////////////////////
    KRotation rR4_458;
    rR4_458 = rv0171::Import_rRi(4, 48);
    KVector vT4_458;
    vT4_458 = rv0171::Import_vTi(4, 48);

    KRotation rR5_458;
    rR5_458 = rv0171::Import_rRi(5, 58);
    KVector vT5_458;
    vT5_458 = rv0171::Import_vTi(5, 58);

    KRotation rR6_618;
    rR6_618 = rv0171::Import_rRi(6, 68);
    KVector vT6_618;
    vT6_618 = rv0171::Import_vTi(6, 68);

    KRotation rR1_618;
    rR1_618 = rv0171::Import_rRi(1, 18);
    KVector vT1_618;
    vT1_618 = rv0171::Import_vTi(1, 18);

    KRotation rR2_238;
    rR2_238 = rv0171::Import_rRi(2, 28);
    KVector vT2_238;
    vT2_238 = rv0171::Import_vTi(2, 28);

    KRotation rR3_238;
    rR3_238 = rv0171::Import_rRi(3, 38);
    KVector vT3_238;
    vT3_238 = rv0171::Import_vTi(3, 38);
    ///////////////////////////////Surround View Stitching//////////////////////////////////
    KMatrix mA(3,3);
    mA[0][0] = 950; //fi
    mA[1][1] = 950; //fi
    mA[2][2] = 1.0;
    mA[0][2] = 0;
    mA[1][2] = 0;

    //Image 4_458
    vector<vector<KVector>> vvXi_tilt4_458;
    vvXi_tilt4_458 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA4_458(3,3);
    mA4_458 = rv0171::Import_mAi(4, 48);

    rv0171::make_Surround_View_Stitching(mA, mA4_458, rR4_458, vT4_458, vvXi_tilt4_458);

    //cylinderical warping
    rv0171::make_Cylinderical_Warp(mA, vvXi_tilt4_458);

    cout << vvXi_tilt4_458.at(0).at(0)._ppA[0][0] << " " << vvXi_tilt4_458.at(0).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt4_458.at(0).at(1279)._ppA[0][0] << " " << vvXi_tilt4_458.at(0).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt4_458.at(1023).at(0)._ppA[0][0] << " " << vvXi_tilt4_458.at(1023).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt4_458.at(1023).at(1279)._ppA[0][0] << " " << vvXi_tilt4_458.at(1023).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << "Image 4_458 End"<<endl;
    cout << endl;

    //Image 5_458
    vector<vector<KVector>> vvXi_tilt5_458;
    vvXi_tilt5_458 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA5_458(3,3);
    mA5_458 = rv0171::Import_mAi(5,58);

    rv0171::make_Surround_View_Stitching(mA, mA5_458, rR5_458, vT5_458, vvXi_tilt5_458);

    //cylinderical warping
    rv0171::make_Cylinderical_Warp(mA, vvXi_tilt5_458);

    cout << vvXi_tilt5_458.at(0).at(0)._ppA[0][0] << " " << vvXi_tilt5_458.at(0).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt5_458.at(0).at(1279)._ppA[0][0] << " " << vvXi_tilt5_458.at(0).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt5_458.at(1023).at(0)._ppA[0][0] << " " << vvXi_tilt5_458.at(1023).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt5_458.at(1023).at(1279)._ppA[0][0] << " " << vvXi_tilt5_458.at(1023).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << "Image 5_458 End" << endl;
    cout << endl;

    //Image 6_618
    vector<vector<KVector>> vvXi_tilt6_618;
    vvXi_tilt6_618 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA6_618(3,3);
    mA6_618 = rv0171::Import_mAi(6, 68);

    rv0171::make_Surround_View_Stitching(mA, mA6_618, rR6_618, vT6_618, vvXi_tilt6_618);

    //cylinderical warping
    rv0171::make_Cylinderical_Warp(mA, vvXi_tilt6_618);

    cout << vvXi_tilt6_618.at(0).at(0)._ppA[0][0] << " " << vvXi_tilt6_618.at(0).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt6_618.at(0).at(1279)._ppA[0][0] << " " << vvXi_tilt6_618.at(0).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt6_618.at(1023).at(0)._ppA[0][0] << " " << vvXi_tilt6_618.at(1023).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt6_618.at(1023).at(1279)._ppA[0][0] << " " << vvXi_tilt6_618.at(1023).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << "Image 6_618 End" << endl;
    cout << endl;

    //Image 1_618
    vector<vector<KVector>> vvXi_tilt1_618;
    vvXi_tilt1_618 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA1_618(3,3);
    mA1_618 = rv0171::Import_mAi(1, 18);

    rv0171::make_Surround_View_Stitching(mA, mA1_618, rR1_618, vT1_618, vvXi_tilt1_618);

    //cylinderical warping
    rv0171::make_Cylinderical_Warp(mA, vvXi_tilt1_618);

    cout << vvXi_tilt1_618.at(0).at(0)._ppA[0][0] << " " << vvXi_tilt1_618.at(0).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt1_618.at(0).at(1279)._ppA[0][0] << " " << vvXi_tilt1_618.at(0).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt1_618.at(1023).at(0)._ppA[0][0] << " " << vvXi_tilt1_618.at(1023).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt1_618.at(1023).at(1279)._ppA[0][0] << " " << vvXi_tilt1_618.at(1023).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << "Image 1_618 End" <<endl;
    cout << endl;

    //Image 2_238
    vector<vector<KVector>> vvXi_tilt2_238;
    vvXi_tilt2_238 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA2_238(3,3);
    mA2_238 = rv0171::Import_mAi(2, 28);

    rv0171::make_Surround_View_Stitching(mA, mA2_238, rR2_238, vT2_238, vvXi_tilt2_238);

    // cylinderical warping
    rv0171::make_Cylinderical_Warp(mA, vvXi_tilt2_238);

    cout << vvXi_tilt2_238.at(0).at(0)._ppA[0][0]<< " " << vvXi_tilt2_238.at(0).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt2_238.at(0).at(1279)._ppA[0][0] << " " << vvXi_tilt2_238.at(0).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt2_238.at(1023).at(0)._ppA[0][0] << " " << vvXi_tilt2_238.at(1023).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt2_238.at(1023).at(1279)._ppA[0][0] << " " << vvXi_tilt2_238.at(1023).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << "Image 2_238 End" << endl;
    cout << endl;

    //Image 3_238
    vector<vector<KVector>> vvXi_tilt3_238;
    vvXi_tilt3_238 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA3_238(3,3);
    mA3_238 = rv0171::Import_mAi(3, 38);

    rv0171::make_Surround_View_Stitching(mA, mA3_238, rR3_238, vT3_238, vvXi_tilt3_238);

    //cylinderical warping
    rv0171::make_Cylinderical_Warp(mA, vvXi_tilt3_238);

    cout << vvXi_tilt3_238.at(0).at(0)._ppA[0][0] << " " << vvXi_tilt3_238.at(0).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt3_238.at(0).at(1279)._ppA[0][0] << " " << vvXi_tilt3_238.at(0).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt3_238.at(1023).at(0)._ppA[0][0] << " " << vvXi_tilt3_238.at(1023).at(0)._ppA[1][0] <<endl; //(u,v,1)
    cout << vvXi_tilt3_238.at(1023).at(1279)._ppA[0][0] << " " << vvXi_tilt3_238.at(1023).at(1279)._ppA[1][0] <<endl; //(u,v,1)
    cout << "Image 3_238 End" <<endl;
    cout << endl;

    Mat icMain(1024*2, 1280*5, CV_8UC3);

//    //IMU
//    Mat icBuffer;
//    Mat icKernel(850, 1280, CV_8UC3);

//    int id;
//    float item[100];

//    if (OpenSerialPort(MY_SERIALPORT, 115200, NOPARITY, 8, ONESTOPBIT) != ERR_OK)
//    {
//        printf("\n\rSerialport Error...");
//        Sleep(2000);
//    }

//    Quaternion q;
//    EulerAngles e;

    while (1) {
        Mat icMain_show;

        img1 = GetMatFrame(handle1);                                // get frame
        img2 = GetMatFrame(handle2);
        img3 = GetMatFrame(handle3);
        img4 = GetMatFrame(handle4);
        img5 = GetMatFrame(handle5);
        img6 = GetMatFrame(handle6);
//        img7 = GetMatFrame(handle7);
//        img8 = GetMatFrame(handle8);

        if(show_flag == 1)
        {
            cv::resize(img1, show_img1, Size(img1.cols/2, img1.rows/2));        // resize
            cv::resize(img2, show_img2, Size(img2.cols/2, img2.rows/2));        // resize
            cv::resize(img3, show_img3, Size(img3.cols/2, img3.rows/2));        // resize
            cv::resize(img4, show_img4, Size(img4.cols/2, img4.rows/2));        // resize
            cv::resize(img5, show_img5, Size(img5.cols/2, img5.rows/2));        // resize
            cv::resize(img6, show_img6, Size(img6.cols/2, img6.rows/2));        // resize
//            cv::resize(img7, show_img7, Size(img7.cols/2, img7.rows/2));        // resize
//            cv::resize(img8, show_img8, Size(img8.cols/2, img8.rows/2));        // resize

            imshow("camera1", show_img1);
            imshow("camera2", show_img2);
            imshow("camera3", show_img3);
            imshow("camera4", show_img4);
            imshow("camera5", show_img5);
            imshow("camera6", show_img6);
//            imshow("camera7", show_img7);
//            imshow("camera8", show_img8);
        }

        QPoint mp4_458;
        QPoint mp5_458;

        QPoint mp6_618;
        QPoint mp1_618;

        QPoint mp2_238;
        QPoint mp3_238;

        for(int i=0; i<1024; i++)
        {
            for(int j=0; j<1280; j++)
            {
                mp4_458.setX((int)(vvXi_tilt4_458.at(i).at(j)._ppA[0][0])+1000);
                mp4_458.setY((int)(vvXi_tilt4_458.at(i).at(j)._ppA[1][0])+1000);
                icMain.at<Vec3b>(mp4_458.y(), mp4_458.x()) = img4.at<Vec3b>(i,j);

                mp5_458.setX((int)(vvXi_tilt5_458.at(i).at(j)._ppA[0][0])+1000);
                mp5_458.setY((int)(vvXi_tilt5_458.at(i).at(j)._ppA[1][0])+1000);
                icMain.at<Vec3b>(mp5_458.y(), mp5_458.x()) = img5.at<Vec3b>(i,j);

                mp6_618.setX((int)(vvXi_tilt6_618.at(i).at(j)._ppA[0][0])+1000+mA[0][0]*_RADIAN(110));
                mp6_618.setY((int)(vvXi_tilt6_618.at(i).at(j)._ppA[1][0])+1000);
                icMain.at<Vec3b>(mp6_618.y(), mp6_618.x()) = img6.at<Vec3b>(i,j);

                mp1_618.setX((int)(vvXi_tilt1_618.at(i).at(j)._ppA[0][0])+1000+mA[0][0]*_RADIAN(110));
                mp1_618.setY((int)(vvXi_tilt1_618.at(i).at(j)._ppA[1][0])+1000);
                icMain.at<Vec3b>(mp1_618.y(), mp1_618.x()) = img1.at<Vec3b>(i,j);

                mp2_238.setX((int)(vvXi_tilt2_238.at(i).at(j)._ppA[0][0])+1000+mA[0][0]*_RADIAN(100*2));
                mp2_238.setY((int)(vvXi_tilt2_238.at(i).at(j)._ppA[1][0])+1000);
                icMain.at<Vec3b>(mp2_238.y(), mp2_238.x()) = img2.at<Vec3b>(i,j);

                mp3_238.setX((int)(vvXi_tilt3_238.at(i).at(j)._ppA[0][0])+1000+mA[0][0]*_RADIAN(100*2));
                mp3_238.setY((int)(vvXi_tilt3_238.at(i).at(j)._ppA[1][0])+1000);
                icMain.at<Vec3b>(mp3_238.y(), mp3_238.x()) = img3.at<Vec3b>(i,j);
            }
        }

//        //IMU
//        icBuffer = icMain.clone();
//        int count = 50;
//        while(count>0)
//        {
//            if (EBimuAsciiParser(&id,item, 5))
//            {
//                q.z = item[0];
//                q.y = item[1];
//                q.x = item[2];
//                q.w = item[3];

//                e = ToEulerAngles(q);
//                cout << "Yaw: " << e.yaw <<endl;

//                int data_row = (int)((e.yaw + 180)*(icBuffer.cols/360));
//                //cout << "data_row: " << data_row << endl;

//                for(int c = 0; c < icKernel.cols; c++)
//                {
//                    for(int r = 0; r < icKernel.rows; r++)
//                    {
//                        if(data_row > icBuffer.cols)
//                        {
//                            icKernel.at<Vec3b>(r, c) = icBuffer.at<Vec3b>(r+600, data_row + c - icBuffer.cols);
//                        }
//                        else
//                        {
//                            icKernel.at<Vec3b>(r, c) = icBuffer.at<Vec3b>(r+600, data_row + c);
//                        }
//                    }
//                }
//                imshow("Kernel Image", icKernel);

//                char ch = waitKey(10);
//                if(ch == 27)
//                    break;       // 27 == ESC key
//            }
//            count--;
//        }

        //UI 활성화 갱신
        cout << "cylinderical Process Finished!" << endl;

        cv::resize(icMain, icMain_show, Size(icMain.cols/2, icMain.rows/2));        // resize
        imshow("icMain", icMain_show);

        char ch = waitKey(10);
        if(ch == 27)
            break;       // 27 == ESC key
    }
}


void MainFrame::on_Cylinderical_Warp_2_clicked()
{
    //Image
    KImageColor Img(1024,1280);

    Mat img1 = imread("./data/realtime/1.bmp");
    Mat img2 = imread("./data/realtime/2.bmp");
    Mat img3 = imread("./data/realtime/3.bmp");
    Mat img4 = imread("./data/realtime/4.bmp");
    Mat img5 = imread("./data/realtime/5.bmp");
    Mat img6 = imread("./data/realtime/6.bmp");

    ///////////////////////////////R,t 불러오기//////////////////////////////////
    KRotation rR4_458;
    rR4_458 = rv0171::Import_rRi(4, 48);
    KVector vT4_458;
    vT4_458 = rv0171::Import_vTi(4, 48);

    KRotation rR5_458;
    rR5_458 = rv0171::Import_rRi(5, 58);
    KVector vT5_458;
    vT5_458 = rv0171::Import_vTi(5, 58);

    KRotation rR6_618;
    rR6_618 = rv0171::Import_rRi(6, 68);
    KVector vT6_618;
    vT6_618 = rv0171::Import_vTi(6, 68);

    KRotation rR1_618;
    rR1_618 = rv0171::Import_rRi(1, 18);
    KVector vT1_618;
    vT1_618 = rv0171::Import_vTi(1, 18);

    KRotation rR2_238;
    rR2_238 = rv0171::Import_rRi(2, 28);
    KVector vT2_238;
    vT2_238 = rv0171::Import_vTi(2, 28);

    KRotation rR3_238;
    rR3_238 = rv0171::Import_rRi(3, 38);
    KVector vT3_238;
    vT3_238 = rv0171::Import_vTi(3, 38);

    ///////////////////////////////Surround View Stitching//////////////////////////////////
    KMatrix mA(3,3);
    mA[0][0] = 1000; //fi
    mA[1][1] = 1000; //fi
    mA[2][2] = 1.0;
    mA[0][2] = 0;
    mA[1][2] = 0;

    //Image 4_458
    vector<vector<KVector>> vvXi_tilt4_458;
    vvXi_tilt4_458 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA4_458(3,3);
    mA4_458 = rv0171::Import_mAi(4, 48);

    KMatrices vvXs_4_458;
    vvXs_4_458 = rv0171::make_Surround_View_Stitching_ver2(mA, mA4_458, rR4_458, vT4_458, vvXi_tilt4_458, 1);

    rv0171::make_Cylinderical_Warp_ver2(mA, vvXs_4_458);
\
    cout << "Image 4_458 End" << endl;

    //Image 5_458
    vector<vector<KVector>> vvXi_tilt5_458;
    vvXi_tilt5_458 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA5_458(3,3);
    mA5_458 = rv0171::Import_mAi(5,58);

    KMatrices vvXs_5_458;
    vvXs_5_458 = rv0171::make_Surround_View_Stitching_ver2(mA, mA5_458, rR5_458, vT5_458, vvXi_tilt5_458, 0);

    rv0171::make_Cylinderical_Warp_ver2(mA, vvXs_5_458);

    cout << "Image 5_458 End" << endl;

    //Image 6_618
    vector<vector<KVector>> vvXi_tilt6_618;
    vvXi_tilt6_618 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA6_618(3,3);
    mA6_618 = rv0171::Import_mAi(6, 68);

    KMatrices vvXs_6_618;
    vvXs_6_618 = rv0171::make_Surround_View_Stitching_ver2(mA, mA6_618, rR6_618, vT6_618, vvXi_tilt6_618, 1);

    rv0171::make_Cylinderical_Warp_ver2(mA, vvXs_6_618);

    cout << "Image 6_618 End" << endl;

    //Image 1_618
    vector<vector<KVector>> vvXi_tilt1_618;
    vvXi_tilt1_618 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA1_618(3,3);
    mA1_618 = rv0171::Import_mAi(1, 18);

    KMatrices vvXs_1_618;
    vvXs_1_618 = rv0171::make_Surround_View_Stitching_ver2(mA, mA1_618, rR1_618, vT1_618, vvXi_tilt1_618, 0);

    rv0171::make_Cylinderical_Warp_ver2(mA, vvXs_1_618);

    cout << "Image 1_618 End" << endl;

    //Image 2_238
    vector<vector<KVector>> vvXi_tilt2_238;
    vvXi_tilt2_238 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA2_238(3,3);
    mA2_238 = rv0171::Import_mAi(2, 28);

    KMatrices vvXs_2_238;
    vvXs_2_238 = rv0171::make_Surround_View_Stitching_ver2(mA, mA2_238, rR2_238, vT2_238, vvXi_tilt2_238, 1);

    rv0171::make_Cylinderical_Warp_ver2(mA, vvXs_2_238);

    cout << "Image 2_238 End" << endl;

    //Image 3_238
    vector<vector<KVector>> vvXi_tilt3_238;
    vvXi_tilt3_238 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA3_238(3,3);
    mA3_238 = rv0171::Import_mAi(3, 38);

    KMatrices vvXs_3_238;
    vvXs_3_238 = rv0171::make_Surround_View_Stitching_ver2(mA, mA3_238, rR3_238, vT3_238, vvXi_tilt3_238, 0);

    rv0171::make_Cylinderical_Warp_ver2(mA, vvXs_3_238);

    cout << "Image 6_468 End" << endl;

    Mat icMain(1024*2, 1280*5, CV_8UC3);
    Mat icMain_show;

    //IMU
    Mat icKernel(850, 1280, CV_8UC3);

    int id;
    float item[100];

    if (OpenSerialPort(MY_SERIALPORT, 115200, NOPARITY, 8, ONESTOPBIT) != ERR_OK)
    {
        printf("\n\rSerialport Error...");
        Sleep(2000);
    }

    Quaternion q;
    EulerAngles e;

    QPoint mp4_458;
    QPoint mp5_458;

    QPoint mp6_618;
    QPoint mp1_618;

    QPoint mp2_238;
    QPoint mp3_238;

    for(int i=0; i<1024; i++)
    {
        for(int j=0; j<1280; j++)
        {
            mp4_458.setX((int)(vvXs_4_458[i]._ppA[0][j])+1000);
            mp4_458.setY((int)(vvXs_4_458[i]._ppA[1][j])+1000);
            if(icMain.at<Vec3b>(mp4_458.y(), mp4_458.x())[0] == 0)
                icMain.at<Vec3b>(mp4_458.y(), mp4_458.x()) = img4.at<Vec3b>(i,j);
            else
                icMain.at<Vec3b>(mp4_458.y(), mp4_458.x()) += img4.at<Vec3b>(i,j)*(0/255);

            mp5_458.setX((int)(vvXs_5_458[i]._ppA[0][j])+1000);
            mp5_458.setY((int)(vvXs_5_458[i]._ppA[1][j])+1000);
            if(icMain.at<Vec3b>(mp5_458.y(), mp5_458.x())[0] == 0)
                icMain.at<Vec3b>(mp5_458.y(), mp5_458.x()) = img5.at<Vec3b>(i,j);
            else
                icMain.at<Vec3b>(mp5_458.y(), mp5_458.x()) += img5.at<Vec3b>(i,j)*(0/255);

            mp6_618.setX((int)(vvXs_6_618[i]._ppA[0][j])+1000+mA[0][0]*_RADIAN(90)+130);
            mp6_618.setY((int)(vvXs_6_618[i]._ppA[1][j])+1000-60);
            icMain.at<Vec3b>(mp6_618.y(), mp6_618.x()) = img6.at<Vec3b>(i,j);

            mp1_618.setX((int)(vvXs_1_618[i]._ppA[0][j])+1000+mA[0][0]*_RADIAN(90)+130);
            mp1_618.setY((int)(vvXs_1_618[i]._ppA[1][j])+1000-60);
            icMain.at<Vec3b>(mp1_618.y(), mp1_618.x()) = img1.at<Vec3b>(i,j);

            mp2_238.setX((int)(vvXs_2_238[i]._ppA[0][j])+1000+mA[0][0]*_RADIAN(90*2)+220);
            mp2_238.setY((int)(vvXs_2_238[i]._ppA[1][j])+1000-40);
            icMain.at<Vec3b>(mp2_238.y(), mp2_238.x()) = img2.at<Vec3b>(i,j);

            mp3_238.setX((int)(vvXs_3_238[i]._ppA[0][j])+1000+mA[0][0]*_RADIAN(90*2)+220);
            mp3_238.setY((int)(vvXs_3_238[i]._ppA[1][j])+1000-40);
            icMain.at<Vec3b>(mp3_238.y(), mp3_238.x()) = img3.at<Vec3b>(i,j);
        }
    }

    //UI 활성화 갱신
    cout << "cylinderical Process Finished!" << endl;

    cv::resize(icMain, icMain_show, Size(icMain.cols/2, icMain.rows/2));        // resize
    imshow("icMain", icMain_show);

//    //IMU
//    while(1)
//    {
//        if (EBimuAsciiParser(&id,item, 5))
//        {
//            q.z = item[0];
//            q.y = item[1];
//            q.x = item[2];
//            q.w = item[3];

//            e = ToEulerAngles(q);
//            cout << "Yaw: " << e.yaw <<endl;

//            int data_row = (int)((e.yaw + 180)*(icMain.cols/360));
//            //cout << "data_row: " << data_row << endl;

//            for(int c = 0; c < icKernel.cols; c++)
//            {
//                for(int r = 0; r < icKernel.rows; r++)
//                {
//                    if(data_row > icMain.cols)
//                    {
//                        icKernel.at<Vec3b>(r, c) = icMain.at<Vec3b>(r+600, data_row + c - icMain.cols);
//                    }
//                    else
//                    {
//                        icKernel.at<Vec3b>(r, c) = icMain.at<Vec3b>(r+600, data_row + c);
//                    }
//                }
//            }
//            imshow("Kernel Image", icKernel);

//            char ch = waitKey(10);
//            if(ch == 27)
//                break;       // 27 == ESC key
//        }
//    }
}


void MainFrame::on_pushRealTime_2_clicked()
{
    int show_flag = 0;

    void* handle1;
    void* handle2;
    void* handle3;
    void* handle4;
    void* handle5;
    void* handle6;
//    void* handle7;
//    void* handle8;

    double setFrame = 30.0f;

    handle1 = CreateCamera("Cam01");            // Create Handle and Open
    handle2 = CreateCamera("Cam02");            // Create Handle and Open
    handle3 = CreateCamera("Cam03");            // Create Handle and Open
    handle4 = CreateCamera("Cam04");            // Create Handle and Open
    handle5 = CreateCamera("Cam05");            // Create Handle and Open
    handle6 = CreateCamera("Cam06");            // Create Handle and Open
//    handle7 = CreateCamera("Cam07");            // Create Handle and Open
//    handle8 = CreateCamera("Cam08");            // Create Handle and Open

    SetExposureAuto(handle1, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle1, setFrame);                    // set Frame rate

    SetExposureAuto(handle2, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle2, setFrame);                    // set Frame rate

    SetExposureAuto(handle3, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle3, setFrame);                    // set Frame rate

    SetExposureAuto(handle4, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle4, setFrame);                    // set Frame rate

    SetExposureAuto(handle5, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle5, setFrame);                    // set Frame rate

    SetExposureAuto(handle6, true);                    // set Exposure Auto off
    //SetExposure(handle, 30000.0f);                    // set Exposure
    SetFramerate(handle6, setFrame);                    // set Frame rate

//    SetExposureAuto(handle7, true);                    // set Exposure Auto off
//    //SetExposure(handle, 30000.0f);                    // set Exposure
//    SetFramerate(handle7, setFrame);                    // set Frame rate

//    SetExposureAuto(handle8, true);                    // set Exposure Auto off
//    //SetExposure(handle, 30000.0f);                    // set Exposure
//    SetFramerate(handle8, setFrame);                    // set Frame rate

    // data type : BayerRG8
    MV_CC_SetEnumValue(handle1, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle2, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle3, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle4, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle5, "PixelFormat", 0x01080009);
    MV_CC_SetEnumValue(handle6, "PixelFormat", 0x01080009);
//    MV_CC_SetEnumValue(handle7, "PixelFormat", 0x01080009);
//    MV_CC_SetEnumValue(handle8, "PixelFormat", 0x01080009);

    GrabCamera(handle1);
    GrabCamera(handle2);
    GrabCamera(handle3);
    GrabCamera(handle4);
    GrabCamera(handle5);
    GrabCamera(handle6);
//    GrabCamera(handle7);
//    GrabCamera(handle8);

    Mat img1, img2, img3, img4, img5, img6, img7, img8;
    Mat show_img1, show_img2, show_img3, show_img4, show_img5, show_img6, show_img7, show_img8;

    //Cylindrical Warp
    KImageColor Img(1024,1280);

    ///////////////////////////////R,t 불러오기//////////////////////////////////
    KRotation rR4_458;
    rR4_458 = rv0171::Import_rRi(4, 48);
    KVector vT4_458;
    vT4_458 = rv0171::Import_vTi(4, 48);

    KRotation rR5_458;
    rR5_458 = rv0171::Import_rRi(5, 58);
    KVector vT5_458;
    vT5_458 = rv0171::Import_vTi(5, 58);

    KRotation rR6_618;
    rR6_618 = rv0171::Import_rRi(6, 68);
    KVector vT6_618;
    vT6_618 = rv0171::Import_vTi(6, 68);

    KRotation rR1_618;
    rR1_618 = rv0171::Import_rRi(1, 18);
    KVector vT1_618;
    vT1_618 = rv0171::Import_vTi(1, 18);

    KRotation rR2_238;
    rR2_238 = rv0171::Import_rRi(2, 28);
    KVector vT2_238;
    vT2_238 = rv0171::Import_vTi(2, 28);

    KRotation rR3_238;
    rR3_238 = rv0171::Import_rRi(3, 38);
    KVector vT3_238;
    vT3_238 = rv0171::Import_vTi(3, 38);

    ///////////////////////////////Surround View Stitching//////////////////////////////////
    KMatrix mA(3,3);
    mA[0][0] = 1000; //fi
    mA[1][1] = 1000; //fi
    mA[2][2] = 1.0;
    mA[0][2] = 0;
    mA[1][2] = 0;

    //Image 4_458
    vector<vector<KVector>> vvXi_tilt4_458;
    vvXi_tilt4_458 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA4_458(3,3);
    mA4_458 = rv0171::Import_mAi(4, 48);

    KMatrices vvXs_4_458;
    vvXs_4_458 = rv0171::make_Surround_View_Stitching_ver2(mA, mA4_458, rR4_458, vT4_458, vvXi_tilt4_458, 1);

    rv0171::make_Cylinderical_Warp_ver2(mA, vvXs_4_458);
\
    cout << "Image 4_458 End" << endl;

    //Image 5_458
    vector<vector<KVector>> vvXi_tilt5_458;
    vvXi_tilt5_458 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA5_458(3,3);
    mA5_458 = rv0171::Import_mAi(5,58);

    KMatrices vvXs_5_458;
    vvXs_5_458 = rv0171::make_Surround_View_Stitching_ver2(mA, mA5_458, rR5_458, vT5_458, vvXi_tilt5_458, 0);

    rv0171::make_Cylinderical_Warp_ver2(mA, vvXs_5_458);

    cout << "Image 5_458 End" << endl;

    //Image 6_618
    vector<vector<KVector>> vvXi_tilt6_618;
    vvXi_tilt6_618 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA6_618(3,3);
    mA6_618 = rv0171::Import_mAi(6, 68);

    KMatrices vvXs_6_618;
    vvXs_6_618 = rv0171::make_Surround_View_Stitching_ver2(mA, mA6_618, rR6_618, vT6_618, vvXi_tilt6_618, 1);

    rv0171::make_Cylinderical_Warp_ver2(mA, vvXs_6_618);

    cout << "Image 6_618 End" << endl;

    //Image 1_618
    vector<vector<KVector>> vvXi_tilt1_618;
    vvXi_tilt1_618 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA1_618(3,3);
    mA1_618 = rv0171::Import_mAi(1, 18);

    KMatrices vvXs_1_618;
    vvXs_1_618 = rv0171::make_Surround_View_Stitching_ver2(mA, mA1_618, rR1_618, vT1_618, vvXi_tilt1_618, 0);

    rv0171::make_Cylinderical_Warp_ver2(mA, vvXs_1_618);

    cout << "Image 1_618 End" << endl;

    //Image 2_238
    vector<vector<KVector>> vvXi_tilt2_238;
    vvXi_tilt2_238 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA2_238(3,3);
    mA2_238 = rv0171::Import_mAi(2, 28);

    KMatrices vvXs_2_238;
    vvXs_2_238 = rv0171::make_Surround_View_Stitching_ver2(mA, mA2_238, rR2_238, vT2_238, vvXi_tilt2_238, 1);

    rv0171::make_Cylinderical_Warp_ver2(mA, vvXs_2_238);

    cout << "Image 2_238 End" << endl;

    //Image 3_238
    vector<vector<KVector>> vvXi_tilt3_238;
    vvXi_tilt3_238 = rv0171::make_3DCameraCoord(Img); //3차원 카메라 좌표계

    KMatrix mA3_238(3,3);
    mA3_238 = rv0171::Import_mAi(3, 38);

    KMatrices vvXs_3_238;
    vvXs_3_238 = rv0171::make_Surround_View_Stitching_ver2(mA, mA3_238, rR3_238, vT3_238, vvXi_tilt3_238, 0);

    rv0171::make_Cylinderical_Warp_ver2(mA, vvXs_3_238);

    cout << "Image 6_468 End" << endl;

    Mat icMain(1024*2, 1280*5, CV_8UC3);

//    //IMU
//    Mat icBuffer;
//    Mat icKernel(850, 1280, CV_8UC3);

//    int id;
//    float item[100];

//    if (OpenSerialPort(MY_SERIALPORT, 115200, NOPARITY, 8, ONESTOPBIT) != ERR_OK)
//    {
//        printf("\n\rSerialport Error...");
//        Sleep(2000);
//    }

//    Quaternion q;
//    EulerAngles e;

    while (1) {
        Mat icMain_show;

        img1 = GetMatFrame(handle1);                                // get frame
        img2 = GetMatFrame(handle2);
        img3 = GetMatFrame(handle3);
        img4 = GetMatFrame(handle4);
        img5 = GetMatFrame(handle5);
        img6 = GetMatFrame(handle6);
//        img7 = GetMatFrame(handle7);
//        img8 = GetMatFrame(handle8);

        if(show_flag == 1)
        {
            cv::resize(img1, show_img1, Size(img1.cols/2, img1.rows/2));        // resize
            cv::resize(img2, show_img2, Size(img2.cols/2, img2.rows/2));        // resize
            cv::resize(img3, show_img3, Size(img3.cols/2, img3.rows/2));        // resize
            cv::resize(img4, show_img4, Size(img4.cols/2, img4.rows/2));        // resize
            cv::resize(img5, show_img5, Size(img5.cols/2, img5.rows/2));        // resize
            cv::resize(img6, show_img6, Size(img6.cols/2, img6.rows/2));        // resize
//            cv::resize(img7, show_img7, Size(img7.cols/2, img7.rows/2));        // resize
//            cv::resize(img8, show_img8, Size(img8.cols/2, img8.rows/2));        // resize

            imshow("camera1", show_img1);
            imshow("camera2", show_img2);
            imshow("camera3", show_img3);
            imshow("camera4", show_img4);
            imshow("camera5", show_img5);
            imshow("camera6", show_img6);
//            imshow("camera7", show_img7);
//            imshow("camera8", show_img8);
        }

        QPoint mp4_458;
        QPoint mp5_458;

        QPoint mp6_618;
        QPoint mp1_618;

        QPoint mp2_238;
        QPoint mp3_238;

        for(int i=0; i<1024; i++)
        {
            for(int j=0; j<1280; j++)
            {
                mp4_458.setX((int)(vvXs_4_458[i]._ppA[0][j])+1000);
                mp4_458.setY((int)(vvXs_4_458[i]._ppA[1][j])+1000);
                icMain.at<Vec3b>(mp4_458.y(), mp4_458.x()) = img4.at<Vec3b>(i,j);

                mp5_458.setX((int)(vvXs_5_458[i]._ppA[0][j])+1000);
                mp5_458.setY((int)(vvXs_5_458[i]._ppA[1][j])+1000);
                icMain.at<Vec3b>(mp5_458.y(), mp5_458.x()) = img5.at<Vec3b>(i,j);

                mp6_618.setX((int)(vvXs_6_618[i]._ppA[0][j])+1000+mA[0][0]*_RADIAN(90)+130);
                mp6_618.setY((int)(vvXs_6_618[i]._ppA[1][j])+1000-60);
                icMain.at<Vec3b>(mp6_618.y(), mp6_618.x()) = img6.at<Vec3b>(i,j);

                mp1_618.setX((int)(vvXs_1_618[i]._ppA[0][j])+1000+mA[0][0]*_RADIAN(90)+130);
                mp1_618.setY((int)(vvXs_1_618[i]._ppA[1][j])+1000-60);
                icMain.at<Vec3b>(mp1_618.y(), mp1_618.x()) = img1.at<Vec3b>(i,j);

                mp2_238.setX((int)(vvXs_2_238[i]._ppA[0][j])+1000+mA[0][0]*_RADIAN(90*2)+220);
                mp2_238.setY((int)(vvXs_2_238[i]._ppA[1][j])+1000-85);
                icMain.at<Vec3b>(mp2_238.y(), mp2_238.x()) = img2.at<Vec3b>(i,j);

                mp3_238.setX((int)(vvXs_3_238[i]._ppA[0][j])+1000+mA[0][0]*_RADIAN(90*2)+220);
                mp3_238.setY((int)(vvXs_3_238[i]._ppA[1][j])+1000-85);
                icMain.at<Vec3b>(mp3_238.y(), mp3_238.x()) = img3.at<Vec3b>(i,j);
            }
        }

//        //IMU
//        icBuffer = icMain.clone();
//        int count = 50;
//        while(count>0)
//        {
//            if (EBimuAsciiParser(&id,item, 5))
//            {
//                q.z = item[0];
//                q.y = item[1];
//                q.x = item[2];
//                q.w = item[3];

//                e = ToEulerAngles(q);
//                cout << "Yaw: " << e.yaw <<endl;

//                int data_row = (int)((e.yaw + 180)*(icBuffer.cols/360));
//                //cout << "data_row: " << data_row << endl;

//                for(int c = 0; c < icKernel.cols; c++)
//                {
//                    for(int r = 0; r < icKernel.rows; r++)
//                    {
//                        if(data_row > icBuffer.cols)
//                        {
//                            icKernel.at<Vec3b>(r, c) = icBuffer.at<Vec3b>(r+600, data_row + c - icBuffer.cols);
//                        }
//                        else
//                        {
//                            icKernel.at<Vec3b>(r, c) = icBuffer.at<Vec3b>(r+600, data_row + c);
//                        }
//                    }
//                }
//                imshow("Kernel Image", icKernel);

//                char ch = waitKey(10);
//                if(ch == 27)
//                    break;       // 27 == ESC key
//            }
//            count--;
//        }

        //UI 활성화 갱신
        cout << "cylinderical Process Finished!" << endl;

        cv::resize(icMain, icMain_show, Size(icMain.cols/2, icMain.rows/2));        // resize
        imshow("icMain", icMain_show);

        char ch = waitKey(10);
        if(ch == 27)
            break;       // 27 == ESC key
    }
}

