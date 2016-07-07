#include "mainwidget.h"
#include "ui_mainwidget.h"

QSettings * MainWidget::settings = NULL;

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    desktop = new QDesktopWidget;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    ui->labelEquity->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    last_requst = 1;

    this->CreateSystrayActions();
    this->CreateSystrayIcon();

    if(tray){
        tray->show();
    }

    QRect desktop_rec = desktop->availableGeometry();
    settings = new QSettings("equities.dll",QSettings::IniFormat,this);
    if(settings == NULL){
        QMessageBox::warning(NULL,tr("ERROR!"),tr("Fatal error! please restart!"));
    }
    if(QFile::exists("equities.dll") == false){

        if(settings){
            //settings->setValue("font/current","Times New Roman,18,-1,5,50,0,0,0,0,0");
            settings->setValue("refresh_time/time",30);
            settings->setValue("opacity/op",2);
            settings->setValue("position/x",desktop_rec.width() - this->width());
            settings->setValue("position/y",desktop_rec.height() - this->height());
            settings->setValue("equity_number/number",1);
            settings->setValue("equities/equity_1","sh600020");
        }
    }

    this->move(settings->value("position/x",desktop_rec.width() - this->width()).toInt(),
               settings->value("position/y",desktop_rec.height() - this->height()).toInt());


    opacity = settings->value("opacity/op",2).toInt();
    this->setWindowOpacity(opacity / 10.0);

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(QNetNetworkAccessManagerreplyFinished(QNetworkReply*)));

    TimerToUpdateEquityInformation();
    timer_to_update = new QTimer(this);
    timer_to_update->setInterval(settings->value("refresh_time/time",30).toInt() * 1000);
    connect(timer_to_update,SIGNAL(timeout()),this,SLOT(TimerToUpdateEquityInformation()));
    timer_to_update->start();

}

MainWidget::~MainWidget()
{
    delete desktop;
    delete ui;
}

void MainWidget::QNetNetworkAccessManagerreplyFinished(QNetworkReply *reply)
{
    QByteArray result =  reply->readAll();
    //qDebug()<<result;
    //QByteArray result("var hq_str_sh600020=\"\xD6\xD0\xD4\xAD\xB8\xDF\xCB\xD9,4.410,4.430,4.400,4.420,4.360,4.390,4.400,8928094,39192987.000,52500,4.390,247956,4.380,273225,4.370,684900,4.360,386200,4.350,100753,4.400,193193,4.410,238200,4.420,191336,4.430,243200,4.440,2016-06-23,15:00:00,00\";\n");
    QTextCodec *tc = QTextCodec::codecForName("GB18030");
    QString re = tc->toUnicode(result);
    //qDebug()<<re;
    int i;
    for(i= 0 ; i < re.length();i++){
        if(re.at(i) == '\"'){
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            e_name = re.mid(0,i);
            //qDebug()<<"equity name "<<e_name;//股票名字
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            open_price = re.mid(0,i).toFloat();
            //qDebug()<<"open price "<<open_price;//今日开盘价
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            yesterday_close_price = re.mid(0,i).toFloat();
            //qDebug()<<"yesterday close price "<<yesterday_close_price;//昨日收盘价
            re.remove(0,i + 1);
            break;
        }
    }
    float spot_price;
    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            spot_price = re.mid(0,i).toFloat();
            //qDebug()<<"spot price "<<spot_price;//当前价格
            re.remove(0,i + 1);
            break;
        }
    }
    float top_price;
    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            top_price = re.mid(0,i).toFloat();
            //qDebug()<<"top price "<<top_price;//今日最高价
            re.remove(0,i + 1);
            break;
        }
    }
    float bottom_price;
    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            bottom_price = re.mid(0,i).toFloat();
            //qDebug()<<"bottom price "<<bottom_price;//今日最低价
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            buy_one = re.mid(0,i).toFloat();
            //qDebug()<<"buy one "<<buy_one;//竞买价，即“买一”报价
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            sale_one = re.mid(0,i).toFloat();
            //qDebug()<<"sale one "<<sale_one;//竞卖价，即“卖一”报价
            re.remove(0,i + 1);
            break;
        }
    }
    int deal_number;
    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            deal_number = re.mid(0,i).toInt();
            //qDebug()<<"deal number "<<deal_number;//成交的股票数，由于股票交易以一百股为基本单位，所以在使用时，通常把该值除以一百
            re.remove(0,i + 1);
            break;
        }
    }
    int turnover;
    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            turnover = re.mid(0,i).toInt();
            //qDebug()<<"total "<<turnover;//成交金额，单位为“元”，为了一目了然，通常以“万元”为成交金额的单位，所以通常把该值除以一万
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            buy_one_number = re.mid(0,i).toInt();
            //qDebug()<<"buy one number "<<buy_one_number;//，“买一”申请股数
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"buy1 "<<re.mid(0,i);//，“买一”报价
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"buy2 "<<re.mid(0,i);//，“买二申请股数”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"buy2 "<<re.mid(0,i);//，“买二”报价
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"buy3 "<<re.mid(0,i);//，“买三申请股数”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"buy3 "<<re.mid(0,i);//，“买三报价”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"buy4 "<<re.mid(0,i);//，“买四申请股数”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"buy4 "<<re.mid(0,i);//，“买四报价”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"buy5 "<<re.mid(0,i);//，“买五申请股数”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"buy5 "<<re.mid(0,i);//，“买五报价”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            sale_one_number = re.mid(0,i).toInt();
            //qDebug()<<"sale one number "<<sale_one_number;//，“卖一申请股数”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"sale1 "<<re.mid(0,i);//，“卖一报价”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"sale2 "<<re.mid(0,i);//，“卖二申请股数”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"sale2 "<<re.mid(0,i);//，“卖二报价”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"sale3 "<<re.mid(0,i);//，“卖三申请股数”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"sale3 "<<re.mid(0,i);//，“卖三报价”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"sale4 "<<re.mid(0,i);//，“卖四申请股数”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"sale4 "<<re.mid(0,i);//，“卖四报价”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"sale5 "<<re.mid(0,i);//，“卖五申请股数”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"sale5 "<<re.mid(0,i);//，“卖五报价”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"date "<<re.mid(0,i);//，“日期”
            re.remove(0,i + 1);
            break;
        }
    }

    for(i = 0 ; i < re.length();i++){
        if(re.at(i) == ','){
            //qDebug()<<"time "<<re.mid(0,i);//，“时间”
            re.remove(0,i + 1);
            break;
        }
    }

    ///
    /// \brief showText
    ///用来处理自动大小时可以使用到
    ///
    ///
    QString showText;
    ///QString showTextLength;
    showText += e_name;
    ///showTextLength += e_name;

    showText += " ";
    ///showTextLength += " ";

    if(open_price >= yesterday_close_price){
        showText += "<font color='#FF0000'>" + QString::number(open_price,'g') +"</font>";
    }else{
        showText += "<font color='#00FF00'>" + QString::number(open_price,'g') +"</font>";
    }
    ///showTextLength += QString::number(open_price,'g');

    showText += "(" + QString::number(yesterday_close_price,'g') + ")";
    ///showTextLength += "(" + QString::number(yesterday_close_price,'g') + ")";

    showText += " ";
    ///showTextLength += " ";

    if(buy_one >= open_price){
        showText += "<font color='#FF0000'>" +  QString::number(buy_one,'g') +"</font>";
    }else{
        showText += "<font color='#00FF00'>" +  QString::number(buy_one,'g') +"</font>";
    }
    ///showTextLength += QString::number(buy_one,'g');

    if(sale_one >= open_price){
        showText += "<font color='#FF0000'>" +  ("(" + QString::number(sale_one,'g') + ")") + "</font>";
    }else{
        showText += "<font color='#00FF00'>" +  ("(" + QString::number(sale_one,'g') + ")") + "</font>";
    }
    ///showTextLength += "(" + QString::number(sale_one,'g') + ")";

    if(buy_one >= open_price){
        showText += "<font color='#FF0000'>""&#x2191;""</font>";//↑ => utf-8bianma
        ///showTextLength += "&#x2191;";
    }else{
        showText += "<font color='#00FF00'>""&#x2193;""</font>";//↓ => utf-8 bianma
        ///showTextLength += "&#x2193;";
    }

    showText += " ";
    ///showTextLength += " ";

    showText += QString::number(buy_one_number/100,10);
    ///showTextLength += QString::number(buy_one_number/100,10);

    showText += "(" + QString::number(sale_one_number/100,10) + ")";
    ///showTextLength += "(" + QString::number(sale_one_number/100,10) + ")";

    ///qDebug()<<showTextLength;;
    ///QFontMetrics fM(qApp->font());
    ///qint32 font_width;
    ///font_width = fM.width(showTextLength);
    ///qDebug()<<font_width;
    ///if(font_width + 10 <= this->width()){
       /// ui->labelEquity->resize(font_width + 4,ui->labelEquity->height());

    ///}else{

    ///}
    ui->labelEquity->setText(showText);
}

void MainWidget::TimerToUpdateEquityInformation()
{
    if(last_requst > settings->value("equity_number/number",0).toInt()){
        last_requst = 1;
    }

    MainWidgetGetEquityData(last_requst);
}

void MainWidget::CreateSystrayIcon()
{
    trayicon =  new QIcon(":/image/equties.ico");
    if(trayicon){
        this->setWindowIcon(*trayicon);
    }


    traymenu = new QMenu(this);
    if(traymenu){
        if(setupAction){
            traymenu->addAction(setupAction);
            traymenu->addSeparator();
        }
        if(quitAction){

            traymenu->addAction(quitAction);
            traymenu->addSeparator();
        }
    }
    tray = new QSystemTrayIcon(this);
    if(tray){
        tray->setIcon(*trayicon);
        tray->setToolTip(tr("equties running in background..."));
        tray->setContextMenu(traymenu);
    }
}

void MainWidget::CreateSystrayActions()
{
#if 0
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
#endif
    setupAction = new QAction(tr("&Setup"), this);
    connect(setupAction, SIGNAL(triggered()), this, SLOT(MainWidgetSetup()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(MainWidgetQuit()));
}

void MainWidget::MainWidgetQuit()
{
    qApp->exit(0);
}

void MainWidget::MainWidgetSetup()
{
    SetupDialog *setup = new SetupDialog(this);
    setup->show();
    connect(setup,SIGNAL(TimeToRefresh(int)),this,SLOT(MainWidgetSetRefreshTime(int)));
    connect(setup,SIGNAL(SliderToRefresh(int)),this,SLOT(SetOpacity(int)));
    connect(setup,SIGNAL(DefaultPosition()),this,SLOT(SetDefaultPosition()));
}

void MainWidget::MainWidgetSetRefreshTime(int time)
{
    timer_to_update->stop();
    settings->setValue("refresh_time/time",time);
    timer_to_update->setInterval(time * 1000);
    timer_to_update->start();
}

void MainWidget::MainWidgetGetEquityData(int number)
{

    QString key;
    key = "equities/equity_" + QString::number(number,10);
    key = settings->value(key,"equity_1").toString();
    QString url = "http://hq.sinajs.cn/list=" + key;
    //qDebug()<<url;
    //QString url = "http://apis.baidu.com/apistore/stockservice/stock";
    //QString url = "http://hq.sinajs.cn/list=sh600020";
    //QString param = "stockid=sh600020&list=1";
    QNetworkRequest request;

    //request.setUrl(QUrl(url + "?" + param));
    request.setUrl(QUrl(url));
    //request.setUrl(QUrl("127.0.0.1"));
    //request.setRawHeader("apikey", "77877c5e8b572bd59d310f5bb7c28de0");
    manager->get(request);
    last_requst ++ ;
}

QSettings * MainWidget::GetGlobalSetting()
{
    return settings;
}

void MainWidget::SetOpacity(int op)
{
    opacity = op;
    this->setWindowOpacity(op / 10.0);
    settings->setValue("opacity/op",op);

}

void MainWidget::SetDefaultPosition()
{
    QRect desktop_rec = desktop->availableGeometry();
    this->move(desktop_rec.width() - this->width(),
               desktop_rec.height() - this->height());
    settings->setValue("position/x",desktop_rec.width() - this->width());
    settings->setValue("position/y",desktop_rec.height() - this->height());
}

void MainWidget::enterEvent(QEvent *event)
{
    this->setWindowOpacity(1);
}

void MainWidget::leaveEvent(QEvent *event)
{
    this->setWindowOpacity(opacity / 10.0);
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{

    press_pos = event->pos();
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    this->move(this->mapToGlobal(event->pos()) - press_pos);
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    settings->setValue("position/x",this->pos().x());
    settings->setValue("position/y",this->pos().y());
}
