#ifndef OTHERDATAFORM_H
#define OTHERDATAFORM_H

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

namespace Ui {
class OtherDataForm;
}

class OtherDataForm : public QWidget
{
    Q_OBJECT

public:
    explicit OtherDataForm(QWidget *parent = nullptr);
    ~OtherDataForm();

private slots:
    void on_showPushButton_clicked();

private:
    Ui::OtherDataForm *ui;
    QSqlQueryModel *sqlQueryModel;
    QSortFilterProxyModel *sortFilterProxyModel;
};

#endif // OTHERDATAFORM_H
