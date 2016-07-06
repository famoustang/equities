#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QFile>
#include "mainwidget.h"
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QDebug>
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
    QSettings *settings;
    int e_num;
    QStringList name_list,code_list;
signals:
    void TimeToRefresh(int time_refresh);
    void SliderToRefresh(int value);
    void DefaultPosition();
public slots:
    void ComboBoxRefreshSlot(int index);
    void HorizontalSliderSlot(int pos);
    void PushButtonAddRowSlot();
    void PushButtonDeleteRowSlot();
    void TableWidgetItemClickedSlot(QTableWidgetItem *item);
    void TableWidgetItemSelectionChangedSlot();
    void TableWidgetItemChangedSlot(QTableWidgetItem *item);
    void PushButtonDefaultPositionClickedSlot();
};

#endif // SETUPDIALOG_H
