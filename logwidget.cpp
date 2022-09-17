﻿#include "logwidget.h"
#include "ui_logwidget.h"

logWidget::logWidget(std::vector<fullLog> *allLog, QStandardItemModel *model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logWidget)
{
    ui->setupUi(this);
    ui->treeView->setModel(model);
    this->allLog = allLog;
    model->setHorizontalHeaderLabels(QStringList() << "操作序号/参数名" << "函数名/参数类型" << "参数值（整数）" << "参数值（字符串）");
}

logWidget::~logWidget()
{
    delete ui;
}