#include <QFileDialog>
#include <QPainter>
#include <sstream>
#include <fstream>

#include "mainframe.h"
#include "ui_mainframe.h"
#include "imageform.h"
#include "rv0171.h"
#include <QString>

using namespace std;

string rvdir = "C:/Robot_Vision/RobotVisionApp_Students/data/";

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



//////////////////////////////
//// Zhang's Method (HW1) ////
//////////////////////////////
void MainFrame::on_zhang_clicked()
{
    int nImg = 7;
    int nFeature = 156;
    int nItr = 100;

    // Load Image & Model Points to vector
    vector<vector<pair<double, double>>> pointzip;
    for(int i = 1; i <= nImg + 1; i++){
        string dir = rvdir;
        if(i != nImg + 1) {
            dir += "hw01/Image";
            dir += to_string(i);
            dir += ".txt";
            pointzip.push_back(rv0171::PointList(dir));
        }
        else {
            dir += "hw01/Model.txt";
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
    int nImg = 7;
    int nFeature = 156;
    int row = 1300;
    int col = 1300;

    // Load Image & Model Points to vector
    vector<vector<pair<double, double>>> pointzip;
    for(int i = 1; i <= nImg + 1; i++){
        string dir = rvdir;
        if(i != nImg + 1) {
            dir += "hw01/Image";
            dir += to_string(i);
            dir += ".txt";
            pointzip.push_back(rv0171::PointList(dir));
        }
        else {
            dir += "hw01/Model.txt";
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
    int nImg = 15;
    int nFeature = 100; // Feature점 개수: 100
    int nItr = 100;

    KPoint* psM;
    psM = new KPoint[nFeature];

    KPoint** psFl;
    KPoint** psFr;
    psFl = new KPoint*[nImg];
    for (int i = 0; i < nImg; i++) {
        psFl[i] = new KPoint[nFeature];
    }
    psFr = new KPoint*[nImg];
    for (int i = 0; i < nImg; i++) {
        psFr[i] = new KPoint[nFeature];
    }

    // 1. Capture stereo pairs
    vector<vector<pair<double, double>>> left_pointzip;
    for(int i = 1; i <= nImg + 1; i++){
        string dir = rvdir;
        if(i != nImg + 1) {
            dir += "hw02/calib_data/left";
            dir += to_string(i);
            dir += ".txt";
            left_pointzip.push_back(rv0171::PointList(dir));
        }
        else {
            dir += "hw02/calib_data/model.txt";
            left_pointzip.push_back(rv0171::PointList(dir));
        }
    }

    vector<vector<pair<double, double>>> right_pointzip;
    for(int i = 1; i <= nImg + 1; i++){
        string dir = rvdir;
        if(i != nImg + 1) {
            dir += "hw02/calib_data/right";
            dir += to_string(i);
            dir += ".txt";
            right_pointzip.push_back(rv0171::PointList(dir));
        }
        else {
            dir += "hw02/calib_data/model.txt";
            right_pointzip.push_back(rv0171::PointList(dir));
        }
    }
    KVector vX_l, vX_r;

    // 2. Calibrate separately the two cameras using Zhang's method
    vX_l = rv0171::ZhangsCalibration(left_pointzip, nImg, nFeature, nItr);
    vX_r = rv0171::ZhangsCalibration(right_pointzip, nImg, nFeature, nItr);

    KMatrix mA_l(3, 3);
    mA_l[0][0] = vX_l[0];
    mA_l[1][1] = vX_l[1];
    mA_l[0][2] = vX_l[2];
    mA_l[1][2] = vX_l[3];
    mA_l[2][2] = 1.0;
    double dK1_l = vX_l[4];
    double dK2_l = vX_l[5];
    double dError_l = vX_l[vX_l.Size() - 1];  // vX's last element is Err

    KHomogeneous* hP_l;
    hP_l = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vX_l[6 * (i + 1)], vX_l[6 * (i + 1) + 1], vX_l[6 * (i + 1) + 2]);
        KVector   vT(vX_l[6*(i + 1) + 3], vX_l[6 * (i + 1) + 4], vX_l[6 * (i + 1) + 5]);
        hP_l[i] = KHomogeneous(rR, vT);      // 4x4 matrix, W->L
    }

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

    KMatrix mA_r(3, 3);
    mA_r[0][0] = vX_r[0];
    mA_r[1][1] = vX_r[1];
    mA_r[0][2] = vX_r[2];
    mA_r[1][2] = vX_r[3];
    mA_r[2][2] = 1.0;
    double dK1_r = vX_r[4];
    double dK2_r = vX_r[5];
    double dError_r = vX_r[vX_r.Size() - 1];  // vX's last element is Err

    KHomogeneous* hP_r;
    hP_r = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vX_l[6 * (i + 1)], vX_l[6 * (i + 1) + 1], vX_l[6 * (i + 1) + 2]);
        KVector   vT(vX_l[6*(i + 1) + 3], vX_l[6 * (i + 1) + 4], vX_l[6 * (i + 1) + 5]);
        hP_r[i] = KHomogeneous(rR, vT);      // 4x4 matrix, W->R
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


    // Stereo Calibration
    // vX: 6*1 matrix, {r, p, y, x, y, z}
    KVector vX = rv0171::StereoCalibration(nImg, nFeature, vX_l, vX_r, psFl, psFr, psM);
//    qDebug() << vX.Size() << vX[0] << vX[1] << vX[2] << vX[3] << vX[4] << vX[5] << vX[6];
//    KRotation rR;
//    rR.FromRodrigues(vX[0], vX[1], vX[2]);
//    KVector vT(vX[3], vX[4], vX[5]);
//    KHomogeneous hLr = KHomogeneous(rR, vT);

//    // Model points
//    KMatrix mM;             // 3 x mFeature matrix
//    for (int i = 0; i < nFeature; i++) {
//        mM |= KVector(psM[i]._dX, psM[i]._dY, 1.0);             // Add a column
//    }

//    // 출력을 위한 ImageForm 생성
//    ImageForm* q_pForm[15] = {0,};
//    string wName[] = {"left1", "left2", "left3", "left4", "left5", "left6", "left7", "left8", "left9", "left10", "left11", "left12", "left13", "left14", "left15"};
//    for (int i = 0; i < nImg; i++) {
//        string sImgName = "C:/RV/RobotVisionApp_Students/data/hw02/calib_data/left";
//        sImgName += to_string(i + 1);
//        sImgName += ".bmp";

//        KImageColor icMain(480, 640);
//        QImage* _img = new QImage();        //이미지를 로드하기 위한 QImage 선언
//        if (_img->load(QString::fromStdString(sImgName))) {
//            for (int ii = 0; ii < 480; ii++) {
//                for (int jj = 0; jj < 640; jj++) {
//                    QColor color = _img->pixelColor(jj,ii);
//                    icMain[ii][jj].r = color.red();
//                    icMain[ii][jj].g = color.green();
//                    icMain[ii][jj].b = color.blue();
//                }
//            }
//        }
//        else {
//            qDebug() << "Image load Error";
//        }

//        for (unsigned int ii = 0; ii < _lImageForm->Count(); ii++) {
//            if ((*_lImageForm)[ii]->ID() == QString::fromStdString(wName[i])) {
//                q_pForm[i] = (*_lImageForm)[ii];
//            }
//        }

//        // Homogeneous matrix
//        KHomogeneous hP = hLr * hP_r[i];

//        // Display window
//        if (q_pForm[i]) {
//            // 이미지 창이 이미 존재하면 아무것도 하지 않음
//        }
//        else {
//            q_pForm[i] = new ImageForm(icMain, QString::fromStdString(wName[i]), this);

//            for(int j = 0; j < nFeature; j++) {
//                // Real point
//                QPoint point;
//                point.setX((int)(psFl[i][j]._dX));
//                point.setY((int)(psFl[i][j]._dY));
//                QColor color;
//                color.setRed(255);
//                color.setGreen(0);
//                color.setBlue(0);
//                q_pForm[i]->DrawEllipse(point, 3, 3, color);

//                // Projection
//                double dR2;
//                KVector vXc, vXn, vU, vDr;
//                vXc = hP.R() * mM.Column(j) + hP.t();               // 3x1 matrix
//                vXn = (vXc / vXc[2]).To(1);                         // 2x1 matrix
//                // Radial distortion
//                dR2 = _SQR(vXn[0]) + _SQR(vXn[1]);                  // double
//                vDr = vXn * (1.0 + dK1_l * dR2 + dK2_l * pow(dR2, 2));              // 2x1 matrix
//                // Final distortion
//                vXc = vDr.Tailed(1.0);                              // 3x1 matrix
//                // Image projection
//                vU = mA_l * vXc;      vU /= vU[2];                   // 3x1 matrix, {ud, vd, 1.0}

//                // Reprojected point
//                QPoint rePoint;
//                rePoint.setX((int)(vU[__X]));
//                rePoint.setY((int)(vU[__Y]));
//                QColor reColor;
//                reColor.setRed(0);
//                reColor.setGreen(0);
//                reColor.setBlue(255);
//                q_pForm[i]->DrawEllipse(rePoint, 3, 3, reColor);
//            }

//            q_pForm[i]->show();
//            _lImageForm.push_back(q_pForm[i]);
//        }
//    }
    UpdateUI();
}



//////////////////////////////
/////// AdaBoost (HW3) ///////
//////////////////////////////
void MainFrame::on_pushAdaboost_clicked()
{
    int nSample = 1000;
    int nFeature = 10;

    // 1. Load Trainig Samples
    QFileDialog::Options q_Options = QFileDialog::DontResolveSymlinks |
                                     QFileDialog::DontUseNativeDialog;
    QString stSample = QFileDialog::getOpenFileName(this,
                                                    tr("Select a Training Sample File"),
                                                    "./data/", "Text Files(*.txt)", 0, q_Options);
    if(stSample.length() == 0) return;

    int nWeak = ui->spinWeakclassifier->text().toInt(); // Get num of weakclassifier
    if(nWeak == 0) nWeak = 10;

    vector<KVector> lX;
    KVector         vTmp(nFeature), vY;    // vTmp(n): n Dim
    int             nLabel;
    FILE*           fp = fopen(stSample.toStdString().c_str(), "rt");

    while(fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d\n", \
                 &vTmp[0], &vTmp[1], &vTmp[2], &vTmp[3], &vTmp[4], \
                 &vTmp[5], &vTmp[6], &vTmp[7], &vTmp[8], &vTmp[9], \
                 &nLabel) != EOF)
    {
        lX.push_back(vTmp); // vTmp: 1000x10
        vY.Tailed(nLabel);  // vY: 1000x1
    }


    // 2. Container of Feature Selectors
    vector<KFeatureCoord> lFeature(nFeature);

    for (int i = 0; i < nFeature; i++) {
        KFeatureCoord col(i);  // operator(): add x1, x2,  ..., x10
        lFeature.push_back(col);
    }

    // 3. Learning of AdaBoost_Coord(Train)
    KVector ClassifyRes;
    KStrongClassifierCoord oAdaBoost;
    double dError = oAdaBoost.Train(lX, vY, lFeature, nWeak, 0.05);
    ClassifyRes.Tailed(dError);

    // Classify
    KVector vPred = oAdaBoost.Classify(lX);
    int TP = 0;
    int TN = 0;
    int FP = 0;
    int FN = 0;
    for (int i = 0; i < nSample; i++) {
        if (vY[i] == 1 && vPred[i] == 1) {
            TP++;
        }
        else if (vY[i] == -1 && vPred[i] == -1) {
            TN++;
        }
        else if (vY[i] == -1 && vPred[i] == 1) {
            FP++;
        }
        else if (vY[i] == 1 && vPred[i] == -1) {
            FN++;
        }
    }
    ClassifyRes.Tailed(TP).Tailed(TN).Tailed(FP).Tailed(FN);

    // 4. Print result
    QString temp_str;
    temp_str = QString::fromStdString(std::to_string((1 - ClassifyRes[0]) * 100));
    ui->textAccuracy->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string((int)ClassifyRes[1]));
    ui->textTP->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string((int)ClassifyRes[2]));
    ui->textTN->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string((int)ClassifyRes[3]));
    ui->textFP->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string((int)ClassifyRes[4]));
    ui->textFN->setText(temp_str);
}

