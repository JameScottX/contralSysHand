#-------------------------------------------------
#
# Project created by QtCreator 2017-10-26T14:38:17
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += bluetooth
QT       += network

android: QT += androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = contralSysHand
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        contralsyshand.cpp \
    contralmodel.cpp \
    btmsghandle.cpp \
    serialpart.cpp \
    wifiui.cpp \
    mwificore.cpp \
#    ../../../../../../Qt/Qt5.9.2/QCustomplot/qcustomplot/qcustomplot.cpp \
    plotshow.cpp

HEADERS += \
        contralsyshand.h \
    contralmodel.h \
    btmsghandle.h \
    serialpart.h \
    msgcode.h \
    wifiui.h \
    mwificore.h \
#    ../../../../../../Qt/Qt5.9.2/QCustomplot/qcustomplot/qcustomplot.h \
    plotshow.h

FORMS += \
        contralsyshand.ui \
    contralmodel.ui \
    wifiui.ui \
    plotshow.ui

CONFIG += mobility
MOBILITY = 

