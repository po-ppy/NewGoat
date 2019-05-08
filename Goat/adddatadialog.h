#ifndef ADDDATADIALOG_H
#define ADDDATADIALOG_H

#include <QDialog>
#include <db.h>
#include <QMap>
#include <QSqlQuery>
#include <QDateTime>
#include <QList>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

namespace Ui {
class AddDataDialog;
}

class AddDataDialog : public QDialog
{
    Q_OBJECT
signals:
    void updateTab();

public:
    explicit AddDataDialog(QWidget *parent = nullptr);
    ~AddDataDialog();
    bool setDataType(int temp);
    bool initGui();
    bool initIds();
    bool initKeyWord();
    void addFun();

private slots:
    void on_houseIdCheckBox_stateChanged(int arg1);

    void on_goatIdCheckBox_stateChanged(int arg1);

    void on_infoNameComboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::AddDataDialog *ui;
    int dataType;
    int preType;
    QMap<QString,QString> keyWordMap;
};

#endif // ADDDATADIALOG_H
