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
#include <QSqlError>
#include <QMap>
#include <QIcon>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <adddatadialog.h>
#include <QTimer>

namespace Ui {
class OtherDataForm;
}

class OtherDataForm : public QWidget
{
    Q_OBJECT

public:
    explicit OtherDataForm(QWidget *parent = nullptr);
    ~OtherDataForm();
    bool setDataType(int temp);//0 -- feedingData, 1 -- antiepidemicData, 2 -- yieldData
private slots:
    void on_showPushButton_clicked();

    void on_showAllCheckBox_stateChanged(int arg1);

    void on_startTimeDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_endTimeDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_keyWordCheckBox_stateChanged(int arg1);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_keyWordLineEdit_returnPressed();

    void on_addButton_clicked();

    void on_removeButton_clicked();

    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void on_addByHouseButton_clicked();

signals:
    void shouldShowUntreatedEvent();
public slots:
    void updateTableView();
    void autoUpdateTableView();
    void updateKeyWord();
    void keyWordSearch();
    void addOne();
    void deleteSelected();
    void exportSelected();
    void showUntreatedEvent();
    int hasUntreatedEvent();
    void initCheckEvent();
    void stopCheckEvent();
    void showAlert();
    void initMenu();
    void initPlayer();
    void playSound(int eventId);

private:
    Ui::OtherDataForm *ui;
    QSqlQueryModel *sqlQueryModel;
    QSortFilterProxyModel *sortFilterProxyModel;
    int dataType;
    int preDataType;
    QMap<QString,QString> keyWordMap;
    AddDataDialog *addDataDialog;
    QTimer eventCheckTimer;
    QMediaPlaylist* playlist;
    QMediaPlayer *player;

    QMenu *cmenu;
    QAction *actionAdd;
    QAction *actionRemove;
    QAction *actionExport;
};

#endif // OTHERDATAFORM_H
