/********************************************************************************
** Form generated from reading UI file 'mainframe.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFRAME_H
#define UI_MAINFRAME_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainFrame
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *buttonOpen;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QToolButton *buttonDeleteContents;
    QSpacerItem *horizontalSpacer;
    QToolButton *buttonShowList;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QPushButton *zhang;
    QPushButton *plot_point;
    QLabel *label;
    QTextEdit *textAlpha;
    QTextEdit *textSkew;
    QTextEdit *textU0;
    QTextEdit *textZero_1;
    QTextEdit *textBeta;
    QTextEdit *textV0;
    QTextEdit *textZero_2;
    QTextEdit *textZero_3;
    QTextEdit *textScale;
    QLabel *label_2;
    QTextEdit *textK1;
    QTextEdit *textK2;
    QTextEdit *textError;
    QLabel *label_3;
    QWidget *tab_2;
    QPushButton *stereo_calibration;
    QLabel *label_4;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QTextEdit *textAlphaCam01;
    QTextEdit *textSkewCam01;
    QTextEdit *textU0Cam01;
    QTextEdit *textZero1Cam01;
    QTextEdit *textBetaCam01;
    QTextEdit *textV0Cam01;
    QTextEdit *textZero2Cam01;
    QTextEdit *textZero3Cam01;
    QTextEdit *textScaleCam01;
    QTextEdit *textK1Cam01;
    QLabel *label_15;
    QTextEdit *textK2Cam01;
    QLabel *label_16;
    QTextEdit *textErrorCam01;
    QTextEdit *textK1Cam02;
    QTextEdit *textZero1Cam02;
    QTextEdit *textZero2Cam02;
    QTextEdit *textSkewCam02;
    QTextEdit *textU0Cam02;
    QTextEdit *textAlphaCam02;
    QTextEdit *textV0Cam02;
    QTextEdit *textZero3Cam01_2;
    QLabel *label_33;
    QTextEdit *textScaleCam02;
    QTextEdit *textBetaCam02;
    QLabel *label_34;
    QTextEdit *textErrorCam02;
    QTextEdit *textK2Cam02;
    QTextEdit *textR1;
    QTextEdit *textR2;
    QTextEdit *textR3;
    QTextEdit *textT1;
    QTextEdit *textT2;
    QTextEdit *textT3;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QWidget *tab;
    QPushButton *pushRtMatrix;
    QPushButton *RT_product;
    QPushButton *CameraBtn;
    QPushButton *Checker_Corner;
    QWidget *tab_3;
    QPushButton *pushPanorama;
    QPushButton *Cylinderical_Warp;
    QPushButton *IMUButton;
    QListWidget *listWidget;

    void setupUi(QDialog *MainFrame)
    {
        if (MainFrame->objectName().isEmpty())
            MainFrame->setObjectName("MainFrame");
        MainFrame->resize(911, 536);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainFrame->sizePolicy().hasHeightForWidth());
        MainFrame->setSizePolicy(sizePolicy);
        MainFrame->setMinimumSize(QSize(0, 536));
        MainFrame->setModal(false);
        verticalLayout = new QVBoxLayout(MainFrame);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        frame = new QFrame(MainFrame);
        frame->setObjectName("frame");
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setMinimumSize(QSize(0, 41));
        frame->setMaximumSize(QSize(16777215, 41));
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        buttonOpen = new QToolButton(frame);
        buttonOpen->setObjectName("buttonOpen");
        buttonOpen->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(buttonOpen->sizePolicy().hasHeightForWidth());
        buttonOpen->setSizePolicy(sizePolicy2);
        buttonOpen->setMinimumSize(QSize(41, 41));
        buttonOpen->setMaximumSize(QSize(41, 41));
        buttonOpen->setLayoutDirection(Qt::LeftToRight);
        buttonOpen->setAutoFillBackground(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonOpen->setIcon(icon);
        buttonOpen->setIconSize(QSize(41, 41));
        buttonOpen->setCheckable(false);
        buttonOpen->setAutoRepeat(false);
        buttonOpen->setAutoExclusive(false);
        buttonOpen->setPopupMode(QToolButton::DelayedPopup);
        buttonOpen->setToolButtonStyle(Qt::ToolButtonIconOnly);
        buttonOpen->setAutoRaise(false);

        horizontalLayout_3->addWidget(buttonOpen);

        toolButton_2 = new QToolButton(frame);
        toolButton_2->setObjectName("toolButton_2");
        sizePolicy2.setHeightForWidth(toolButton_2->sizePolicy().hasHeightForWidth());
        toolButton_2->setSizePolicy(sizePolicy2);
        toolButton_2->setMinimumSize(QSize(41, 41));
        toolButton_2->setMaximumSize(QSize(41, 41));

        horizontalLayout_3->addWidget(toolButton_2);

        toolButton_3 = new QToolButton(frame);
        toolButton_3->setObjectName("toolButton_3");
        sizePolicy2.setHeightForWidth(toolButton_3->sizePolicy().hasHeightForWidth());
        toolButton_3->setSizePolicy(sizePolicy2);
        toolButton_3->setMinimumSize(QSize(41, 41));
        toolButton_3->setMaximumSize(QSize(41, 41));

        horizontalLayout_3->addWidget(toolButton_3);

        buttonDeleteContents = new QToolButton(frame);
        buttonDeleteContents->setObjectName("buttonDeleteContents");
        sizePolicy2.setHeightForWidth(buttonDeleteContents->sizePolicy().hasHeightForWidth());
        buttonDeleteContents->setSizePolicy(sizePolicy2);
        buttonDeleteContents->setMinimumSize(QSize(41, 41));
        buttonDeleteContents->setMaximumSize(QSize(41, 41));
        buttonDeleteContents->setAutoFillBackground(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/1-21.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDeleteContents->setIcon(icon1);
        buttonDeleteContents->setIconSize(QSize(41, 41));

        horizontalLayout_3->addWidget(buttonDeleteContents);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        buttonShowList = new QToolButton(frame);
        buttonShowList->setObjectName("buttonShowList");
        sizePolicy2.setHeightForWidth(buttonShowList->sizePolicy().hasHeightForWidth());
        buttonShowList->setSizePolicy(sizePolicy2);
        buttonShowList->setMinimumSize(QSize(41, 41));
        buttonShowList->setMaximumSize(QSize(41, 41));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/2-3.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonShowList->setIcon(icon2);
        buttonShowList->setIconSize(QSize(82, 41));

        horizontalLayout_3->addWidget(buttonShowList);


        verticalLayout->addWidget(frame);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(2, -1, -1, -1);
        tabWidget = new QTabWidget(MainFrame);
        tabWidget->setObjectName("tabWidget");
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy3);
        tabWidget->setMinimumSize(QSize(600, 400));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        QBrush brush1(QColor(255, 85, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::NoRole, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::NoRole, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::NoRole, brush1);
        tabWidget->setPalette(palette);
        tabWidget->setCursor(QCursor(Qt::ArrowCursor));
        tabWidget->setAutoFillBackground(false);
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setIconSize(QSize(31, 31));
        tabWidget->setElideMode(Qt::ElideMiddle);
        tab_1 = new QWidget();
        tab_1->setObjectName("tab_1");
        zhang = new QPushButton(tab_1);
        zhang->setObjectName("zhang");
        zhang->setGeometry(QRect(160, 20, 131, 31));
        plot_point = new QPushButton(tab_1);
        plot_point->setObjectName("plot_point");
        plot_point->setGeometry(QRect(300, 20, 131, 31));
        label = new QLabel(tab_1);
        label->setObjectName("label");
        label->setGeometry(QRect(160, 70, 111, 16));
        textAlpha = new QTextEdit(tab_1);
        textAlpha->setObjectName("textAlpha");
        textAlpha->setGeometry(QRect(160, 90, 91, 31));
        textSkew = new QTextEdit(tab_1);
        textSkew->setObjectName("textSkew");
        textSkew->setGeometry(QRect(250, 90, 91, 31));
        textU0 = new QTextEdit(tab_1);
        textU0->setObjectName("textU0");
        textU0->setGeometry(QRect(340, 90, 91, 31));
        textZero_1 = new QTextEdit(tab_1);
        textZero_1->setObjectName("textZero_1");
        textZero_1->setGeometry(QRect(160, 130, 91, 31));
        textBeta = new QTextEdit(tab_1);
        textBeta->setObjectName("textBeta");
        textBeta->setGeometry(QRect(250, 130, 91, 31));
        textV0 = new QTextEdit(tab_1);
        textV0->setObjectName("textV0");
        textV0->setGeometry(QRect(340, 130, 91, 31));
        textZero_2 = new QTextEdit(tab_1);
        textZero_2->setObjectName("textZero_2");
        textZero_2->setGeometry(QRect(160, 170, 91, 31));
        textZero_3 = new QTextEdit(tab_1);
        textZero_3->setObjectName("textZero_3");
        textZero_3->setGeometry(QRect(250, 170, 91, 31));
        textScale = new QTextEdit(tab_1);
        textScale->setObjectName("textScale");
        textScale->setGeometry(QRect(340, 170, 91, 31));
        label_2 = new QLabel(tab_1);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(160, 220, 121, 16));
        textK1 = new QTextEdit(tab_1);
        textK1->setObjectName("textK1");
        textK1->setGeometry(QRect(160, 240, 91, 31));
        textK2 = new QTextEdit(tab_1);
        textK2->setObjectName("textK2");
        textK2->setGeometry(QRect(250, 240, 91, 31));
        textError = new QTextEdit(tab_1);
        textError->setObjectName("textError");
        textError->setGeometry(QRect(160, 300, 91, 31));
        label_3 = new QLabel(tab_1);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(160, 280, 121, 16));
        tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        stereo_calibration = new QPushButton(tab_2);
        stereo_calibration->setObjectName("stereo_calibration");
        stereo_calibration->setGeometry(QRect(170, 10, 241, 31));
        label_4 = new QLabel(tab_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 50, 81, 21));
        label_12 = new QLabel(tab_2);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(310, 50, 81, 21));
        label_13 = new QLabel(tab_2);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(20, 70, 111, 21));
        label_14 = new QLabel(tab_2);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(310, 70, 111, 21));
        textAlphaCam01 = new QTextEdit(tab_2);
        textAlphaCam01->setObjectName("textAlphaCam01");
        textAlphaCam01->setGeometry(QRect(20, 90, 91, 31));
        textSkewCam01 = new QTextEdit(tab_2);
        textSkewCam01->setObjectName("textSkewCam01");
        textSkewCam01->setGeometry(QRect(110, 90, 91, 31));
        textU0Cam01 = new QTextEdit(tab_2);
        textU0Cam01->setObjectName("textU0Cam01");
        textU0Cam01->setGeometry(QRect(200, 90, 91, 31));
        textZero1Cam01 = new QTextEdit(tab_2);
        textZero1Cam01->setObjectName("textZero1Cam01");
        textZero1Cam01->setGeometry(QRect(20, 130, 91, 31));
        textBetaCam01 = new QTextEdit(tab_2);
        textBetaCam01->setObjectName("textBetaCam01");
        textBetaCam01->setGeometry(QRect(110, 130, 91, 31));
        textV0Cam01 = new QTextEdit(tab_2);
        textV0Cam01->setObjectName("textV0Cam01");
        textV0Cam01->setGeometry(QRect(200, 130, 91, 31));
        textZero2Cam01 = new QTextEdit(tab_2);
        textZero2Cam01->setObjectName("textZero2Cam01");
        textZero2Cam01->setGeometry(QRect(20, 170, 91, 31));
        textZero3Cam01 = new QTextEdit(tab_2);
        textZero3Cam01->setObjectName("textZero3Cam01");
        textZero3Cam01->setGeometry(QRect(110, 170, 91, 31));
        textScaleCam01 = new QTextEdit(tab_2);
        textScaleCam01->setObjectName("textScaleCam01");
        textScaleCam01->setGeometry(QRect(200, 170, 91, 31));
        textK1Cam01 = new QTextEdit(tab_2);
        textK1Cam01->setObjectName("textK1Cam01");
        textK1Cam01->setGeometry(QRect(20, 240, 91, 31));
        label_15 = new QLabel(tab_2);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(20, 220, 111, 21));
        textK2Cam01 = new QTextEdit(tab_2);
        textK2Cam01->setObjectName("textK2Cam01");
        textK2Cam01->setGeometry(QRect(110, 240, 91, 31));
        label_16 = new QLabel(tab_2);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(20, 280, 111, 21));
        textErrorCam01 = new QTextEdit(tab_2);
        textErrorCam01->setObjectName("textErrorCam01");
        textErrorCam01->setGeometry(QRect(20, 300, 91, 31));
        textK1Cam02 = new QTextEdit(tab_2);
        textK1Cam02->setObjectName("textK1Cam02");
        textK1Cam02->setGeometry(QRect(310, 240, 91, 31));
        textZero1Cam02 = new QTextEdit(tab_2);
        textZero1Cam02->setObjectName("textZero1Cam02");
        textZero1Cam02->setGeometry(QRect(310, 130, 91, 31));
        textZero2Cam02 = new QTextEdit(tab_2);
        textZero2Cam02->setObjectName("textZero2Cam02");
        textZero2Cam02->setGeometry(QRect(310, 170, 91, 31));
        textSkewCam02 = new QTextEdit(tab_2);
        textSkewCam02->setObjectName("textSkewCam02");
        textSkewCam02->setGeometry(QRect(400, 90, 91, 31));
        textU0Cam02 = new QTextEdit(tab_2);
        textU0Cam02->setObjectName("textU0Cam02");
        textU0Cam02->setGeometry(QRect(490, 90, 91, 31));
        textAlphaCam02 = new QTextEdit(tab_2);
        textAlphaCam02->setObjectName("textAlphaCam02");
        textAlphaCam02->setGeometry(QRect(310, 90, 91, 31));
        textV0Cam02 = new QTextEdit(tab_2);
        textV0Cam02->setObjectName("textV0Cam02");
        textV0Cam02->setGeometry(QRect(490, 130, 91, 31));
        textZero3Cam01_2 = new QTextEdit(tab_2);
        textZero3Cam01_2->setObjectName("textZero3Cam01_2");
        textZero3Cam01_2->setGeometry(QRect(400, 170, 91, 31));
        label_33 = new QLabel(tab_2);
        label_33->setObjectName("label_33");
        label_33->setGeometry(QRect(310, 220, 111, 21));
        textScaleCam02 = new QTextEdit(tab_2);
        textScaleCam02->setObjectName("textScaleCam02");
        textScaleCam02->setGeometry(QRect(490, 170, 91, 31));
        textBetaCam02 = new QTextEdit(tab_2);
        textBetaCam02->setObjectName("textBetaCam02");
        textBetaCam02->setGeometry(QRect(400, 130, 91, 31));
        label_34 = new QLabel(tab_2);
        label_34->setObjectName("label_34");
        label_34->setGeometry(QRect(310, 280, 111, 21));
        textErrorCam02 = new QTextEdit(tab_2);
        textErrorCam02->setObjectName("textErrorCam02");
        textErrorCam02->setGeometry(QRect(310, 300, 91, 31));
        textK2Cam02 = new QTextEdit(tab_2);
        textK2Cam02->setObjectName("textK2Cam02");
        textK2Cam02->setGeometry(QRect(400, 240, 91, 31));
        textR1 = new QTextEdit(tab_2);
        textR1->setObjectName("textR1");
        textR1->setGeometry(QRect(20, 390, 81, 31));
        textR2 = new QTextEdit(tab_2);
        textR2->setObjectName("textR2");
        textR2->setGeometry(QRect(110, 390, 81, 31));
        textR3 = new QTextEdit(tab_2);
        textR3->setObjectName("textR3");
        textR3->setGeometry(QRect(200, 390, 81, 31));
        textT1 = new QTextEdit(tab_2);
        textT1->setObjectName("textT1");
        textT1->setGeometry(QRect(290, 390, 81, 31));
        textT2 = new QTextEdit(tab_2);
        textT2->setObjectName("textT2");
        textT2->setGeometry(QRect(380, 390, 81, 31));
        textT3 = new QTextEdit(tab_2);
        textT3->setObjectName("textT3");
        textT3->setGeometry(QRect(470, 390, 81, 31));
        label_5 = new QLabel(tab_2);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 370, 48, 16));
        label_6 = new QLabel(tab_2);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(110, 370, 48, 16));
        label_7 = new QLabel(tab_2);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(200, 370, 48, 16));
        label_8 = new QLabel(tab_2);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(290, 370, 48, 16));
        label_9 = new QLabel(tab_2);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(380, 370, 48, 16));
        label_10 = new QLabel(tab_2);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(470, 370, 48, 16));
        label_11 = new QLabel(tab_2);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(20, 350, 171, 16));
        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        pushRtMatrix = new QPushButton(tab);
        pushRtMatrix->setObjectName("pushRtMatrix");
        pushRtMatrix->setGeometry(QRect(160, 200, 231, 51));
        RT_product = new QPushButton(tab);
        RT_product->setObjectName("RT_product");
        RT_product->setGeometry(QRect(160, 280, 231, 51));
        CameraBtn = new QPushButton(tab);
        CameraBtn->setObjectName("CameraBtn");
        CameraBtn->setGeometry(QRect(180, 60, 191, 51));
        Checker_Corner = new QPushButton(tab);
        Checker_Corner->setObjectName("Checker_Corner");
        Checker_Corner->setGeometry(QRect(180, 130, 191, 51));
        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        pushPanorama = new QPushButton(tab_3);
        pushPanorama->setObjectName("pushPanorama");
        pushPanorama->setGeometry(QRect(210, 130, 131, 51));
        Cylinderical_Warp = new QPushButton(tab_3);
        Cylinderical_Warp->setObjectName("Cylinderical_Warp");
        Cylinderical_Warp->setGeometry(QRect(200, 200, 151, 51));
        IMUButton = new QPushButton(tab_3);
        IMUButton->setObjectName("IMUButton");
        IMUButton->setGeometry(QRect(230, 80, 91, 41));
        tabWidget->addTab(tab_3, QString());

        horizontalLayout->addWidget(tabWidget);

        listWidget = new QListWidget(MainFrame);
        listWidget->setObjectName("listWidget");
        QSizePolicy sizePolicy4(QSizePolicy::Ignored, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy4);
        listWidget->setMinimumSize(QSize(0, 394));
        QPalette palette1;
        QBrush brush2(QColor(255, 255, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush3(QColor(0, 0, 127, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush3);
        QBrush brush4(QColor(255, 255, 255, 128));
        brush4.setStyle(Qt::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::Active, QPalette::PlaceholderText, brush4);
#endif
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush3);
        QBrush brush5(QColor(255, 255, 255, 128));
        brush5.setStyle(Qt::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush5);
#endif
        QBrush brush6(QColor(120, 120, 120, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        QBrush brush7(QColor(240, 240, 240, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        QBrush brush8(QColor(255, 255, 255, 128));
        brush8.setStyle(Qt::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush8);
#endif
        listWidget->setPalette(palette1);
        QFont font;
        font.setFamilies({QString::fromUtf8("Times New Roman")});
        font.setPointSize(10);
        listWidget->setFont(font);

        horizontalLayout->addWidget(listWidget);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(MainFrame);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainFrame);
    } // setupUi

    void retranslateUi(QDialog *MainFrame)
    {
        MainFrame->setWindowTitle(QCoreApplication::translate("MainFrame", "\353\241\234\353\264\207\353\271\204\354\240\204\354\235\221\354\232\251", nullptr));
#if QT_CONFIG(tooltip)
        buttonOpen->setToolTip(QCoreApplication::translate("MainFrame", "open an image file", nullptr));
#endif // QT_CONFIG(tooltip)
        buttonOpen->setText(QString());
        toolButton_2->setText(QCoreApplication::translate("MainFrame", "...", nullptr));
        toolButton_3->setText(QCoreApplication::translate("MainFrame", "...", nullptr));
#if QT_CONFIG(tooltip)
        buttonDeleteContents->setToolTip(QCoreApplication::translate("MainFrame", "close all forms", nullptr));
#endif // QT_CONFIG(tooltip)
        buttonDeleteContents->setText(QString());
#if QT_CONFIG(tooltip)
        buttonShowList->setToolTip(QCoreApplication::translate("MainFrame", "show the list view", nullptr));
#endif // QT_CONFIG(tooltip)
        buttonShowList->setText(QString());
        zhang->setText(QCoreApplication::translate("MainFrame", "Zhang's Calibration", nullptr));
        plot_point->setText(QCoreApplication::translate("MainFrame", "Plot Point", nullptr));
        label->setText(QCoreApplication::translate("MainFrame", "Intrinsic Parameter", nullptr));
        textAlpha->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Alpha</p></body></html>", nullptr));
        textSkew->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Skew(0.0)</p></body></html>", nullptr));
        textU0->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">u0</p></body></html>", nullptr));
        textZero_1->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.0</p></body></html>", nullptr));
        textBeta->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Beta</p></body></html>", nullptr));
        textV0->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">v0</p></body></html>", nullptr));
        textZero_2->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.0</p></body></html>", nullptr));
        textZero_3->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.0</p></body></html>", nullptr));
        textScale->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">1.0</p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("MainFrame", "Distortion Parameter", nullptr));
        textK1->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">k1</p></body></html>", nullptr));
        textK2->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">k2</p></body></html>", nullptr));
        textError->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.0</p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("MainFrame", "Error", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QCoreApplication::translate("MainFrame", "Mono Calibration", nullptr));
        stereo_calibration->setText(QCoreApplication::translate("MainFrame", "Stereo Calibration", nullptr));
        label_4->setText(QCoreApplication::translate("MainFrame", "Cam_L", nullptr));
        label_12->setText(QCoreApplication::translate("MainFrame", "Cam_R", nullptr));
        label_13->setText(QCoreApplication::translate("MainFrame", "Intrinsic Patrameter", nullptr));
        label_14->setText(QCoreApplication::translate("MainFrame", "Intrinsic Patrameter", nullptr));
        textAlphaCam01->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Alpha</p></body></html>", nullptr));
        textSkewCam01->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Skew(0.0)</p></body></html>", nullptr));
        textU0Cam01->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">u0</p></body></html>", nullptr));
        textZero1Cam01->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.0</p></body></html>", nullptr));
        textBetaCam01->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Beta</p></body></html>", nullptr));
        textV0Cam01->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">v0</p></body></html>", nullptr));
        textZero2Cam01->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.0</p></body></html>", nullptr));
        textZero3Cam01->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.0</p></body></html>", nullptr));
        textScaleCam01->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">1.0</p></body></html>", nullptr));
        textK1Cam01->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">k1</p></body></html>", nullptr));
        label_15->setText(QCoreApplication::translate("MainFrame", "Distortion Parameter", nullptr));
        textK2Cam01->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">k2</p></body></html>", nullptr));
        label_16->setText(QCoreApplication::translate("MainFrame", "Error", nullptr));
        textErrorCam01->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.0</p></body></html>", nullptr));
        textK1Cam02->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">k1</p></body></html>", nullptr));
        textZero1Cam02->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.0</p></body></html>", nullptr));
        textZero2Cam02->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.0</p></body></html>", nullptr));
        textSkewCam02->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Skew(0.0)</p></body></html>", nullptr));
        textU0Cam02->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">u0</p></body></html>", nullptr));
        textAlphaCam02->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Alpha</p></body></html>", nullptr));
        textV0Cam02->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">v0</p></body></html>", nullptr));
        textZero3Cam01_2->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.0</p></body></html>", nullptr));
        label_33->setText(QCoreApplication::translate("MainFrame", "Distortion Parameter", nullptr));
        textScaleCam02->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">1.0</p></body></html>", nullptr));
        textBetaCam02->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Beta</p></body></html>", nullptr));
        label_34->setText(QCoreApplication::translate("MainFrame", "Error", nullptr));
        textErrorCam02->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.0</p></body></html>", nullptr));
        textK2Cam02->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">k2</p></body></html>", nullptr));
        textR1->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        textR2->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        textR3->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        textT1->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        textT2->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        textT3->setHtml(QCoreApplication::translate("MainFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        label_5->setText(QCoreApplication::translate("MainFrame", "r1", nullptr));
        label_6->setText(QCoreApplication::translate("MainFrame", "r2", nullptr));
        label_7->setText(QCoreApplication::translate("MainFrame", "r3", nullptr));
        label_8->setText(QCoreApplication::translate("MainFrame", "t1", nullptr));
        label_9->setText(QCoreApplication::translate("MainFrame", "t2", nullptr));
        label_10->setText(QCoreApplication::translate("MainFrame", "t3", nullptr));
        label_11->setText(QCoreApplication::translate("MainFrame", "R|t from Right to Left", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainFrame", "Stereo Calibration", nullptr));
        pushRtMatrix->setText(QCoreApplication::translate("MainFrame", "RT Matrix", nullptr));
        RT_product->setText(QCoreApplication::translate("MainFrame", "RT Product", nullptr));
        CameraBtn->setText(QCoreApplication::translate("MainFrame", "Camera", nullptr));
        Checker_Corner->setText(QCoreApplication::translate("MainFrame", "CheckerBoard to Points", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainFrame", "Page", nullptr));
        pushPanorama->setText(QCoreApplication::translate("MainFrame", "Panorama", nullptr));
        Cylinderical_Warp->setText(QCoreApplication::translate("MainFrame", "Cylindrical Warp", nullptr));
        IMUButton->setText(QCoreApplication::translate("MainFrame", "IMU", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainFrame", "Page", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainFrame: public Ui_MainFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFRAME_H
