#-------------------------------------------------
#
# Project created by QtCreator 2014-09-29T17:12:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = robotVisionApp
TEMPLATE = app

SOURCES += main.cpp\
    HikCamera.cpp \
        mainframe.cpp \
    imageform.cpp \
    kfc.cpp \
    rv0171.cpp

HEADERS  += mainframe.h \
    HikCamera.h \
    imageform.h \
    kfc.h \
    rv0171.h

FORMS    += mainframe.ui \
    imageform.ui

RESOURCES += \
    images/mainframe.qrc

#++++
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += C:\openCV4.7.0\opencv\build_qt\install\include\

LIBS += C:\openCV4.7.0\opencv\build_qt\bin\libopencv_core470d.dll
LIBS += C:\openCV4.7.0\opencv\build_qt\bin\libopencv_highgui470d.dll
LIBS += C:\openCV4.7.0\opencv\build_qt\bin\libopencv_imgcodecs470d.dll
LIBS += C:\openCV4.7.0\opencv\build_qt\bin\libopencv_imgproc470d.dll
LIBS += C:\openCV4.7.0\opencv\build_qt\bin\libopencv_features2d470d.dll
LIBS += C:\openCV4.7.0\opencv\build_qt\bin\libopencv_calib3d470d.dll

INCLUDEPATH += "C:\Program Files (x86)\MVS\Development\Includes"

LIBS += -L$$PWD/'../../../../../Program Files (x86)/MVS/Development/Libraries/win64/' -lMvCameraControl
INCLUDEPATH += $$PWD/'../../../../../Program Files (x86)/MVS/Development/Libraries/win64'
DEPENDPATH += $$PWD/'../../../../../Program Files (x86)/MVS/Development/Libraries/win64'


