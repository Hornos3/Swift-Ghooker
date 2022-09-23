#include "memorywidget.h"
#include "ui_memorywidget.h"

memoryWidget::memoryWidget(std::map<uint64_t, std::map<int, memoryInfo>> *memories,
                           QStandardItemModel* memModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::memoryWidget)
{
    ui->setupUi(this);
    this->memories = memories;
    memoryViewModel = memModel;
    ui->memoryView->setModel(memModel);
    ui->memoryContent->setModel(memoryContentModel);
    ui->memoryView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->memoryContent->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setWindowIcon(QIcon(":/background/icon.ico"));
}

memoryWidget::~memoryWidget()
{
    delete ui;
}

void memoryWidget::on_memoryView_clicked(const QModelIndex &index)
{
    memoryContentModel->clear();
    memoryContentModel->setHorizontalHeaderLabels(QStringList() << "偏移" << "内容" << "字符");
    if(!index.parent().isValid())
        return;
    QStandardItem* father = memoryViewModel->item(index.parent().row());
    uint64_t addr = father->text().toULongLong(nullptr, 16);
    int id = father->child(index.row(), 0)->text().toInt();
    auto outer_iter = memories->find(addr);
    assert(outer_iter != memories->end());
    auto inner_iter = outer_iter->second.find(id);
    assert(inner_iter != outer_iter->second.end());
    char* buf = inner_iter->second.content;
    int length = inner_iter->second.length;
    int ptr = 0;
    char oneBuffer[4] = {0};
    for(int i=0; i<length / 16; i++){
        ptr = i << 4;
        QString contentLine;
        char charsLine[17] = {0};
        for(int j=0; j<16; j++){
            sprintf_s(oneBuffer, "%02hhx ", buf[ptr + j]);
            contentLine += oneBuffer;
            if(isprint((unsigned char)buf[ptr + j]))
                charsLine[j] = buf[ptr + j];
            else
                charsLine[j] = '.';
        }
        memoryContentModel->appendRow(QList<QStandardItem*>() <<
                                      new QStandardItem(ull2a(i << 4)) <<
                                      new QStandardItem(contentLine) <<
                                      new QStandardItem(charsLine));
    }
    if(length & 15){
        QString lastLine;
        char charsLastLine[17] = {0};
        for(int i=0; ptr + i < length; i++){
            sprintf_s(oneBuffer, "%02hhx ", buf[ptr + i]);
            lastLine += oneBuffer;
            if(isprint((unsigned char)buf[ptr + i]))
                charsLastLine[i] = buf[ptr + i];
            else
                charsLastLine[i] = '.';
        }
        memoryContentModel->appendRow(QList<QStandardItem*>() <<
                                      new QStandardItem(ull2a(ptr)) <<
                                      new QStandardItem(lastLine) <<
                                      new QStandardItem(charsLastLine));
    }
    ui->memoryContent->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

