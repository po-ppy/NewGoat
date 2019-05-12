#include "workthread.h"

WorkThread::WorkThread()
{
    runFlag = true;
    msgcount = 0;
}

WorkThread::~WorkThread(){
    qDebug() << "gg??";
    stopThread();
    //quit();
    //wait();
}

void WorkThread::run(){
    qDebug() << runFlag;
    //while(runFlag){
     //   dataProcessing();
    //}
}

void WorkThread::startThread(){
    runFlag = true;
    connect(client,SIGNAL(readyRead()),this,SLOT(dataProcessing3()));
//    connect(client,SIGNAL(disconnected()),this,SLOT(stopThread()));
    this->start();
}

void WorkThread::stopThread(){
    //qDebug() << "stop ???";
    runFlag = false;
    disconnect(client,SIGNAL(readyRead()),this,SLOT(dataProcessing3()));
    this->db.close();
    this->quit();
    this->wait();
}

void WorkThread::dataProcessing(){
    //qDebug() << "start dataProcessing!";
    //QSqlQuery query(db);
    //if (runFlag && client->isReadable()) {
       // 数据处理
        //qDebug() << "inter!!";
        QByteArray receiveInfo = client->readAll().trimmed().replace("\n","").replace('\x01',"");
        //qDebug() << "is there nothing?";
        //qDebug() << receiveInfo;
        QList<QByteArray> tempDataList = receiveInfo.split('$');
        tempDataList.removeAll(" ");
        tempDataList.removeAll("");
        int allLength = tempDataList.length();
        qDebug() << "alllength: " << allLength;
        int len = 0;
        while(len+20 < allLength){
            DataPorcessingThread tempThread(tempDataList.mid(len,5));
            len += 20;
            tempThread.setObjectName(QString::number(len));
            tempThread.setDB(this->db);
            tempThread.moveToThread(&wthread);
            tempThread.start();
        }
        DataPorcessingThread tempThread(tempDataList.mid(len));
        tempThread.setObjectName(QString::number(len));
        tempThread.setDB(this->db);
        tempThread.moveToThread(&wthread);
        tempThread.start();
        //wthread.start();

//        for(int i = 0;i < tempDataList.length();i++){
//            QList<QByteArray> datalist = tempDataList.at(i).split('*');
//            datalist.removeAll(" ");
//            datalist.removeAll("");
//            // qDebug() << datalist.size();
//            if(datalist.size() < 4){
//                return;
//            }
//            // qDebug() << "datalist.size=" ;
//            // qDebug() << datalist.size();
//            for(int k = 0; k < datalist.size();k++){
//                switch (k) {
//                case 0:
//                    receiveTime = datalist.at(0).trimmed();
//                    break;
//                case 1:
//                    temperature = datalist.at(1).trimmed();
//                    break;
//                case 2:
//                    humidity = datalist.at(2).trimmed();
//                    break;
//                case 3:
//                {
//                    QList<QByteArray> goats = datalist.at(3).trimmed().split('#');
//                    //qDebug() << goats.size();
//                    for(int i = 0;i < goats.size();i++){
//                        //qDebug() <<goats.at(i);
//                        QList<QByteArray> tempgoat = goats.at(i).trimmed().split(' ');
//                        tempgoat.removeAll(" ");
//                        tempgoat.removeAll("");
//                        if(tempgoat.size() != 7){
//                            continue;
//                        }
//                        //qDebug() << "tempgot.size = ";
//                        //qDebug() << tempgoat.size();
//                        query.prepare("insert into sportData(goatid,datatimem,sportx,sporty,sportz,anglex,angley,anglez,status) values(?,?,?,?,?,?,?,?,?)");
//                        //query.bindValue(1,QDateTime::fromMSecsSinceEpoch(receiveTime.toLongLong()));
//                        query.bindValue(1,receiveTime);
//                        query.bindValue(8,"unknown");
//                        for(int j = 0 ;j < tempgoat.size();j++){
//                            if(j == 0){
//                                query.bindValue(0,tempgoat.at(0));
//                                //qDebug() << tempgoat.at(0);
//                            }else{
//                                query.bindValue(j+1,tempgoat.at(j));
//                                // qDebug() << tempgoat.at(j) ;
//                                // qDebug() << "null???";
//                                // qDebug() << j;
//                            }
//                        }
//                        if(!query.exec()){
//                             qDebug() << "数据库插入失败！";
//                            qDebug() << query.lastQuery();
//                              QSqlError err;
//                            err= query.lastError();
//                             qDebug() << err.text();
//                        }else{
//                            //if(i == goats.size()-1){
//                            if(msgcount == 5){
//                                //QString temp = receiveTime+"#"+temperature+"#"+humidity;
//                                //QString temp = receiveTime+"#"+"23.4"+"#"+"45.6";
//                                //qDebug() << temp;
//                                query.prepare("insert into houseData(houseId,datatimem,wendu,anqi) values(:houseId,:datatimem,:wendu,:anqi);");
//                                query.bindValue(":houseId",tempgoat.at(0).trimmed());
//                                query.bindValue(":datatimem",receiveTime);
//                                query.bindValue(":wendu",temperature);
//                                query.bindValue(":anqi",humidity);
//                                //qDebug() << temperature;
//                                if(!query.exec()){
//                                    qDebug() << "houseData数据库插入失败！";
//                                   qDebug() << query.lastQuery();
//                                     QSqlError err;
//                                   err= query.lastError();
//                                    qDebug() << err.text();
//                                }
//                                //emit saveOk(temp);
//                            }

//                        }
//                    }

//                }
//                    if(msgcount == 5){
//                        msgcount = 0;
//                    }else{
//                        msgcount++;
//                    }

//                    break;
//                default:
//                    qDebug() << "有些意料之外的事发生了,数据分拣!";
//                    break;
//                }
//            }
//        }

   // }
}

void WorkThread::dataProcessing2(){
    // 数据处理
     //qDebug() << "inter!!";
     QByteArray receiveInfo = client->readAll().trimmed().replace("\n","").replace('\x01',"");
     //qDebug() << "is there nothing?";
     //qDebug() << receiveInfo;
     QList<QByteArray> tempDataList = receiveInfo.split('$');
     tempDataList.removeAll(" ");
     tempDataList.removeAll("");
     int allLength = tempDataList.length();
     qDebug() << allLength;
     int len = 0;
     while(len+5 < allLength){
         DataPorcessingThread tempThread(tempDataList.mid(len,5));
         len += 5;
         tempThread.setObjectName(QString::number(len));
         tempThread.setDB(this->db);
         tempThread.start();
     }
     DataPorcessingThread tempThread(tempDataList.mid(len));
     tempThread.setObjectName(QString::number(len));
     tempThread.setDB(this->db);
     tempThread.start();
}

void WorkThread::dataProcessing3(){
   // msleep(800);
    QTime tempTime;
    tempTime.start();
    // 数据处理
     //qDebug() << "inter!!";
     QByteArray receiveInfo = client->readAll().trimmed().replace("\n","").replace("\x01","");
     //qDebug() << "is there nothing?";
     qDebug() << receiveInfo;
     QList<QByteArray> tempDataList = receiveInfo.split('*');
     tempDataList.removeAll(" ");
     tempDataList.removeAll("");
     int allLength = tempDataList.length();
     qDebug() << "allLength: " << allLength;
     if(allLength < 1){
         return;
     }
//     int len = 0;
//     while(len+100 < allLength){

//         QtConcurrent::run(this,&WorkThread::testData,tempDataList.mid(len,5),QSqlDatabase::cloneDatabase(this->db,QString::number(QDateTime::currentMSecsSinceEpoch()+tempTime.elapsed())));
////         QtConcurrent::run(this,&WorkThread::testData,tempDataList.mid(len,5),this->db);
//         len += 100;
//         msleep(5);
//     }
//     msleep(5);
//     QtConcurrent::run(this,&WorkThread::testData,tempDataList.mid(len),QSqlDatabase::cloneDatabase(this->db,QString::number(QDateTime::currentMSecsSinceEpoch()+tempTime.elapsed())));
////     QtConcurrent::run(this,&WorkThread::testData,tempDataList.mid(len),this->db);
     QtConcurrent::run(this,&WorkThread::testData,tempDataList,this->db);
//        QtConcurrent::run(this,&WorkThread::testData,tempDataList,QSqlDatabase::cloneDatabase(this->db,QString::number(QDateTime::currentMSecsSinceEpoch()+tempTime.elapsed())));
}

void WorkThread::setDB(QSqlDatabase &inDB){
    this->db = QSqlDatabase::cloneDatabase(inDB,QString::number(QDateTime::currentMSecsSinceEpoch()));
    this->db.open();
}

bool WorkThread::setClient(QTcpSocket *inClient){
    if(inClient->isOpen()){
        return this->client = inClient;
    }
    return false;
}

bool WorkThread::clientIsOpen(){
    //return this->client->isOpen();
    return this->client->isValid();
}

void WorkThread::dataPro(QList<QByteArray> todoList,QSqlDatabase &inDB){
    qDebug() << "this is in dataPro();";
    todoList.removeAll("");
    todoList.removeAll(" ");
    if(todoList.size() != 11){
        qDebug() << "环境数据个数不为11";
        return;
    }

//    if(inDB.open()){
//        inDB.transaction();
//    }else{
//        qDebug() << "in dataPro(),--> db not open!";
//        return;
//    }
    QSqlQuery query(inDB);
    query.prepare("insert into houseData(houseId,datatimem,wendu,shidu,eryang,anqi,guangzhao,pm25,pm10,yanwu) values(:temp1,:temp2,:temp7,:temp8,:temp6,:temp5,:temp9,:temp3,:temp4,:temp10);");
    for(int i = 1;i < 11; i++){
        query.bindValue(":temp"+QString::number(i),todoList.at(i));
    }
    if(!query.exec()){
        qDebug() << "dataPro() insert error!";
        QSqlError err;
        err= query.lastError();
        qDebug() << err.text();


    }
//    if(!inDB.commit()){
//        qDebug() << "faill to commit!!";
//        inDB.rollback();
//    }
//    inDB.close();
}

void WorkThread::sportPro(QList<QByteArray> todoList,QSqlDatabase &inDB){
    qDebug() << "this is in sportPro();";

}

void WorkThread::eventPro(QList<QByteArray> todoList,QSqlDatabase &inDB){
    qDebug() << "this is in eventPro();";
    todoList.removeAll("");
    todoList.removeAll(" ");
    if(todoList.size() != 5){
        qDebug() << "事件信息个数不为5";
        return;
    }

//    if(inDB.open()){
//        inDB.transaction();
//    }else{
//        qDebug() << "in dataPro(),--> db not open!";
//        return;
//    }
    QSqlQuery query(inDB);
    query.prepare("insert into eventData(routerId,datatimem,eventId,deviceId) values(:temp1,:temp2,:temp3,:temp4);");
    for(int i = 1;i < 5; i++){
        query.bindValue(":temp"+QString::number(i),todoList.at(i));
    }
    if(!query.exec()){
        qDebug() << "eventPro() insert error!";
        QSqlError err;
        err= query.lastError();
        qDebug() << err.text();


    }
//    if(!inDB.commit()){
//        qDebug() << "faill to commit!!";
//        inDB.rollback();
//    }
//    inDB.close();

}

void WorkThread::testData(QList<QByteArray> todoList,QSqlDatabase &inDB){
    QTime time;
    time.start();
    qDebug() << QThread::currentThreadId() << " start";
    //qDebug() << QThread::currentThread()->objectName() << "  start";
//    if(inDB.open()){
//        inDB.transaction();
//    }else{
//        qDebug()<< "open transaction error: " << inDB.lastError().text();
//    }

    if(!inDB.isOpen()){
        if(!inDB.open()){
            qDebug() << "testData open inDB error!!";
            return;
        }
    }
    if(!inDB.transaction()){
        qDebug()<< "open transaction error: " << inDB.lastError().text();
    }


    foreach (QByteArray temp, todoList) {
        if(temp.startsWith('D')){
            qDebug() << temp;
            dataPro(temp.split('#'),inDB);
        }else if (temp.startsWith('C')) {
            qDebug() << temp;
            eventPro(temp.split('#'),inDB);
        }else {
            qDebug() << temp;
            sportPro(temp.split('#'),inDB);
        }
    }
    if(!inDB.commit()){
        qDebug() << "commit error: " << inDB.lastError().text();
        inDB.rollback();

    }
//    inDB.close();
    qDebug() << QThread::currentThreadId() << time.elapsed()/1000.0 << "s stop";
}





//void WorkThread::testData(QList<QByteArray> todoList,QSqlDatabase &inDB){
//    QTime time;
//    time.start();
//    qDebug() << QThread::currentThreadId() << " start";
//    //qDebug() << QThread::currentThread()->objectName() << "  start";
//    QString receiveTime = "";
//    QString temperature = "";
//    QString humidity = "";
//    int msgcount = 0;
//    if(inDB.open()){
//        inDB.transaction();
//    }
//    QSqlQuery query(inDB);


//    for(int i = 0;i<todoList.length();i++){
//        QList<QByteArray> datalist = todoList.at(i).split('*');
//        datalist.removeAll(" ");
//        datalist.removeAll("");
//        // qDebug() << datalist.size();
//        if(datalist.size() < 4){
//            return;
//        }
//        // qDebug() << "datalist.size=" ;
//        // qDebug() << datalist.size();
//        for(int k = 0; k < datalist.size();k++){
//            switch (k) {
//            case 0:
//                receiveTime = datalist.at(0).trimmed();
//                break;
//            case 1:
//                temperature = datalist.at(1).trimmed();
//                break;
//            case 2:
//                humidity = datalist.at(2).trimmed();
//                break;
//            case 3:
//            {
//                QList<QByteArray> goats = datalist.at(3).trimmed().split('#');
//                //qDebug() << goats.size();
//                for(int i = 0;i < goats.size();i++){
//                    //qDebug() <<goats.at(i);
//                    QList<QByteArray> tempgoat = goats.at(i).trimmed().split(' ');
//                    tempgoat.removeAll(" ");
//                    tempgoat.removeAll("");
//                    if(tempgoat.size() != 7){
//                        continue;
//                    }
//                    //qDebug() << "tempgot.size = ";
//                    //qDebug() << tempgoat.size();
//                    query.prepare("insert into sportData(goatid,datatimem,sportx,sporty,sportz,anglex,angley,anglez,status) values(?,?,?,?,?,?,?,?,?)");
//                    //query.bindValue(1,QDateTime::fromMSecsSinceEpoch(receiveTime.toLongLong()));
//                    query.bindValue(1,receiveTime);
//                    query.bindValue(8,"unknown");
//                    for(int j = 0 ;j < tempgoat.size();j++){
//                        if(j == 0){
//                            query.bindValue(0,tempgoat.at(0));
//                            //qDebug() << tempgoat.at(0);
//                        }else{
//                            query.bindValue(j+1,tempgoat.at(j));
//                            // qDebug() << tempgoat.at(j) ;
//                            // qDebug() << "null???";
//                            // qDebug() << j;
//                        }
//                    }
//                    //qDebug() << tempgoat.at(6);
//                    if(!query.exec()){
//                         qDebug() << "数据库插入失败！";
//                        qDebug() << query.lastQuery();
//                          QSqlError err;
//                        err= query.lastError();
//                         qDebug() << err.text();
//                    }else{
//                        //if(i == goats.size()-1){
//                        if(msgcount == todoList.length()/2){
//                            //QString temp = receiveTime+"#"+temperature+"#"+humidity;
//                            //QString temp = receiveTime+"#"+"23.4"+"#"+"45.6";
//                            //qDebug() << temp;
//                            query.prepare("insert into houseData(houseId,datatimem,wendu,anqi) values(:houseId,:datatimem,:wendu,:anqi);");
//                            query.bindValue(":houseId",tempgoat.at(0).trimmed());
//                            query.bindValue(":datatimem",receiveTime);
//                            query.bindValue(":wendu",temperature);
//                            query.bindValue(":anqi",humidity);
//                            //qDebug() << temperature;
//                            if(!query.exec()){
//                                qDebug() << "houseData数据库插入失败！";
//                               qDebug() << query.lastQuery();
//                                 QSqlError err;
//                               err= query.lastError();
//                                qDebug() << err.text();
//                            }
//                            //emit saveOk(temp);
//                        }

//                    }
//                }

//            }
//               if(msgcount == 5){
//                    msgcount = 0;
//               }else{
//                    msgcount++;
//               }

//                break;
//            default:
//                qDebug() << "有些意料之外的事发生了,数据分拣!";
//                break;
//            }
//        }
//    }
//    //qDebug() << QThread::currentThread()->objectName() << "  stop";
//    if(!inDB.commit()){
//        inDB.rollback();
//    }
//    inDB.close();
//    qDebug() << QThread::currentThreadId() << time.elapsed()/1000.0 << "s stop";
//}


