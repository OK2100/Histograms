#include "handlerwindow.h"

QString HexStrtoBinStr(QString inp){
    QString result;
    bool ok;
    for(int i=0;i<inp.size();i++){
        QString tmp = inp.mid(i,1);
        result.append(QString("%1").arg(tmp.toLong(&ok,16),4,2,QLatin1Char('0')));
    }
    return result;
};

HandlerWindow::HandlerWindow(HandlerWindow* prevWindow,QWidget *parent) :
    QMainWindow(parent)
{
    if(prevWindow!=nullptr) {
    connect(prevWindow,SIGNAL(showNewWindow(quint8)),this,SLOT(startNewWindow(quint8)));
    }
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

    chooseADCAction=new QAction("&Choose ADC number",this);
    chooseADCAction->setShortcut(tr("Ctrl+d"));
    optionsBar->addAction(chooseADCAction);
    connect(chooseADCAction, SIGNAL(triggered()),this,SLOT(chooseADC()));


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
    DataBlockFileReaderFT0 *rawReader = new DataBlockFileReaderFT0(qPrintable(filePath),"",dataBlock);


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
    QString gbtword,firstch_gbt,secondch_gbt;
    quint16 nWords,channelID;
    DataConversion convertor;
    quint8 tADCNum;
    bool ok;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream readStream(&file);
        while(!file.atEnd()) {
//        for (quint8 j=0;j<100;j++){
            gbtword = readStream.readLine();
//            qDebug() << gbtword;

            if(gbtword.left(10) == "E000000000"){continue;}
            if(gbtword.left(3) == "DAF"){continue;}

            if(gbtword.left(1)=="F"){
                nWords = gbtword.mid(1,1).toUShort(&ok,16);
                for(quint16 i=0;i<nWords;i++) {
                    gbtword = readStream.readLine();
                    qDebug() << gbtword;

                    // divide GBT word to 2 channels
                    firstch_gbt = gbtword.left(10);
                    secondch_gbt = gbtword.right(10);

                    // convert hex to binary QString
                    firstch_gbt = HexStrtoBinStr(firstch_gbt);
                    secondch_gbt = HexStrtoBinStr(secondch_gbt);

//                    qDebug() << firstch_gbt;

                    // get time, charge and channelID for first channel in gbt word
                    convertor.dataBlocks.time = firstch_gbt.right(12).toULongLong(&ok,2);
                    convertor.dataBlocks.charge = firstch_gbt.mid(15,13).toULongLong(&ok,2);
                    channelID = firstch_gbt.left(4).toUShort(&ok,2);
                    tADCNum = firstch_gbt.mid(14,1).toUShort();

//                    qDebug() << tADCNum;

                    if(ADCNumber ==2){
                        if((channelID >= 1) && (channelID<=12)) {
                            if(channel[channelID-1]!=nullptr) channel[channelID-1]->
                                                AddEvent(convertor.dataReal.charge,convertor.dataReal.time);
                        }
                    }
                    else{
                        if((channelID >= 1) && (channelID<=12) && (tADCNum == ADCNumber)) {
                            if(channel[channelID-1]!=nullptr) channel[channelID-1]->
                                                AddEvent(convertor.dataReal.charge,convertor.dataReal.time);
                        }
                    }

//                    qDebug() << secondch_gbt;

                    // get time, charge and channelID for second channel in gbt word
                    convertor.dataBlocks.time = secondch_gbt.right(12).toULongLong(&ok,2);
                    convertor.dataBlocks.charge = secondch_gbt.mid(15,13).toULongLong(&ok,2);
                    channelID = secondch_gbt.left(4).toUShort(&ok,2);
                    tADCNum = secondch_gbt.mid(14,1).toUShort();

//                    qDebug() << tADCNum;

                    if(ADCNumber ==2){
                        if(channelID >= 1 && channelID<=12) {
                            if(channel[channelID-1]!=nullptr) channel[channelID-1]->
                                                AddEvent(convertor.dataReal.charge,convertor.dataReal.time);
                        }
                    }
                    else{
                        if((channelID >= 1) && (channelID<=12) && (tADCNum == ADCNumber)) {
                            if(channel[channelID-1]!=nullptr) channel[channelID-1]->
                                                AddEvent(convertor.dataReal.charge,convertor.dataReal.time);
                        }
                    }

                }
            }
        } // end of while
    } // end of if
    else {
        qDebug() << "File is not opened";
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
                                                    "GBT files (*.GBT *.gbt);;Binary files (*.bin);;All files (*.*)",
                                                    &fileType);
    filePath = enteredFilePath;
//    filePathSave = enteredFilePath;
//    filePath = filePathSave.toUtf8().data();
//    if(filePathSave.isEmpty()) { return 0; }
    if(filePath.isEmpty()) { return 0; }
    else { return 1; }
}

void HandlerWindow::readFile()
{
//    if(filePathSave.isEmpty()) {
    if(filePath.isEmpty()) {
        if(!openSourceFile()) return;       // File is not choosen
    }

    if(fileType == "Binary files (*.bin)"){ ReadBinaryFile();}
    if(fileType == "GBT files (*.GBT *.gbt)"){ ReadTxtFile();}
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
    if(!bOk){
//        qDebug() << "Cancel";
        return;
    }

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

    if(!bOk){
//        qDebug() << "Cancel";
        return;
    }

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

    if(!bOk){
//        qDebug() << "Cancel";
        return;
    }

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

void HandlerWindow::chooseADC()
{

    bool bOk;
    QStringList listID = {"0","1","2"};
    QString adcNum=QInputDialog::getItem(this,"ADC","Choose ADC number:",listID,0,0,&bOk);

    if(!bOk){
//        qDebug() << "Cancel";
        return;
    }

    quint8 adc = adcNum.toUShort();
//    qDebug() << adc;
    ADCNumber = adc;

}

void HandlerWindow::showStatWindow()
{
    QString text;
    QMessageBox msgBox;
    msgBox.setWindowTitle("Stat");
    msgBox.show();
    for(quint16 i=0;i<12;i++) {
        if(channel[i]!=nullptr){
            text.append(channel[i]->GetStatInfo());
//            qDebug()<<"";
//            channel[i]->PrintInfo(1);
        }
    }
    msgBox.setText(text);
    msgBox.exec();

}

HandlerWindow::~HandlerWindow()
{
}
