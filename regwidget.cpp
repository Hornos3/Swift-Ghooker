#include "regwidget.h"
#include "ui_regwidget.h"

regWidget::regWidget(QStandardItemModel *regModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::regWidget)
{
    ui->setupUi(this);
    ui->treeView->setModel(regModel);
}

regWidget::~regWidget()
{
    delete ui;
}
