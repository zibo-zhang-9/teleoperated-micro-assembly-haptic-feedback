QT       += core gui serialport network
QMAKE_CXX = g++

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Teleoperation_ChronolabTT
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
        mainwindow.cpp \
    chronolab.cpp \
    optitrack.cpp \
    teletweezer.cpp \
    log.cpp \
    mutweezer.cpp

HEADERS += \
        mainwindow.h \
    chronolab.h \
    optitrack.h \
    teletweezer.h \
    log.h \
    NATUtils.h \
    mutweezer.h

FORMS += \
        mainwindow.ui

SMARACT_SDK = C:\SmarAct\MCS\SDK

# INCLUDEPATH +=  C:/Eigen

# LIBS += MCSControl.dll -lws2_32


#SMARACT_SDK = C:/SmarAct/MCS/SDK

INCLUDEPATH +=  C:/eigen337/eigen337/Eigen

LIBS += C:/SmarAct/MCS/SDK/lib64/MCSControl.dll -lws2_32
