//===========================================================================
//               Widget: three channel's historrams
//===========================================================================
#ifndef CHANNELHISTWIDGET_H
#define CHANNELHISTWIDGET_H

#include "qcustomplot.h"
#include "histdata.h"
#include "setupchannelwindow.h"

#include <QFormLayout>
#include <QWidget>
#include <QHBoxLayout>
#include <QString>
#include <QLabel>
#include <QMouseEvent>

class ChannelHistWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelHistWidget(QString _chID,QWidget *parent = nullptr);
    ~ChannelHistWidget();

    bool doHideZeroBars=0;          // Flag
    void PlotHistograms();
    void Clear();
    void ClearScreen();
    void Update();
    void AddEvent(qint16 charge, qint16 time);
    void PrintInfo(bool onlyStat = false);

private slots:
    void channelIDButton_clicked();
    void binWidth_charge_was_changed(const QString&);
    void binWidth_time_was_changed(const QString&);
    void nBins_charge_was_changed(const QString&);
    void nBins_time_was_changed(const QString&);

private:
    HistData* chargeData;
    HistData* timeData;

    SetupChannelWindow* setupWindow;
    QString chID;
    QVBoxLayout* box;
    QPushButton* channelIDButton;

    QCustomPlot* chargeHist;            // First hist
    QCPBars* chargeBars;
    QVector<double> chargeKey;
    QVector<double> chargeValue;

    QCustomPlot* timeHist;              // Second hist
    QCPBars* timeBars;
    QVector<double> timeKey;
    QVector<double> timeValue;

    QCustomPlot* chargeTimeHist;        // Graph
    QCPGraph* chargeTimeGraph;

//    QCPColorMapData* dataChargeTime;

    void HideZeroBars();
    void InitKeysAndValues();
};

#endif // CHANNELHISTWIDGET_H
