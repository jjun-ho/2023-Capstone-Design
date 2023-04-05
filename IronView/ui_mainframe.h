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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
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
    QSpinBox *SpinHue;
    QPushButton *ApplySepia;
    QDoubleSpinBox *SpinSaturation;
    QLabel *label_Hue;
    QLabel *label_Sat;
    QWidget *tab_2;
    QPushButton *Apply_Otsu_Threshold;
    QPushButton *Apply_Labeling;
    QSpinBox *SpinThreshold;
    QLabel *label;
    QPushButton *Apply_Threshold;
    QSpinBox *spinKernel;
    QLabel *label_2;
    QPushButton *Apply_Dilation;
    QPushButton *Apply_Erosion;
    QWidget *tab_3;
    QPushButton *Histogram_Equalization;
    QPushButton *Histogram_Matching;
    QPushButton *Register_Reference;
    QWidget *tab_4;
    QPushButton *Apply_Gaussian;
    QPushButton *Apply_SNP;
    QLabel *label_3;
    QDoubleSpinBox *SNP_tau;
    QDoubleSpinBox *Gaussian_sigma;
    QPushButton *Apply_BoxFilter;
    QPushButton *Apply_GaussianFilter;
    QPushButton *Apply_MedianFilter;
    QLabel *label_4;
    QComboBox *BoxFilter_Size;
    QDoubleSpinBox *Gaussian_Filter_Sigma;
    QLabel *label_5;
    QComboBox *MedianFilter_Size;
    QLabel *label_6;
    QWidget *tab_5;
    QPushButton *Apply_Canny_edge;
    QDoubleSpinBox *spin_Sigma;
    QSpinBox *spin_High_Threshold;
    QSpinBox *spin_Low_Threshold;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QWidget *tab_6;
    QPushButton *Apply_HoughCircle;
    QPushButton *Apply_Generalized_Hough_Transform;
    QWidget *tab_7;
    QPushButton *Apply_Optical_Flow;
    QPushButton *Apply_Compare_Img;
    QWidget *tab_8;
    QPushButton *CameraBtn;
    QPushButton *Checker_Corner;
    QListWidget *listWidget;

    void setupUi(QDialog *MainFrame)
    {
        if (MainFrame->objectName().isEmpty())
            MainFrame->setObjectName("MainFrame");
        MainFrame->resize(622, 461);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainFrame->sizePolicy().hasHeightForWidth());
        MainFrame->setSizePolicy(sizePolicy);
        MainFrame->setMinimumSize(QSize(0, 461));
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
        tabWidget->setMinimumSize(QSize(299, 394));
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
        SpinHue = new QSpinBox(tab_1);
        SpinHue->setObjectName("SpinHue");
        SpinHue->setGeometry(QRect(80, 70, 71, 31));
        SpinHue->setMaximum(360);
        SpinHue->setSingleStep(1);
        ApplySepia = new QPushButton(tab_1);
        ApplySepia->setObjectName("ApplySepia");
        ApplySepia->setGeometry(QRect(20, 20, 261, 41));
        SpinSaturation = new QDoubleSpinBox(tab_1);
        SpinSaturation->setObjectName("SpinSaturation");
        SpinSaturation->setGeometry(QRect(200, 70, 81, 31));
        SpinSaturation->setMaximum(1.000000000000000);
        SpinSaturation->setSingleStep(0.100000000000000);
        label_Hue = new QLabel(tab_1);
        label_Hue->setObjectName("label_Hue");
        label_Hue->setGeometry(QRect(20, 70, 61, 31));
        label_Sat = new QLabel(tab_1);
        label_Sat->setObjectName("label_Sat");
        label_Sat->setGeometry(QRect(150, 70, 51, 31));
        tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        Apply_Otsu_Threshold = new QPushButton(tab_2);
        Apply_Otsu_Threshold->setObjectName("Apply_Otsu_Threshold");
        Apply_Otsu_Threshold->setGeometry(QRect(20, 20, 251, 41));
        Apply_Labeling = new QPushButton(tab_2);
        Apply_Labeling->setObjectName("Apply_Labeling");
        Apply_Labeling->setGeometry(QRect(20, 300, 251, 41));
        SpinThreshold = new QSpinBox(tab_2);
        SpinThreshold->setObjectName("SpinThreshold");
        SpinThreshold->setGeometry(QRect(100, 70, 61, 31));
        SpinThreshold->setMaximum(255);
        label = new QLabel(tab_2);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 80, 71, 16));
        Apply_Threshold = new QPushButton(tab_2);
        Apply_Threshold->setObjectName("Apply_Threshold");
        Apply_Threshold->setGeometry(QRect(20, 110, 251, 41));
        spinKernel = new QSpinBox(tab_2);
        spinKernel->setObjectName("spinKernel");
        spinKernel->setGeometry(QRect(100, 160, 61, 31));
        spinKernel->setMinimum(3);
        spinKernel->setMaximum(9);
        spinKernel->setSingleStep(2);
        label_2 = new QLabel(tab_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 160, 71, 31));
        Apply_Dilation = new QPushButton(tab_2);
        Apply_Dilation->setObjectName("Apply_Dilation");
        Apply_Dilation->setGeometry(QRect(20, 200, 251, 41));
        Apply_Erosion = new QPushButton(tab_2);
        Apply_Erosion->setObjectName("Apply_Erosion");
        Apply_Erosion->setGeometry(QRect(20, 250, 251, 41));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        Histogram_Equalization = new QPushButton(tab_3);
        Histogram_Equalization->setObjectName("Histogram_Equalization");
        Histogram_Equalization->setGeometry(QRect(20, 20, 251, 41));
        Histogram_Matching = new QPushButton(tab_3);
        Histogram_Matching->setObjectName("Histogram_Matching");
        Histogram_Matching->setGeometry(QRect(20, 140, 251, 41));
        Register_Reference = new QPushButton(tab_3);
        Register_Reference->setObjectName("Register_Reference");
        Register_Reference->setGeometry(QRect(20, 90, 251, 41));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        Apply_Gaussian = new QPushButton(tab_4);
        Apply_Gaussian->setObjectName("Apply_Gaussian");
        Apply_Gaussian->setGeometry(QRect(140, 40, 131, 31));
        Apply_SNP = new QPushButton(tab_4);
        Apply_SNP->setObjectName("Apply_SNP");
        Apply_SNP->setGeometry(QRect(140, 80, 131, 31));
        label_3 = new QLabel(tab_4);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 20, 71, 16));
        SNP_tau = new QDoubleSpinBox(tab_4);
        SNP_tau->setObjectName("SNP_tau");
        SNP_tau->setGeometry(QRect(20, 80, 111, 31));
        SNP_tau->setMaximum(1.000000000000000);
        SNP_tau->setSingleStep(0.100000000000000);
        Gaussian_sigma = new QDoubleSpinBox(tab_4);
        Gaussian_sigma->setObjectName("Gaussian_sigma");
        Gaussian_sigma->setGeometry(QRect(20, 40, 111, 31));
        Gaussian_sigma->setMaximum(10.000000000000000);
        Apply_BoxFilter = new QPushButton(tab_4);
        Apply_BoxFilter->setObjectName("Apply_BoxFilter");
        Apply_BoxFilter->setGeometry(QRect(140, 140, 131, 31));
        Apply_GaussianFilter = new QPushButton(tab_4);
        Apply_GaussianFilter->setObjectName("Apply_GaussianFilter");
        Apply_GaussianFilter->setGeometry(QRect(140, 200, 131, 31));
        Apply_MedianFilter = new QPushButton(tab_4);
        Apply_MedianFilter->setObjectName("Apply_MedianFilter");
        Apply_MedianFilter->setGeometry(QRect(140, 260, 131, 31));
        label_4 = new QLabel(tab_4);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 120, 71, 16));
        BoxFilter_Size = new QComboBox(tab_4);
        BoxFilter_Size->addItem(QString());
        BoxFilter_Size->addItem(QString());
        BoxFilter_Size->setObjectName("BoxFilter_Size");
        BoxFilter_Size->setGeometry(QRect(20, 140, 111, 31));
        Gaussian_Filter_Sigma = new QDoubleSpinBox(tab_4);
        Gaussian_Filter_Sigma->setObjectName("Gaussian_Filter_Sigma");
        Gaussian_Filter_Sigma->setGeometry(QRect(20, 200, 111, 31));
        Gaussian_Filter_Sigma->setMaximum(10.000000000000000);
        label_5 = new QLabel(tab_4);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 180, 48, 16));
        MedianFilter_Size = new QComboBox(tab_4);
        MedianFilter_Size->addItem(QString());
        MedianFilter_Size->addItem(QString());
        MedianFilter_Size->setObjectName("MedianFilter_Size");
        MedianFilter_Size->setGeometry(QRect(20, 260, 111, 31));
        label_6 = new QLabel(tab_4);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(20, 240, 71, 16));
        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName("tab_5");
        Apply_Canny_edge = new QPushButton(tab_5);
        Apply_Canny_edge->setObjectName("Apply_Canny_edge");
        Apply_Canny_edge->setGeometry(QRect(20, 20, 251, 41));
        spin_Sigma = new QDoubleSpinBox(tab_5);
        spin_Sigma->setObjectName("spin_Sigma");
        spin_Sigma->setGeometry(QRect(140, 90, 91, 31));
        spin_Sigma->setMaximum(5.000000000000000);
        spin_Sigma->setSingleStep(0.500000000000000);
        spin_High_Threshold = new QSpinBox(tab_5);
        spin_High_Threshold->setObjectName("spin_High_Threshold");
        spin_High_Threshold->setGeometry(QRect(140, 130, 91, 31));
        spin_High_Threshold->setMaximum(255);
        spin_High_Threshold->setSingleStep(30);
        spin_Low_Threshold = new QSpinBox(tab_5);
        spin_Low_Threshold->setObjectName("spin_Low_Threshold");
        spin_Low_Threshold->setGeometry(QRect(140, 170, 91, 31));
        spin_Low_Threshold->setMaximum(255);
        spin_Low_Threshold->setSingleStep(30);
        label_7 = new QLabel(tab_5);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(60, 90, 41, 31));
        label_8 = new QLabel(tab_5);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(40, 130, 91, 31));
        label_9 = new QLabel(tab_5);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(40, 170, 91, 31));
        tabWidget->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName("tab_6");
        Apply_HoughCircle = new QPushButton(tab_6);
        Apply_HoughCircle->setObjectName("Apply_HoughCircle");
        Apply_HoughCircle->setGeometry(QRect(20, 20, 251, 41));
        Apply_Generalized_Hough_Transform = new QPushButton(tab_6);
        Apply_Generalized_Hough_Transform->setObjectName("Apply_Generalized_Hough_Transform");
        Apply_Generalized_Hough_Transform->setGeometry(QRect(20, 80, 251, 41));
        tabWidget->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName("tab_7");
        Apply_Optical_Flow = new QPushButton(tab_7);
        Apply_Optical_Flow->setObjectName("Apply_Optical_Flow");
        Apply_Optical_Flow->setGeometry(QRect(20, 20, 251, 41));
        Apply_Compare_Img = new QPushButton(tab_7);
        Apply_Compare_Img->setObjectName("Apply_Compare_Img");
        Apply_Compare_Img->setGeometry(QRect(20, 80, 251, 41));
        tabWidget->addTab(tab_7, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName("tab_8");
        CameraBtn = new QPushButton(tab_8);
        CameraBtn->setObjectName("CameraBtn");
        CameraBtn->setGeometry(QRect(110, 100, 80, 24));
        Checker_Corner = new QPushButton(tab_8);
        Checker_Corner->setObjectName("Checker_Corner");
        Checker_Corner->setGeometry(QRect(100, 140, 101, 31));
        tabWidget->addTab(tab_8, QString());

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
        font.setPointSize(10);
        listWidget->setFont(font);

        horizontalLayout->addWidget(listWidget);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(MainFrame);

        tabWidget->setCurrentIndex(7);
        BoxFilter_Size->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainFrame);
    } // setupUi

    void retranslateUi(QDialog *MainFrame)
    {
        MainFrame->setWindowTitle(QCoreApplication::translate("MainFrame", "Homeworks", nullptr));
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
        ApplySepia->setText(QCoreApplication::translate("MainFrame", "Apply Sepia", nullptr));
        label_Hue->setText(QCoreApplication::translate("MainFrame", "    Hue", nullptr));
        label_Sat->setText(QCoreApplication::translate("MainFrame", "    Sat", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QCoreApplication::translate("MainFrame", "1", nullptr));
        Apply_Otsu_Threshold->setText(QCoreApplication::translate("MainFrame", "Apply Otsu Threshold", nullptr));
        Apply_Labeling->setText(QCoreApplication::translate("MainFrame", "Apply Labeling", nullptr));
        label->setText(QCoreApplication::translate("MainFrame", "Threshold", nullptr));
        Apply_Threshold->setText(QCoreApplication::translate("MainFrame", "Apply Threshold", nullptr));
        label_2->setText(QCoreApplication::translate("MainFrame", "Kernel Size", nullptr));
        Apply_Dilation->setText(QCoreApplication::translate("MainFrame", "Apply Dilation", nullptr));
        Apply_Erosion->setText(QCoreApplication::translate("MainFrame", "Apply Erosion", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainFrame", "2", nullptr));
        Histogram_Equalization->setText(QCoreApplication::translate("MainFrame", "Histogram Equalization", nullptr));
        Histogram_Matching->setText(QCoreApplication::translate("MainFrame", "Histogram Matching", nullptr));
        Register_Reference->setText(QCoreApplication::translate("MainFrame", "Register Reference", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainFrame", "3", nullptr));
        Apply_Gaussian->setText(QCoreApplication::translate("MainFrame", "Gaussian_sigma", nullptr));
        Apply_SNP->setText(QCoreApplication::translate("MainFrame", "SaltNPapper_tau", nullptr));
        label_3->setText(QCoreApplication::translate("MainFrame", "Noise", nullptr));
        Apply_BoxFilter->setText(QCoreApplication::translate("MainFrame", "Box Filter", nullptr));
        Apply_GaussianFilter->setText(QCoreApplication::translate("MainFrame", "Gaussian Filter", nullptr));
        Apply_MedianFilter->setText(QCoreApplication::translate("MainFrame", "Median Filter", nullptr));
        label_4->setText(QCoreApplication::translate("MainFrame", "Kernel Size", nullptr));
        BoxFilter_Size->setItemText(0, QCoreApplication::translate("MainFrame", "3 x 3", nullptr));
        BoxFilter_Size->setItemText(1, QCoreApplication::translate("MainFrame", "5 x 5", nullptr));

        BoxFilter_Size->setCurrentText(QCoreApplication::translate("MainFrame", "3 x 3", nullptr));
        label_5->setText(QCoreApplication::translate("MainFrame", "Sigma", nullptr));
        MedianFilter_Size->setItemText(0, QCoreApplication::translate("MainFrame", "3 x 3", nullptr));
        MedianFilter_Size->setItemText(1, QCoreApplication::translate("MainFrame", "5 x 5", nullptr));

        label_6->setText(QCoreApplication::translate("MainFrame", "Kernel Size", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainFrame", "4", nullptr));
        Apply_Canny_edge->setText(QCoreApplication::translate("MainFrame", "Apply Canny Edge", nullptr));
        label_7->setText(QCoreApplication::translate("MainFrame", "Sigma", nullptr));
        label_8->setText(QCoreApplication::translate("MainFrame", "High Threshold", nullptr));
        label_9->setText(QCoreApplication::translate("MainFrame", "Low Threshold", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("MainFrame", "5", nullptr));
        Apply_HoughCircle->setText(QCoreApplication::translate("MainFrame", "Hough Circle Detection", nullptr));
        Apply_Generalized_Hough_Transform->setText(QCoreApplication::translate("MainFrame", "Generalized Hough Transform", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QCoreApplication::translate("MainFrame", "6", nullptr));
        Apply_Optical_Flow->setText(QCoreApplication::translate("MainFrame", "Optical Flow", nullptr));
        Apply_Compare_Img->setText(QCoreApplication::translate("MainFrame", "Compare Image", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_7), QCoreApplication::translate("MainFrame", "7", nullptr));
        CameraBtn->setText(QCoreApplication::translate("MainFrame", "Camera", nullptr));
        Checker_Corner->setText(QCoreApplication::translate("MainFrame", "Checker Points", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_8), QCoreApplication::translate("MainFrame", "HikCamera", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainFrame: public Ui_MainFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFRAME_H
