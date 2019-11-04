#include <QCoreApplication>
#include "../../sources/DataBlockFileReaderFT0.cxx"
#include "../../sources/DataBlockFT0.cxx"
#include "../../sources/DataBlockReaderFT0.cxx"
#include <QThread>
#include <QDebug>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DataBlockFT0 *dataBlock = new DataBlockFT0();
    DataBlockFileReaderFT0 *rawReader = new DataBlockFileReaderFT0("","Data.bin",dataBlock);
//    for(int iEvent=0;iEvent<10000;iEvent++) {

    qint32 nEvents = 0;

    rawReader->InitFile();
    while(nEvents!=10000){
        dataBlock->GenerateRandomEvent();
        dataBlock->Print();
        rawReader->WriteNextEvent();
        nEvents++;
//        QThread::sleep(1);
    }

    delete rawReader;
    delete dataBlock;
    return a.exec();
}
