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
        mainframe.cpp \
    imageform.cpp \
    kfc.cpp \
    rv0171.cpp

HEADERS  += mainframe.h \
    imageform.h \
    kfc.h \
    rv0171.h

FORMS    += mainframe.ui \
    imageform.ui

RESOURCES += \
    images/mainframe.qrc

