#include "modulewidget.h"
#include "ui_modulewidget.h"

moduleWidget::moduleWidget(QStandardItemModel *moduleModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::moduleWidget)
{
    ui->setupUi(this);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(moduleModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->setWindowIcon(QIcon(":/background/icon.ico"));
    Qt::WindowFlags m_flags = windowFlags();
    setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
}

moduleWidget::~moduleWidget()
{
    delete ui;
}
