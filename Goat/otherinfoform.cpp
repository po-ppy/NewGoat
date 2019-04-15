#include "otherinfoform.h"
#include "ui_otherinfoform.h"

OtherInfoForm::OtherInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OtherInfoForm)
{
    ui->setupUi(this);
    sqlQueryModel = new QSqlQueryModel(this);
    sortFilterProxyModel = new QSortFilterProxyModel(this);

}

OtherInfoForm::~OtherInfoForm()
{
    delete ui;
}

void OtherInfoForm::on_addMoreButton_clicked()
{
//    DB::instance().data()->getDb().open();
    QSqlQuery query;
    query.exec("select feedId as 饲料编号, feedName as 饲料名称, feedRange as 使用范围, feedUsage as 用法用量, feedRemark as 备注 from feedInfo;");
    sqlQueryModel->setQuery(query);

    sortFilterProxyModel->setDynamicSortFilter(true);
    sortFilterProxyModel->setSourceModel(sqlQueryModel);

    ui->tableView->setModel(sortFilterProxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
}
