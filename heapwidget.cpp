#include "heapwidget.h"
#include "ui_heapwidget.h"

heapWidget::heapWidget(QStandardItemModel* heapModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::heapWidget)
{
    ui->setupUi(this);
    ui->treeView->setModel(heapModel);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setWindowIcon(QIcon(":/background/icon.ico"));
    Qt::WindowFlags m_flags = windowFlags();
    setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
}

heapWidget::~heapWidget()
{
    delete ui;
}
