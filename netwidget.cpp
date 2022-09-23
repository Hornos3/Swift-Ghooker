#include "netwidget.h"
#include "ui_netwidget.h"

netWidget::netWidget(QStandardItemModel* netModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::netWidget)
{
    ui->setupUi(this);
    this->model = netModel;
    ui->treeView->setModel(model);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setWindowIcon(QIcon(":/background/icon.ico"));
}

netWidget::~netWidget()
{
    delete ui;
}
