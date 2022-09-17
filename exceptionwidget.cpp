#include "exceptionwidget.h"
#include "ui_exceptionwidget.h"

exceptionWidget::exceptionWidget(QStandardItemModel *exceptionModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::exceptionWidget)
{
    ui->setupUi(this);
    ui->tableView->setModel(exceptionModel);
}

exceptionWidget::~exceptionWidget()
{
    delete ui;
}
