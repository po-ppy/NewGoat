#ifndef INFORMATIONFORM_H
#define INFORMATIONFORM_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QTimer>
#include <QByteArray>
#include <QPointF>
#include <QDebug>
#include <QtMath>
#include <QModelIndex>
#include <QSqlError>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QWheelEvent>
#include <QLegendMarker>
#include <QLabel>
#include <db.h>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class InformationForm;
}

class InformationForm : public QWidget
{
    Q_OBJECT

public:
    explicit InformationForm(QWidget *parent = 0);
    ~InformationForm();
public slots:
    void createChart();
    void setHouseId(QString inHouseId);
    QString getHouseId();
//    void initSqlTable();
//    void updateSqlTable();
//    void exportToFile(QString houseId);
//    void updateAllData();
//    void updateGoatList();
//    void updateHouseData();
    void updateChart();
    void updateDashboard();
//    double toG(double in);
//    double toG(QByteArray in);
//    double getG(double in1,double in2,double in3);
//    void autoShow();
//    void selectOne(int row,int col);
    void connectMarkers();
    void disconnectMarkers();

    void handleMarkerClicked();
    void pointHovered(const QPointF &point, bool state);
//    void scrollChart(const QPointF &point);
protected:
    void wheelEvent(QWheelEvent *event) override;
//    void mousePressEvent(QMouseEvent *event) override;
//    void mouseReleaseEvent(QMouseEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
private slots:
//    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();


signals:
    void exportSignal();
private:
    Ui::InformationForm *ui;

    QString houseId;

    QTimer *freshTimer;
    QLabel* valueLabel;
    bool mousePress;
    QPoint prePoint;
    QSqlQueryModel *sqlQueryModel;
    QSqlQueryModel *sqlQueryModel2;
    QSortFilterProxyModel *sortFilterProxyModel;

    QChart *chart;
    QChartView *chartView;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;

    QSplineSeries *lineWendu;
    QSplineSeries *lineShidu;
    QSplineSeries *lineCO2;
    QSplineSeries *lineNH3;
    QSplineSeries *linePM2;
    QSplineSeries *linePM10;
    QSplineSeries *lineGuangzhao;

    //QScatterSeries *scatterx;
    //QScatterSeries *scattery;
    //QScatterSeries *scatterz;
//    QScatterSeries *scatterg;

    QList<QPointF> dataWendu ;
    QList<QPointF> dataShidu ;
    QList<QPointF> dataCO2 ;
    QList<QPointF> dataNH3 ;
    QList<QPointF> dataPM2;
    QList<QPointF> dataPM10;
    QList<QPointF> dataGuangzhao;
};

#endif // INFORMATIONFORM_H
