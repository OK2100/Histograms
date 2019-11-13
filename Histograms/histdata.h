//===========================================================================
//                           Histogram's data class
//===========================================================================
#ifndef HISTDATA_H
#define HISTDATA_H
#include <stdint.h>
#include <QVector>
#include <QDebug>
#include <QtMath>

class HistData
{
public:
    HistData(qint16 _leftLimit,qint16 _rightLimit,quint16 _nBins);

    QVector <double> inputs;                      //    All input values

    void printInfo(bool doPrintAllInputs=0,bool doPrintNumberEventsPerBin=0, bool doPrintOnlyStat = 0);
    quint16 addEvent(qint16 _event);
    void clear();

    void setnBins(quint16 _nBins);
    quint16 getnBins();
    void setbinWidth(qreal _binWidth);
    qreal getbinWidth();
    void setHistName(QString _name);
    QString getHistName();
    quint32 getTotalEvents();
    qint16 getLeftLimit(){ return leftLimit;}
    qint16 getRightLimit(){ return rightLimit;}
    double getSampleMean();
    double getSampleVariance();
    double getSigma();
    double getFWHM();

    quint32& operator[] (const quint16 index);      //  return nEvents per bin

    QString name;

private:
    qint16 leftLimit;
    qint16 rightLimit;
    quint16 nBins;
    qreal binWidth;
    double sampleMean;
    double sampleVariance;
    quint32 nLosts=0;

    QVector<quint32> nEvents;       // Number of events per bin

    void calcbinWidth();
    void calcnBins();
};

#endif // HISTDATA_H
