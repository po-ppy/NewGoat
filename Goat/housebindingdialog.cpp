#include "housebindingdialog.h"
#include "ui_housebindingdialog.h"

HouseBindingDialog::HouseBindingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HouseBindingDialog)
{
    ui->setupUi(this);
    houseSqlQueryModel = new QSqlQueryModel(this);
    houseSortFilterProxyModel = new QSortFilterProxyModel(this);

    deviceSqlQueryModel = new QSqlQueryModel(this);
    deviceSortFilterProxyModel = new QSortFilterProxyModel(this);
}

HouseBindingDialog::~HouseBindingDialog()
{
    delete ui;
}

void HouseBindingDialog::updateHouseTable(){

    if(ui->houseCheckBox->isChecked()){
        houseSqlQueryModel->setQuery("select a.houseId as 舍号,ifnull(b.deviceId ,'无') as 绑定设备id,b.inTime as 购入时间 from houseInfo a left join houseBindingInfo c on a.houseId = c.houseId left join houseDeviceInfo b on b.deviceId = c.deviceId where b.deviceId is NULL;");
    }else{
        houseSqlQueryModel->setQuery("select a.houseId as 舍号,ifnull(b.deviceId ,'无') as 绑定设备id,b.inTime as 购入时间 from houseInfo a left join houseBindingInfo c on a.houseId = c.houseId left join houseDeviceInfo b on b.deviceId = c.deviceId;");
    }
    houseSortFilterProxyModel->setDynamicSortFilter(true);
    houseSortFilterProxyModel->setSourceModel(houseSqlQueryModel);

    ui->houseTableView->setModel(houseSortFilterProxyModel);
    ui->houseTableView->setSortingEnabled(true);
    ui->houseTableView->horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);
}

void HouseBindingDialog::updateDeviceTable(){
    if(ui->houseDeviceCheckBox->isChecked()){
        deviceSqlQueryModel->setQuery("select a.deviceId as 设备编号,a.deviceState as 设备状态,ifnull(b.houseId,'无') as 舍号,a.inTime as 购入时间 from houseDeviceInfo a left join houseBindingInfo c on a.deviceId = c.deviceId left join houseInfo b on b.hosueId = c.houseId where a.deviceState = '闲置';");
    }else{
        deviceSqlQueryModel->setQuery("select a.deviceId as 设备编号,a.deviceState as 设备状态,ifnull(b.houseId,'无') as 舍号,a.inTime as 购入时间 from houseDeviceInfo a left join houseBindingInfo c on a.deviceId = c.deviceId left join houseInfo b on b.hosueId = c.houseId where a.deviceState <> '故障';");
    }
    deviceSortFilterProxyModel->setDynamicSortFilter(true);
    deviceSortFilterProxyModel->setSourceModel(deviceSqlQueryModel);

    ui->houseDeviceTableView->setModel(deviceSortFilterProxyModel);
    ui->houseDeviceTableView->setSortingEnabled(true);
    ui->houseDeviceTableView->horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);
}

void HouseBindingDialog::startBinding(){
    QSqlQuery* qSqlQuery = new QSqlQuery();
    qSqlQuery->prepare("insert into houseBindingInfo(houseId ,deviceId) values(:houseId ,:deviceId) on duplicate key update houseId=values(houseId),deviceId=values(deviceId);");
    qSqlQuery->bindValue(":houseId",ui->houseSelected->text().trimmed());
    qSqlQuery->bindValue(":deviceId",ui->houseDeviceSelected->text().trimmed());
    if(qSqlQuery->exec()){
        ui->bindingResult->setText("羊舍("+ui->houseSelected->text()+") 和 设备("+ui->houseDeviceSelected->text()+") 绑定成功 。");
        ui->bindingResult->setStyleSheet("color:green;");
        ui->houseSelected->setText("");
        ui->houseDeviceSelected->setText("");
        emit updateSignal();
        //updateGoatTable();
        //updateDeviceTable();
    }else{
        ui->bindingResult->setText("绑定失败！");
        ui->bindingResult->setStyleSheet("color:red;");
        qDebug() << qSqlQuery->lastQuery();
          QSqlError err;
        err= qSqlQuery->lastError();
         qDebug() << err.text();
    }

}

void HouseBindingDialog::receiveGoatId(QString goatId){
    if(!this->isVisible()){
        this->show();
    }
    ui->houseSelected->setText(goatId);
}

void HouseBindingDialog::receiveDeviceId(QString deviceId){
    if(!this->isVisible()){
        this->show();
    }
    ui->houseDeviceSelected->setText(deviceId);
}

void HouseBindingDialog::addFromFile(){
    QString filePath = QFileDialog::getOpenFileName(this,"打开",".","文本文档(*.txt)");
    if(filePath.isNull()){
        return;
    }
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QList<QString> errList;
        int count = 0;
        QSqlQuery query;
        query.prepare("insert into bindingInfo(goatId ,deviceId) values(:goatId ,:deviceId) on duplicate key update goatId=values(goatId),deviceId=values(deviceId);");
        while(!file.atEnd()){
            QList<QString> tempInfoList = QString::fromLocal8Bit(file.readLine()).remove("\n").split(" ");
            tempInfoList.removeAll("");
            tempInfoList.removeAll(" ");
            query.bindValue(":deviceId",tempInfoList.at(0));
            query.bindValue(":goatId",tempInfoList.at(1));
            if(!query.exec()){
                errList.append(tempInfoList.at(0));
            }else{
                count += 1;
            }
        }
        emit updateSignal();
        QMessageBox::information(this,"结果",QString::number(count)+"条导入成功，"+QString::number(errList.length())+"条导入失败。(失败结果保存在剪切板中");
        if(errList.length()>0){
            QClipboard *tempBoard = QApplication::clipboard();
            QString temp = "";
            foreach (QString elem, errList) {
                temp += elem + "\n";
            }
            tempBoard->setText("导入失败设备编号如下:\n"+temp);
        }
    }else{
        QMessageBox::warning(this,"警告","文件打开失败!");
    }
}
