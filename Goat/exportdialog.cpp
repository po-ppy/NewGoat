#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{

    ui->setupUi(this);
    ui->checkBox_chooseAll->setTristate(true);
    connect(ui->buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),[=](QAbstractButton *button){chechIfChooseAll();});

    map.insert("羊舍设备信息","houseDeviceInfo");
    map.insert("产量记录","yieldData");
    map.insert("饲料信息","feedInfo");
    map.insert("防疫记录","antiepidemicData");
    map.insert("羊舍与设备绑定信息","houseBindingInfo");
    map.insert("环境数据记录","houseData");
    map.insert("事件记录","eventData");
    map.insert("事件信息","eventInfo");
    map.insert("羊舍信息","houseInfo");
    map.insert("疫苗信息","vacineInfo");
    map.insert("奶山羊设备信息","deviceInfo");
    map.insert("奶山羊与设备绑定信息","bindingInfo");
    map.insert("奶山羊基本信息","goatInfo");
    map.insert("产品信息","productInfo");
    map.insert("饲喂记录","feedingData");
//    map.insert("");

    process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus){ exportEndState(exitCode, exitStatus);});
    exportFlag = true;
    initFlag = false;
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::chechIfChooseAll(){
    bool isAll = true;
    bool isZero = true;
    foreach (QAbstractButton* temp, ui->buttonGroup->buttons()) {
        if(temp->isChecked()){
            isZero = false;
        }else{
            isAll = false;
        }
    }
    if(isAll){
        ui->checkBox_chooseAll->setCheckState(Qt::Checked);
    }else if(isZero){
        ui->checkBox_chooseAll->setCheckState(Qt::Unchecked);
    }else{
        ui->checkBox_chooseAll->setCheckState(Qt::PartiallyChecked);
    }
}

void ExportDialog::chooseAll(){
    foreach (QAbstractButton* temp, ui->buttonGroup->buttons()) {
        temp->setChecked(true);
    }
}

void ExportDialog::chooseZero(){
    foreach (QAbstractButton* temp, ui->buttonGroup->buttons()) {
        temp->setChecked(false);
    }
}

void ExportDialog::on_checkBox_chooseAll_clicked()
{
    switch (ui->checkBox_chooseAll->checkState()) {
    case Qt::Checked :
        chooseAll();
        break;
    case Qt::Unchecked :
        chooseZero();
        break;
    }
}

void ExportDialog::doExport(){
    exportFlag = true;
    initFlag = false;
    if(!ui->checkBox_chooseAll->isChecked()){
        QMessageBox::warning(this,"警告","请先选择要导出的数据!!");
        return;
    }
    if(!DB::instance().data()->getDb().isOpen()){
        QMessageBox::warning(this,"警告","请先登录!");
        return;
    }

    qDebug() << getChoosed();

    QString fileName = QFileDialog::getSaveFileName(this,"选择",QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"/backup.sql","SQL (*.sql)");

    if(fileName.isNull() || fileName.isEmpty()){
        return;
    }
    qDebug() << fileName;



//    connect();
//    process->setProgram("mysqldump");
//    //mysqldump -uroot -proot goatdb houseInfo goatinfo > test.sql
//    QList<QString> argList;
//    argList << "-u" << DB::instance().data()->getDb().userName();
//    argList << "-p"+DB::instance().data()->getDb().password();
//    qDebug() << DB::instance().data()->getDb().password();
//    argList << "goatdb" << getChoosed();
////    argList << ">" << fileName;
//    qDebug() << argList;
//    process->setArguments(argList);
//    process->setStandardOutputFile(fileName);
//    process->start();
//    process->

    QString arglist = "";
    arglist += " -u" + DB::instance().data()->getDb().userName();
    arglist += " -p" + DB::instance().data()->getDb().password();
    arglist += " goatdb ";
    if(ui->checkBox_chooseAll->checkState() == Qt::PartiallyChecked){
        arglist += getChoosed();
    }
//    arglist += " > " + fileName;
    QString cmd = "mysqldump"+arglist;
    qDebug() << cmd;
    process->setStandardOutputFile(fileName);
    process->start(cmd);
//    process->terminate();
//    process->kill();



#ifdef Q_OS_LINUX
    qDebug() << "now in linux!!!!!!!!!!!";

#else
    qDebug() << "now in windows!!!!!!!!!!!";
#endif
}

bool ExportDialog::exportEndState(int exitCode, QProcess::ExitStatus exitStatus){
    qDebug() << "exitCode is " << exitCode;
    switch (exitStatus) {
    case QProcess::NormalExit :
        if(initFlag){
            QMessageBox::information(this,"成功","初始化/清除成功!");
        }else{
            if(exportFlag){
                QMessageBox::information(this,"成功","导出成功!");
            }else{
                QMessageBox::information(this,"成功","导入成功!");
            }
        }
        process->terminate();
        return true;
        break;
    case QProcess::CrashExit :
        if(initFlag){
            QMessageBox::warning(this,"警告",QString("初始化/清除失败!\nexitCode: %1.").arg(exitCode));
        }else{
            if(exportFlag){
                QMessageBox::warning(this,"警告",QString("导出失败!\nexitCode: %1.").arg(exitCode));
            }else{
                QMessageBox::warning(this,"警告",QString("导入失败!\nexitCode: %1.").arg(exitCode));
            }
        }
        process->terminate();
        return false;
        break;
    }
}

QString ExportDialog::getChoosed(){
    QString result = " ";
    foreach (QAbstractButton* temp, ui->buttonGroup->buttons()) {
        if(temp->isChecked()){
            result += map[temp->text()] + " ";
        }
    }
    return result;
}

void ExportDialog::doImport(){
    exportFlag = false;
    initFlag = false;
    if(!DB::instance().data()->getDb().isOpen()){
        QMessageBox::warning(this,"警告","请先登录!");
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,"选择",QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"/backup.sql","SQL (*.sql)");
    if(fileName.isNull() || fileName.isEmpty()){
        return;
    }
    qDebug() << fileName;

    QString arglist = "";
    arglist += " -u" + DB::instance().data()->getDb().userName();
    arglist += " -p" + DB::instance().data()->getDb().password();
    arglist += " goatdb ";
//    arglist += " > " + fileName;
    QString cmd = "mysql"+arglist;
    qDebug() << cmd;
    process->setStandardInputFile(fileName);
//    process->setStandardOutputFile(fileName);
    process->start(cmd);
}

void ExportDialog::doInit(){
    if(!DB::instance().data()->getDb().isOpen()){
        QMessageBox::warning(this,"警告","请先登录!");
        return;
    }
    QMessageBox::information(this,"提示","初始化已开始，请耐心等待!");
    initFlag = true;
    QString arglist = "";
    arglist += " -u" + DB::instance().data()->getDb().userName();
    arglist += " -p" + DB::instance().data()->getDb().password();
    arglist += " goatdb ";
//    arglist += " > " + fileName;
    QString cmd = "mysql"+arglist;
    qDebug() << cmd;

    process->setStandardInputFile("SQL/init.sql");
    if(!QFile::exists("SQL/init.sql")){
        QMessageBox::warning(this,"警告","未找到inti.sql!\n请重新安装软件。");
    }
//    process->setStandardOutputFile(fileName);
    process->start(cmd);
}

void ExportDialog::on_pushButton_export_clicked()
{
    doExport();
}
