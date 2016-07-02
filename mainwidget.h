#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QString>
#include <QDesktopWidget>
#include <QRect>
#include <QTimer>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QAction>
#include <QIcon>
#include <QTextCodec>
#include <QEvent>
#include <QMouseEvent>

#include "setupdialog.h"

namespace Ui {
class MainWidget;
}
class SetupDialog;
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    static QSettings *GetGlobalSetting();

private:
    Ui::MainWidget *ui;
    QNetworkAccessManager *manager;
    QDesktopWidget *desktop;
    QString sina_url;
    QString equity;
    QTimer *timer_to_update;
    QSystemTrayIcon *tray;
    QMenu *traymenu;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *setupAction;
    QAction *quitAction;
    QIcon *trayicon;

    //显示格式
    /*
    股票名字 今日开盘价(昨日收盘价) 买一价格(卖一价格) 买一股数(卖一股数)
    */
    //
    QString e_name;//股票名字
    float open_price;//今日开盘价
    float yesterday_close_price;//昨日收盘价
    float buy_one;//买一价格
    float sale_one;//卖一价格
    int buy_one_number;//买一股数
    int sale_one_number;//卖一股数
    int last_requst;
    int opacity;
    QPoint press_pos;
    //follow variable are use global
    static QSettings *settings;
    //until here
protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
public slots:
    void QNetNetworkAccessManagerreplyFinished(QNetworkReply*reply);
    void TimerToUpdateEquityInformation();
    void CreateSystrayIcon();
    void CreateSystrayActions();
    void MainWidgetQuit();
    void MainWidgetSetup();
    void MainWidgetSetRefreshTime(int time);
    void MainWidgetGetEquityData(int number);
    void SetOpacity(int opacity);
};

#endif // MAINWIDGET_H
