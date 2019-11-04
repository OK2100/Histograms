//===========================================================================
//                           Setup historgams window
//===========================================================================
#ifndef SETUPCHANNELWINDOW_H
#define SETUPCHANNELWINDOW_H

#include <QDialog>
#include <QString>
#include <QDebug>

namespace Ui {
class SetupChannelWindow;
}

class SetupChannelWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SetupChannelWindow(QWidget *parent = nullptr);
    ~SetupChannelWindow();

    void set_chID(QString _chID);
    void set_binWidth_charge(QString);
    void set_binWidth_time(QString);
    void set_nBins_charge(QString);
    void set_nBins_time(QString);

private:
    Ui::SetupChannelWindow *ui;

signals:
    void binWidth_charge_changed(const QString& );
    void binWidth_time_changed(const QString& );
    void nBins_charge_changed(const QString& );
    void nBins_time_changed(const QString& );

private:

    QString leftLimit_charge;
    QString rightLimit_charge;
    QString nBins_charge;
    QString binWidth_charge;

    QString leftLimit_time;
    QString rightLimit_time;
    QString nBins_time;
    QString binWidth_time;

private slots:
    void apply_clicked();
};

#endif // SETUPCHANNELWINDOW_H
