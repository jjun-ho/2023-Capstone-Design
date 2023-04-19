#include <QFileDialog>
#include <QPainter>

#include "mainframe.h"
#include "ui_mainframe.h"
#include "imageform.h"

#include "msjrv.h"

MainFrame::MainFrame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainFrame)
{
    ui->setupUi(this);

    _plpImageForm       = new KPtrList<ImageForm*>(10,false,false);
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
    delete _plpImageForm;

}

void MainFrame::CloseImageForm(ImageForm *pForm)
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
    if(ui->tabWidget->currentIndex() == 0) //rn1
    {
        //ui->buttonGVD->setEnabled(_q_pFormFocused && _q_pFormFocused->ImageGray().Address() && _q_pFormFocused->ID() == "OPEN" && _opGVD==0);

    }
    else if(ui->tabWidget->currentIndex() == 1) //rn2
    {

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

    //리스트에서 삭제한다.
    _plpImageForm->RemoveAll();
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

void MainFrame::on_tabWidget_currentChanged(int index)
{    

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

//////////
// HW01 //
////////// Mono Camera Calibration
void MainFrame::on_calib_btn_clicked()
{
    int nImg = 7;
    int nFeature = 156;

    KPoint** psF;
    KPoint* psM;

    psF = new KPoint*[nImg];
    for (int i = 0; i < nImg; i++) {
        psF[i] = new KPoint[nFeature];
    }
    psM = new KPoint[nFeature];

    // Get Model coordinates
    QString defaultPath = QCoreApplication::applicationDirPath();
    string model_path = defaultPath.toStdString() + "//..//..//Homeworks//data//hw01//Model.txt";
    ifstream model_file(model_path);
    if (model_file.is_open()) {
        for (int i = 0; i < nFeature; i++) {
            model_file >> psM[i]._dX;
            model_file >> psM[i]._dY;
        }
    }
    else {
        qDebug() << "File open error!\n";
    }
    model_file.close();

    // Get images coordinates
    for (int i = 1; i <= nImg; i++) {
        // Iterate for 7 images
        string fname = defaultPath.toStdString() + "//..//..//Homeworks//data//hw01//Image";
        fname += to_string(i);
        fname += ".txt";

        ifstream image_file(fname);
        if (image_file.is_open()) {
            for (int j = 0; j < nFeature; j++) {
                image_file >> psF[i - 1][j]._dX;
                image_file >> psF[i - 1][j]._dY;
            }
        }
        else {
            qDebug() << "File open error!\n";
        }
        image_file.close();
    }

    KVector vX = msjrv::ZhangMethod(nImg, nFeature, psF, psM);

    // Reprojection
    KMatrix mA(3, 3);
    mA[0][0] = vX[0];   // alpha
    mA[1][1] = vX[1];   // beta
    mA[0][2] = vX[2];   // u0
    mA[1][2] = vX[3];   // v0
    mA[2][2] = 1.0;     //
    double dK1 = vX[4]; // distortion parameter k1
    double dK2 = vX[5]; // distortion parameter k2
    double dError = vX[vX.Size() - 1];  // Error

    // Print calibration parameters
    QString temp_str;
    temp_str = QString::fromStdString(std::to_string(mA[0][0]));
    ui->textAlpha->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA[1][1]));
    ui->textBeta->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA[0][2]));
    ui->textU0->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA[1][2]));
    ui->textV0->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK1));
    ui->textK1->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK2));
    ui->textK2->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dError));
    ui->textError->setText(temp_str);

    KHomogeneous* hP;
    hP = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vX[6 * (i + 1)], vX[6 * (i + 1) + 1], vX[6 * (i + 1) + 2]);
        KVector   vT(vX[6*(i + 1) + 3], vX[6 * (i + 1) + 4], vX[6 * (i + 1) + 5]);
        hP[i] = KHomogeneous(rR, vT);
    }

    // 출력을 위한 ImageForm 생성
    ImageForm* q_pForm[7] = {0,};
    string wName[] = {"Image1", "Image2", "Image3", "Image4", "Image5", "Image6", "Image7"};
    for (int i = 0; i < nImg; i++) {
        KImageColor icMain(480, 640);
        for (unsigned int ii = 0; ii < _plpImageForm->Count(); ii++) {
            if ((*_plpImageForm)[ii]->ID() == QString::fromStdString(wName[i])) {
                q_pForm[i] = (*_plpImageForm)[ii];
            }
        }

        // Display window
        if (q_pForm[i]) {
            // 이미지 창이 이미 존재하면 아무것도 하지 않음
        }
        else {
            q_pForm[i] = new ImageForm(icMain, QString::fromStdString(wName[i]), this);

            for(int j = 0; j < nFeature; j++) {
                // Real point
                QPoint point;
                point.setX((int)(psF[i][j]._dX));
                point.setY((int)(psF[i][j]._dY));
                QColor color;
                color.setRed(255);
                color.setGreen(0);
                color.setBlue(0);
                q_pForm[i]->DrawEllipse(point, 2, 2, color);

                // Projection
                KVector vU = msjrv::PointProjection(mA, KVector(dK1, dK2), hP[i], KVector(psM[j]._dX, psM[j]._dY, 1.0));

                // Reprojected point
                QPoint rePoint;
                rePoint.setX((int)(vU[__X]));
                rePoint.setY((int)(vU[__Y]));
                QColor reColor;
                reColor.setRed(0);
                reColor.setGreen(0);
                reColor.setBlue(255);
                q_pForm[i]->DrawEllipse(rePoint, 1, 1, reColor);
            }

            q_pForm[i]->show();
            _plpImageForm->Add(q_pForm[i]);
        }
    }

    // UI 활성화 갱신
    UpdateUI();

    // Delete memory
    for (int i = 0; i < nImg; i++) {
        delete[] psF[i];
    } delete[] psF;
    delete[] psM;
    delete[] hP;
    qDebug() << "Process end\n";
}

//////////
// HW02 //
////////// Stereo Camera Calibration
void MainFrame::on_pushStereoCalib_clicked()
{
    int nImg = 4;
    int nFeature = 54;

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

    // Get Model coordinates
    string model_path = "C:/Users/ironview/Desktop/Homeworks/Homeworks/data/Cam123txt/model.txt";
    ifstream model_file(model_path);
    if (model_file.is_open()) {
        for (int i = 0; i < nFeature; i++) {
            model_file >> psM[i]._dX;
            model_file >> psM[i]._dY;
            //qDebug() << psM[i]._dX << psM[i]._dY;
        }
    }
    else {
        qDebug() << "File open error!\n";
    }
    model_file.close();

    // Get images coordinates
    for (int i = 1; i <= nImg; i++) {
        // Iterate for 15 images
        // For left images
        string fname = "C:/Users/ironview/Desktop/Homeworks/Homeworks/data/Cam123txt/left";
        fname += to_string(i);
        fname += ".txt";
        ifstream image_file_L(fname);
        if (image_file_L.is_open()) {
            for (int j = 0; j < nFeature; j++) {
                image_file_L >> psFl[i - 1][j]._dX;
                image_file_L >> psFl[i - 1][j]._dY;
                //qDebug() << "left" << i << j << psFl[i - 1][j]._dX << psFl[i - 1][j]._dY;
            }
        } image_file_L.close();

        // For right images
        fname = "C:/Users/ironview/Desktop/Homeworks/Homeworks/data/Cam123txt/main";
        fname += to_string(i);
        fname += ".txt";
        ifstream image_file_R(fname);
        if (image_file_R.is_open()) {
            for (int j = 0; j < nFeature; j++) {
                image_file_R >> psFr[i - 1][j]._dX;
                image_file_R >> psFr[i - 1][j]._dY;
                //qDebug() << "right" << i << j << psFr[i - 1][j]._dX << psFr[i - 1][j]._dY;
            }
        }
        else {
            qDebug() << "File open error!\n";
        } image_file_R.close();
    }

    KVector vXl, vXr;
    vXl = msjrv::ZhangMethod(nImg, nFeature, psFl, psM);
    vXr = msjrv::ZhangMethod(nImg, nFeature, psFr, psM);

    // For left images
    KMatrix mAl(3, 3);
    mAl[0][0] = vXl[0];     // alpha
    mAl[1][1] = vXl[1];     // beta
    mAl[0][2] = vXl[2];     // u0
    mAl[1][2] = vXl[3];     // v0
    mAl[2][2] = 1.0;        //
    double dK1l = vXl[4];   // distortion parameter k1
    double dK2l = vXl[5];   // distortion parameter k2

    KHomogeneous* hPl;
    hPl = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vXl[6 * (i + 1)], vXl[6 * (i + 1) + 1], vXl[6 * (i + 1) + 2]);
        KVector   vT(vXl[6*(i + 1) + 3], vXl[6 * (i + 1) + 4], vXl[6 * (i + 1) + 5]);
        hPl[i] = KHomogeneous(rR, vT);      // 4x4 matrix, W->TL
    }

    // For right images
    KMatrix mAr(3, 3);
    mAr[0][0] = vXr[0];     // alpha
    mAr[1][1] = vXr[1];     // beta
    mAr[0][2] = vXr[2];     // u0
    mAr[1][2] = vXr[3];     // v0
    mAr[2][2] = 1.0;        //
    double dK1r = vXr[4];   // distortion parameter k1
    double dK2r = vXr[5];   // distortion parameter k2

    KHomogeneous* hPr;
    hPr = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vXr[6 * (i + 1)], vXr[6 * (i + 1) + 1], vXr[6 * (i + 1) + 2]);
        KVector   vT(vXr[6*(i + 1) + 3], vXr[6 * (i + 1) + 4], vXr[6 * (i + 1) + 5]);
        hPr[i] = KHomogeneous(rR, vT);      // 4x4 matrix, W->TR
    }

    // Stereo Calibration
    // vX: 6*1 matrix, {r, p, y, x, y, z}
    KVector vX = msjrv::StereoCalibration(nImg, nFeature, vXl, vXr, psFl, psFr, psM);
    KRotation rR;
    rR.FromRodrigues(vX[0], vX[1], vX[2]);
    KVector vT(vX[3], vX[4], vX[5]);
    KHomogeneous hLr = KHomogeneous(rR, vT);
    cout << endl;

    cout << "r1: " << vX[0] << ", r2: " << vX[1] << ", r3: " << vX[2] << ", t1: " << vX[3] << ", t2: " << vX[4] << ", t3: " << vX[5] << endl;

    // Model points
    KMatrix mM;             // 3 x mFeature matrix
    for (int i = 0; i < nFeature; i++) {
        mM |= KVector(psM[i]._dX, psM[i]._dY, 1.0);             // Add a column
    }

    // 출력을 위한 ImageForm 생성
    ImageForm* q_pForm[4] = {0,};
    string wName[] = {"left1", "left2", "left3", "left4"};
    for (int i = 0; i < nImg; i++) {
        string sImgName = "C:/Users/ironview/Desktop/Homeworks/Homeworks/data/Cam123txt/left";
        sImgName += to_string(i + 1);
        sImgName += ".bmp";

        KImageColor icMain(1024, 1280);
        QImage* _img = new QImage();        //이미지를 로드하기 위한 QImage 선언
        if (_img->load(QString::fromStdString(sImgName))) {
            for (int ii = 0; ii < 1024; ii++) {
                for (int jj = 0; jj < 1280; jj++) {
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

        for (unsigned int ii = 0; ii < _plpImageForm->Count(); ii++) {
            if ((*_plpImageForm)[ii]->ID() == QString::fromStdString(wName[i])) {
                q_pForm[i] = (*_plpImageForm)[ii];
            }
        }

        // Homogeneous matrix
        KHomogeneous hP = hLr * hPr[i];

        // Display window
        if (q_pForm[i]) {
            // 이미지 창이 이미 존재하면 아무것도 하지 않음
        }
        else {
            q_pForm[i] = new ImageForm(icMain, QString::fromStdString(wName[i]), this);

            for(int j = 0; j < nFeature; j++) {
                // Real point
                QPoint point;
                point.setX((int)(psFl[i][j]._dX));
                point.setY((int)(psFl[i][j]._dY));
                QColor color;
                color.setRed(255);
                color.setGreen(0);
                color.setBlue(0);
                q_pForm[i]->DrawEllipse(point, 3, 3, color);

                // Projection
                KVector vU = msjrv::PointProjection(mAl, KVector(dK1l, dK2l), hP, mM.Column(j));

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

            q_pForm[i]->show();
            _plpImageForm->Add(q_pForm[i]);
        }
    }

    // UI 활성화 갱신
    UpdateUI();

    // Delete memory
    for (int i = 0; i < nImg; i++) {
        delete[] psFl[i];
    } delete[] psFl;
    for (int i = 0; i < nImg; i++) {
        delete[] psFr[i];
    } delete[] psFr;
    delete[] psM;
    delete[] hPl;
    delete[] hPr;
    qDebug() << "Process end\n";
}

//////////
// HW03 //
////////// Rectification
void MainFrame::on_pushRect_clicked()
{
    int nImg = 15;
    int nFeature = 100;

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

    // Get Model coordinates
    QString defaultPath = QCoreApplication::applicationDirPath();
    string model_path = defaultPath.toStdString() + "//..//..//Homeworks//data//hw02//calib_data//model.txt";
    ifstream model_file(model_path);
    if (model_file.is_open()) {
        for (int i = 0; i < nFeature; i++) {
            model_file >> psM[i]._dX;
            model_file >> psM[i]._dY;
            //qDebug() << psM[i]._dX << psM[i]._dY;
        }
    }
    else {
        qDebug() << "File open error!\n";
    }
    model_file.close();

    // Get images coordinates
    for (int i = 1; i <= nImg; i++) {
        // Iterate for 15 images
        // For left images
        string fname = defaultPath.toStdString() + "//..//..//Homeworks//data//hw02//calib_data//left";
        fname += to_string(i);
        fname += ".txt";
        ifstream image_file_L(fname);
        if (image_file_L.is_open()) {
            for (int j = 0; j < nFeature; j++) {
                image_file_L >> psFl[i - 1][j]._dX;
                image_file_L >> psFl[i - 1][j]._dY;
                //qDebug() << "left" << i << j << psFl[i - 1][j]._dX << psFl[i - 1][j]._dY;
            }
        } image_file_L.close();

        // For right images
        fname = defaultPath.toStdString() + "//..//..//Homeworks//data//hw02//calib_data//right";
        fname += to_string(i);
        fname += ".txt";
        ifstream image_file_R(fname);
        if (image_file_R.is_open()) {
            for (int j = 0; j < nFeature; j++) {
                image_file_R >> psFr[i - 1][j]._dX;
                image_file_R >> psFr[i - 1][j]._dY;
                //qDebug() << "right" << i << j << psFr[i - 1][j]._dX << psFr[i - 1][j]._dY;
            }
        }
        else {
            qDebug() << "File open error!\n";
        } image_file_R.close();
    }

    KVector vXl, vXr;
    vXl = msjrv::ZhangMethod(nImg, nFeature, psFl, psM);
    vXr = msjrv::ZhangMethod(nImg, nFeature, psFr, psM);

    // For left images
    KMatrix mAl(3, 3);
    mAl[0][0] = vXl[0];     // alpha
    mAl[1][1] = vXl[1];     // beta
    mAl[0][2] = vXl[2];     // u0
    mAl[1][2] = vXl[3];     // v0
    mAl[2][2] = 1.0;        //
    double dK1l = vXl[4];   // distortion parameter k1
    double dK2l = vXl[5];   // distortion parameter k2

    KHomogeneous* hPl;      // Homogeneous matrix
    hPl = new KHomogeneous[nImg];
    KMatrix* mPl;           // Projection matrix
    mPl = new KMatrix[nImg];
    KVector* vCl;           // Optical center
    vCl = new KVector[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vXl[6 * (i + 1)], vXl[6 * (i + 1) + 1], vXl[6 * (i + 1) + 2]);
        KVector   vT(vXl[6*(i + 1) + 3], vXl[6 * (i + 1) + 4], vXl[6 * (i + 1) + 5]);
        hPl[i] = KHomogeneous(rR, vT);      // 4x4 matrix, W->TL

        // Projection matrix about each image
        mPl[i] = mAl * hPl[i];

        // Compute the optical center
        vCl[i] = msjrv::OpticalCenter(mPl[i]);
    }

    // For right images
    KMatrix mAr(3, 3);
    mAr[0][0] = vXr[0];     // alpha
    mAr[1][1] = vXr[1];     // beta
    mAr[0][2] = vXr[2];     // u0
    mAr[1][2] = vXr[3];     // v0
    mAr[2][2] = 1.0;        //
    double dK1r = vXr[4];   // distortion parameter k1
    double dK2r = vXr[5];   // distortion parameter k2

    KHomogeneous* hPr;      // Homogeneous matrix
    hPr = new KHomogeneous[nImg];
    KMatrix* mPr;           // Projection matrix
    mPr = new KMatrix[nImg];
    KVector* vCr;           // Optical center
    vCr = new KVector[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vXr[6 * (i + 1)], vXr[6 * (i + 1) + 1], vXr[6 * (i + 1) + 2]);
        KVector   vT(vXr[6*(i + 1) + 3], vXr[6 * (i + 1) + 4], vXr[6 * (i + 1) + 5]);
        hPr[i] = KHomogeneous(rR, vT);      // 4x4 matrix, W->TR

        // Projection matrix about each image
        mPr[i] = mAr * hPr[i];

        // Compute the optical center
        vCr[i] = msjrv::OpticalCenter(mPr[i]);
    }

    // Two cameras should have the same intrinsic parameters, right camera' -> standard
    KMatrix mAn = mAr;

    // Both cameras must have the same rotation
    // Rwl = Rwr = R = (r11 r12 r13
    //                  r21 r22 r23
    //                  r31 r32 r33) = (vR1 vR2 vR3)
    // R' = R.Transpose
    KMatrix* mR;
    mR = new KMatrix[nImg];
    for (int i = 0; i < nImg; i++) {
        KVector vR1, vR2, vR3;
        vR1 = vCr[i] - vCl[i];
        vR1 /= sqrt(vR1.Norm2());   // Normalization

        KVector vK;
        vK = hPr[i].R().Tr().Column(2);
        vR2 = vK.Skew() * vR1;

        vR3 = vR1.Skew() * vR2;

        // R' = R.Transpose
        mR[i] ^= vR1.Tr();          // Add a row
        mR[i] ^= vR2.Tr();          // Add a row
        mR[i] ^= vR3.Tr();          // Add a row
    }

    // 출력을 위한 ImageForm 생성
    ImageForm* q_pForm[15] = {0,};
    string wName[] = {"img1", "img2", "img3", "img4", "img5", "img6", "img7", "img8", "img9", "img10", "img11", "img12", "img13", "img14", "img15"};
    for (int i = 0; i < nImg; i++) {
        string sImgNameL = defaultPath.toStdString() + "//..//..//Homeworks//data//hw02//calib_data//left";
        string sImgNameR = defaultPath.toStdString() + "//..//..//Homeworks//data//hw02//calib_data//right";
        sImgNameL += to_string(i + 1);
        sImgNameR += to_string(i + 1);
        sImgNameL += ".bmp";
        sImgNameR += ".bmp";

        // u~' = \lambda * Q' * Q^-1 * u~
        // Q = AR -> Q' = A'R'
        KMatrix* mQl;
        mQl = new KMatrix[nImg];
        KMatrix* mQr;
        mQr = new KMatrix[nImg];
        KMatrix* mQn;
        mQn = new KMatrix[nImg];
        mQl[i] = mAl * hPl[i].R();
        mQr[i] = mAr * hPr[i].R();
        mQn[i] = mAn * mR[i];

        KImageColor icMain(480, 1280);
        QImage* _imgL = new QImage();        // Left 이미지를 로드하기 위한 QImage 선언
        QImage* _imgR = new QImage();        // Right 이미지를 로드하기 위한 QImage 선언
        if (_imgL->load(QString::fromStdString(sImgNameL)) && _imgR->load(QString::fromStdString(sImgNameR))) {
            for (int ii = 0; ii < 480; ii++) {
                for (int jj = 0; jj < 640; jj++) {
                    if (ii % 80 == 0) {
                        // 일정 구간마다 가로로 red line을 그어
                        // 결과 확인
                        icMain[ii][jj].r = 255;
                        icMain[ii][jj].g = 0;
                        icMain[ii][jj].b = 0;
                        icMain[ii][jj + 640].r = 255;
                        icMain[ii][jj + 640].g = 0;
                        icMain[ii][jj + 640].b = 0;
                    }
                    else {
                        // Left image
                        // u~' = \lambda * Q' * Q^-1 * u~
                        KMatrix mTl = mQn[i] * mQl[i].Iv();
                        // Bilinear interpolation
                        KVector vPl = msjrv::BilinearInterpolation(_imgL, 640, 480, mTl, KVector(jj, ii, 1.0));
                        icMain[ii][jj].r = vPl[0];
                        icMain[ii][jj].g = vPl[1];
                        icMain[ii][jj].b = vPl[2];

                        // Right image
                        // u~' = \lambda * Q' * Q^-1 * u~
                        KMatrix mTr = mQn[i] * mQr[i].Iv();
                        // Bilinear interpolation
                        KVector vPr = msjrv::BilinearInterpolation(_imgR, 640, 480, mTr, KVector(jj, ii, 1.0));
                        icMain[ii][jj + 640].r = vPr[0];
                        icMain[ii][jj + 640].g = vPr[1];
                        icMain[ii][jj + 640].b = vPr[2];
                    }
                }
            }
        }
        else {
            qDebug() << "Image load Error";
        }

        for (unsigned int ii = 0; ii < _plpImageForm->Count(); ii++) {
            if ((*_plpImageForm)[ii]->ID() == QString::fromStdString(wName[i])) {
                q_pForm[i] = (*_plpImageForm)[ii];
            }
        }

        // Display window
        if (q_pForm[i]) {
            // 이미지 창이 이미 존재하면 아무것도 하지 않음
        }
        else {
            q_pForm[i] = new ImageForm(icMain, QString::fromStdString(wName[i]), this);
            q_pForm[i]->show();
            _plpImageForm->Add(q_pForm[i]);
        }

        delete _imgL;
        delete _imgR;
        delete[] mQl;
        delete[] mQr;
        delete[] mQn;
    }

    // UI 활성화 갱신
    UpdateUI();

    // Delete memory
    for (int i = 0; i < nImg; i++) {
        delete[] psFl[i];
    } delete[] psFl;
    for (int i = 0; i < nImg; i++) {
        delete[] psFr[i];
    } delete[] psFr;
    delete[] psM;
    delete[] hPl;
    delete[] hPr;
    delete[] mPl;
    delete[] mPr;
    qDebug() << "Process end\n";
}

//////////
// HW04 //
////////// AdaBoost
void MainFrame::on_pushAdaBoost_clicked()
{
    // Get sample features
    int nSample = 1000;
    int nFeature = 10 + 1;      // 10 features, label{+1, -1}
    double** dsF;
    dsF = new double*[nSample];
    for (int i = 0; i < nSample; i++) {
        dsF[i] = new double[nFeature];
    }
    QString defaultPath = QCoreApplication::applicationDirPath();
    string modelPath = "C:/RV/RobotVisionApp_Students/data/hw03/samples.txt";
    ifstream sampleFile(modelPath);
    if (sampleFile.is_open()) {
        for (int i = 0; i < nSample; i++) {
            for (int j = 0; j < nFeature; j++) {
                sampleFile >> dsF[i][j];
            }
//            qDebug() << dsF[i][0] << dsF[i][1] << dsF[i][2] << dsF[i][3] << dsF[i][4] << dsF[i][5] << dsF[i][6] << dsF[i][7] << dsF[i][8] << dsF[i][9] << dsF[i][10];
        }
    }
    else {
        qDebug() << "File open error!\n";
    }
    sampleFile.close();

    // Get num of weakclassifier
    int nWeak = ui->spinWeakclassifier->text().toInt();

    // {Error, TP, TN, FP, FN}
    KVector resClassify = msjrv::AdaBoost(nSample, nFeature, dsF, nWeak);

    // Print result
    QString temp_str;
    temp_str = QString::fromStdString(std::to_string(1 - resClassify[0]));
    ui->textAccuracy->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string((int)resClassify[1]));
    ui->textTp->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string((int)resClassify[2]));
    ui->textTn->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string((int)resClassify[3]));
    ui->textFp->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string((int)resClassify[4]));
    ui->textFn->setText(temp_str);

    // Delete memory
    for (int i = 0; i < nSample; i++) {
        delete[] dsF[i];
    } delete[] dsF;
    qDebug() << "Process end\n";
}
