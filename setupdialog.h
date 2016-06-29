#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QFile>
#include "mainwidget.h"
#include <QMessageBox>
namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = 0);
    ~SetupDialog();

private:
    Ui::SetupDialog *ui;
signals:
    void TimeToRefresh(int time_refresh);
    void SliderToRefresh(int value);
public slots:
    void PushButtonAddSlot();
    void ComboBoxRefreshSlot(int index);
    void HorizontalSliderSlot(int pos);
};

#endif // SETUPDIALOG_H
