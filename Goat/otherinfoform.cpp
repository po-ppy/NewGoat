#include "otherinfoform.h"
#include "ui_otherinfoform.h"

OtherInfoForm::OtherInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OtherInfoForm)
{
    ui->setupUi(this);
    sqlQueryModel = new QSqlQueryModel(this);
    sqlTableModel = new QSqlTableModel(this,DB::instance().data()->getDb());
    sortFilterProxyModel = new QSortFilterProxyModel(this);

    feedHeadList = QList<QString>();
    vacineHeadList = QList<QString>();
    productHeadList = QList<QString>();
    eventHeadList = QList<QString>();

    initMenu();

    ui->addMoreButton->hide();
//    ui->addMoreButton->setDisabled(true);

    feedHeadList << tr("饲料编号") << tr("饲料名称") << tr("适用范围") << tr("用法用量") << tr("备注");
    vacineHeadList << tr("疫苗编号") << tr("疫苗名称") << tr("疫苗种类") << tr("免疫时间") << tr("免疫剂量") << tr("注射部位") << tr("备注");
    productHeadList << tr("产品编号") << tr("产品名称") << tr("备注");
    eventHeadList << tr("事件编号") << tr("事件含义");
//    QObject::connect(sqlTableModel,SIGNAL(beforeUpdate(int row,QSqlRecord &record)),this,SLOT(updateData(int row, QSqlRecord &record)));



    preInfoType = 6;
    infoType = 0;

}

OtherInfoForm::~OtherInfoForm()
{
    delete ui;
}

void OtherInfoForm::on_addMoreButton_clicked()
{

    emit sqlTableModel->dataChanged(ui->tableView->selectionModel()->selectedIndexes().first(),ui->tableView->selectionModel()->selectedIndexes().last());

}

void OtherInfoForm::updateTableView(){
    switch (infoType) {
    case 0 :
        sqlTableModel->setTable("feedInfo");
        sqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        sqlTableModel->select();
        for(int i = 0; i < feedHeadList.size(); i++){
            sqlTableModel->setHeaderData(i,Qt::Horizontal,feedHeadList.at(i));
        }
        break;
    case 1 :
        sqlTableModel->setTable("vacineInfo");
        sqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        sqlTableModel->select();
        for(int i = 0; i < vacineHeadList.size(); i++){
            sqlTableModel->setHeaderData(i,Qt::Horizontal,vacineHeadList.at(i));
        }
        break;
    case 2 :
        sqlTableModel->setTable("productInfo");
        sqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        sqlTableModel->select();
        for(int i = 0; i < productHeadList.size(); i++){
            sqlTableModel->setHeaderData(i,Qt::Horizontal,productHeadList.at(i));
        }
        break;
    case 3 :
        sqlTableModel->setTable("eventInfo");
        sqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        sqlTableModel->select();
        for(int i = 0; i < eventHeadList.size(); i++){
            sqlTableModel->setHeaderData(i,Qt::Horizontal,eventHeadList.at(i));
        }
        break;
    }

    ui->tableView->setModel(sqlTableModel);
    ui->tableView->show();


//    QSqlQuery query;

//    switch (infoType) {
//    case 0 :
//        query.exec("select feedId as 饲料编号, feedName as 饲料名称, feedRange as 使用范围, feedUsage as 用法用量, feedRemark as 备注 from feedInfo;");
//        break;
//    case 1 :
//        query.exec("select vacineId as 疫苗编号, vacineName as 疫苗名称, vacineType as 疫苗种类, vacineType as 疫病种类, vacineDose as 免疫剂量, vacinePart as 免疫部位,vacineRemark as 备注 from vacineInfo;");
//        break;
//    case 2:
//        query.exec("select productId as 产品编号, productName as 产品名称, productRemark as 备注 from productInfo;");
//        break;
//    default:
//        infoType = 0;
//        query.exec("select '类型出错，请联系软件开发者！';");
//    }

//    sqlQueryModel->setQuery(query);

//    sortFilterProxyModel->setDynamicSortFilter(true);
//    sortFilterProxyModel->setSourceModel(sqlQueryModel);

//    ui->tableView->setModel(sortFilterProxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    int tempCount = ui->tableView->horizontalHeader()->count() - 1 ;
    for(int i = 0; i < tempCount;i++){
        ui->tableView->horizontalHeader()->setSectionResizeMode(i,QHeaderView::ResizeToContents);
    }
}

void OtherInfoForm::autoUpdateTableView(){
    if(ui->tableView->model() == nullptr){
        updateTableView();
    }

    if(infoType != preInfoType){
        updateTableView();
        preInfoType = infoType;
    }
}

bool OtherInfoForm::setInfoType(int temp){
    infoType = temp;
    switch (infoType) {
    case 0 :
        ui->titleLabel->setText("饲料信息");
        break;
    case 1 :
        ui->titleLabel->setText("疫苗信息");
        break;
    case 2 :
        ui->titleLabel->setText("产品信息");
        break;
    case 3 :
        ui->titleLabel->setText("事件信息");
        break;
    default:
        infoType = 0;
        ui->titleLabel->setText("饲料信息");
        return false;
    }
    return true;
}

void OtherInfoForm::on_addButton_clicked()
{

    addOne();
}

void OtherInfoForm::on_removeButton_clicked()
{
    deleteSelected();
}

void OtherInfoForm::deleteSelected(){

    QModelIndexList tempList = ui->tableView->selectionModel()->selectedIndexes();
    if(tempList.size() > 0){
        int confirm = QMessageBox::question(this,"确认","确定删除选中数据吗?",QMessageBox::Yes,QMessageBox::No);
        if(confirm != QMessageBox::Yes){
            return;
        }
    }
//    ui->tableView->model()->removeRows(tempList.first().row(),tempList.size());
//    qDebug() << "tempList.first().row(): " << tempList.first().row();
//    qDebug() << "tempList.size(): " << tempList.size();
    QList<int> list;
    foreach(QModelIndex temp, tempList){
        if(!list.contains(temp.row())){
            list.append(temp.row());
        }
    }
    qDebug() << ui->tableView->model()->removeRows(tempList.first().row(),list.size());
    qDebug() << "tempList.first().row(): " << tempList.first().row();
    qDebug() << "list.size(): " << list.size();
    sqlTableModel->submitAll();
    sqlTableModel->select();

//    QSqlQuery query;
//    switch (infoType) {
//    case 0 :
//        query.prepare("delete from feedInfo where feedId = :theId;");
//        break;
//    case 1 :
//        query.prepare("delete from vacineInfo where vacineId = :theId;");
//        break;
//    case 2:
//        query.prepare("delete from productInfo where productId = :theId;");
//        break;
//    }
//    //query.prepare("delete from deviceInfo where deviceId = :deviceId;");
//    foreach (int temp, list) {
//        query.bindValue(":theId",ui->tableView->model()->index(temp,0).data().toString());
//        query.exec();
//        //qDebug() << ui->tableView->model()->index(temp,0).data().toString();
//    }
////    emit updateSignal();
//    updateTableView();
}

//bool QAbstractItemModel::insertRows(int row, int count, const QModelIndex &parent){
//    beginInsertRows(parent,row,row+count-1);

//    endInsertRows();
//    return  true;
//}

//bool QAbstractItemModel::removeRows(int row, int count, const QModelIndex &parent){
//    beginRemoveRows(parent,row,row+count-1);
//    qDebug() << "row: " << row;
//    qDebug() << "row+count-1 : " << row+count-1;

//    endRemoveRows();
//    return true;
//}

void OtherInfoForm::updateData(int row, QSqlRecord &record){
    sqlTableModel->submitAll();
//    sqlTableModel->setData();
}

void OtherInfoForm::onInsertNewRow(int row, QSqlRecord &record){
    record.setValue(0,"hello");
}

QString OtherInfoForm::createId(){
    bool createFlag = false;
    int rowNum = sqlTableModel->rowCount();
    QString firstWord = "VC";
    QString SQL_select = "select vacineId from vacineInfo where vacineId = :newId;";
    if(rowNum < 1 ){
        return nullptr;
    }
    switch (infoType) {
    case 0 :
        firstWord = "F";
        SQL_select = "select feedId from feedInfo where feedId = :newId ;";
        break;
    case 1 :
        firstWord = "VC";
        SQL_select = "select vacineId from vacineInfo where vacineId = :newId ;";
        break;
    case 2 :
        firstWord = "P";
        SQL_select = "select productId from productInfo where productId = :newId ;";
        break;
    case 3 :
        firstWord = "";
        SQL_select = "select eventId from eventInfo where eventId = :newId ;";
        break;
    }
    int startNum = rowNum;
    if(infoType != 3){
        startNum += 1000;
    }
//    int startNum = 1000 + rowNum;
    QString newId;
    QSqlQuery query;
    query.prepare(SQL_select);

    while (!createFlag) {
        newId = firstWord+QString::number(startNum);
        query.bindValue(":newId",newId);
        if(query.exec()){
            if(query.size() < 1){
                createFlag = true;
                break;
            }else{
                startNum += 1;
            }
        }else{
            return nullptr;
        }
    }
    return newId;


}

void OtherInfoForm::on_saveChangeButton_clicked()
{
    saveChange();
}

void OtherInfoForm::initMenu(){
    cmenu = new QMenu(ui->tableView);
    actionAdd = cmenu->addAction("添加");
    actionRemove = cmenu->addAction("移除");
    actionAddMore = cmenu->addAction("批量导入");
    actionSaveChange = cmenu->addAction("保存修改");
    actionExport = cmenu->addAction("导出选中");

    connect(actionAdd,SIGNAL(triggered(bool)),this,SLOT(addOne()));
    connect(actionAddMore,SIGNAL(triggered(bool)),this,SLOT(addMore()));
    connect(actionRemove,SIGNAL(triggered(bool)),this,SLOT(deleteSelected()));
    connect(actionSaveChange,SIGNAL(triggered(bool)),this,SLOT(saveChange()));
    connect(actionExport,SIGNAL(triggered(bool)),this,SLOT(exportSelected()));

}

void OtherInfoForm::addOne(){
    int rowNum = ui->tableView->model()->rowCount();
    QSqlRecord temp = sqlTableModel->record();
    temp.setValue(0,createId());
    if(infoType == 1){
        temp.setValue(1,"填写疫苗名称");
        temp.setValue(2,"填写疫苗种类");
    }

    sqlTableModel->insertRecord(rowNum,temp);

    sqlTableModel->submitAll();
//    sqlTableModel->select();
}

void OtherInfoForm::addMore(){

}

void OtherInfoForm::saveChange(){
    if(sqlTableModel->submitAll()){
        QMessageBox::information(this,tr("保存成功"),tr("保存成功！！"));
    }else{
        QMessageBox::warning(this,"保存失败","保存失败!!");
    }
}

void OtherInfoForm::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    if(ui->tableView->selectionModel()->selectedIndexes().size() > 0){
        cmenu->exec(QCursor::pos());
    }
}

void OtherInfoForm::exportSelected(){
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
            QString header = "";
            switch (infoType) {
            case 0 :
                foreach(QString temp, feedHeadList){
                    fileOut << temp.toUtf8() << "\t";
                }
                break;
            case 1 :
                foreach(QString temp, vacineHeadList){
                    fileOut << temp.toUtf8() << "\t";
                }
                break;
            case 2 :
                foreach(QString temp, productHeadList){
                    fileOut << temp.toUtf8() << "\t";
                }
                break;
            case 3 :
                foreach(QString temp, eventHeadList){
                    fileOut << temp.toUtf8() << "\t";
                }
                break;
            default:
                header = "出错了！！！";
            }
            fileOut << "\n";
            foreach (int temp, list) {
                for(int i = 0;i<colCount;i++){
                    fileOut << ui->tableView->model()->index(temp,i).data().toString().toUtf8() << "\t";
                }
                fileOut << "\n";
            }
            file.close();
        }
    }
}
