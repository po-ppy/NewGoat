#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <bindingdialog.h>
#include <sportdataform.h>
#include <goatqueryform.h>
#include <devicequeryform.h>
#include <otherinfoform.h>
#include <otherdataform.h>
#include <logindialog.h>
#include <helpdialog.h>
#include <housebindingdialog.h>
#include <exportdialog.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void updateAllTables();
    void loginOK();
    void doLogout();
    void doExit();
    void showTheEvent();
private slots:
    void change_to_goat_query_form();
    void change_to_sport_data_form();
    void change_to_device_query_form();
    void change_to_other_info_form();
    void change_to_other_data_form();
    void change_to_info_feed();
    void change_to_info_vacine();
    void change_to_info_product();
    void change_to_info_event();
    void change_to_data_feeding();
    void change_to_data_antiepidemic();
    void change_to_data_yield();
    void change_to_data_event();
private:
    Ui::MainWindow *ui;
    SportDataForm *sportDataForm;
    GoatQueryForm *goatQueryForm;
    DeviceQueryForm *deviceQueryForm;
    OtherInfoForm *otherInfoForm;
    OtherDataForm *otherDataForm;
    bindingDialog *bdDialog;
    HouseBindingDialog *houseBindingDialog;
    LoginDialog *loginDialog;
    HelpDialog *helpDialog;
    ExportDialog *exportDialog;
};

#endif // MAINWINDOW_H
