#include "setupchannelwindow.h"
#include "ui_setupchannelwindow.h"

SetupChannelWindow::SetupChannelWindow(QWidget *parent) : QDialog(parent), ui(new Ui::SetupChannelWindow)
{
    ui->setupUi(this);

    ui->le_chargeRange_left->setText("-100");
    ui->le_chargeRange_right->setText("4095");

    ui->le_timeRange_left->setText("-2048");
    ui->le_timeRange_right->setText("2047");

    connect( ui->OkButton, SIGNAL( clicked() ), SLOT( accept() ) );
    connect( ui->ApplyButton, SIGNAL( clicked() ), SLOT( apply_clicked() ) );
    connect( ui->CancelButton, SIGNAL( clicked() ), SLOT( reject() ) );

}

void SetupChannelWindow::set_chID(QString _chID){ ui->lbl_channelID->setText("Channel: " + _chID); }

void SetupChannelWindow::set_binWidth_charge(QString _binWidth)
{
    ui->le_binWidth_charge->setText(_binWidth);
    binWidth_charge = _binWidth;
}

void SetupChannelWindow::set_binWidth_time(QString _binWidth)
{
    ui->le_binWidth_time->setText(_binWidth);
    binWidth_time = _binWidth;
}

void SetupChannelWindow::set_nBins_charge(QString _nBins)
{
    ui->le_nBins_charge->setText(_nBins);
    nBins_charge = _nBins;
}

void SetupChannelWindow::set_nBins_time(QString _nBins)
{
    ui->le_nBins_time->setText(_nBins);
    nBins_time = _nBins;
}

void SetupChannelWindow::apply_clicked()
{
    if(ui->rb_binW_charge->isChecked()){
        if(ui->le_binWidth_charge->text()!=binWidth_charge){
            emit binWidth_charge_changed(ui->le_binWidth_charge->text());
        }
        set_nBins_charge(nBins_charge);
    }
    else if(ui->rb_nBins_charge->isChecked()){
            if(ui->le_nBins_charge->text()!=nBins_charge){
                emit nBins_charge_changed(ui->le_nBins_charge->text());
            }
            set_binWidth_charge(binWidth_charge);
    }

    if(ui->rb_binW_time->isChecked()){
        if(ui->le_binWidth_time->text()!=binWidth_time){
            emit binWidth_time_changed(ui->le_binWidth_time->text());
        }
        set_nBins_time(nBins_time);
    }
    else if(ui->rb_nBins_time->isChecked()){
            if(ui->le_nBins_time->text()!=nBins_time){
                emit nBins_time_changed(ui->le_nBins_time->text());
            }
            set_binWidth_time(binWidth_time);
    }
}

SetupChannelWindow::~SetupChannelWindow()
{
    delete ui;
}


