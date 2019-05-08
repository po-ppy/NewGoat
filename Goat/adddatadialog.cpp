#include "adddatadialog.h"
#include "ui_adddatadialog.h"

AddDataDialog::AddDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDataDialog)
{
    ui->setupUi(this);

    dataType = 0;
    preType = 3;
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->houseIdCheckBox->setCheckState(Qt::Checked);

}

AddDataDialog::~AddDataDialog()
{
    delete ui;
}

bool AddDataDialog::setDataType(int temp){
    dataType = temp;
    if(preType != dataType){
        preType = dataType;
        initGui();
    }
    return dataType == temp;
}

bool AddDataDialog::initGui(){
    ui->levelLabel->show();
    ui->levelLineEdit->show();
    switch (dataType) {
    case 0 :
        ui->titleLabel->setText("添加饲喂信息");
        ui->levelLabel->setText("饲喂量");
        ui->infoIdLabel->setText("饲料编号");
        ui->dateTimeLabel->setText("饲喂时间");
        break;
    case 1 :
        ui->titleLabel->setText("添加防疫信息");
        ui->infoIdLabel->setText("疫苗编号");
        ui->dateTimeLabel->setText("防疫时间");
        ui->levelLabel->hide();
        ui->levelLineEdit->hide();
        break;
    case 2 :
        ui->titleLabel->setText("添加产量信息");
        ui->levelLabel->setText("产量");
        ui->infoIdLabel->setText("产品编号");
        ui->dateTimeLabel->setText("产出时间");
        break;
    default:
        dataType = 0;
        ui->titleLabel->setText("软件运行出错，请重启软件或联系管理员！！");
        return false;
    }
    initIds();
    initKeyWord();
    return true;
}

bool AddDataDialog::initKeyWord(){
    QSqlQuery query;
    switch (dataType) {
    case 0 :
        query.exec("select feedName, feedId from feedInfo;");
        break;
    case 1 :
//        ui->titleLabel->setText("添加防疫信息");
        query.exec("select vacineName, vacineId from vacineInfo;");
        break;
    case 2 :
//        ui->titleLabel->setText("添加产量信息");
        query.exec("select productName, productId from productInfo;");
        break;
    default:
        dataType = 0;
        ui->titleLabel->setText("软件运行出错，请重启软件或联系管理员！！");
        return false;
    }
    if(query.size() > 0){
        keyWordMap.clear();
        ui->infoNameComboBox->clear();
        while (query.next()) {
            keyWordMap.insert(query.value(0).toString(),query.value(1).toString());
            ui->infoNameComboBox->addItem(query.value(0).toString());
        }
        ui->infoNameComboBox->setCurrentText(keyWordMap.firstKey());
        ui->infoIdShowLabel->setText(keyWordMap.first());
    }else{
        return false;
    }
    return true;
}

bool AddDataDialog::initIds(){
    QSqlQuery query ;
    query.exec("select goatId from goatInfo;");
    ui->goatIdComboBox->clear();
    while(query.next()){
        ui->goatIdComboBox->addItem(query.value(0).toString());
    }

    query.exec("select houseId from houseInfo;");
    ui->houseIdComboBox->clear();
    while(query.next()){
        ui->houseIdComboBox->addItem(query.value(0).toString());
    }
    return true;
}

void AddDataDialog::on_houseIdCheckBox_stateChanged(int arg1)
{
    switch (arg1) {
    case Qt::Unchecked :
        ui->goatIdCheckBox->setCheckState(Qt::Checked);
        ui->goatIdLabel->setEnabled(true);
        ui->goatIdComboBox->setEnabled(true);
        ui->houseIdLabel->setDisabled(true);
        ui->houseIdComboBox->setDisabled(true);
        break;
    case Qt::Checked :
        ui->goatIdCheckBox->setCheckState(Qt::Unchecked);
        ui->goatIdLabel->setEnabled(false);
        ui->goatIdComboBox->setEnabled(false);
        ui->houseIdLabel->setDisabled(false);
        ui->houseIdComboBox->setDisabled(false);
        break;
    }
}

void AddDataDialog::on_goatIdCheckBox_stateChanged(int arg1)
{
    switch (arg1) {
    case Qt::Unchecked :
        ui->houseIdCheckBox->setCheckState(Qt::Checked);
        break;
    case Qt::Checked :
        ui->houseIdCheckBox->setCheckState(Qt::Unchecked);
        break;
    }
}

void AddDataDialog::on_infoNameComboBox_currentIndexChanged(const QString &arg1)
{
    ui->infoIdShowLabel->setText(keyWordMap[arg1]);
}

void AddDataDialog::addFun(){
    QSqlQuery query;
    QString SQL_add = "";
    switch (dataType) {
    case 0 :
        SQL_add = "insert into feedingData(goatId,feedId,feedLevel,inTime,feedingRemark) values(:goatId,:infoId,:level,:datetime,:remark);";
        break;
    case 1 :
        SQL_add = "insert into antiepidemicData(goatId,vacineId,inTime,antiepidemicRemark) values(:goatId,:infoId,:datetime,:remark);";
        break;
    case 2 :
        SQL_add = "insert into yieldData(goatId,productId,yield,outTime,yieldRemark) values(:goatId,:infoId,:level,:datetime,:remark);";
        break;
    default:
        ui->remarkTextEdit->setText("程序出错！！！！");
    }

    QList<QString> goatIds;
    goatIds.clear();
    if(ui->houseIdCheckBox->isChecked()){
        query.prepare("select goatId from goatInfo where houseId = :houseId ;");
        query.bindValue(":houseId",ui->houseIdComboBox->currentText());
        query.exec();
        while(query.next()){
            goatIds.append(query.value(0).toString());
        }
    }

    query.prepare(SQL_add);
    query.bindValue(":infoId",ui->infoIdShowLabel->text());
    if(dataType != 1){
        query.bindValue(":level",ui->levelLineEdit->text());
    }
    query.bindValue(":datetime",ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    qDebug() << ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    query.bindValue(":remark",ui->remarkTextEdit->toPlainText());

    if(ui->goatIdCheckBox->isChecked()){
        query.bindValue(":goatId",ui->goatIdComboBox->currentText());
        qDebug() << ui->goatIdComboBox->currentText();
        if(query.exec()){
            QMessageBox::information(this,"添加成功","新数据添加成功！！");
        }else{
            QSqlError err;
            err= query.lastError();
            qDebug() << err.text();
            QMessageBox::warning(this,"添加失败","新数据添加失败！！");
        }
    }else{

        QSqlDatabase::database().transaction();
        foreach (QString temp, goatIds) {
            query.bindValue(":goatId",temp);
            query.exec();
        }
        QSqlDatabase::database().commit();
    }
    emit updateTab();
}

void AddDataDialog::on_pushButton_clicked()
{
    addFun();
    this->hide();
}
