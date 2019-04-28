#include "otherdataform.h"
#include "ui_otherdataform.h"

OtherDataForm::OtherDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OtherDataForm)
{
    ui->setupUi(this);
    sqlQueryModel = new QSqlQueryModel(this);
    sortFilterProxyModel = new QSortFilterProxyModel(this);

    dataType = 0;
    preDataType = 2;

    keyWordMap.insert("奶山羊编号","c.goatId");
    keyWordMap.insert("舍号","houseId");
    keyWordMap.insert("饲料名称","feedName");
    keyWordMap.insert("饲喂量","feedLevel");
    keyWordMap.insert("饲喂时间","inTime");
    keyWordMap.insert("疫苗名称","vacineName");
    keyWordMap.insert("防疫时间","vacineTime");
    keyWordMap.insert("产品名称","productName");
    keyWordMap.insert("产量","yield");
    keyWordMap.insert("产出时间","outTime");
    keyWordMap.insert("产量备注","yieldRemark");
    keyWordMap.insert("饲喂备注","feedingRemark");
    keyWordMap.insert("防疫备注","antiepidemicRemark");



    ui->showAllCheckBox->setCheckState(Qt::Checked);
    ui->keyWordCheckBox->setCheckState(Qt::Unchecked);

    ui->startTimeCheckBox->hide();
    ui->startTimeDateTimeEdit->hide();
    ui->label_3->hide();
    ui->endTimeCheckBox->hide();
    ui->endTimeDateTimeEdit->hide();

}

OtherDataForm::~OtherDataForm()
{
    delete ui;
}

void OtherDataForm::on_showPushButton_clicked()
{
    updateTableView();
}

bool OtherDataForm::setDataType(int temp){
    dataType = temp;
    switch (dataType) {
    case 0 :
        ui->titleLabel->setText("饲喂信息");
        break;
    case 1 :
        ui->titleLabel->setText("防疫信息");
        break;
    case 2 :
        ui->titleLabel->setText("产量信息");
        break;
    default:
        dataType = 0;
        ui->titleLabel->setText("饲喂信息");
        return false;
    }
    ui->showAllCheckBox->setCheckState(Qt::Checked);
    ui->keyWordCheckBox->setCheckState(Qt::Unchecked);
    //updateKeyWord();
    return true;
}

void OtherDataForm::updateTableView(){

        QSqlQuery query;
        QString SQL_keyword = "";

        if(ui->keyWordCheckBox->isChecked()){
            switch (dataType) {
            case 0 :
                SQL_keyword = "select a.goatId as 奶山羊编号, c.houseId as 舍号, b.feedName as 饲料名称, a.feedLevel as 饲喂量, a.inTime as 饲喂时间, a.feedingRemark as 饲喂备注 from feedingData a join feedInfo b join goatInfo c on a.feedId = b.feedId and a.goatId = c.goatId where :thekey like :theword ;";
                break;
            case 1 :
                SQL_keyword = "select a.goatId as 奶山羊编号, c.houseId as 舍号, b.vacineName as 疫苗名称, a.inTime as 防疫时间, a.antiepidemicRemark as 防疫备注 from antiepidemicData a join vacineInfo b join goatInfo c on a.vacineId = b.vacineId and a.goatId = c.goatId where :thekey like :theword;";
                break;
            case 2:
                SQL_keyword = "select a.goatId as 奶山羊编号, c.houseId as 舍号, b.productName as 产品名称, a.yield as 产量, a.outTime as 产出时间, a.yieldRemark as 产量备注 from yieldData a join productInfo b join goatInfo c on a.productId = b.productId and a.goatId = c.goatId where :thekey like :theword;";
                break;
            default:
                dataType = 0;
                query.exec("select '类型出错，请联系软件开发者！';");
            }

            SQL_keyword.replace(":thekey",keyWordMap[ui->keyWordComboBox->currentText()]);

            query.prepare(SQL_keyword);
            query.bindValue(":theword","%"+ui->keyWordLineEdit->text()+"%");
//            qDebug() << ":theword --> " << ui->keyWordLineEdit->text();
            if(!query.exec()){
                QSqlError err;
                err= query.lastError();
                qDebug() << err.text();
            }else{
//                qDebug() << query.lastQuery();
            }
        }else{
            switch (dataType) {
            case 0 :
                query.exec("select a.goatId as 奶山羊编号, c.houseId as 舍号, b.feedName as 饲料名称, a.feedLevel as 饲喂量, a.inTime as 饲喂时间, a.feedingRemark as 备注 from feedingData a join feedInfo b join goatInfo c on a.feedId = b.feedId and a.goatId = c.goatId;");
                break;
            case 1 :
                query.exec("select a.goatId as 奶山羊编号, c.houseId as 舍号, b.vacineName as 疫苗名称, a.inTime as 防疫时间, a.antiepidemicRemark as 备注 from antiepidemicData a join vacineInfo b join goatInfo c on a.vacineId = b.vacineId and a.goatId = c.goatId;");
                break;
            case 2:
                query.exec("select a.goatId as 奶山羊编号, c.houseId as 舍号, b.productName as 产品名称, a.yield as 产量, a.outTime as 产出时间, a.yieldRemark as 备注 from yieldData a join productInfo b join goatInfo c on a.productId = b.productId and a.goatId = c.goatId;;");
                break;
            default:
                dataType = 0;
                query.exec("select '类型出错，请联系软件开发者！';");
            }
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
}

void OtherDataForm::autoUpdateTableView(){
    if(ui->tableView->model() == nullptr){
        updateTableView();
        updateKeyWord();
    }

    if(dataType != preDataType){
        updateTableView();
        updateKeyWord();
        preDataType = dataType;
    }
}

void OtherDataForm::updateKeyWord(){
    ui->keyWordComboBox->clear();
    for(int i = 0; i < ui->tableView->model()->columnCount(); i++){
        ui->keyWordComboBox->addItem(ui->tableView->model()->headerData(i,Qt::Horizontal).toString());
    }
}

void OtherDataForm::keyWordSearch(){

}

void OtherDataForm::on_showAllCheckBox_stateChanged(int arg1)
{
    switch (arg1) {
    case Qt::Unchecked :
        ui->keyWordCheckBox->setCheckState(Qt::Checked);
        break;
    case Qt::Checked :
        ui->keyWordCheckBox->setCheckState(Qt::Unchecked);
        updateTableView();
        break;
    }
}

void OtherDataForm::addOne(){
    sqlQueryModel->insertRow(sqlQueryModel->rowCount());
}

void OtherDataForm::on_startTimeDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    if(dateTime > ui->endTimeDateTimeEdit->dateTime()){
        ui->startTimeDateTimeEdit->setDateTime(ui->endTimeDateTimeEdit->dateTime());
        QMessageBox::warning(this,"时间错误","起始时间不可超过截止时间！！");

    }
}

void OtherDataForm::on_endTimeDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    if(dateTime < ui->startTimeDateTimeEdit->dateTime()){
        ui->endTimeDateTimeEdit->setDateTime(ui->startTimeDateTimeEdit->dateTime());
        QMessageBox::warning(this,"时间错误","截止时间不可晚于起始时间！！");
    }
}

void OtherDataForm::on_keyWordCheckBox_stateChanged(int arg1)
{
    switch (arg1) {
    case Qt::Unchecked :
        ui->showAllCheckBox->setCheckState(Qt::Checked);
        break;
    case Qt::Checked :
        ui->showAllCheckBox->setCheckState(Qt::Unchecked);
        updateTableView();
        break;
    }
}

void OtherDataForm::on_tableView_doubleClicked(const QModelIndex &index)
{
    ui->keyWordComboBox->setCurrentText(ui->tableView->model()->headerData(index.column(),Qt::Horizontal).toString());
    ui->keyWordLineEdit->setText(index.data().toString());
    ui->keyWordCheckBox->setCheckState(Qt::Checked);
    updateTableView();
}

void OtherDataForm::on_keyWordLineEdit_returnPressed()
{
    updateTableView();
}

void OtherDataForm::on_addButton_clicked()
{
    QMessageBox::warning(this,"火警","某舍发出火警信息，请管理员尽快查看，排除险情！！");
    addOne();
}

bool QAbstractItemModel::insertRows(int row, int count, const QModelIndex &parent){
    beginInsertRows(parent,row,row+count-1);

    endInsertRows();
    return  true;
}

