#-------------------------------------------------
#
# Project created by QtCreator 2019-09-13T19:54:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Histograms
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

CONFIG += c++11
#CONFIG += console

SOURCES += \
        ../sources/DataBlockFT0.cxx \
        ../sources/DataBlockFileReaderFT0.cxx \
        ../sources/DataBlockReaderFT0.cxx \
        ../sources/qcustomplot.cpp \
        channelhistwidget.cpp \
        handlerwindow.cpp \
        histdata.cpp \
        main.cpp \
        setupchannelwindow.cpp

HEADERS += \
        ../sources/DataBlockFT0.h \
        ../sources/DataBlockFileReaderFT0.h \
        ../sources/DataBlockReaderFT0.h \
        ../sources/qcustomplot.h \
        channelhistwidget.h \
        handlerwindow.h \
        histdata.h \
        setupchannelwindow.h

FORMS += \
    setupchannelwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
