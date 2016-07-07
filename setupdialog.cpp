#include "setupdialog.h"
#include "ui_setupdialog.h"

SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(tr("Setup"));
    int time;

    settings =  MainWidget::GetGlobalSetting();
    if(settings == NULL){
        QMessageBox::critical(this,tr("Error!"),tr("Configure file miss!"));
        this->close();
    }
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

    ui->tableWidgetMaintain->verticalHeader()->setFixedWidth(40);
    int table_w ;
    table_w = ui->tableWidgetMaintain->width();
    table_w = (table_w - 40) ;
    ui->tableWidgetMaintain->horizontalHeader()->resizeSection(0,table_w - 1);
    ui->tableWidgetMaintain->horizontalHeader()->resizeSection(1,table_w);
    ui->tableWidgetMaintain->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableWidgetMaintain->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行


    e_num = settings->value("equity_number/number",0).toInt();
    ui->tableWidgetMaintain->setRowCount(e_num);
    for(int i = 1;i <= e_num;i++){
        QTableWidgetItem *item;
        item = new QTableWidgetItem;
        QString equity = "equities/equity_" + QString::number(i,10);
        equity = settings->value(equity,"sh000000").toString();
        item->setText(equity);
        ui->tableWidgetMaintain->setItem(i - 1,0,item);
        code_list<<equity;

    }

    ui->pushButtonDeleteRow->setEnabled(false);

    connect(ui->comboBoxRefresh,SIGNAL(activated(int)),this,SLOT(ComboBoxRefreshSlot(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(HorizontalSliderSlot(int)));
    connect(ui->pushButtonAddRow,SIGNAL(clicked()),this,SLOT(PushButtonAddRowSlot()));
    connect(ui->pushButtonDeleteRow,SIGNAL(clicked()),this,SLOT(PushButtonDeleteRowSlot()));
    //connect(ui->tableWidgetMaintain,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(TableWidgetItemClickedSlot(QTableWidgetItem*)));
    connect(ui->tableWidgetMaintain,SIGNAL(itemSelectionChanged()),this,SLOT(TableWidgetItemSelectionChangedSlot()));
    connect(ui->tableWidgetMaintain,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(TableWidgetItemChangedSlot(QTableWidgetItem*)));
    connect(ui->pushButtonDefaultPosition,SIGNAL(clicked()),this,SLOT(PushButtonDefaultPositionClickedSlot()));
    connect(ui->pushButtonDefaultFont,SIGNAL(clicked()),this,SLOT(PushButtonFontChangeSlot()));
}

SetupDialog::~SetupDialog()
{
    delete ui;
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

void SetupDialog::PushButtonAddRowSlot()
{
    ui->tableWidgetMaintain->insertRow(ui->tableWidgetMaintain->rowCount());
    e_num ++;
}

void SetupDialog::PushButtonDeleteRowSlot()
{
    ui->tableWidgetMaintain->removeRow(ui->tableWidgetMaintain->currentRow());
    e_num --;
    settings->setValue("equity_number/number",e_num);
    for(int i = 0 ;i < e_num ;i++){
        QString equity = "equities/equity_" + QString::number(i + 1,10);
        settings->setValue(equity,ui->tableWidgetMaintain->item(i,1)->text());
    }
}

void SetupDialog::TableWidgetItemClickedSlot(QTableWidgetItem *item)
{

}

void SetupDialog::TableWidgetItemSelectionChangedSlot()
{
    if(ui->tableWidgetMaintain->currentRow() != -1){
        ui->pushButtonDeleteRow->setEnabled(true);
    }else{
        ui->pushButtonDeleteRow->setEnabled(false);
    }
}

void SetupDialog::TableWidgetItemChangedSlot(QTableWidgetItem *item)
{
    settings->setValue("equity_number/number",e_num);
    for(int i = 0 ;i < e_num ;i++){
        QString equity = "equities/equity_" + QString::number(i + 1,10);
        settings->setValue(equity,ui->tableWidgetMaintain->item(i,1)->text());
    }
}

void SetupDialog::PushButtonDefaultPositionClickedSlot()
{
    emit DefaultPosition();
}

void SetupDialog::PushButtonFontChangeSlot()
{
    //QFontDialog *fontDialog = new QFontDialog(this);
    //fontDialog->setModal(true);
    //fontDialog->show();
    bool ok;
    QFont font = QFontDialog::getFont(&ok,qApp->font(),this,tr("select font"));
    if(ok){
        qApp->setFont(font);
        //settings->setValue("font/current",font);
    }else{

    }
}
