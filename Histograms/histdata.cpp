#include "histdata.h"

HistData::HistData(qint16 _leftLimit,qint16 _rightLimit,quint16 _nBins)
{
    leftLimit = _leftLimit;
    rightLimit = _rightLimit;
    nBins = _nBins;
    calcbinWidth();
    nEvents.resize(nBins);
    nEvents.fill(0);
}

quint16 HistData::addEvent(qint16 _event)
{
    inputs<<_event;
    qreal leftborder=leftLimit;
    qreal rightborder=leftborder+binWidth;

    for(quint16 i=0;i<nBins-1;i++) {
        if((leftborder<=_event)&&(_event<=rightborder)) {           // left bin<-border value; exept leftLimit;
            nEvents[i]++;
            return i;
        }
        leftborder+=binWidth;
        rightborder+=binWidth;
    }
    if((rightborder-binWidth<=_event)&&(_event<=rightLimit)) {
        nEvents.back()++;
        return nBins-1;
    }
    nLosts++;
    return nBins;
}

void HistData::printInfo(bool doPrintAllInputs,bool doPrintNumberEventsPerBin, bool doPrintOnlyStat){
    if(doPrintOnlyStat){
        qDebug()<<"@@ "<<name;
        qDebug()<<"Sample mean:"<<getSampleMean();
        qDebug()<<"Sample variance:"<<getSampleVariance();
        return;
    }
    qDebug()<<"#### INFO ####";
    qDebug()<<"Histogram name"<<name;
    qDebug()<<"From:"<<leftLimit<< "to:"<<rightLimit;
    qDebug()<<"Bin width:"<<binWidth;
    qDebug()<<"Number of bins:"<<nBins;
    qDebug()<<"Total events:" <<getTotalEvents();
    qDebug()<<"Events out of range:"<<nLosts;
    if(doPrintAllInputs) {qDebug()<<"Inputs:"<<inputs;}
    if(doPrintNumberEventsPerBin) {qDebug()<<"Number of events per bin:"<<nEvents;}
    qDebug()<<"##############";
    qDebug()<<"";
}

void HistData::clear()
{
    inputs.clear();
    nEvents.clear();
    nLosts = 0;
    nEvents.resize(nBins);
    nEvents.fill(0);
}

void HistData::setnBins(quint16 _nBins)
{
    nEvents.clear();
    nBins=_nBins;
    calcbinWidth();
    nEvents.resize(nBins);

    QVector<double> _inputs(inputs);
    inputs.clear();
    foreach(double _inp,_inputs) {
        addEvent(_inp);
    }
}

quint16 HistData::getnBins()
{
    return nBins;
}

void HistData::setbinWidth(qreal _binWidth)
{
    quint16 _nBins=qCeil((rightLimit-leftLimit)/_binWidth);            //convert _binWidth to nearest
    setnBins(_nBins);
}

qreal HistData::getbinWidth()
{
    return binWidth;
}

void HistData::setHistName(QString _name)
{
    name=_name;
}

QString HistData::getHistName()
{
    if (name.isEmpty()) {return "";}
    else { return name;}
}

quint32 HistData::getTotalEvents()
{
    quint32 sum=0;
    foreach(quint32 ev,nEvents) {
        sum+=ev;
    }
    return sum;
}

quint32& HistData::operator[] (const quint16 index)
{
    return nEvents[index];
};

double HistData::getSampleMean()
{
    double sum_elems(0);
    foreach(double _elem,inputs){
        sum_elems += _elem;
    }
    sampleMean = sum_elems/(getTotalEvents());

    return sampleMean;
}

double HistData::getSampleVariance()
{
    double sum_sqs(0);
    foreach(double _elem,inputs){
        sum_sqs += (_elem-getSampleMean())*(_elem-getSampleMean());
    }
    sampleVariance = sum_sqs/(getTotalEvents()-1);
    return sampleVariance;
}

void HistData::calcbinWidth()
{
    binWidth=(rightLimit-leftLimit)/(1.0*nBins);
}

void HistData::calcnBins()
{
    nBins=(rightLimit-leftLimit)/binWidth;
}

