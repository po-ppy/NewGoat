#ifndef HOUSEBINDINGDIALOG_H
#define HOUSEBINDINGDIALOG_H

#include <QDialog>
#include <db.h>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QClipboard>

namespace Ui {
class HouseBindingDialog;
}

class HouseBindingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HouseBindingDialog(QWidget *parent = nullptr);
    ~HouseBindingDialog();

signals:
    void updateSignal();

public slots:
    void updateHouseTable();
    void updateDeviceTable();
    void startBinding();
    void receiveGoatId(QString goatId);
    void receiveDeviceId(QString deviceId);
    void addFromFile();

private:
    Ui::HouseBindingDialog *ui;
    QSqlQueryModel *houseSqlQueryModel;
    QSortFilterProxyModel *houseSortFilterProxyModel;
    QSqlQueryModel *deviceSqlQueryModel;
    QSortFilterProxyModel *deviceSortFilterProxyModel;
};

#endif // HOUSEBINDINGDIALOG_H
