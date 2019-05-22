#include "devicequeryform.h"
#include "ui_devicequeryform.h"

DeviceQueryForm::DeviceQueryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceQueryForm)
{
    ui->setupUi(this);
    refreshFlag = 1;
    sqlQueryModel = new QSqlQueryModel(this);
    sortFilterProxyModel = new QSortFilterProxyModel(this);

    cmenu = new QMenu(ui->tableView);
    actionR1 = cmenu->addAction("绑定");
    actionR2 = cmenu->addAction("解绑");
    actionR6 = cmenu->addAction("导出");
    actionR3 = cmenu->addAction("设备故障");
    actionR4 = cmenu->addAction("恢复使用");
    actionR5 = cmenu->addAction("删除");

    connect(actionR1,SIGNAL(triggered(bool)),this,SLOT(bindSelected()));
    connect(actionR2,SIGNAL(triggered(bool)),this,SLOT(unbindSelected()));
    connect(actionR3,SIGNAL(triggered(bool)),this,SLOT(errorSelected()));
    connect(actionR4,SIGNAL(triggered(bool)),this,SLOT(restartSelected()));
    connect(actionR5,SIGNAL(triggered(bool)),this,SLOT(deleteSelected()));
    connect(actionR6,SIGNAL(triggered(bool)),this,SLOT(exportSelected()));

    ui->goatCheckBox->setCheckState(Qt::Unchecked);
}

DeviceQueryForm::~DeviceQueryForm()
{
    delete ui;
}

void DeviceQueryForm::updateTableWidgest(){
    refreshFlag = 0;
    QSqlQuery query;
    if(ui->goatCheckBox->isChecked()){
        query.prepare("select a.deviceId as 设备编号,a.deviceState as 设备状态,ifnull(b.goatId,'无') as 绑定山羊编号,ifnull(b.houseId,'无') as 舍号,a.inTime as 购入时间 from deviceInfo a left join bindingInfo c on a.deviceId = c.deviceId left join goatInfo b on b.goatId = c.goatId where b.houseId = :houseId;");
    }else {
        query.prepare("select a.deviceId as 设备编号, a.deviceState as 设备状态,b.houseId as 绑定舍号,a.inTime as 购入时间 from houseDeviceInfo a left join houseBindingInfo b on a.deviceId = b.deviceId where a.houseId = :houseId;");
    }

    query.bindValue(":houseId",ui->comboBox->currentText());
    query.exec();
    sqlQueryModel->setQuery(query);
    sortFilterProxyModel->setDynamicSortFilter(true);
    sortFilterProxyModel->setSourceModel(sqlQueryModel);

    ui->tableView->setModel(sortFilterProxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);
}

void DeviceQueryForm::showAllData(){
    refreshFlag = 1;
    if(ui->goatCheckBox->isChecked()){
        sqlQueryModel->setQuery("select a.deviceId as 设备编号,a.deviceState as 设备状态,ifnull(b.goatId,'无') as 绑定山羊编号,ifnull(b.houseId,'无') as 舍号,a.inTime as 购入时间 from deviceInfo a left join bindingInfo c on a.deviceId = c.deviceId left join goatInfo b on b.goatId = c.goatId;",DB::instance().data()->getDb());
    }else {
        sqlQueryModel->setQuery("select a.deviceId as 设备编号, a.deviceState as 设备状态,b.houseId as 绑定舍号,a.inTime as 购入时间 from houseDeviceInfo a left join houseBindingInfo b on a.deviceId = b.deviceId;",DB::instance().data()->getDb());
    }
    sortFilterProxyModel->setDynamicSortFilter(true);
    sortFilterProxyModel->setSourceModel(sqlQueryModel);

    ui->tableView->setModel(sortFilterProxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);
}

void DeviceQueryForm::updateTableWidgestByState(){
    refreshFlag = 2;
    QSqlQuery query;
    if(ui->goatCheckBox->isChecked()){
        query.prepare("select a.deviceId as 设备编号,a.deviceState as 设备状态,ifnull(b.goatId,'无') as 绑定山羊编号,ifnull(b.houseId,'无') as 舍号,a.inTime as 购入时间 from deviceInfo a left join bindingInfo c on a.deviceId = c.deviceId left join goatInfo b on b.goatId = c.goatId where a.deviceState = :deviceState;");
    }else {
        query.prepare("select a.deviceId as 设备编号, a.deviceState as 设备状态,b.houseId as 绑定舍号,a.inTime as 购入时间 from houseDeviceInfo a left join houseBindingInfo b on a.deviceId = b.deviceId where a.deviceState = :deviceState;");
    }
    query.bindValue(":deviceState",ui->comboBox_2->currentText());
    query.exec();
    sqlQueryModel->setQuery(query);
    sortFilterProxyModel->setDynamicSortFilter(true);
    sortFilterProxyModel->setSourceModel(sqlQueryModel);

    ui->tableView->setModel(sortFilterProxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);
}

void DeviceQueryForm::on_pushButton_clicked()
{
    showAllData();
}

void DeviceQueryForm::on_comboBox_currentIndexChanged(const QString &arg1)
{
    updateTableWidgest();
}

void DeviceQueryForm::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    updateTableWidgestByState();
}

void DeviceQueryForm::on_comboBox_activated(const QString &arg1)
{
    updateTableWidgest();
}

void DeviceQueryForm::on_comboBox_2_activated(const QString &arg1)
{
    updateTableWidgestByState();
}

void DeviceQueryForm::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    if(ui->tableView->selectionModel()->selectedIndexes().size() > 0){
        cmenu->exec(QCursor::pos());
    }
}

void DeviceQueryForm::bindSelected(){
    if(ui->goatCheckBox->isChecked()){
        emit deviceIdSignal(ui->tableView->model()->index(ui->tableView->selectionModel()->selectedIndexes().at(0).row(),0).data().toString());
    }else{
        emit houseDeviceIdSignal(ui->tableView->model()->index(ui->tableView->selectionModel()->selectedIndexes().at(0).row(),0).data().toString());
    }

}

void DeviceQueryForm::unbindSelected(){
    QModelIndexList tempList = ui->tableView->selectionModel()->selectedIndexes();
    QList<int> list;
    foreach(QModelIndex temp, tempList){
        if(!list.contains(temp.row())){
            list.append(temp.row());
        }
    }

    QSqlQuery query;
    if(ui->goatCheckBox->isChecked()){
        query.prepare("delete from bindingInfo where deviceId = :deviceId;");
    }else {
        query.prepare("delete from houseBindingInfo where deviceId = :deviceId;");
    }
    foreach (int temp, list) {
        query.bindValue(":deviceId",ui->tableView->model()->index(temp,0).data().toString());
        query.exec();
        //qDebug() << ui->tableView->model()->index(temp,0).data().toString();
    }
    emit updateSignal();
}

void DeviceQueryForm::exportSelected(){
    QModelIndexList tempList = ui->tableView->selectionModel()->selectedIndexes();
    if(tempList.size() < 1){
        QMessageBox::warning(this,"警告","请先选择要导出的数据!");
        return;
    }
    QList<int> list;
    foreach(QModelIndex temp, tempList){
        if(!list.contains(temp.row())){
            list.append(temp.row());
        }
    }
    QString filePath = QFileDialog::getSaveFileName(this,tr("打开"),".",tr("文本文档(*.txt)"));
    if(!filePath.isNull()){
        QFile file(filePath);
        if(!file.open((QIODevice::WriteOnly | QIODevice::Text))){
            qDebug() << "Open failed!";
        }else{
            QTextStream fileOut(&file);
            int colCount = ui->tableView->model()->columnCount();
            for(int i = 0; i<colCount;i++){
                fileOut << ui->tableView->model()->headerData(i,Qt::Horizontal).toString().toUtf8() << "\t";
            }
            fileOut << "\n";
            foreach (int temp, list) {
                for(int i = 0;i<ui->tableView->model()->columnCount();i++){
                    fileOut << ui->tableView->model()->index(temp,i).data().toString().toUtf8() << "\t";
                }
                fileOut << "\n";
            }
            file.close();
        }
    }
}

void DeviceQueryForm::errorSelected(){
    QModelIndexList tempList = ui->tableView->selectionModel()->selectedIndexes();
    QList<int> list;
    foreach(QModelIndex temp, tempList){
        if(!list.contains(temp.row())){
            list.append(temp.row());
        }
    }

    QSqlQuery query;
    QSqlQuery delQuery;
    if(ui->goatCheckBox->isChecked()){
        delQuery.prepare("delete from bindingInfo where deviceId = :deviceId;");
        query.prepare("update deviceInfo set deviceState = '故障' where deviceId = :deviceId;");
    }else {
        delQuery.prepare("delete from houseBindingInfo where deviceId = :deviceId;");
        query.prepare("update houseDeviceInfo set deviceState = '故障' where deviceId = :deviceId;");
    }

    //query.prepare("delete from bindingInfo where goatId = :goatId;");

    foreach (int temp, list) {
        delQuery.bindValue(":deviceId",ui->tableView->model()->index(temp,0).data().toString());
        query.bindValue(":deviceId",ui->tableView->model()->index(temp,0).data().toString());
        delQuery.exec();
        query.exec();
        //qDebug() << ui->tableView->model()->index(temp,0).data().toString();
    }
    emit updateSignal();
}

void DeviceQueryForm::restartSelected(){
    QModelIndexList tempList = ui->tableView->selectionModel()->selectedIndexes();
    QList<int> list;
    foreach(QModelIndex temp, tempList){
        if(!list.contains(temp.row())){
            list.append(temp.row());
        }
    }

    QSqlQuery query;
    if(ui->goatCheckBox->isChecked()){
        query.prepare("update deviceInfo set deviceState = '闲置' where deviceId = :deviceId;");
    }else{
        query.prepare("update houseDeviceInfo set deviceState = '闲置' where deviceId = :deviceId;");
    }

    foreach (int temp, list) {
        query.bindValue(":deviceId",ui->tableView->model()->index(temp,0).data());
        query.exec();
        //qDebug() << ui->tableView->model()->index(temp,0).data().toString();
    }
    emit updateSignal();
}

void DeviceQueryForm::deleteSelected(){
    int confirm = QMessageBox::question(this,"确认","确定删除选中设备吗?",QMessageBox::Yes,QMessageBox::No);
    if(confirm != QMessageBox::Yes){
        return;
    }
    QModelIndexList tempList = ui->tableView->selectionModel()->selectedIndexes();
    QList<int> list;
    foreach(QModelIndex temp, tempList){
        if(!list.contains(temp.row())){
            list.append(temp.row());
        }
    }

    QSqlQuery query;
    if(ui->goatCheckBox->isChecked()){
        query.prepare("delete from deviceInfo where deviceId = :deviceId;");
    }else{
        query.prepare("delete from houseDeviceInfo where deviceId = :deviceId;");
    }
    foreach (int temp, list) {
        query.bindValue(":deviceId",ui->tableView->model()->index(temp,0).data().toString());
        query.exec();
        //qDebug() << ui->tableView->model()->index(temp,0).data().toString();
    }
    emit updateSignal();
}

void DeviceQueryForm::refreshView(){
    updateHouseId();
    switch (refreshFlag) {
    case 0:
        updateTableWidgest();
        break;
    case 1:
        showAllData();
        break;
    case 2:
        updateTableWidgestByState();
        break;
    default:
        updateTableWidgest();
        break;
    }
}

void DeviceQueryForm::updateHouseId(){
    int runFlag = refreshFlag;
    QString curText = ui->comboBox->currentText();
    ui->comboBox->clear();
    QSqlQuery query(DB::instance().data()->getDb());
    query.exec("select * from houseInfo");
    while(query.next()){
        ui->comboBox->addItem(query.value(0).toString());
    }
    if(!curText.isNull()){
        if(ui->comboBox->findText(curText) > -1){
            ui->comboBox->setCurrentText(curText);
        }
    }
    refreshFlag = runFlag;
}

void DeviceQueryForm::on_goatCheckBox_stateChanged(int arg1)
{
    updateTableWidgest();
}
