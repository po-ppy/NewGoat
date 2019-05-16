#ifndef OTHERINFOFORM_H
#define OTHERINFOFORM_H

#include <QWidget>
#include <db.h>
#include <QSql>
#include <QSortFilterProxyModel>
//#include <QSqlQueryModel>
#include <QSqlTableModel>
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
#include <QSqlRecord>
#include <QVector>

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
    void initMenu();

private slots:
    void on_addMoreButton_clicked();
    void on_addButton_clicked();

    void on_removeButton_clicked();
    void deleteSelected();
    void updateData(int row, QSqlRecord &record);
    void onInsertNewRow(int row,QSqlRecord &record);
    QString createId();
    void addOne();
    void addMore();
    void saveChange();


    void on_saveChangeButton_clicked();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

public slots:
    void updateTableView();
    void autoUpdateTableView();
    void exportSelected();


private:
    Ui::OtherInfoForm *ui;
    int infoType;
    int preInfoType;
    QList<QString> feedHeadList;
    QList<QString> vacineHeadList;
    QList<QString> productHeadList;
    QList<QString> eventHeadList;

    QSqlQueryModel *sqlQueryModel;
    QSqlTableModel *sqlTableModel;
    QSortFilterProxyModel *sortFilterProxyModel;

    QMenu *cmenu;
    QAction *actionAdd;
    QAction *actionRemove;
    QAction *actionAddMore;
    QAction *actionSaveChange;
    QAction *actionExport;
//    QAction *actionR5;
//    QAction *actionR6;

};

#endif // OTHERINFOFORM_H
