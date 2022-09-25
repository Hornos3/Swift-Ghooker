#include "filewidget.h"
#include "ui_filewidget.h"

fileWidget::fileWidget(QStandardItemModel* fileModel,
                       colorfulModel *access,
                       colorfulModel *shareMode,
                       colorfulModel *createDisp,
                       colorfulModel *flagAttr,
                       hookAnalysis* analyser,
                       QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileWidget)
{
    ui->setupUi(this);
    ui->treeView->setModel(fileModel);
    ui->access->setModel(access);
    ui->shareMode->setModel(shareMode);
    ui->createDisp->setModel(createDisp);
    ui->flagAttr->setModel(flagAttr);
    setFixedSize(850, 500);
    this->fileModel = fileModel;
    this->access = access;
    this->shareMode = shareMode;
    this->createDisp = createDisp;
    this->flagAttr = flagAttr;
    this->analyser = analyser;
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->access->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->shareMode->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->createDisp->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->flagAttr->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setWindowIcon(QIcon(":/background/icon.ico"));
}

fileWidget::~fileWidget()
{
    delete ui;
}

void fileWidget::on_treeView_clicked(const QModelIndex &index)
{
    auto realIdx = index;
    if(index.parent().isValid())
        realIdx = index.parent();
    int selectedRow = realIdx.row();
    fileHandleAttr handle = analyser->fileHandles[fileModel->item(selectedRow)->text().toULongLong(nullptr, 16)].rbegin()->second;
    list<int> fileAccess = analyser->getGenericAccess(handle.access);
    auto iter = fileAccess.begin();

    for(int i=1; i<=3; i++){
        auto idx = access->index(i);
        if(iter != fileAccess.end() && *iter == i){
            if(!access->data(idx, Qt::DisplayRole).toString().contains("√"))
                access->setData(idx, access->data(idx).toString() + " √", Qt::DisplayRole);
            access->data(idx, Qt::BackgroundRole);
            iter++;
        }else{
            if(access->data(idx, Qt::DisplayRole).toString().contains("√")){
                auto s = access->data(idx).toString();
                access->setData(idx, s.mid(0, s.length() - 2), Qt::DisplayRole);
            }
            access->data(idx, Qt::BackgroundRole);
        }
    }

    list<int> fileShareMode = analyser->getShareMode(handle.shareMode);
    iter = fileShareMode.begin();
    for(int i=1; i<=3; i++){
        auto idx = shareMode->index(i);
        if(iter != fileShareMode.end() && *iter == i){
            if(!shareMode->data(idx, Qt::DisplayRole).toString().contains("√"))
                shareMode->setData(idx, shareMode->data(idx).toString() + " √", Qt::DisplayRole);
            shareMode->data(idx, Qt::BackgroundRole);
            iter++;
        }else{
            if(shareMode->data(idx, Qt::DisplayRole).toString().contains("√")){
                auto s = shareMode->data(idx).toString();
                shareMode->setData(idx, s.mid(0, s.length() - 2), Qt::DisplayRole);
            }
            shareMode->data(idx, Qt::BackgroundRole);
        }
    }

    list<int> fileCreateDisp = analyser->getCreateDisp(handle.createDisp);
    iter = fileCreateDisp.begin();
    for(int i=1; i<=5; i++){
        auto idx = createDisp->index(i);
        if(iter != fileCreateDisp.end() && *iter == i){
            if(!createDisp->data(idx, Qt::DisplayRole).toString().contains("√"))
                createDisp->setData(idx, createDisp->data(idx).toString() + " √", Qt::DisplayRole);
            createDisp->data(idx, Qt::BackgroundRole);
            iter++;
        }else{
            if(createDisp->data(idx, Qt::DisplayRole).toString().contains("√")){
                auto s = createDisp->data(idx).toString();
                createDisp->setData(idx, s.mid(0, s.length() - 2), Qt::DisplayRole);
            }
            createDisp->data(idx, Qt::BackgroundRole);
        }
    }

    list<int> fileFlagAttr = analyser->getFileAttr(handle.flagAttr);
    iter = fileFlagAttr.begin();
    for(int i=1; i<=12; i++){
        auto idx = flagAttr->index(i);
        if(iter != fileFlagAttr.end() && *iter == i){
            if(!flagAttr->data(idx, Qt::DisplayRole).toString().contains("√"))
                flagAttr->setData(idx, flagAttr->data(idx).toString() + " √", Qt::DisplayRole);
            flagAttr->data(idx, Qt::BackgroundRole);
            iter++;
        }else{
            if(flagAttr->data(idx, Qt::DisplayRole).toString().contains("√")){
                auto s = flagAttr->data(idx).toString();
                flagAttr->setData(idx, s.mid(0, s.length() - 2), Qt::DisplayRole);
            }
            flagAttr->data(idx, Qt::BackgroundRole);
        }
    }
}

