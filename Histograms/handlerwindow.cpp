#include "handlerwindow.h"



HandlerWindow::HandlerWindow(HandlerWindow* prevWindow,QWidget *parent) :
    QMainWindow(parent)
{
    connect(prevWindow,SIGNAL(showNewWindow(quint8)),this,SLOT(startNewWindow(quint8)));
    SetUp();
}

//      Functions

void HandlerWindow::SetUp()
{
    setCentralWidget(centralWidget);
    centralWidget->setLayout(grid);
    for(quint16 i=0;i<12;i++) {
        channel[i]=nullptr;
    }
    this->setMinimumSize(300,680);
    this->resize(300,680);

//--------------------------- Appearance ---------------------------------

    fileBar=menuBar()->addMenu("&File");                    //  #File field

    openSourceFileAction=new QAction("&Open source file...",this);
    openSourceFileAction->setShortcut(tr("Ctrl+o"));
    fileBar->addAction(openSourceFileAction);
    connect(openSourceFileAction, SIGNAL(triggered()),this,SLOT(openSourceFile()));

    dataBar=menuBar()->addMenu("&Data");

    showStatWindowAction=new QAction("&Show statistics (to console)",this);
    showStatWindowAction->setShortcut(tr("Ctrl+t"));
    dataBar->addAction(showStatWindowAction);
    connect(showStatWindowAction, SIGNAL(triggered()),this,SLOT(showStatWindow()));

    optionsBar=menuBar()->addMenu("&Options");              //  #Options field

    addChannelAction=new QAction("&Add channel...",this);
    addChannelAction->setShortcut(tr("Ctrl+a"));
    optionsBar->addAction(addChannelAction);
    connect(addChannelAction, SIGNAL(triggered()),this,SLOT(addChannel()));

    removeChannelAction=new QAction("&Remove channel...",this);
    removeChannelAction->setShortcut(tr("Ctrl+r"));
    optionsBar->addAction(removeChannelAction);
    connect(removeChannelAction, SIGNAL(triggered()),this,SLOT(removeChannel()));

    removeAllChannelAction=new QAction("&Remove all channel",this);
    removeAllChannelAction->setShortcut(tr("Ctrl+Shift+r"));
    optionsBar->addAction(removeAllChannelAction);
    connect(removeAllChannelAction, SIGNAL(triggered()),this,SLOT(removeAllChannel()));

    readFileAction=new QAction("&Read file",this);
    readFileAction->setShortcut(tr("Ctrl+f"));
    optionsBar->addAction(readFileAction);
    connect(readFileAction, SIGNAL(triggered()),this,SLOT(readFile()));

    resetAction=new QAction("&Reset channel...",this);
    resetAction->setShortcut(tr("Ctrl+e"));
    optionsBar->addAction(resetAction);
    connect(resetAction, SIGNAL(triggered()),this,SLOT(reset()));

    resetAllAction=new QAction("&Reset all channels",this);
    resetAllAction->setShortcut(tr("Ctrl+Shift+e"));
    optionsBar->addAction(resetAllAction);
    connect(resetAllAction, SIGNAL(triggered()),this,SLOT(resetAll()));

    hideZeroBarsAction=new QAction("&Hide empty bars",this);
    hideZeroBarsAction->setShortcut(tr("Ctrl+h"));
    optionsBar->addAction(hideZeroBarsAction);
    connect(hideZeroBarsAction, SIGNAL(triggered()),this,SLOT(hideZeroBars()));

    updateAction=new QAction("&Update screen",this);
    updateAction->setShortcut(tr("Ctrl+u"));
    optionsBar->addAction(updateAction);
    connect(updateAction, SIGNAL(triggered()),this,SLOT(updateScreen()));


    lbl.setText("\n"
                "\n"
                "\n"
                "Viewer\n"
                "See some shortcuts:\n"
                "\n"
                "Ctrl+A - add channel...   \n"
                "Ctrl+R - remove channel...\n"
                "Ctrl+F - plot from file   \n"
                "Ctrl+E - reset channel... \n"
                "Ctrl+H - on/off hiding empty bars\n"
                "Ctrl+U - update screen    \n"
                "\n"
                "For more details see 'Options'");

    lbl.setAlignment(Qt::AlignTop);
    QFont f("Times");
    lbl.setFont(f);
    grid->addWidget(&lbl);

//--------------------------------------------------------------------------
}

void HandlerWindow::PlotHistograms()
{
    for (quint16 i=0;i<12;i++) {
        if(channel[i]!=nullptr){
            channel[i]->PlotHistograms();
//            channel[i]->PrintInfo();
        }
    }
}

void HandlerWindow::ReadBinaryFile()
{
    for(quint16 i=0;i<12;i++) {
        if(channel[i]!=nullptr){
            channel[i]->Clear();
        }
    }

    DataConversion convertor;
    EventHeader* pHeader;
    EventData* pData;

    DataBlockFT0 *dataBlock = new DataBlockFT0;
    DataBlockFileReaderFT0 *rawReader = new DataBlockFileReaderFT0(filePath,"",dataBlock);


    for(quint16 i=0;i<12;i++) {
        if(channel[i]!=nullptr) {
            channel[i]->Clear();
        }
    }
    rawReader->InitFile();

    while(rawReader->ReadNextEvent()) {

        pData  = dataBlock->GetEventDataPtr();
        pHeader = dataBlock->GetEventHeaderPtr();

        for(int iCh=0;iCh<2*(pHeader->Nwords);iCh++)
        {
            for (quint16 i=0;i<12;i++) {
                if(channel[i]!=nullptr){
                    if(pData[iCh].channelID == i) {
                        convertor.dataBlocks.charge=pData[iCh].charge;
                        convertor.dataBlocks.time=pData[iCh].time;
                        channel[i]->AddEvent(convertor.dataReal.charge,convertor.dataReal.time);
                    }
                }
            }
        }
    }

    delete rawReader;
    delete dataBlock;
    PlotHistograms();
}

void HandlerWindow::ReadTxtFile()
{
    for(quint16 i=0;i<12;i++) {
        if(channel[i]!=nullptr){
            channel[i]->Clear();
        }
    }

    QFile file(filePath);
    QString str,strFirstChannel,strSecondChannel;
    quint16 nWords,channelID;
    DataConversion convertor;
    bool ok;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream readStream(&file);

        while(!file.atEnd()) {
            readStream >> str;
            if(strFirstChannel == "E000000000"){continue;}
            if(str.left(1)=="F"){
                nWords = str.mid(1,1).toULong(&ok,16);

                for(quint16 i=0;i<nWords;i++) {
                    readStream >> str;

                    // divide GBT word to 2 channels
                    strFirstChannel = str.left(10);
                    strSecondChannel = str.right(10);

                    // convert hex to binary QString
                    strFirstChannel = QString("%1").arg(strFirstChannel.toLongLong(&ok,16),40,2,QLatin1Char('0'));
                    strSecondChannel = QString("%1").arg(strSecondChannel.toLongLong(&ok,16),40,2,QLatin1Char('0'));


                    // get time, charge and channelID for first channel in gbt word
                    convertor.dataBlocks.time = strFirstChannel.right(12).toULongLong(&ok,2);
                    convertor.dataBlocks.charge = strFirstChannel.mid(15,13).toULongLong(&ok,2);
                    channelID = strFirstChannel.left(4).toULong(&ok,2);

                    if((channelID >= 1) && (channelID<=12)) {
                        if(channel[channelID-1]!=nullptr) channel[channelID-1]->
                                            AddEvent(convertor.dataReal.charge,convertor.dataReal.time);
                    }

                    // get time, charge and channelID for second channel in gbt word
                    convertor.dataBlocks.time = strSecondChannel.right(12).toULongLong(&ok,2);
                    convertor.dataBlocks.charge = strSecondChannel.mid(15,13).toULongLong(&ok,2);
                    channelID = strSecondChannel.left(4).toULong(&ok,2);

                    if(channelID >= 1 && channelID<=12) {
                        if(channel[channelID-1]!=nullptr) channel[channelID-1]->
                                            AddEvent(convertor.dataReal.charge,convertor.dataReal.time);
                    }
                }
            }
            else {
            }
        }
    }
    file.close();
    PlotHistograms();
}

//      Slots

void HandlerWindow::addEvent(quint8 chID,qint16 charge,qint16 time)
{
    if((chID<=12)&&(chID>=1)){
        if(channel[chID-1]!=nullptr){
            channel[chID-1]->AddEvent(charge,time);
        }
    }
}

void HandlerWindow::updateScreen()
{
    for(quint16 i=0;i<12;i++) {
        if(channel[i]!=nullptr){
            channel[i]->Update();
        }
    }
}

void HandlerWindow::startNewWindow(quint8 firstChannelID)
{
    this->show();
    nextChannelID = firstChannelID;
    addChannel();
}

bool HandlerWindow::openSourceFile()
{
    QString enteredFilePath = QFileDialog::getOpenFileName(this,
                                                    QString::fromUtf8("Open file"),
                                                    QDir::currentPath(),
                                                    "GBT files (*.GBT);;Binary files (*.bin);;All files (*.*)",
                                                    &fileType);
    filePathSave = enteredFilePath;
    filePath = filePathSave.toUtf8().data();
    if(filePathSave.isEmpty()) { return 0; }
    else { return 1; }
}

void HandlerWindow::readFile()
{
    if(filePathSave.isEmpty()) {
        if(!openSourceFile()) return;       // File is not choosen
    }
    else {
        filePath = filePathSave.toUtf8().data();
    }

    if(fileType == "Binary files (*.bin)"){ ReadBinaryFile();}
    if(fileType == "GBT files (*.GBT)"){ ReadTxtFile();}
}

void HandlerWindow::addChannel()
{
    if(nAddedChannels == 4){
//        qDebug() <<"start new window" ;
        emit showNewWindow(nextChannelID);
        return;
    }

    bool bOk;
    QStringList listID = {"1","2","3","4","5","6","7","8","9","10","11","12"};
    QString chID=QInputDialog::getItem(this,tr("Input"),"Add channel:",listID,nextChannelID-1,0,&bOk);
    if(!bOk){ qDebug() << "Cancel";return;}

    if(channel[chID.toInt()-1]==nullptr) {
        channel[chID.toInt()-1] = new ChannelHistWidget(chID);
        lbl.hide();
        grid->addWidget(channel[chID.toInt()-1]);
        this->resize(300+300*nAddedChannels,this->height());
        nAddedChannels++;
        nextChannelID++;
    }
}

void HandlerWindow::removeChannel()
{
    bool bOk;
    QStringList listID = {"1","2","3","4","5","6","7","8","9","10","11","12"};
    QString chID=QInputDialog::getItem(this,"Delete","Remove channel:",listID,nextChannelID-2,0,&bOk);

    if(!bOk){ qDebug() << "Cancel";return;}

    if(channel[chID.toInt()-1] != nullptr){
        delete channel[chID.toInt()-1];
        channel[chID.toInt()-1] = nullptr;
        this->resize(this->width()-300,this->height());
        nAddedChannels--;
        nextChannelID--;
    }
    if(nAddedChannels==0){ lbl.show(); }
}

void HandlerWindow::removeAllChannel()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QString::fromUtf8("Message"),
                      QString::fromUtf8("Delete all channels?"),
                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        for(quint16 i=0;i<12;i++){
            if(channel[i]!=nullptr) {
                delete channel[i];
                channel[i] = nullptr;
                nextChannelID--;
            }
        }

        nAddedChannels=0;
        this->resize(0,0);
        lbl.show();
    }
}

void HandlerWindow::reset()
{
    bool bOk;
    QStringList listID = {"1","2","3","4","5","6","7","8","9","10","11","12"};
    QString chID=QInputDialog::getItem(this,"Reset","Reset channel:",listID,0,0,&bOk);

    if(!bOk){ qDebug() << "Cancel";return;}

    if(channel[chID.toInt()-1]!=nullptr){
        channel[chID.toInt()-1]->Clear();
        channel[chID.toInt()-1]->PlotHistograms();
    }
}

void HandlerWindow::resetAll()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QString::fromUtf8("Message"),
                      QString::fromUtf8("Reset all channels?"),
                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        for(quint16 i=0;i<12;i++) {
            if(channel[i]!=nullptr){
                channel[i]->Clear();
            }
        }
        PlotHistograms();
    }
}

void HandlerWindow::hideZeroBars()
{
    for(quint16 i=0;i<12;i++) {
        if(channel[i]!=nullptr){
            channel[i]->doHideZeroBars = !channel[i]->doHideZeroBars;
            channel[i]->Update();
        }
    }

}

void HandlerWindow::showStatWindow()
{
    for(quint16 i=0;i<12;i++) {
        if(channel[i]!=nullptr){
            qDebug()<<"";
            channel[i]->PrintInfo(1);
        }
    }
}

HandlerWindow::~HandlerWindow()
{
}
