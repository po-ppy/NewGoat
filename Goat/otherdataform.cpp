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
    preDataType = 6;

    initMenu();

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
    keyWordMap.insert("舍号/路由编号","routerId");
    keyWordMap.insert("事件编号","a.eventId");
    keyWordMap.insert("事件含义","eventMeaning");
    keyWordMap.insert("记录时间","datatimem");
    keyWordMap.insert("发出设备","deviceId");
    keyWordMap.insert("处理状况","eventState");

    ui->showAllCheckBox->setCheckState(Qt::Checked);
    ui->keyWordCheckBox->setCheckState(Qt::Unchecked);

    addDataDialog = new AddDataDialog(this);

    connect(addDataDialog,SIGNAL(updateTab()),this,SLOT(updateTableView()));

    ui->startTimeCheckBox->hide();
    ui->startTimeDateTimeEdit->hide();
    ui->label_3->hide();
    ui->endTimeCheckBox->hide();
    ui->endTimeDateTimeEdit->hide();

//    player = new QMediaPlayer(this);


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
    case 3 :
        ui->titleLabel->setText("事件记录");
        break;
    default:
        dataType = 0;
        ui->titleLabel->setText("饲喂信息");
        return false;
    }
    ui->showAllCheckBox->setCheckState(Qt::Checked);
    ui->keyWordCheckBox->setCheckState(Qt::Unchecked);
    //updateKeyWord();
    if(dataType == 3){
        ui->addButton->setIcon(QIcon(":/duigou.png"));
        ui->addByHouseButton->hide();
        actionAdd->setText("处理标记");
    }else{
        addDataDialog->setDataType(dataType);
        ui->addButton->setIcon(QIcon(":/plus.png"));
        ui->addByHouseButton->show();
        actionAdd->setText("添加");
    }
    return true;
}

void OtherDataForm::updateTableView(){

        QSqlQuery query;
        QString SQL_keyword = "";

        if(ui->keyWordCheckBox->isChecked()){
            switch (dataType) {
            case 0 :
                SQL_keyword = "select a.id as ID, a.goatId as 奶山羊编号, c.houseId as 舍号, b.feedName as 饲料名称, a.feedLevel as 饲喂量, a.inTime as 饲喂时间, a.feedingRemark as 饲喂备注 from feedingData a join feedInfo b join goatInfo c on a.feedId = b.feedId and a.goatId = c.goatId where :thekey like :theword;";
                break;
            case 1 :
                SQL_keyword = "select a.id as ID, a.goatId as 奶山羊编号, c.houseId as 舍号, b.vacineName as 疫苗名称, a.inTime as 防疫时间, a.antiepidemicRemark as 防疫备注 from antiepidemicData a join vacineInfo b join goatInfo c on a.vacineId = b.vacineId and a.goatId = c.goatId where :thekey like :theword;";
                break;
            case 2:
                SQL_keyword = "select a.id as ID, a.goatId as 奶山羊编号, c.houseId as 舍号, b.productName as 产品名称, a.yield as 产量, a.outTime as 产出时间, a.yieldRemark as 产量备注 from yieldData a join productInfo b join goatInfo c on a.productId = b.productId and a.goatId = c.goatId where :thekey like :theword;";
                break;
            case 3 :
                SQL_keyword = "select a.id as ID,a.eventState as 处理状况, a.routerId as '舍号/路由编号', a.eventId as 事件编号, b.eventMeaning as 事件含义,from_unixtime(left(a.datatimem,10),'%Y-%m-%d %H:%i:%S') as 记录时间, a.deviceId as 发出设备 from eventData a join eventInfo b on a.eventId = b.eventId where :thekey like :theword;";
                break;
            default:
                dataType = 0;
                query.exec("select '类型出错，请联系软件开发者！';");
            }

            SQL_keyword.replace(":thekey",keyWordMap[ui->keyWordComboBox->currentText()]);

            query.prepare(SQL_keyword);
            query.bindValue(":theword","%"+ui->keyWordLineEdit->text().trimmed()+"%");
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
                query.exec("select a.id as ID, a.goatId as 奶山羊编号, c.houseId as 舍号, b.feedName as 饲料名称, a.feedLevel as 饲喂量, a.inTime as 饲喂时间, a.feedingRemark as 备注 from feedingData a join feedInfo b join goatInfo c on a.feedId = b.feedId and a.goatId = c.goatId;");
                break;
            case 1 :
                query.exec("select a.id as ID, a.goatId as 奶山羊编号, c.houseId as 舍号, b.vacineName as 疫苗名称, a.inTime as 防疫时间, a.antiepidemicRemark as 备注 from antiepidemicData a join vacineInfo b join goatInfo c on a.vacineId = b.vacineId and a.goatId = c.goatId;");
                break;
            case 2 :
                query.exec("select a.id as ID, a.goatId as 奶山羊编号, c.houseId as 舍号, b.productName as 产品名称, a.yield as 产量, a.outTime as 产出时间, a.yieldRemark as 备注 from yieldData a join productInfo b join goatInfo c on a.productId = b.productId and a.goatId = c.goatId;");
                break;
            case 3 :
                query.exec("select a.id as ID,a.eventState as 处理状况, a.routerId as '舍号/路由编号', a.eventId as 事件编号, b.eventMeaning as 事件含义, from_unixtime(left(a.datatimem,10),'%Y-%m-%d %H:%i:%S') as 记录时间, a.deviceId as 发出设备 from eventData a join eventInfo b on a.eventId = b.eventId;");
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
        ui->tableView->horizontalHeader()->setSortIndicator(0,Qt::DescendingOrder);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        int tempCount = ui->tableView->horizontalHeader()->count() - 1 ;
        for(int i = 0; i < tempCount;i++){
            ui->tableView->horizontalHeader()->setSectionResizeMode(i,QHeaderView::ResizeToContents);
        }
}

void OtherDataForm::showUntreatedEvent(){
    if(dataType != 3){
        this->dataType = 3;
        this->preDataType = 3;
        updateTableView();
    }
    ui->tableView->horizontalHeader()->setSortIndicator(0,Qt::DescendingOrder);
    ui->tableView->horizontalHeader()->setSortIndicator(1,Qt::DescendingOrder);
}

int OtherDataForm::hasUntreatedEvent(){
    QSqlQuery query;
    if(query.exec("select count(*) from eventData where eventState = '未处理';")){
        if(query.next()){
            return query.value(0).toInt();
        }
    }
    return -1;

}

void OtherDataForm::initCheckEvent(){
    int untreatedEventNum = hasUntreatedEvent();
    if(untreatedEventNum < 0){
        qDebug() << "hasUntreatedEvent() has error, the number is " << untreatedEventNum;
    }else if(untreatedEventNum > 0){
        int backInfo = QMessageBox::question(this,"信息提醒",QString("您有%1个事件仍未处理，是否查看?").arg(untreatedEventNum),QMessageBox::Ok,QMessageBox::Cancel);
        if(backInfo == QMessageBox::Ok){
            emit shouldShowUntreatedEvent();
        }
    }
    connect(&eventCheckTimer,SIGNAL(timeout()),this,SLOT(showAlert()));
    eventCheckTimer.start(10000);
}

void OtherDataForm::stopCheckEvent(){
    eventCheckTimer.stop();
    disconnect(&eventCheckTimer,SIGNAL(timeout()),this,SLOT(showAlert()));
}

void OtherDataForm::showAlert(){
    if(hasUntreatedEvent() < 1){
        return;
    }

    QSqlQuery query;
    if(query.exec("select * from eventData a left join eventInfo b on a.eventId= b.eventId where eventState = '未处理' order by id DESC limit 1;")){
        if(query.next()){
            QString message = QString("舍号/路由节点:%1\n事件:%2\n发送传感器:%3").arg(query.value("routerId").toString()).arg(query.value("eventMeaning").toString()).arg(query.value("deviceId").toString());
            int eventDataId = query.value("id").toInt();
            int backInfo = QMessageBox::warning(this,"警报",message,QMessageBox::Ok);
            if(backInfo == QMessageBox::Ok){
                query.prepare("update eventData set eventState = '已知晓' where id = :id;");
                query.bindValue(":id",eventDataId);
                if(!query.exec()){
//                    player->setMedia(QUrl::fromLocalFile("/home/poppy/Downloads/huozaijingbaosheng.mp3"));

//                    player->setVolume(100);
//                    player->play();
//                    qDebug() <<  player->errorString();

//                    QSoundEffect effect;
//                    effect.setSource(QUrl::fromLocalFile("/home/poppy/Downloads/huozaijingbaosheng.mp3"));
//                    effect.setLoopCount(2);
//                    effect.setVolume(1.0f);
//                    effect.play();
                    QMessageBox::warning(this,"警报","管理软件连接数据库出错!!");
                }else{
                    updateTableView();
                }
            }
        }
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
    if(dataType == 3){
        QModelIndexList tempList = ui->tableView->selectionModel()->selectedIndexes();
        if(tempList.size() > 0){
            int confirm = QMessageBox::question(this,"确认","确定已经处理过该事件吗?",QMessageBox::Yes,QMessageBox::No);
            if(confirm != QMessageBox::Yes){
                return;
            }
        }else{
            return;
        }

        QList<int> list;
        foreach(QModelIndex temp, tempList){
            if(!list.contains(temp.row())){
                list.append(temp.row());
            }
        }

        QSqlQuery query;
        query.prepare("update eventData set eventState = '已处理' where id = :id;");
        foreach (int temp, list) {
            query.bindValue(":id",ui->tableView->model()->index(temp,0).data().toInt());
            query.exec();
        }
        updateTableView();
    }else{
        addDataDialog->show();
    }

}

void OtherDataForm::deleteSelected(){
    QModelIndexList tempList = ui->tableView->selectionModel()->selectedIndexes();
    if(tempList.size() > 0){
        int confirm = QMessageBox::question(this,"确认","确定删除选中数据吗?",QMessageBox::Yes,QMessageBox::No);
        if(confirm != QMessageBox::Yes){
            return;
        }
    }else{
        return;
    }

    QList<int> list;
    foreach(QModelIndex temp, tempList){
        if(!list.contains(temp.row())){
            list.append(temp.row());
        }
    }


    QSqlQuery query;
    switch (dataType) {
    case 0 :
        query.prepare("delete from feedingData where id = :theId;");
        break;
    case 1 :
        query.prepare("delete from antiepidemicData where id = :theId;");
        break;
    case 2:
        query.prepare("delete from yieldData where id = :theId;");
        break;
    case 3:
        query.prepare("delete from eventData where id = :theId;");
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
//    QMessageBox::warning(this,"火警","某舍发出火警信息，请管理员尽快查看，排除险情！！");
    addOne();
}

bool QAbstractItemModel::insertRows(int row, int count, const QModelIndex &parent){
    beginInsertRows(parent,row,row+count-1);

    endInsertRows();
    return  true;
}

void OtherDataForm::initMenu(){
    cmenu = new QMenu(ui->tableView);
    actionAdd = cmenu->addAction("添加");
    actionRemove = cmenu->addAction("移除");

    connect(actionAdd,SIGNAL(triggered(bool)),this,SLOT(addOne()));
    connect(actionRemove,SIGNAL(triggered(bool)),this,SLOT(deleteSelected()));

}


void OtherDataForm::on_removeButton_clicked()
{
    deleteSelected();
}

void OtherDataForm::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    if(ui->tableView->selectionModel()->selectedIndexes().size() > 0){
        cmenu->exec(QCursor::pos());
    }
}
