#include "setupdialog.h"
#include "ui_setupdialog.h"

SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    int time;
    QSettings *settings;
    settings =  MainWidget::GetGlobalSetting();
    time = settings->value("refresh_time/time",30).toInt();
    if(time <= 1){
        ui->comboBoxRefresh->setCurrentIndex(0);
    }else if(time <= 5){
        ui->comboBoxRefresh->setCurrentIndex(1);
    }else if(time <= 10){
        ui->comboBoxRefresh->setCurrentIndex(2);
    }else if(time <=30){
        ui->comboBoxRefresh->setCurrentIndex(3);
    }else if(time <= 60){
        ui->comboBoxRefresh->setCurrentIndex(4);
    }else if(time <= 300){
        ui->comboBoxRefresh->setCurrentIndex(5);
    }else if(time <= 600){
        ui->comboBoxRefresh->setCurrentIndex(6);
    }else if(time <= 1800){
        ui->comboBoxRefresh->setCurrentIndex(7);
    }else if(time <= 3600){
        ui->comboBoxRefresh->setCurrentIndex(8);
    }else{
        ui->comboBoxRefresh->setCurrentIndex(3);
    }
    int opacity;
    opacity = settings->value("opacity/op",0).toInt();
    ui->horizontalSlider->setValue(opacity);
    this->setFixedSize(this->size());
    connect(ui->pushButtonAdd,SIGNAL(clicked()),this,SLOT(PushButtonAddSlot()));
    connect(ui->comboBoxRefresh,SIGNAL(activated(int)),this,SLOT(ComboBoxRefreshSlot(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(HorizontalSliderSlot(int)));
}

SetupDialog::~SetupDialog()
{
    delete ui;
}

void SetupDialog::PushButtonAddSlot()
{
    QSettings *settings;
    int number;
    settings =  MainWidget::GetGlobalSetting();
    //number = MainWidget::GetGlobalEquityNumber();
    if(settings){
        number = settings->value("equity_number/number",0).toInt();
        QString equity = "equities/equity_" + QString::number(number + 1,10);
        settings->setValue("equity_number/number",number + 1);
        settings->setValue(equity,ui->lineEditEquity->text());

    }else{
        QMessageBox::warning(NULL,tr("Warning!"),tr("settings is null!"));
    }
}

void SetupDialog::ComboBoxRefreshSlot(int index)
{
    int refresh_time = 0;
    switch(index){
    case 0:
        refresh_time = 1;
        break;
    case 1:
        refresh_time = 5;
        break;
    case 2:
        refresh_time = 10;
        break;
    case 3:
        refresh_time = 30;
        break;
    case 4:
        refresh_time = 60;
        break;
    case 5:
        refresh_time = 300;
        break;
    case 6:
        refresh_time = 600;
        break;
    case 7:
        refresh_time = 1800;
        break;
    case 8:
        refresh_time = 3600;
        break;
    default:
        refresh_time = 30;
        break;
    }

    emit TimeToRefresh(refresh_time);
}

void SetupDialog::HorizontalSliderSlot(int pos)
{
    if(pos <= 0){
        pos = 1;
    }
    if(pos > 10){
        pos = 10;
    }
    emit SliderToRefresh(pos);
}
