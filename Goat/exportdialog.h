#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QCheckBox>
#include <QMap>
#include <QMessageBox>
#include <QProcess>
#include <QFileDialog>
#include <QStandardPaths>
#include <QList>
#include <QDebug>
#include <db.h>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = nullptr);
    ~ExportDialog();
public slots:
    void chechIfChooseAll();
    void chooseAll();
    void chooseZero();
    void doExport();
    QString getChoosed();
    bool exportEndState(int exitCode, QProcess::ExitStatus exitStatus);
private slots:
    void on_checkBox_chooseAll_clicked();

    void on_pushButton_export_clicked();

private:
    Ui::ExportDialog *ui;
    QMap<QString,QString> map;
    QProcess* process;
};

#endif // EXPORTDIALOG_H
