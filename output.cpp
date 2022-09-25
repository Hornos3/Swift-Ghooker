#include "Util.h"
#include "output.h"
#include "hookanalysis.h"
#include <QStringLiteral>
#include "ui_output.h"
#include <QReadWriteLock>
#include <fstream>
#include <QMutex>
using namespace std;

extern bool (*getMutexSignal)(void);
extern void (*setMutexSignal)(void);

std::recursive_mutex mutexLock;
int counter = 0;

Output::Output(std::vector<bool> choices, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Output)
{
    ui->setupUi(this);
    ui->logInfo->setReadOnly(true);
    this->setFixedSize(1500, 800);
    this->setWindowIcon(QIcon(":/background/icon.ico"));

    ui->regeditView->header()->setStyleSheet("background-color:rgba(0,0,0,0)");

    QStringList verticalList;
    verticalList << "进程名" << "文件描述" << "版本" << "内部名" << "公司名" << "版权" << "原始名" << "产品版本";
    QStringList horizontalList;
    horizontalList << "值";
    model->setHorizontalHeaderLabels(horizontalList);
    model->setVerticalHeaderLabels(verticalList);

    heapViewModel->setHorizontalHeaderLabels(QStringList() << "地址" << "类别" << "大小" << "状态");
    fileViewModel->setHorizontalHeaderLabels(QStringList() << "句柄指针/操作编号" << "文件名/操作类型" << "状态/字节数" << "成功字节数" << "是否成功");
    exceptionModel->setHorizontalHeaderLabels(QStringList() << "异常操作编号" << "异常类型" << "详细信息");
    regeditModel->setHorizontalHeaderLabels(QStringList() << "句柄/操作编号" << "键值/操作类型" << "状态/操作细节" << "操作返回");
    netModel->setHorizontalHeaderLabels(QStringList() << "SOCKET地址/操作编号" << "通信类型"
                                        << "IP/目标SOCKET地址" << "端口号/该套接字对应IP与端口" << "目标套接字对应IP与端口" << "消息长度");
    memoryModel->setHorizontalHeaderLabels(QStringList() << "地址/操作ID" << "长度" << "保存内容/操作类型" << "来源/去向");
    moduleModel->setHorizontalHeaderLabels(QStringList() << "模块名" << "基地址" << "入口地址" << "大小" << "文件所在路径");
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

    fWdg = new fileWidget(fileViewModel, fileAccessModel, fileShareModeModel, fileCreateDispModel, fileFlagAttrModel, analyser);
    rWdg = new regWidget(regeditModel);
    hWdg = new heapWidget(heapViewModel);
    eWdg = new exceptionWidget(exceptionModel);
    lWdg = new logWidget(&analyser->logList, logWidgetModel);
    mWdg = new memoryWidget(&analyser->keyMemories, memoryModel);
    nWdg = new netWidget(netModel);
    moWdg = new moduleWidget(moduleModel);

    ui->processInfo->setModel(model);
    ui->heapView->setModel(heapViewModel);
    ui->fileView->setModel(fileViewModel);
    ui->exceptionWindow->setModel(exceptionModel);
    ui->netView->setModel(netModel);
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

    initialize(choices);

    // ui->processInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    watcher->addPath("./hookLog/lasthook.tmp");
    connect(this->watcher, &QFileSystemWatcher::fileChanged, this, &Output::updateLog);
    connect(ui->currentStep, &QSpinBox::editingFinished, this, [=](){changeStep(ui->currentStep->value());});
}

void Output::initialize(std::vector<bool> choices){
    injectionOptions = choices;
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
    analyser->regHandlesExpl.insert({(uint64_t)HKEY_CURRENT_CONFIG, map<unsigned, pair<QString, bool>>()});
    analyser->regHandlesExpl.insert({(uint64_t)HKEY_CURRENT_USER, map<unsigned, pair<QString, bool>>()});
    analyser->regHandlesExpl.insert({(uint64_t)HKEY_CLASSES_ROOT, map<unsigned, pair<QString, bool>>()});
    analyser->regHandlesExpl.insert({(uint64_t)HKEY_LOCAL_MACHINE, map<unsigned, pair<QString, bool>>()});
    analyser->regHandlesExpl.insert({(uint64_t)HKEY_USERS, map<unsigned, pair<QString, bool>>()});
    if(choices.size() == 0)
        return;
    analyser->injMessageBoxA = choices[0];
    analyser->injMessageBoxW = choices[1];
    analyser->injHeapCreate = choices[2];
    analyser->injHeapDestroy = choices[3];
    analyser->injHeapAlloc = choices[4];
    analyser->injHeapFree = choices[5];
    analyser->injCreateFile = choices[6];
    analyser->injReadFile = choices[7];
    analyser->injWriteFile = choices[8];
    analyser->injCloseHandle = choices[9];
    analyser->injRegCreateKeyEx = choices[10];
    analyser->injRegSetValueEx = choices[11];
    analyser->injRegDeleteValue = choices[12];
    analyser->injRegCloseKey = choices[13];
    analyser->injRegOpenKeyEx = choices[14];
    analyser->injRegDeleteKeyEx = choices[15];
    analyser->injSend = choices[16];
    analyser->injRecv = choices[17];
    analyser->injBind = choices[18];
    analyser->injConnect = choices[19];
    analyser->injSocket = choices[20];
    analyser->injAccept = choices[21];
    analyser->analyseHeap = choices[2] && choices[3] && choices[4] && choices[5];
    analyser->analyseFile = choices[6] && choices[7] && choices[8] && choices[9];
    analyser->analyseReg = choices[10] && choices[11] && choices[12] && choices[13] && choices[14] && choices[15];
    analyser->analyseNet = choices[16] && choices[17] && choices[18] && choices[19] && choices[20] && choices[21];
}

QString Output::getInjOptions(){
    QString ret = "injected APIs:\n";
    ret += injectionOptions[0] ? "\tMessageBoxA\n" : "";
    ret += injectionOptions[1] ? "\tMessageBoxW\n" : "";
    ret += injectionOptions[2] ? "\tHeapCreate\n" : "";
    ret += injectionOptions[3] ? "\tHeapDestroy\n" : "";
    ret += injectionOptions[4] ? "\tHeapAlloc\n" : "";
    ret += injectionOptions[5] ? "\tHeapFree\n" : "";
    ret += injectionOptions[6] ? "\tCreateFile\n" : "";
    ret += injectionOptions[7] ? "\tReadFile\n" : "";
    ret += injectionOptions[8] ? "\tWriteFile\n" : "";
    ret += injectionOptions[9] ? "\tCloseHandle\n" : "";
    ret += injectionOptions[10] ? "\tRegCreateKeyEx\n" : "";
    ret += injectionOptions[11] ? "\tRegSetValueEx\n" : "";
    ret += injectionOptions[12] ? "\tRegDeleteValue\n" : "";
    ret += injectionOptions[13] ? "\tRegCloseKey\n" : "";
    ret += injectionOptions[14] ? "\tRegOpenKeyEx\n" : "";
    ret += injectionOptions[15] ? "\tRegDeleteKeyEx\n" : "";
    ret += injectionOptions[16] ? "\tSend\n" : "";
    ret += injectionOptions[17] ? "\tRecv\n" : "";
    ret += injectionOptions[18] ? "\tBind\n" : "";
    ret += injectionOptions[19] ? "\tConnect\n" : "";
    ret += injectionOptions[20] ? "\tSocket\n" : "";
    ret += injectionOptions[21] ? "\tAccept\n" : "";
    return ret;
}

void Output::updateLogCount(){
    ui->currentStep->setMaximum(analyser->logList.size());
    ui->currentStep->setValue(analyser->logList.size());
    ui->stepCount->setMaximum(analyser->logList.size());
    ui->stepCount->setValue(analyser->logList.size());
}

void Output::updateLog(){
//    lock->lockForWrite();
    // mutexLock.lock();
    while(!getMutexSignal()){
        // QCoreApplication::processEvents();
        if(!injThread || injThread->isFinished())
            return;
    }

    ifstream in("./hookLog/lasthook.tmp");
    char newLog[0x800] = {0};
    in.read(newLog, 0x800);
    in.close();

    unsigned fileSize = 0;
    char* buf = nullptr;
    QFileInfo i("./hookLog/lastCap.dat");
    fileSize = i.size();
    if(fileSize != 0){
        ifstream binaryIn("./hookLog/lastCap.dat");
        buf = (char*)calloc(fileSize, 1);
        binaryIn.read(buf, fileSize);
        binaryIn.close();
    }

    setMutexSignal();

    // mutexLock.unlock();

    wstring logW = stringTowstring(newLog);
    QString logQ = QString::fromStdWString(logW);
    if(logQ.isEmpty()){
        return;
    }
    bool exeInfoGot = analyser->exeInfoGot;

    bool mayRepeat = analyser->appendRecord(logQ, buf);

    if(exeInfoGot ^ analyser->exeInfoGot){
        showExeInfo();
        assert(ui->logInfo->toPlainText().isEmpty());
        ui->logInfo->appendPlainText(analyser->exeInfo.to_string() + getInjOptions() +
                                     "\n************************************************************\n");
    }
    if(!mayRepeat){
        trimExeInfo(logQ);
        ui->logInfo->appendPlainText(logQ);
        ui->currentStep->setMaximum(ui->currentStep->maximum() + 1);
        ui->currentStep->setValue(ui->currentStep->value() + 1);
        ui->stepCount->setMaximum(ui->stepCount->maximum() + 1);
        ui->stepCount->setValue(ui->stepCount->value() + 1);
        ++tracer->totalStep;
        ++tracer->currentStep;
    }else
        qDebug() << ++counter;
    in.close();
//    lock->unlock();
}

void Output::closeEvent(QCloseEvent * event){
    if(loadHistory){
        event->accept();
        delete this->analyser;
        delete this;
        return;
    }
    if(injThread != nullptr && !injThread->isFinished()){
        QMessageBox::warning(this, "警告", "目标进程还没有执行完毕，无法关闭此分析界面！");
        event->ignore();
        return;
    }
    QMessageBox::information(this, "退出", "本次分析结果会保存到文件系统中，但下一次分析可能会进行覆盖，请注意备份。");
    saveFullLog();
    event->accept();
    delete this->analyser;
    delete this;
}

void Output::saveFullLog(){
    QString allLog = ui->logInfo->toPlainText();

    ofstream logFile("./hookLog/hookLog_complete.log", ios::trunc);
    QString header = ("hook count: " + to_string(analyser->logList.size()) + "\n").c_str();
    logFile.write(header.toStdString().c_str(), header.length());
    logFile.write(allLog.toStdString().c_str(), allLog.length());
    logFile.close();

    // 内存内容保存格式：
    // 8字节内存地址 + 4字节操作id + 4字节内存长度 + 4字节内存类型 + 内容
    ofstream memCapFile("./hookLog/memoryCaps.mcp", ios::trunc | ios::binary);
    for(const auto &outer : analyser->keyMemories){
        uint64_t memAddr = outer.first;
        for(const auto &inner : outer.second){
            int id = inner.first;
            int bufLen = inner.second.length;
            int type = (int)inner.second.type;
            memCapFile.write((char*)(&memAddr), 8);
            memCapFile.write((char*)(&id), 4);
            memCapFile.write((char*)(&bufLen), 4);
            memCapFile.write((char*)(&type), 4);
            memCapFile.write(inner.second.content, bufLen);
        }
    }
    memCapFile.close();
}

void Output::trimExeInfo(QString& fullInfo){
    auto lines = fullInfo.split("\n");
    size_t startRemPos = 4;
    while(lines.at(startRemPos).startsWith("\t"))
        startRemPos++;
    lines.remove(startRemPos, 1);
    startRemPos++;
    lines.remove(startRemPos, 10);
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
    if(index.parent().isValid())
        return;

    fileHandleAttr handle = analyser->fileHandles[fileViewModel->item(selectedRow)->text().toULongLong(nullptr, 16)].rbegin()->second;
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


void Output::on_showFileWidget_clicked()
{
    fWdg->show();
}


void Output::on_showRegWidget_clicked()
{
    rWdg->show();
}


void Output::on_showHeapWidget_clicked()
{
    hWdg->show();
}


void Output::on_showExceptionWidget_clicked()
{
    eWdg->show();
}


void Output::on_showLogWidget_clicked()
{
    lWdg->show();
}


void Output::on_showMemoryWidget_clicked()
{
    mWdg->show();
}


void Output::on_showNetWidget_clicked()
{
    nWdg->show();
}


void Output::on_showModulesWidget_clicked()
{
    moWdg->show();
}


void Output::on_prevStep_clicked()
{
    if(ui->currentStep != 0){
        tracer->stepBack();
        ui->currentStep->setValue(ui->currentStep->value() - 1);
    }
}

void Output::on_nextStep_clicked()
{
    if(ui->currentStep != ui->stepCount){
        tracer->stepFront();
        ui->currentStep->setValue(ui->currentStep->value() + 1);
    }
}

void Output::appendLog(QString log){
    ui->logInfo->appendPlainText(log);
    ui->currentStep->setMaximum(ui->currentStep->maximum() + 1);
    ui->stepCount->setMaximum(ui->currentStep->maximum() + 1);
    ui->currentStep->setValue(ui->currentStep->maximum());
    ui->stepCount->setValue(ui->currentStep->maximum());
}

void Output::changeStep(int changedValue)
{
    if(executing)
        return;
    int originalStep = tracer->currentStep;
    if(originalStep < changedValue){
        while(originalStep < changedValue){
            tracer->stepFront();
            originalStep++;
        }
    }else{
        if(originalStep - changedValue < changedValue){ // 如果回溯需要的步骤少，就回溯
            while(originalStep > changedValue){
                tracer->stepBack();
                originalStep--;
            }
        }else{
            // TODO: 清空所有显示信息并从头开始推演
        }
    }
}

