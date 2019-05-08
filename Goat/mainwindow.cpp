#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loginDialog = new LoginDialog(this);
    helpDialog = new HelpDialog(this);

    sportDataForm = new SportDataForm(this);
    goatQueryForm = new GoatQueryForm(this);
    deviceQueryForm = new DeviceQueryForm(this);
    otherInfoForm = new OtherInfoForm(this);
    otherDataForm = new OtherDataForm(this);

    //ui->action_feed->setCheckable(true);

    bdDialog = new bindingDialog(this);
    houseBindingDialog = new HouseBindingDialog(this);

    ui->stackedWidget->addWidget(sportDataForm);
    ui->stackedWidget->addWidget(goatQueryForm);
    ui->stackedWidget->addWidget(deviceQueryForm);
    ui->stackedWidget->addWidget(otherInfoForm);
    ui->stackedWidget->addWidget(otherDataForm);

    this->setWindowTitle("奶山羊行为检测与信息管理系统");

    ui->stackedWidget->setCurrentWidget(sportDataForm);

    connect(ui->actionD,SIGNAL(triggered(bool)),this,SLOT(change_to_goat_query_form()));
    connect(ui->action_3,SIGNAL(triggered(bool)),this,SLOT(change_to_sport_data_form()));
    connect(ui->action_Device_Query,SIGNAL(triggered(bool)),this,SLOT(change_to_device_query_form()));

    connect(ui->action_feed,SIGNAL(triggered(bool)),this,SLOT(change_to_info_feed()));
    connect(ui->action_vacine,SIGNAL(triggered(bool)),this,SLOT(change_to_info_vacine()));
    connect(ui->action_product,SIGNAL(triggered(bool)),this,SLOT(change_to_info_product()));

    connect(ui->action_feeding,SIGNAL(triggered(bool)),this,SLOT(change_to_data_feeding()));
    connect(ui->action_antiepidemic,SIGNAL(triggered(bool)),this,SLOT(change_to_data_antiepidemic()));
    connect(ui->action_yield,SIGNAL(triggered(bool)),this,SLOT(change_to_data_yield()));
    connect(ui->action,SIGNAL(triggered(bool)),bdDialog,SLOT(show()));
    connect(ui->action_houseBinding,SIGNAL(triggered(bool)),houseBindingDialog,SLOT(show()));

    connect(goatQueryForm,SIGNAL(updateSignal()),this,SLOT(updateAllTables()));
    connect(deviceQueryForm,SIGNAL(updateSignal()),this,SLOT(updateAllTables()));
    connect(bdDialog,SIGNAL(updateSignal()),this,SLOT(updateAllTables()));
    connect(houseBindingDialog,SIGNAL(updateSignal()),this,SLOT(updateAllTables()));
    //connect(ui->action_2,SIGNAL(triggered(bool)),this,SLOT(change_to_sport_data_form()));

    connect(goatQueryForm,SIGNAL(goatIdSignal(QString)),bdDialog,SLOT(receiveGoatId(QString)));
    connect(deviceQueryForm,SIGNAL(deviceIdSignal(QString)),bdDialog,SLOT(receiveDeviceId(QString)));

    connect(ui->actionLogin,SIGNAL(triggered(bool)),loginDialog,SLOT(show()));
    connect(loginDialog,SIGNAL(loginSignal()),this,SLOT(loginOK()));
    connect(ui->actionLogout,SIGNAL(triggered(bool)),this,SLOT(doLogout()));
    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(doExit()));
    connect(ui->actionHelp,SIGNAL(triggered(bool)),helpDialog,SLOT(show()));
    setWindowIcon(QIcon(":/nu.ico"));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::change_to_goat_query_form(){
    ui->stackedWidget->setCurrentWidget(goatQueryForm);
}

void MainWindow::change_to_sport_data_form(){
    ui->stackedWidget->setCurrentWidget(sportDataForm);
}

void MainWindow::change_to_device_query_form(){
    ui->stackedWidget->setCurrentWidget(deviceQueryForm);
}

void MainWindow::change_to_other_info_form(){
    ui->stackedWidget->setCurrentWidget(otherInfoForm);
}

void MainWindow::change_to_other_data_form(){
    ui->stackedWidget->setCurrentWidget(otherDataForm);
}

void MainWindow::change_to_info_feed(){
    otherInfoForm->setInfoType(0);
    otherInfoForm->autoUpdateTableView();
    change_to_other_info_form();
}

void MainWindow::change_to_info_vacine(){
    otherInfoForm->setInfoType(1);
    otherInfoForm->autoUpdateTableView();
    change_to_other_info_form();
}

void MainWindow::change_to_info_product(){
    otherInfoForm->setInfoType(2);
    otherInfoForm->autoUpdateTableView();
    change_to_other_info_form();
}

void MainWindow::change_to_data_feeding(){
    otherDataForm->setDataType(0);
    otherDataForm->autoUpdateTableView();
    change_to_other_data_form();
}

void MainWindow::change_to_data_antiepidemic(){
    otherDataForm->setDataType(1);
    otherDataForm->autoUpdateTableView();
    change_to_other_data_form();
}

void MainWindow::change_to_data_yield(){
    otherDataForm->setDataType(2);
    otherDataForm->autoUpdateTableView();
    change_to_other_data_form();
}

void MainWindow::updateAllTables(){
    goatQueryForm->refreshView();
    deviceQueryForm->refreshView();
    bdDialog->updateGoatTable();
    bdDialog->updateDeviceTable();
    houseBindingDialog->updateHouseTable();
    houseBindingDialog->updateDeviceTable();
    sportDataForm->updateTabs();
}

void MainWindow::loginOK(){
    ui->actionLogin->setText("已登录");
    ui->actionLogin->setDisabled(true);
    updateAllTables();
    otherInfoForm->autoUpdateTableView();
    otherDataForm->autoUpdateTableView();
}

void MainWindow::doLogout(){
    DB::instance().data()->getDb().close();
    ui->actionLogin->setText("登录");
    ui->actionLogin->setEnabled(true);
}

void MainWindow::doExit(){
    doLogout();
    this->close();
}
