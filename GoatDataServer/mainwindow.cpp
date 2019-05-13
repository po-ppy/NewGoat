#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    runFlag = false;
    mSysTrayIcon = new QSystemTrayIcon(this);
    cmenu = new QMenu(this);

    db = QSqlDatabase::addDatabase("QMYSQL");
    server = new QTcpServer(this);
    clientList = new QList<QTcpSocket *>();
    threadList = new QList<WorkThread *>();

    showAction = cmenu->addAction("显示主界面");
    cmenu->addSeparator();
    exitAction = cmenu->addAction("退出");

    mSysTrayIcon->setIcon(QIcon(":/yang.png"));
    mSysTrayIcon->setToolTip("运动分析服务");
    mSysTrayIcon->setContextMenu(cmenu);
    mSysTrayIcon->show();

    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(activedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    connect(showAction,SIGNAL(triggered(bool)),this,SLOT(showWindow()));
    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(exitWindow()));
    connect(ui->initAction,SIGNAL(triggered(bool)),this,SLOT(initSQL()));
    connect(server,SIGNAL(newConnection()),this,SLOT(acceptConnect()));

    connect(&checkTimer,SIGNAL(timeout()),this,SLOT(checkConnect()));

    checkTimer.start(10000);


}

MainWindow::~MainWindow()
{
    delete ui;
    stopServer();
}

void MainWindow::on_startButton_clicked()
{
    startServer();

}

void MainWindow::on_closeButton_clicked()
{
    stopServer();
}

void MainWindow::startServer(){
    if(db.isOpen()){
        db.close();
    }
    db.setHostName(ui->serverIp->text().trimmed());
    db.setUserName(ui->serverUser->text().trimmed());
    db.setPassword(ui->serverPassword->text());
    db.setDatabaseName("goatdb");
    if(db.open()){
        QNetworkProxy proxy;
        QNetworkProxy::setApplicationProxy(proxy);
        quint16 port=8888;//端口号！！！！！！！！
        server->listen(QHostAddress::Any,port);
        if(server->isListening()){
            //emit dbOpenSiganl();
            disableInput();
            messageShow("服务正在运行","green");
            runFlag = true;
            for(int i = 0;i < threadList->size();i++){
                threadList->at(i)->setDB(db);
                threadList->at(i)->startThread();
                threadList->at(i)->moveToThread(&wthread);
                wthread.start();
            }
        }else{
            //监听服务未开启
            messageShow("监听服务启动失败，请检查8888端口是否占用","red");
            qDebug() << server->errorString();
        }

    }else{
        //数据库打开失败的处理
        messageShow("数据库打开失败","red");
        //qDebug() << "database open error";
    }
}

void MainWindow::stopServer(){
    runFlag = false;
    for(int i = 0; i < threadList->size();i++){
        threadList->at(i)->stopThread();
    }
    if(db.isOpen()){
        db.close();
    }
    enableInput();
}

void MainWindow::minimumWindow(){
    this->hide();
}

void MainWindow::activedSysTrayIcon(QSystemTrayIcon::ActivationReason reason){
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        //单击
        mSysTrayIcon->showMessage("帮助","这是奶山羊运动数据分析服务！",QSystemTrayIcon::Information,1500);
        break;
    case QSystemTrayIcon::DoubleClick:
        //双击
        showWindow();
        break;
    default:
        break;
    }
}

void MainWindow::showWindow(){
    this->show();
}

void MainWindow::exitWindow(){
    stopServer();
    for(int i = 0; i < clientList->length(); i++){
        clientList->at(i)->close();
    }
    if(server->isListening()){
        server->close();
    }
    exit(0);
}

void MainWindow::disableInput(){
    ui->serverIp->setDisabled(true);
    ui->serverPassword->setDisabled(true);
    ui->serverUser->setDisabled(true);
    ui->startButton->setDisabled(true);
    minimumWindow();
}

void MainWindow::enableInput(){
    ui->serverIp->setEnabled(true);
    ui->serverUser->setEnabled(true);
    ui->serverPassword->setEnabled(true);
    ui->startButton->setEnabled(true);
    ui->messageLabel->setText("服务已关闭");
    ui->messageLabel->setStyleSheet("color:red;");
}

void MainWindow::messageShow(QString msg, QString color){
    ui->messageLabel->setText(msg);
    ui->messageLabel->setStyleSheet("color:"+color+";");
}

void MainWindow::acceptConnect(){
    QTcpSocket *client = server->nextPendingConnection();
    clientList->append(client);
    WorkThread *tempThread = new WorkThread();
    tempThread->setDB(db);
    if(tempThread->setClient(client)){
        threadList->append(tempThread);
    }
    if(runFlag){  
        tempThread->setDB(db);
        tempThread->startThread();
        tempThread->moveToThread(&wthread);
        wthread.start();
    }

}

void MainWindow::checkConnect(){
    foreach (WorkThread* temp, *threadList) {
        if(!temp->clientIsOpen()){
            temp->stopThread();
            threadList->removeOne(temp);
        }
    }
    qDebug() << "current thread number: " << threadList->size();
    foreach(QTcpSocket* temp, *clientList){
//        qint64 writeNum = temp->write("1234");
//        if(writeNum < 1){
//            temp->close();
//        }
        if(!temp->isValid()){
            clientList->removeOne(temp);
        }
    }
    qDebug() << "current client number: " << clientList->size();
}


void MainWindow::initSQL(){
    //stopServer();
    QString cmd = QString("mysql -u%1 -p%2 -h%3").arg(ui->serverUser->text().trimmed(),ui->serverPassword->text(),ui->serverIp->text().trimmed());
    QString sqlFilePath = QString("%1/SQL/init.sql").arg(QCoreApplication::applicationDirPath());
    QProcess *poc = new QProcess();
    poc->setStandardInputFile(sqlFilePath);
    poc->start(cmd);
    if(!poc->isOpen()){
        //清除失败
    }
//    qDebug() << sqlFilePath;
//    if(!db.open()){

//    }
//    QSqlQuery query(db);
//    if(!query.exec(sqlFilePath)){
//        QSqlError err;
//      err= query.lastError();
//       qDebug() << err.text();
//        qDebug() << "error source";
//    }

}
