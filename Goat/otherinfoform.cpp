#include "otherinfoform.h"
#include "ui_otherinfoform.h"

OtherInfoForm::OtherInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OtherInfoForm)
{
    ui->setupUi(this);
    sqlQueryModel = new QSqlQueryModel(this);
    sortFilterProxyModel = new QSortFilterProxyModel(this);


    preInfoType = 0;
    infoType = 0;

}

OtherInfoForm::~OtherInfoForm()
{
    delete ui;
}

void OtherInfoForm::on_addMoreButton_clicked()
{

}

void OtherInfoForm::updateTableView(){
    QSqlQuery query;
    switch (infoType) {
    case 0 :
        query.exec("select feedId as 饲料编号, feedName as 饲料名称, feedRange as 使用范围, feedUsage as 用法用量, feedRemark as 备注 from feedInfo;");
        break;
    case 1 :
        query.exec("select vacineId as 疫苗编号, vacineName as 疫苗名称, vacineType as 疫苗种类, vacineType as 疫病种类, vacineDose as 免疫剂量, vacinePart as 免疫部位,vacineRemark as 备注 from vacineInfo;");
        break;
    case 2:
        query.exec("select productId as 产品编号, productName as 产品名称, productRemark as 备注 from productInfo;");
        break;
    default:
        infoType = 0;
        query.exec("select '类型出错，请联系软件开发者！';");
    }

    sqlQueryModel->setQuery(query);

    sortFilterProxyModel->setDynamicSortFilter(true);
    sortFilterProxyModel->setSourceModel(sqlQueryModel);

    ui->tableView->setModel(sortFilterProxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    int tempCount = ui->tableView->horizontalHeader()->count() - 1 ;
    for(int i = 0; i < tempCount;i++){
        ui->tableView->horizontalHeader()->setSectionResizeMode(i,QHeaderView::ResizeToContents);
    }
    //ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
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
    default:
        infoType = 0;
        ui->titleLabel->setText("饲料信息");
        return false;
    }
    return true;
}

void OtherInfoForm::on_addButton_clicked()
{

    int rowNum = ui->tableView->model()->rowCount();
    int colNum = ui->tableView->model()->columnCount();
//    qDebug() << "rowNum "  << rowNum;
//    qDebug() << "colNum " << colNum;
//    for(int i = 0; i < colNum; i++){

//    }

    QModelIndex temp;
    temp.data() = "hello";
    ui->tableView->model()->insertRow(rowNum);
    qDebug() << ui->tableView->model()->index(rowNum,0).isValid();


}

void OtherInfoForm::on_removeButton_clicked()
{
    deleteSelected();
}

void OtherInfoForm::deleteSelected(){

    QModelIndexList tempList = ui->tableView->selectionModel()->selectedIndexes();
    if(tempList.size() > 0){
        int confirm = QMessageBox::question(this,"确认","确定删除选中设备吗?",QMessageBox::Yes,QMessageBox::No);
        if(confirm != QMessageBox::Yes){
            return;
        }
    }
    QList<int> list;
    foreach(QModelIndex temp, tempList){
        if(!list.contains(temp.row())){
            list.append(temp.row());
        }
    }

    QSqlQuery query;
    switch (infoType) {
    case 0 :
        query.prepare("delete from feedInfo where feedId = :theId;");
        break;
    case 1 :
        query.prepare("delete from vacineInfo where vacineId = :theId;");
        break;
    case 2:
        query.prepare("delete from productInfo where productId = :theId;");
        break;
    }
    //query.prepare("delete from deviceInfo where deviceId = :deviceId;");
    foreach (int temp, list) {
        query.bindValue(":theId",ui->tableView->model()->index(temp,0).data().toString());
        query.exec();
        //qDebug() << ui->tableView->model()->index(temp,0).data().toString();
    }
//    emit updateSignal();
    updateTableView();
}

bool QAbstractItemModel::insertRows(int row, int count, const QModelIndex &parent){
    beginInsertRows(parent,row,row+count-1);

    endInsertRows();
    return  true;
}
