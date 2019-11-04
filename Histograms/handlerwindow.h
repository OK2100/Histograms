//===========================================================================
//                           Main window class
//===========================================================================
#ifndef HANDLERWINDOW_H
#define HANDLERWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QDebug>
#include <QInputDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QWidget>
#include <QThread>
#include <QtGui>

#include "channelhistwidget.h"
#include "../sources/DataBlockReaderFT0.h"
#include "../sources/DataBlockFT0.h"
#include "../sources/DataBlockFileReaderFT0.h"


class HandlerWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit HandlerWindow(HandlerWindow* prevWindow = nullptr,QWidget *parent = nullptr);
    ~HandlerWindow();

public slots:                       //  Slots for connecting with other progs
    void updateScreen();
    void addEvent(quint8 chID,qint16 charge,qint16 time);

signals:
    void showNewWindow(quint8 firstChannelID);

private:
    QWidget* centralWidget = new QWidget;
    QHBoxLayout* grid = new QHBoxLayout;
    ChannelHistWidget* channel[12];
    char* filePath;                 //  Path and name. It breaks somewhere in DataBlock lib
    QString filePathSave;           //  Save of 'filePath'
    QString fileType;

    quint8 nAddedChannels = 0;
    quint8 nextChannelID = 1;

    HandlerWindow* secondWindow;

    void SetUp();
    void PlotHistograms();
    void ReadBinaryFile();
    void ReadTxtFile();

//------------------------- Apearance --------------------------

    QMenu* fileBar;                 //  'File' field in menu
    QAction* openSourceFileAction;

    QMenu* editBar;                 //  'Edit' field in menu
    QAction* defaultSettingAction;

    QMenu* dataBar;
    QAction* showStatWindowAction;

    QMenu* optionsBar;              //  'Options' field in menu
    QAction* addChannelAction;
    QAction* removeChannelAction;
    QAction* removeAllChannelAction;
    QAction* updateAction;
    QAction* readFileAction;
    QAction* resetAction;
    QAction* resetAllAction;
    QAction* hideZeroBarsAction;

    QLabel lbl;                     //  Opening label
//----------------------------------------------------

private slots:
    void startNewWindow(quint8 firstChannelID);
    bool openSourceFile();
    void readFile();
    void addChannel();
    void removeChannel();
    void removeAllChannel();
    void reset();
    void resetAll();
    void hideZeroBars();
    void showStatWindow();
};

#endif // HANDLERWINDOW_H
