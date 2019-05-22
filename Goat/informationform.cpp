#include "informationform.h"
#include "ui_informationform.h"

InformationForm::InformationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationForm)
{

    ui->setupUi(this);

    freshTimer = new QTimer(this);
    sqlQueryModel = new QSqlQueryModel(this);
    sqlQueryModel2 = new QSqlQueryModel(this);
    sortFilterProxyModel = new QSortFilterProxyModel(this);

    mousePress = false;

    ui->tableView->resizeColumnsToContents();
    ui->tableView_2->hide();
//    initSqlTable();
    createChart();
    connectMarkers();

    valueLabel = new QLabel(this);

    valueLabel->setStyleSheet("background-color:orange;color:gray;border-radius:5px;");
    valueLabel->setAlignment(Qt::AlignCenter|Qt::AlignHCenter);
    valueLabel->setScaledContents(true);
    valueLabel->setFont(QFont("黑体",14));
    valueLabel->setAlignment(Qt::AlignTop);
    valueLabel->hide();
    this->setMouseTracking(true);
    connect(freshTimer,SIGNAL(timeout()),this,SLOT(updateChart()));
//    connect(freshTimer,SIGNAL(timeout()),this,SLOT(initSqlTable()));
    freshTimer->start(10000);
}

InformationForm::~InformationForm()
{
//    freshTimer->stop();
    delete ui;
}

void InformationForm::setHouseId(QString inHouseId){
    houseId = inHouseId;
}

QString InformationForm::getHouseId(){
    return houseId;
}

//void InformationForm::initSqlTable(){
//    QSqlQuery query;
////    if(!query.isActive()){
////        return;
////    }
//    query.prepare("select deviceId from houseBindingInfo where houseId = :houseId;");
//    query.bindValue(":houseId",houseId);
//    query.exec();
//    if(query.next()){
//        ui->deviceIdLabel->setText(query.value(0).toString());
//    }
//    query.prepare("select houseId as 舍号, wendu as 温度, anqi as 氨气, shidu as 湿度, eryang as 二氧化碳, guangzhao as 光照, pm25 as 'PM2.5', pm10 as PM10, yanwu as 烟雾,from_unixtime(left(datatimem,10),'%Y-%m-%d %H:%i:%S') as 记录时间  from houseData where houseId = :houseId order by datatimem DESC limit 20;");
//    query.bindValue(":houseId",houseId);
//    query.exec();
//    if(query.next()){
//        ui->wenduLabel->setText(query.value(1).toString());
//        ui->anqiLabel->setText(query.value(2).toString());
//        ui->shiduLabel->setText(query.value(3).toString());
//        ui->eryangLabel->setText(query.value(4).toString());
//        ui->guangzhaoLabel->setText(query.value(5).toString());
//        ui->pm25Label->setText(query.value(6).toString());
//        ui->pm10Label->setText(query.value(7).toString());
//        ui->yanwuLabel->setText(query.value(8).toString());
//        ui->datatimeLabel->setText(query.value(9).toString());
//    }
//    sqlQueryModel2->setQuery(query);
//    ui->tableView_2->setModel(sqlQueryModel2);

//    ui->tableView_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    int tempCount = ui->tableView_2->horizontalHeader()->count() - 1 ;
//    for(int i = 0; i < tempCount;i++){
//        ui->tableView_2->horizontalHeader()->setSectionResizeMode(i,QHeaderView::ResizeToContents);
//    }
//    ui->tableView_2->horizontalHeader()->setSectionResizeMode(tempCount,QHeaderView::Stretch);

//    QObject::connect(freshTimer,SIGNAL(timeout()),this,SLOT(updateSqlTable()));
//    freshTimer->start(10000);
////    qDebug() << sqlTableModel->lastError().text();
//}

//void InformationForm::updateSqlTable(){
//    if(!DB::instance().data()->getDb().isOpen()){
//        QObject::disconnect(freshTimer,SIGNAL(timeout()),this,SLOT(updateSqlTable()));
////        qDebug() << "updateSqlTable db is not open";
//        freshTimer->stop();
//        return;
//    }
//    if(!this->isVisible()){
//        return;
//    }
//    QSqlQuery query;
////    if(!query.isValid()){
////        freshTimer->stop();
////        qDebug() << "query is not active!!";
////        return;
////    }
//    query.prepare("select deviceId from houseBindingInfo where houseId = :houseId;");
//    query.bindValue(":houseId",houseId);
//    query.exec();
//    if(query.next()){
//        ui->deviceIdLabel->setText(query.value(0).toString());
//    }
//    query.prepare("select houseId as 舍号, wendu as 温度, anqi as 氨气, shidu as 湿度, eryang as 二氧化碳, guangzhao as 光照, pm25 as 'PM2.5', pm10 as PM10, yanwu as 烟雾,from_unixtime(left(datatimem,10),'%Y-%m-%d %H:%i:%S') as 记录时间  from houseData where houseId = :houseId order by datatimem DESC limit 20;");
//    query.bindValue(":houseId",houseId);
//    query.exec();
//    if(query.next()){
//        ui->wenduLabel->setText(query.value(1).toString());
//        ui->anqiLabel->setText(query.value(2).toString());
//        ui->shiduLabel->setText(query.value(3).toString());
//        ui->eryangLabel->setText(query.value(4).toString());
//        ui->guangzhaoLabel->setText(query.value(5).toString());
//        ui->pm25Label->setText(query.value(6).toString());
//        ui->pm10Label->setText(query.value(7).toString());
//        ui->yanwuLabel->setText(query.value(8).toString());
//        ui->datatimeLabel->setText(query.value(9).toString());
//    }
//    sqlQueryModel2->setQuery(query);
//    ui->tableView_2->setModel(sqlQueryModel2);

//    ui->tableView_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    int tempCount = ui->tableView_2->horizontalHeader()->count() - 1 ;
//    for(int i = 0; i < tempCount;i++){
//        ui->tableView_2->horizontalHeader()->setSectionResizeMode(i,QHeaderView::ResizeToContents);
//    }
//    ui->tableView_2->horizontalHeader()->setSectionResizeMode(tempCount,QHeaderView::Stretch);
//}


void InformationForm::createChart(){
    chart = new QChart();
    chart->legend()->hide();
    chart->setTitle("环境数据曲线");



    axisX = new QDateTimeAxis;
    axisX->setTickCount(20);
    axisX->setFormat("hh:mm:ss");
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis;
//    axisY->setMax(100);
//    axisY->setMin(0);
    axisY->setRange(0,200);
    //axisY->setLinePenColor(series->pen().color());

    chart->addAxis(axisY, Qt::AlignLeft);



    lineWendu = new QSplineSeries;
    lineShidu = new QSplineSeries;
    lineCO2 = new QSplineSeries;
    lineNH3 = new QSplineSeries;
    lineGuangzhao = new QSplineSeries;
    linePM2 = new QSplineSeries;
    linePM10 = new QSplineSeries;

//    lineWendu->setPointsVisible(true);
//    lineShidu->setPointsVisible(true);
//    lineCO2->setPointsVisible(true);
//    lineNH3->setPointsVisible(true);
//    lineGuangzhao->setPointsVisible(true);
//    linePM2->setPointsVisible(true);
//    linePM10->setPointsVisible(true);

//    lineWendu->setPointLabelsVisible(true);
//    linePM2->setPointLabelsVisible(true);
//    linePM2->setPointLabelsFormat("@yPoint");

    connect(lineWendu,&QSplineSeries::hovered,this,&InformationForm::pointHovered);
    connect(lineShidu,&QSplineSeries::hovered,this,&InformationForm::pointHovered);
    connect(lineCO2,&QSplineSeries::hovered,this,&InformationForm::pointHovered);
    connect(lineNH3,&QSplineSeries::hovered,this,&InformationForm::pointHovered);
    connect(lineGuangzhao,&QSplineSeries::hovered,this,&InformationForm::pointHovered);
    connect(linePM2,&QSplineSeries::hovered,this,&InformationForm::pointHovered);
    connect(linePM10,&QSplineSeries::hovered,this,&InformationForm::pointHovered);


    //scatterx = new QScatterSeries;
    //scattery = new QScatterSeries;
    //scatterz = new QScatterSeries;
//    scatterg = new QScatterSeries;

    chart->addSeries(lineWendu);
    chart->addSeries(lineShidu);
    chart->addSeries(lineCO2);
    chart->addSeries(lineNH3);
    chart->addSeries(lineGuangzhao);
    chart->addSeries(linePM2);
    chart->addSeries(linePM10);

    //chart->addSeries(scatterx);
    //chart->addSeries(scattery);
    //chart->addSeries(scatterz);
//    chart->addSeries(scatterg);


    lineWendu->attachAxis(axisX);
    lineWendu->attachAxis(axisY);
    lineShidu->attachAxis(axisX);
    lineShidu->attachAxis(axisY);
    lineCO2->attachAxis(axisX);
    lineCO2->attachAxis(axisY);
    lineNH3->attachAxis(axisX);
    lineNH3->attachAxis(axisY);
    linePM2->attachAxis(axisX);
    linePM2->attachAxis(axisY);
    linePM10->attachAxis(axisX);
    linePM10->attachAxis(axisY);
    lineGuangzhao->attachAxis(axisX);
    lineGuangzhao->attachAxis(axisY);

    //scatterx->attachAxis(axisX);
    //scatterx->attachAxis(axisY);
    //scattery->attachAxis(axisX);
    //scattery->attachAxis(axisY);
    //scatterz->attachAxis(axisX);
    //scatterz->attachAxis(axisY);
//    scatterg->attachAxis(axisX);
//    scatterg->attachAxis(axisY);

    lineWendu->setName("温度");
    lineShidu->setName("湿度");
    lineCO2->setName("二氧化碳");
    lineNH3->setName("氨气");
    linePM2->setName("PM2.5");
    linePM10->setName("PM10");
    lineGuangzhao->setName("光照");

    //scatterx->setName("point:x");
    //scattery->setName("point:y");
    //scatterz->setName("point:z");
//    scatterg->setName("point:g");

    lineNH3->setColor(QColor("black"));
    linePM2->setColor(QColor("cyan"));
    linePM10->setColor(QColor("darkCyan"));
    //scatterx->setColor(lineWendu->color());
    //scattery->setColor(lineShidu->color());
    //scatterz->setColor(lineCO2->color());
//    scatterg->setColor(lineNH3->color());

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

//    chart->setTheme(QChart::ChartThemeDark);

    chartView = new QChartView(chart);
    ui->gridLayout->addWidget(chartView);
}

void InformationForm::pointHovered(const QPointF &point, bool state){
    if(state){
        valueLabel->setText(QString::number(point.y()));
        valueLabel->adjustSize();
        QPoint curPos = mapFromGlobal(QCursor::pos());
        valueLabel->move(curPos.x(),curPos.y() - valueLabel->height());
//        qDebug() << "point.y() : " << point.y();
//        valueLabel-
//        valueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
//        valueLabel->setAlignment(Qt::AlignTop);
        valueLabel->show();
    }else{
        valueLabel->hide();
    }
}

//void InformationForm::updateAllData(){
//    if(this->isVisible()){
//        updateGoatList();
//        updateHouseData();
//        updateChart();
//    }
//}

//void InformationForm::updateGoatList(){
//    QSqlQuery query;
//   // query.prepare("select goatId as 山羊编号, shehao as 舍号, deviceId as 绑定设备id, weight as 体重, inTime as 入圈时间, outTime as 出圈时间 from goatInfo where shehao = :shehao;");
//    query.prepare("select goatId as 奶山羊编号,status as 运动状态 from sportDataView where houseId = :houseId;");
//    query.bindValue(":houseId",houseId);
//    if(query.exec()){
//        //qDebug() << houseId;
//        sqlQueryModel->setQuery(query);

//        sortFilterProxyModel->setDynamicSortFilter(true);
//        sortFilterProxyModel->setSourceModel(sqlQueryModel);

//        ui->tableView->setModel(sortFilterProxyModel);
//        ui->tableView->setSortingEnabled(true);
//        ui->tableView->horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);
//        ui->tableView->horizontalHeader()->setStretchLastSection(true);

//        autoShow();
//    }else{
//        qDebug() << "goatList error";
//    }

//}

//void InformationForm::updateHouseData(){
//    QSqlQuery query;
//    query.prepare("select * from houseDataView where houseId = :houseId;");
//    query.bindValue(":houseId",houseId);

//    if(query.exec()){
//       // qDebug() << query.size();
//        if(query.next()){
//            //qDebug() << houseId;
//            this->ui->datatijmeLabel->setText(QDateTime::fromMSecsSinceEpoch(query.value("datatimem").toLongLong()).toString("yyyy-MM-dd hh:mm:ss:zzz"));
//            this->ui->wenduLabel->setText(query.value("wendu").toString());
//            //this->ui->shiduLabel->setText(query.value("shidu").toString());
//            //this->ui->eryangLabel->setText(query.value("eryang").toString());
//            this->ui->anqiLabel->setText(query.value("anqi").toString());
//            this->ui->shiduLabel->setText("51.4");
//            this->ui->eryangLabel->setText("537.2");
//        }
//    }
//}

//void InformationForm::on_tableView_doubleClicked(const QModelIndex &index)
//{
//    selectOne(index.row(),0);
//}


void InformationForm::updateChart(){
    if(this->isVisible()){
        QSqlQuery query;
        //query.prepare("select * from (select * from sportData where goatId = :goatId ORDER By datatimem desc limit 10) aa order by id;");
        query.prepare("select * from houseData where houseId = :houseId ORDER By datatimem desc limit 20;");
//        query.bindValue(":goatId",ui->goatIdLabel->text());
        query.bindValue(":houseId",houseId);
        if(query.exec()){
            if(query.size() == 0){
                qDebug() << "query size == 0";
                return;
            }
            //qDebug() << houseId;
            dataWendu.clear();
            dataShidu.clear();
            dataCO2.clear();
            dataNH3.clear();
            dataPM2.clear();
            dataPM10.clear();
            dataGuangzhao.clear();

            while(query.next()){
                dataWendu.push_front(QPointF(query.value("datatimem").toLongLong(),query.value("wendu").toLongLong()));
                dataShidu.push_front(QPointF(query.value("datatimem").toLongLong(),query.value("shidu").toLongLong()));
                dataCO2.push_front(QPointF(query.value("datatimem").toLongLong(),query.value("eryang").toLongLong()));
                dataNH3.push_front(QPointF(query.value("datatimem").toLongLong(),query.value("anqi").toLongLong()));
                dataGuangzhao.push_front(QPointF(query.value("datatimem").toLongLong(),query.value("guangzhao").toLongLong()));
                dataPM2.push_front(QPointF(query.value("datatimem").toLongLong(),query.value("pm25").toLongLong()));
                dataPM10.push_front(QPointF(query.value("datatimem").toLongLong(),query.value("pm10").toLongLong()));
            }
            //qDebug() << dataNH3.last().ry();
            //qDebug() << QDateTime::fromMSecsSinceEpoch(dataWendu.first().rx()).toMSecsSinceEpoch();
            //qDebug() << QDateTime::fromMSecsSinceEpoch(dataWendu.last().rx()).toMSecsSinceEpoch();

            lineWendu->replace(dataWendu);
            lineShidu->replace(dataShidu);
            lineCO2->replace(dataCO2);
            lineNH3->replace(dataNH3);
            linePM2->replace(dataPM2);
            linePM10->replace(dataPM10);
            lineGuangzhao->replace(dataGuangzhao);

            //scatterx->replace(dataWendu);
            //scattery->replace(dataShidu);
            //scatterz->replace(dataCO2);
//            scatterg->replace(dataNH3);

            axisX->setMax(QDateTime::fromMSecsSinceEpoch(dataWendu.last().rx()));
            axisX->setMin(QDateTime::fromMSecsSinceEpoch(dataWendu.first().rx()));

            updateDashboard();
        }else{
            qDebug() << "图表查询失败";
        }
    }
}


//void InformationForm::wheelEvent(QWheelEvent *event){
//    if(event->delta() > 0){
//        chart->zoom(1.1);
//    }else{
//        chart->zoom(10.0/11);
//    }
//    QWidget::wheelEvent(event);
//}

void InformationForm::updateDashboard(){
    if(dataCO2.size() < 1){
        return;
    }
    ui->wenduLabel->setText(QString::number(dataWendu.last().y()));
    ui->anqiLabel->setText(QString::number(dataNH3.last().y()));
    ui->shiduLabel->setText(QString::number(dataShidu.last().y()));
    ui->eryangLabel->setText(QString::number(dataCO2.last().y()));
    ui->guangzhaoLabel->setText(QString::number(dataGuangzhao.last().y()));
    ui->pm25Label->setText(QString::number(dataPM2.last().y()));
    ui->pm10Label->setText(QString::number(dataPM10.last().y()));
    ui->yanwuLabel->setText("0");
    ui->datatimeLabel->setText(QDateTime::fromMSecsSinceEpoch(dataWendu.last().rx()).toString());

}

void InformationForm::connectMarkers(){
    // Connect all markers to handler
    const auto markers = chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &InformationForm::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &InformationForm::handleMarkerClicked);
    }
}

void InformationForm::disconnectMarkers(){
    const auto markers = chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &InformationForm::handleMarkerClicked);
    }
}

void InformationForm::handleMarkerClicked(){
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);
//![3]

//![4]
    switch (marker->type())
//![4]
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
//![5]
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);
//![5]

//![6]
        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible())
            alpha = 0.5;

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);

//![6]
        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}

//double InformationForm::toG(double in){
//    //qDebug() << (in/17884)*9.8 << "  ggg";
//    return (in/17884)*9.8;
//}

//double InformationForm::toG(QByteArray in){
//    //qDebug() << (in.toLongLong()/16884)*9.8 << "  ggg";
//    return (in.toLongLong()/17884)*9.8;
//}

//double InformationForm::getG(double in1, double in2, double in3){
//    qDebug() << toG(sqrt(in1*in1+in2*in2+in3*in3)) << "  ggg";
//    return toG(sqrt(in1*in1+in2*in2+in3*in3));
//}

//void InformationForm::autoShow(){
//    if(ui->goatIdLabel->text() == ""){
//        selectOne(0,0);
//    }
//}

//void InformationForm::selectOne(int row, int col){
//    QString tempGoatId = ui->tableView->model()->index(row,col).data().toString();
//    ui->goatIdLabel->setText(tempGoatId);
//    ui->deviceIdLabel->setText("");
//    QSqlQuery query;
//    query.prepare("select deviceId from bindingInfo where goatId = :goatId;");
//    query.bindValue(":goatId",tempGoatId);
//    if(query.exec()){
//        if(query.next()){
//            ui->deviceIdLabel->setText(query.value("deviceId").toString());
//        }
//    }
//}

void InformationForm::wheelEvent(QWheelEvent *event){

    if(event->pos().x() > chartView->pos().x() && event->x() < (chartView->pos().x() + chartView->width())){
        if(event->pos().y() > chartView->pos().y() && event->y() < (chartView->pos().y() + chartView->height())){
            axisY->setRange(0,axisY->max()+event->angleDelta().y()/2);
        }
    }

//    chart->scroll(0,event->angleDelta().y());
    event->accept();
}

//void InformationForm::mousePressEvent(QMouseEvent *event){
//    qDebug() << "press event";
////    this->setCursor(Qt::ClosedHandCursor);
//    if(event->button() == Qt::LeftButton){
//        mousePress = true;
//        prePoint = event->pos();
//    }
////    event->accept();
////    return InformationForm::mousePressEvent(event);
//    event->accept();
//}

//void InformationForm::mouseReleaseEvent(QMouseEvent *event){
//    qDebug() << "release event";
//    if(event->button() == Qt::LeftButton && mousePress){
//        mousePress = false;
//    }
//    event->accept();
//}

//void InformationForm::mouseMoveEvent(QMouseEvent *event){
//    qDebug() << "move event";
//    if(mousePress){
//        axisY->setRange(axisY->min()-event->pos().y() + prePoint.y(),axisY->max()-event->pos().y() + prePoint.y());
//        qDebug() << "event.y:" << event->pos().y();
//        qDebug() << "prePoint.y:" << prePoint.y();
//    }
//    event->accept();
//}

void InformationForm::on_pushButton_clicked()
{
    emit exportSignal();
}

//void QChartView::mousePressEvent(QMouseEvent *event){
//    event->ignore();
//}

//void QChartView::mouseReleaseEvent(QMouseEvent *event){
//    event->ignore();
//}

//void QChartView::mouseMoveEvent(QMouseEvent *event){
//    event->ignore();
//}
