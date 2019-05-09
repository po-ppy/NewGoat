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
    void receiveHouseId(QString houseId);
    void receiveDeviceId(QString deviceId);
    void addFromFile();

private slots:
    void on_houseDeviceCheckBox_stateChanged(int arg1);

    void on_houseCheckBox_stateChanged(int arg1);

    void on_houseTableView_doubleClicked(const QModelIndex &index);

    void on_houseDeviceTableView_doubleClicked(const QModelIndex &index);

    void on_doubleButton_2_clicked();

    void on_confirmButton_2_clicked();

    void on_selectFileButton_2_clicked();

private:
    Ui::HouseBindingDialog *ui;
    QSqlQueryModel *houseSqlQueryModel;
    QSortFilterProxyModel *houseSortFilterProxyModel;
    QSqlQueryModel *deviceSqlQueryModel;
    QSortFilterProxyModel *deviceSortFilterProxyModel;
};

#endif // HOUSEBINDINGDIALOG_H
