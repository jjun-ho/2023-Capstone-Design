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

string data_dir = "C:/Users/ironview/Desktop/2023-Capstone-Design/IronView2/data/";
//////////
// HW01 //
////////// Mono Camera Calibration
void MainFrame::on_calib_btn_clicked()
{
    int nImg = 4;
    int nFeature = 54;

    KPoint** psF;
    KPoint* psM;

    psF = new KPoint*[nImg];
    for (int i = 0; i < nImg; i++) {
        psF[i] = new KPoint[nFeature];
    }
    psM = new KPoint[nFeature];

    // Get Model coordinates
    //QString defaultPath = QCoreApplication::applicationDirPath();
    string model_path = data_dir + "Cam123txt/model.txt";
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
        string fname = data_dir + "Cam123txt/left";
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
    ImageForm* q_pForm[4] = {0,};
    string wName[] = {"Image1", "Image2", "Image3", "Image4"};
    for (int i = 0; i < nImg; i++) {
        KImageColor icMain(1024, 1280);
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
    int left_cam_num = 1;
    int right_cam_num = 3;

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
    string model_path = data_dir + "Cam123txt/model.txt";
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
        string fname = data_dir + "Cam123txt/left";
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
        fname = data_dir + "Cam123txt/right";
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

    //RT save
    ofstream ofs;
    string RT_fname = data_dir + "RTtxt/RT_";
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

    // Model points
    KMatrix mM;             // 3 x mFeature matrix
    for (int i = 0; i < nFeature; i++) {
        mM |= KVector(psM[i]._dX, psM[i]._dY, 1.0);             // Add a column
    }

    // 출력을 위한 ImageForm 생성
    ImageForm* q_pForm[4] = {0,};
    string wName[] = {"left1", "left2", "left3", "left4"};
    for (int i = 0; i < nImg; i++) {
        string sImgName = data_dir + "Cam123txt/left";
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

void MainFrame::on_pushRtMatrix_clicked()
{

    int left_cam_num = 1;
    int right_cam_num = 2;

    string RT_path = data_dir + "RTtxt/RT_";
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
    int left_cam_num = 1;
    int main_cam_num = 2;
    int right_cam_num = 3;

    //T12
    string RT_path12 = data_dir + "RTtxt/RT_";
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

    KRotation Rt12;
    Rt12.FromRodrigues(buf12[0], buf12[1], buf12[2]);

    // T23
    string RT_path23 = data_dir + "RTtxt/RT_";
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

    KRotation Rt23;
    Rt23.FromRodrigues(buf23[0], buf23[1], buf23[2]);


    // T13
    KMatrix RT12(3,3);
    KMatrix RT23(3,3);
    KMatrix RT13(4,4);

    RT12 = KMatrix(Rt12);
    RT23 = KMatrix(Rt23);

    RT12 |= KVector(buf12[3],buf12[4],buf12[5]);
    RT12 ^= KVector(0,0,0,1).Tr();
    RT23 |= KVector(buf23[3],buf23[4],buf23[5]);
    RT23 ^= KVector(0,0,0,1).Tr();

    RT13 = RT12 * RT23;

    //view
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
