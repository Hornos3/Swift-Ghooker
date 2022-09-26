#include "regwidget.h"
#include "ui_regwidget.h"

regWidget::regWidget(QStandardItemModel *regModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::regWidget)
{
    ui->setupUi(this);
    ui->treeView->setModel(regModel);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setWindowIcon(QIcon(":/background/icon.ico"));
    Qt::WindowFlags m_flags = windowFlags();
    setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
}

regWidget::~regWidget()
{
    delete ui;
}
