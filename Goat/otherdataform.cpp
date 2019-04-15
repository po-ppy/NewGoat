#include "otherdataform.h"
#include "ui_otherdataform.h"

OtherDataForm::OtherDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OtherDataForm)
{
    ui->setupUi(this);
    sqlQueryModel = new QSqlQueryModel(this);
    sortFilterProxyModel = new QSortFilterProxyModel(this);
}

OtherDataForm::~OtherDataForm()
{
    delete ui;
}

void OtherDataForm::on_showPushButton_clicked()
{
    QSqlQuery query;
//    query.exec("select feedId as 饲料编号, feedName as 饲料名称, feedRange as 使用范围, feedUsage as 用法用量, feedRemark as 备注 from feedInfo;");
    query.exec("select a.goatId as 奶山羊编号, b.feedName as 饲料名称, a.feedLevel as 饲喂量, a.inTime as 饲喂时间, a.feedingRemark as 备注 from feedingData a left join feedInfo b on a.feedId = b.feedId;");
    sqlQueryModel->setQuery(query);

    sortFilterProxyModel->setDynamicSortFilter(true);
    sortFilterProxyModel->setSourceModel(sqlQueryModel);

    ui->tableView->setModel(sortFilterProxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
}
