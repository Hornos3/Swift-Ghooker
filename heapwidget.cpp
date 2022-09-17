#include "heapwidget.h"
#include "ui_heapwidget.h"

heapWidget::heapWidget(QStandardItemModel* heapModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::heapWidget)
{
    ui->setupUi(this);
    ui->treeView->setModel(heapModel);
}

heapWidget::~heapWidget()
{
    delete ui;
}
