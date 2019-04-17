#ifndef OTHERINFOFORM_H
#define OTHERINFOFORM_H

#include <QWidget>
#include <db.h>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>
#include <QDebug>
#include <QDateTime>
#include <QModelIndexList>
#include <QList>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QFileDialog>
#include <QAbstractItemView>

namespace Ui {
class OtherInfoForm;
}

class OtherInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit OtherInfoForm(QWidget *parent = nullptr);
    ~OtherInfoForm();
    bool setInfoType(int temp);//0 -- feed, 1 -- vacine, 2 -- product

private slots:
    void on_addMoreButton_clicked();
    void on_addButton_clicked();

    void on_removeButton_clicked();
    void deleteSelected();

public slots:
    void updateTableView();
    void autoUpdateTableView();


private:
    Ui::OtherInfoForm *ui;
    int infoType;
    int preInfoType;
    QSqlQueryModel *sqlQueryModel;
    QSortFilterProxyModel *sortFilterProxyModel;
};

#endif // OTHERINFOFORM_H
