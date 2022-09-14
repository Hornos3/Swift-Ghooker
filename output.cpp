#include "Util.h"
#include "output.h"
#include "hookanalysis.h"
#include <QStringLiteral>
#include "ui_output.h"
#include <QReadWriteLock>
#include <fstream>
using namespace std;

extern bool (*getMutexSignal)(void);
extern void (*setMutexSignal)(void);

extern QReadWriteLock* lock;
int counter = 0;

Output::Output(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Output)
{
    ui->setupUi(this);
    ui->logInfo->setReadOnly(true);

    QStringList verticalList;
    verticalList << "进程名" << "文件描述" << "版本" << "内部名" << "公司名" << "版权" << "原始名" << "产品版本";
    QStringList horizontalList;
    horizontalList << "值";
    model->setHorizontalHeaderLabels(horizontalList);
    model->setVerticalHeaderLabels(verticalList);

    heapViewModel->setHorizontalHeaderLabels(QStringList() << "地址" << "类别" << "大小" << "状态");
    fileViewModel->setHorizontalHeaderLabels(QStringList() << "句柄指针/操作类型" << "文件名/字节数" << "状态/成功字节数" << "是否成功");
    exceptionModel->setHorizontalHeaderLabels(QStringList() << "异常操作编号" << "异常类型" << "详细信息");
    regeditModel->setHorizontalHeaderLabels(QStringList() << "句柄/操作类型" << "键值/操作细节" << "状态/操作返回");
    fileAccessModel = new colorfulModel(QStringList() << "权限" << "READ" << "WRITE" << "EXECUTE");
    fileShareModeModel = new colorfulModel(QStringList() << "共享选项" << "SHARE_READ" << "SHARE_WRITE" << "SHARE_DELETE");
    fileCreateDispModel = new colorfulModel(QStringList() << "创建选项" << "CREATE_NEW" << "CREATE_ALWAYS" <<
                                            "OPEN_EXISTING" << "OPEN_ALWAYS" << "TRUNCATE_EXISTING");
    fileFlagAttrModel = new colorfulModel(QStringList() <<  "文件属性" << "FILE_ATTRIBUTE_ARCHIVE" <<
                                            "FILE_ATTRIBUTE_COMPRESSED" <<
                                            "FILE_ATTRIBUTE_NORMAL" << "FILE_ATTRIBUTE_HIDDEN" <<
                                            "FILE_ATTRIBUTE_READONLY" << "FILE_ATTRIBUTE_SYSTEM" <<
                                            "FILE_FLAG_WRITE_THROUGH" << "FILE_FLAG_OVERLAPPED" <<
                                            "FILE_FLAG_NO_BUFFERING" << "FILE_FLAG_RANDOM_ACCESS" <<
                                            "FILE_FLAG_SEQUENTIAL_SCAN" << "FILE_FLAG_DELETE_ON_CLOSE");

    ui->processInfo->setModel(model);
    ui->heapView->setModel(heapViewModel);
    ui->fileView->setModel(fileViewModel);
    ui->exceptionWindow->setModel(exceptionModel);
    ui->regeditView->setModel(regeditModel);
    ui->fileAccess->setModel(fileAccessModel);
    ui->fileShareMode->setModel(fileShareModeModel);
    ui->fileCreationDisposition->setModel(fileCreateDispModel);
    ui->fileFlagsAndAttributes->setModel(fileFlagAttrModel);

    ui->processInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->heapView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fileView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->exceptionWindow->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->regeditView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fileAccess->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fileShareMode->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fileCreationDisposition->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fileFlagsAndAttributes->setEditTriggers(QAbstractItemView::NoEditTriggers);

    regeditModel->insertRow(0, QList<QStandardItem*>() <<
                            new QStandardItem(ull2a((uint64_t)HKEY_CLASSES_ROOT)) <<
                            new QStandardItem("HKEY_CLASSES_ROOT") <<
                            new QStandardItem("正在使用"));
    regeditModel->insertRow(1, QList<QStandardItem*>() <<
                            new QStandardItem(ull2a((uint64_t)HKEY_CURRENT_USER)) <<
                            new QStandardItem("HKEY_CURRENT_USER") <<
                            new QStandardItem("正在使用"));
    regeditModel->insertRow(2, QList<QStandardItem*>() <<
                            new QStandardItem(ull2a((uint64_t)HKEY_LOCAL_MACHINE)) <<
                            new QStandardItem("HKEY_LOCAL_MACHINE") <<
                            new QStandardItem("正在使用"));
    regeditModel->insertRow(3, QList<QStandardItem*>() <<
                            new QStandardItem(ull2a((uint64_t)HKEY_USERS)) <<
                            new QStandardItem("HKEY_USERS") <<
                            new QStandardItem("正在使用"));
    regeditModel->insertRow(4, QList<QStandardItem*>() <<
                            new QStandardItem(ull2a((uint64_t)HKEY_CURRENT_CONFIG)) <<
                            new QStandardItem("HKEY_CURRENT_CONFIG") <<
                            new QStandardItem("正在使用"));

    // ui->processInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    watcher->addPath("./hookLog/lasthook.tmp");
    connect(this->watcher, &QFileSystemWatcher::fileChanged, this, &Output::updateLog);
}

void Output::updateLog(){
//    lock->lockForWrite();

    while(!getMutexSignal);

    ifstream in("./hookLog/lasthook.tmp");
    char newLog[0x800] = {0};
    in.read(newLog, 0x800);
    wstring logW = stringTowstring(newLog);
    QString logQ = QString::fromStdWString(logW);
    if(logQ.isEmpty()){
//        lock->unlock();
        setMutexSignal();
        return;
    }
    bool exeInfoGot = analyser->exeInfoGot;
//    bool mayRepeat = false;
    bool mayRepeat = analyser->appendRecord(logQ);
    if(exeInfoGot ^ analyser->exeInfoGot){
        showExeInfo();
        assert(ui->logInfo->toPlainText().isEmpty());
        ui->logInfo->appendPlainText(analyser->exeInfo.to_string());
    }
    if(!mayRepeat){
        trimExeInfo(logQ);
        ui->logInfo->appendPlainText(logQ);
    }else
        qDebug() << ++counter;
    in.close();

    setMutexSignal();
//    lock->unlock();
}

void Output::closeEvent(QCloseEvent * event){
    ui->logInfo->clear();
}

void Output::trimExeInfo(QString& fullInfo){
    auto lines = fullInfo.split("\n");
    size_t startRemPos = 4;
    while(lines.at(startRemPos).startsWith("\t"))
        startRemPos++;
    lines.remove(startRemPos, 11);
    fullInfo = lines.join("\n");
}

void Output::showExeInfo(){
    exeInfo info = analyser->exeInfo;
    model->setItem(0, 0, new QStandardItem(info.processName));
    model->setItem(1, 0, new QStandardItem(info.fileDescription));
    model->setItem(2, 0, new QStandardItem(info.fileVersion));
    model->setItem(3, 0, new QStandardItem(info.internalName));
    model->setItem(4, 0, new QStandardItem(info.companyName));
    model->setItem(5, 0, new QStandardItem(info.legalCopyright));
    model->setItem(6, 0, new QStandardItem(info.originalFileName));
    model->setItem(7, 0, new QStandardItem(info.productVersion));
}

Output::~Output()
{
    delete ui;
}

void Output::on_fileView_clicked(const QModelIndex &index)
{
    int selectedRow = index.row();
    fileHandleAttr handle = analyser->fileHandles[fileViewModel->item(selectedRow)->text().toULongLong(nullptr, 16)];
    list<int> fileAccess = analyser->getGenericAccess(handle.access);
    auto iter = fileAccess.begin();

    for(int i=1; i<=FILEACCESS_COUNT; i++){
        auto idx = fileAccessModel->index(i);
        if(iter != fileAccess.end() && *iter == i){
            if(!fileAccessModel->data(idx, Qt::DisplayRole).toString().contains("√"))
                fileAccessModel->setData(idx, fileAccessModel->data(idx).toString() + " √", Qt::DisplayRole);
            fileAccessModel->data(idx, Qt::BackgroundRole);
            iter++;
        }else{
            if(fileAccessModel->data(idx, Qt::DisplayRole).toString().contains("√")){
                auto s = fileAccessModel->data(idx).toString();
                fileAccessModel->setData(idx, s.mid(0, s.length() - 2), Qt::DisplayRole);
            }
            fileAccessModel->data(idx, Qt::BackgroundRole);
        }
    }

    list<int> fileShareMode = analyser->getShareMode(handle.shareMode);
    iter = fileShareMode.begin();
    for(int i=1; i<=FILESHAREMODE_COUNT; i++){
        auto idx = fileShareModeModel->index(i);
        if(iter != fileShareMode.end() && *iter == i){
            if(!fileShareModeModel->data(idx, Qt::DisplayRole).toString().contains("√"))
                fileShareModeModel->setData(idx, fileShareModeModel->data(idx).toString() + " √", Qt::DisplayRole);
            fileShareModeModel->data(idx, Qt::BackgroundRole);
            iter++;
        }else{
            if(fileShareModeModel->data(idx, Qt::DisplayRole).toString().contains("√")){
                auto s = fileShareModeModel->data(idx).toString();
                fileShareModeModel->setData(idx, s.mid(0, s.length() - 2), Qt::DisplayRole);
            }
            fileShareModeModel->data(idx, Qt::BackgroundRole);
        }
    }

    list<int> fileCreateDisp = analyser->getCreateDisp(handle.createDisp);
    iter = fileCreateDisp.begin();
    for(int i=1; i<=FILECREATEDISP_COUNT; i++){
        auto idx = fileCreateDispModel->index(i);
        if(iter != fileCreateDisp.end() && *iter == i){
            if(!fileCreateDispModel->data(idx, Qt::DisplayRole).toString().contains("√"))
                fileCreateDispModel->setData(idx, fileCreateDispModel->data(idx).toString() + " √", Qt::DisplayRole);
            fileCreateDispModel->data(idx, Qt::BackgroundRole);
            iter++;
        }else{
            if(fileCreateDispModel->data(idx, Qt::DisplayRole).toString().contains("√")){
                auto s = fileCreateDispModel->data(idx).toString();
                fileCreateDispModel->setData(idx, s.mid(0, s.length() - 2), Qt::DisplayRole);
            }
            fileCreateDispModel->data(idx, Qt::BackgroundRole);
        }
    }

    list<int> fileFlagAttr = analyser->getFileAttr(handle.flagAttr);
    iter = fileFlagAttr.begin();
    for(int i=1; i<=FILEFLAGATTR; i++){
        auto idx = fileFlagAttrModel->index(i);
        if(iter != fileFlagAttr.end() && *iter == i){
            if(!fileFlagAttrModel->data(idx, Qt::DisplayRole).toString().contains("√"))
                fileFlagAttrModel->setData(idx, fileFlagAttrModel->data(idx).toString() + " √", Qt::DisplayRole);
            fileFlagAttrModel->data(idx, Qt::BackgroundRole);
            iter++;
        }else{
            if(fileFlagAttrModel->data(idx, Qt::DisplayRole).toString().contains("√")){
                auto s = fileFlagAttrModel->data(idx).toString();
                fileFlagAttrModel->setData(idx, s.mid(0, s.length() - 2), Qt::DisplayRole);
            }
            fileFlagAttrModel->data(idx, Qt::BackgroundRole);
        }
    }
}

