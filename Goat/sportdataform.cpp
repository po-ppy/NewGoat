#include "sportdataform.h"
#include "ui_sportdataform.h"

SportDataForm::SportDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SportDataForm)
{
    ui->setupUi(this);
    //wkThread = new QThread();
//    setContextMenuPolicy(Qt::DefaultContextMenu);
//    connect(ui->tabWidget,SIGNAL(tabBarDoubleClicked(int)),this,SLOT(delete_tab()));
    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(delete_tab(int)));
}

SportDataForm::~SportDataForm()
{
    delete ui;
}

void SportDataForm::updateTabs(){
    //MainForm* curTab = (MainForm*)ui->tabWidget->currentWidget();
    QString curTabLabel;
    bool memoryFlag = false;
    int tabCount = 0;
    if(ui->tabWidget->count() > 0){
        memoryFlag = true;
        curTabLabel = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
        tabCount = ui->tabWidget->count();
//        ui->tabWidget->clear();
    }
    QSqlQuery query;
    query.exec("select * from houseInfo;");
    while(query.next()){
        InformationForm *temp = new InformationForm();
        connect(temp,SIGNAL(exportSignal()),this,SLOT(exportToFile()));
        //MainForm *temp = new MainForm();
        temp->setHouseId(query.value(0).toString());
//        connect(this,SIGNAL(updateSignal()),temp,SLOT(updateGoatList()));
//        connect(this,SIGNAL(updateSignal()),temp,SLOT(initSqlTable()));
        //temp->moveToThread(wkThread);
        //wkThread->start();
//        temp->initSqlTable();
        temp->updateChart();
        ui->tabWidget->addTab(temp,QIcon(":/she.png"),query.value(0).toString());
    }
    for(int i = 0;i<tabCount;i++){
        QWidget* temp = ui->tabWidget->widget(0);
        ui->tabWidget->removeTab(0);
        delete temp;
    }
    tabCount = ui->tabWidget->count();
    if(memoryFlag){
        for(int i = 0; i < tabCount;i++){
            if(curTabLabel == ui->tabWidget->tabText(i)){
                ui->tabWidget->setCurrentIndex(i);
                break;
            }
        }
    }
//    emit updateSignal();
}

void SportDataForm::delete_tab(int index){
    ui->tabWidget->removeTab(index);
}

void SportDataForm::insert_tab(){
    //MainForm *temp = new MainForm();
    InformationForm *temp = new InformationForm();
//    ui->tabWidget->addTab(temp,"舍?");
//    QIcon icon;
//    icon.addFile(tr(":/nu.png"));
    ui->tabWidget->addTab(temp,QIcon(":/she.png"),QString("舍?"));
}

void SportDataForm::delete_tab(){
    ui->tabWidget->removeTab(ui->tabWidget->count() - 1);
}

void SportDataForm::exportToFile(){
    QString Sql_exec = "";
    int backInfo = QMessageBox::question(this,"确认","确定只导出本舍环境数据吗？\n选NO将导出所有羊舍数据。",QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    switch (backInfo) {
    case QMessageBox::Yes :
        Sql_exec = QString("select * from houseData where houseId = '%1';").arg(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
        break;
    case QMessageBox::No :
        Sql_exec = "select * from houseData;";
        break;
    default:
        return;
    }

    QSqlQuery query;
    if(!query.exec(Sql_exec)){
        QMessageBox::warning(this,"警告","数据库连接失败！");
        return;
    }else{
        if(query.size() < 1){
            QMessageBox::information(this,"提示","未找到绑定数据!");
            return;
        }
    }

    QString filePath = QFileDialog::getSaveFileName(this,tr("打开"),".",tr("文本文档(*.txt)"));
    if(!filePath.isNull()){
        QFile file(filePath);
        if(!file.open((QIODevice::WriteOnly | QIODevice::Text))){
            QMessageBox::warning(this,"警告","文件打开失败！");
            qDebug() << "HouseBindnig Open failed!";
            return;
        }else{
            QTextStream fileOut(&file);
            fileOut << QString("舍号").toUtf8() <<"\t" << QString("温度").toUtf8() <<"\t" << QString("氨气浓度").toUtf8() <<"\t" << QString("湿度").toUtf8() <<"\t" << QString("二氧化碳浓度").toUtf8() <<"\t" << QString("光照强度").toUtf8() <<"\t" << QString("PM2.5").toUtf8() <<"\t" << QString("PM10").toUtf8() <<"\t" << QString("烟雾").toUtf8() <<"\t" << QString("记录时间").toUtf8() << "\n";
            while(query.next()){
                fileOut << query.value("houseId").toString().toUtf8() << "\t" << query.value("wendu").toString().toUtf8()<< "\t" << query.value("anqi").toString().toUtf8() << "\t" << query.value("shidu").toString().toUtf8() << "\t" << query.value("eryang").toString().toUtf8() << "\t" << query.value("guangzhao").toString().toUtf8() << "\t" << query.value("pm25").toString().toUtf8() << "\t" << query.value("pm10").toString().toUtf8() << "\t" << query.value("yanwu").toString().toUtf8() << "\t" << QDateTime::fromMSecsSinceEpoch(query.value("datatimem").toLongLong()).toString().toUtf8()<< "\n";
            }
            file.close();
            QMessageBox::information(this,"成功","成功导入到指定文件!");
        }
    }
}

//void SportDataForm::delete_current_index(){

//}

//void SportDataForm::contextMenuEvent(QContextMenuEvent *event){
//    qDebug() << event->x();
//    QMenu *menu = new QMenu(this);
//    QAction *action_delete_this = new QAction(tr("&Delete"),menu);
//    menu->addAction(new QAction(tr("&Del"),menu));
//    menu->move(cursor().pos());
//    menu->show();
//}

void SportDataForm::on_tabWidget_currentChanged(int index)
{
    InformationForm* temp = (InformationForm *)ui->tabWidget->widget(index);
//    temp->updateSqlTable();
    temp->updateChart();
}
