#include "channelhistwidget.h"

ChannelHistWidget::ChannelHistWidget(QString _chID,QWidget *parent) : QWidget(parent)
{
    setupWindow = new SetupChannelWindow;
    chID = _chID;
    box = new QVBoxLayout();
    chargeHist = new QCustomPlot();
    timeHist = new QCustomPlot();
    chargeTimeHist = new QCustomPlot();
    channelIDButton = new QPushButton("Channel "+chID+":");
//---------------------------------------------
    channelIDButton->setFixedWidth(100);
    box->addWidget(channelIDButton);
    box->setAlignment(channelIDButton,Qt::AlignHCenter);
    box->addWidget(chargeHist);
    box->addWidget(timeHist);
    box->addWidget(chargeTimeHist);
    this->setLayout(box);
    connect(channelIDButton,SIGNAL(clicked()),this,SLOT(channelIDButton_clicked()));
//---------------------------------------------
    chargeData = new HistData(-100,4095,1); chargeData->setbinWidth(1);
    chargeData->name = "Channel "+chID+" charge";
    timeData = new HistData(-2048,2047,1);  timeData->setbinWidth(1);
    timeData->name = "Channel "+chID+" time";
    chargeBars = new QCPBars(chargeHist->xAxis,chargeHist->yAxis);
    timeBars = new QCPBars(timeHist->xAxis,timeHist->yAxis);
    chargeTimeGraph = new QCPGraph(chargeTimeHist->xAxis,chargeTimeHist->yAxis);

    QPen pen;
    pen.setColor(QColor(Qt::darkBlue));

    chargeHist->xAxis->setLabel("Charge");
    chargeHist->yAxis->setLabel("N_inputs");
    chargeBars->setPen(pen);
    chargeBars->setBrush(QColor(Qt::darkBlue));

    timeHist->xAxis->setLabel("Time");
    timeHist->yAxis->setLabel("N_inputs");
    timeBars->setPen(pen);
    timeBars->setBrush(QColor(Qt::darkBlue));

    chargeTimeHist->xAxis->setLabel("Charge");
    chargeTimeHist->yAxis->setLabel("Time");

    chargeHist->xAxis->setRange(chargeData->getLeftLimit(),chargeData->getRightLimit());
    timeHist->xAxis->setRange(timeData->getLeftLimit(),timeData->getRightLimit());

    chargeTimeHist->xAxis->setRange(-100,4095);
    chargeTimeHist->yAxis->setRange(-2048,2047);

    InitKeysAndValues();

    connect(setupWindow,SIGNAL(binWidth_charge_changed(const QString&)),this,SLOT(binWidth_charge_was_changed(const QString&)));
    connect(setupWindow,SIGNAL(binWidth_time_changed(const QString&)),this,SLOT(binWidth_time_was_changed(const QString&)));
    connect(setupWindow,SIGNAL(nBins_charge_changed(const QString&)),this,SLOT(nBins_charge_was_changed(const QString&)));
    connect(setupWindow,SIGNAL(nBins_time_changed(const QString&)),this,SLOT(nBins_time_was_changed(const QString&)));
}

void ChannelHistWidget::InitKeysAndValues()
{
    chargeKey.clear();
    chargeValue.clear();
    chargeValue.resize(chargeData->getnBins());

    for (quint16 i=0;i<chargeData->getnBins();i++) {
        chargeKey << chargeData->getLeftLimit() + (i*chargeData->getbinWidth()) + (chargeData->getbinWidth()/2);
        chargeValue[i] = (*chargeData)[i];
    }

    timeKey.clear();
    timeValue.clear();
    timeValue.resize(timeData->getnBins());
    for (quint16 i=0;i<timeData->getnBins();i++) {
        timeKey << timeData->getLeftLimit() + (i*timeData->getbinWidth()) + (timeData->getbinWidth()/2);
        timeValue[i] = (*timeData)[i];
    }
}

void ChannelHistWidget::AddEvent(qint16 charge, qint16 time)
{
    quint16 ich = chargeData->addEvent(charge);
    quint16 it = timeData->addEvent(time);
    if(ich<chargeData->getnBins()) chargeValue[ich]++;
    if(it<timeData->getnBins()) timeValue[it]++;
}

void ChannelHistWidget::Update()
{
    ClearScreen();
    InitKeysAndValues();
    PlotHistograms();
}

void ChannelHistWidget::ClearScreen()
{
    chargeTimeHist->graph()->data().data()->clear();
    chargeBars->data().data()->clear();
    timeBars->data().data()->clear();
    chargeHist->yAxis->setRange(0,5);
    timeHist->yAxis->setRange(0,5);
    chargeHist->replot();
    timeHist->replot();
    chargeTimeHist->replot();
}

void ChannelHistWidget::PlotHistograms()
{


//---------------------- CHARGE ------------------------
        chargeBars->setData(chargeKey,chargeValue);
        chargeHist->yAxis->rescale();
        chargeBars->setWidth(chargeData->getbinWidth());
        chargeHist->xAxis->setRange(chargeData->getLeftLimit(),chargeData->getRightLimit());

//----------------------- TIME --------------------------
        timeBars->setData(timeKey,timeValue);
        timeHist->yAxis->rescale();
        timeBars->setWidth(timeData->getbinWidth());
        timeHist->xAxis->setRange(timeData->getLeftLimit(),timeData->getRightLimit());

//-------------------- CHARGE-TIME ----------------------
    chargeTimeHist->addGraph();
    chargeTimeHist->graph()->setLineStyle(QCPGraph::lsNone);
    chargeTimeHist->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,2));

    chargeTimeHist->graph()->setData(chargeData->inputs,timeData->inputs);

    chargeTimeHist->rescaleAxes();

//=======================================================

    if(!chargeData->inputs.isEmpty()&&!timeData->inputs.isEmpty()) {
        if(doHideZeroBars) HideZeroBars();
    }

    chargeHist->replot();
    timeHist->replot();
    chargeTimeHist->replot();
}

void ChannelHistWidget::Clear()
{
    chargeData->clear();
    timeData->clear();
    chargeTimeHist->clearGraphs();
    InitKeysAndValues();
}

void ChannelHistWidget::PrintInfo(bool onlyStat)
{
        chargeData->printInfo(0,0,onlyStat);
        timeData->printInfo(0,0,onlyStat);
}

void ChannelHistWidget::channelIDButton_clicked()
{
    qDebug()<<"Clicked";
    setupWindow->set_chID(chID);
    setupWindow->set_binWidth_charge(QString::number(chargeData->getbinWidth()));
    setupWindow->set_binWidth_time(QString::number(timeData->getbinWidth()));
    setupWindow->set_nBins_charge(QString::number(chargeData->getnBins()));
    setupWindow->set_nBins_time(QString::number(timeData->getnBins()));
    setupWindow->show();
}

void ChannelHistWidget::HideZeroBars()
{
    double leftBorder,rightBorder,binWidth;
    quint16 i,nBins;
    i=0;
    leftBorder = chargeData->getLeftLimit();
    binWidth = chargeData->getbinWidth();
    while((*chargeData)[i]==0){
        i++;
        leftBorder+=binWidth;
    }
    i=0;
    rightBorder = chargeData->getRightLimit();
    nBins = chargeData->getnBins();
    while((*chargeData)[nBins-1-i]==0){
        i++;
        rightBorder-=binWidth;
    }
    chargeHist->xAxis->setRange(leftBorder,rightBorder);

    i=0;
    leftBorder = timeData->getLeftLimit();
    binWidth = timeData->getbinWidth();
    while((*timeData)[i]==0){
        i++;
        leftBorder+=binWidth;
    }

    i=0;
    rightBorder = timeData->getRightLimit();
    nBins = timeData->getnBins();
    while((*timeData)[nBins-1-i]==0){
        i++;
        rightBorder-=binWidth;
    }
    timeHist->xAxis->setRange(leftBorder,rightBorder);
}

void ChannelHistWidget::binWidth_charge_was_changed(const QString& _binWidth)
{
    chargeData->setbinWidth(_binWidth.toDouble());
    setupWindow->set_binWidth_charge(QString::number(chargeData->getbinWidth()));
    setupWindow->set_binWidth_time(QString::number(timeData->getbinWidth()));
    setupWindow->set_nBins_charge(QString::number(chargeData->getnBins()));
    setupWindow->set_nBins_time(QString::number(timeData->getnBins()));
    Update();
    chargeData->printInfo();
}

void ChannelHistWidget::binWidth_time_was_changed(const QString& _binWidth)
{
    timeData->setbinWidth(_binWidth.toDouble());
    setupWindow->set_binWidth_charge(QString::number(chargeData->getbinWidth()));
    setupWindow->set_binWidth_time(QString::number(timeData->getbinWidth()));
    setupWindow->set_nBins_charge(QString::number(chargeData->getnBins()));
    setupWindow->set_nBins_time(QString::number(timeData->getnBins()));
    Update();
    timeData->printInfo();
}

void ChannelHistWidget::nBins_charge_was_changed(const QString& _nBins)
{
    chargeData->setnBins(_nBins.toUInt());
    setupWindow->set_binWidth_charge(QString::number(chargeData->getbinWidth()));
    setupWindow->set_binWidth_time(QString::number(timeData->getbinWidth()));
    setupWindow->set_nBins_charge(QString::number(chargeData->getnBins()));
    setupWindow->set_nBins_time(QString::number(timeData->getnBins()));
    Update();
    chargeData->printInfo(0,1);
}

void ChannelHistWidget::nBins_time_was_changed(const QString& _nBins)
{
    timeData->setnBins(_nBins.toUInt());
    setupWindow->set_binWidth_charge(QString::number(chargeData->getbinWidth()));
    setupWindow->set_binWidth_time(QString::number(timeData->getbinWidth()));
    setupWindow->set_nBins_charge(QString::number(chargeData->getnBins()));
    setupWindow->set_nBins_time(QString::number(timeData->getnBins()));
    Update();
    timeData->printInfo();
}

ChannelHistWidget::~ChannelHistWidget()
{
}
