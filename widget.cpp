#include "widget.h"
#include "ui_widget.h"
#include "Util.h"
#include "injectthread.h"
#include <QDebug>
#include <QMediaPlayer>

extern void (*InjMessageBoxAFunc)(bool);
extern void (*InjMessageBoxWFunc)(bool);
extern void (*InjHeapCreateFunc)(bool);
extern void (*InjHeapDestroyFunc)(bool);
extern void (*InjHeapAllocFunc)(bool);
extern void (*InjHeapFreeFunc)(bool);
extern void (*InjOpenFileFunc)(bool);
extern void (*InjCreateFileFunc)(bool);
extern void (*InjReadFileFunc)(bool);
extern void (*InjWriteFileFunc)(bool);
extern void (*InjCloseHandleFunc)(bool);
extern void (*InjRegCreateKeyExFunc)(bool);
extern void (*InjRegSetValueExFunc)(bool);
extern void (*InjRegDeleteValueFunc)(bool);
extern void (*InjRegCloseKeyFunc)(bool);
extern void (*InjRegOpenKeyExFunc)(bool);
extern void (*InjRegDeleteKeyExFunc)(bool);
extern void (*InjSendFunc)(bool);
extern void (*InjRecvFunc)(bool);
extern void (*InjConnectFunc)(bool);
extern void (*InjBindFunc)(bool);
extern void (*InjSocketFunc)(bool);
extern void (*InjAcceptFunc)(bool);

extern char* (*getLastHookBeforeCall)(void);

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(1200, 650);
    this->setWindowIcon(QIcon(":/background/icon.ico"));

    this->output = new Output();
    connect(ui->InjMessageBoxA, &QCheckBox::stateChanged, this, &Widget::setMessageBoxAInj);
    connect(ui->InjMessageBoxW, &QCheckBox::stateChanged, this, &Widget::setMessageBoxWInj);
    connect(ui->InjHeapCreate, &QCheckBox::stateChanged, this, &Widget::setHeapCreateInj);
    connect(ui->InjHeapDestroy, &QCheckBox::stateChanged, this, &Widget::setHeapDestroyInj);
    connect(ui->InjHeapAlloc, &QCheckBox::stateChanged, this, &Widget::setHeapAllocInj);
    connect(ui->InjHeapFree, &QCheckBox::stateChanged, this, &Widget::setHeapFreeInj);
    connect(ui->InjCreateFile, &QCheckBox::stateChanged, this, &Widget::setCreateFileInj);
    connect(ui->InjReadFile, &QCheckBox::stateChanged, this, &Widget::setReadFileInj);
    connect(ui->InjWriteFile, &QCheckBox::stateChanged, this, &Widget::setWriteFileInj);
    connect(ui->InjCloseHandle, &QCheckBox::stateChanged, this, &Widget::setCloseHandleInj);
    connect(ui->InjRegCreateKeyEx, &QCheckBox::stateChanged, this, &Widget::setRegCreateKeyExInj);
    connect(ui->InjRegSetValueEx, &QCheckBox::stateChanged, this, &Widget::setRegSetValueExInj);
    connect(ui->InjRegDeleteValue, &QCheckBox::stateChanged, this, &Widget::setRegDeleteValueInj);
    connect(ui->InjRegCloseKey, &QCheckBox::stateChanged, this, &Widget::setRegCloseKeyInj);
    connect(ui->InjRegOpenKeyEx, &QCheckBox::stateChanged, this, &Widget::setRegOpenKeyExInj);
    connect(ui->InjRegDeleteKeyEx, &QCheckBox::stateChanged, this, &Widget::setRegDeleteKeyExInj);
    connect(ui->InjSend, &QCheckBox::stateChanged, this, &Widget::setSendInj);
    connect(ui->InjRecv, &QCheckBox::stateChanged, this, &Widget::setRecvInj);
    connect(ui->InjBind, &QCheckBox::stateChanged, this, &Widget::setBindInj);
    connect(ui->InjConnect, &QCheckBox::stateChanged, this, &Widget::setConnectInj);
    connect(ui->InjSocket, &QCheckBox::stateChanged, this, &Widget::setSocketInj);
    connect(ui->InjAccept, &QCheckBox::stateChanged, this, &Widget::setAcceptInj);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_selectFile_clicked(){
    getFilenameAndShow(ui->targetFile, "executable file(*.exe)");
}

void Widget::on_startAnalysis_clicked(){
    if(output->uselessFlag)
        output = new Output(getAllChoices());
    output->initialize(getAllChoices());
    if(!ui->allHeap->isChecked()){
        QMessageBox::warning(this, "警告", "您没有打开所有堆操作API选项，堆空间实时监测将不可用。");
        output->analyser->analyseHeap = false;
    }
    if(!ui->allFile->isChecked()){
        QMessageBox::warning(this, "警告", "您没有打开所有文件操作API选项，文件相关操作监测将不可用。");
        output->analyser->analyseFile = false;
    }
    if(!ui->allReg->isChecked()){
        QMessageBox::warning(this, "警告", "您没有打开所有注册表操作API选项，注册表相关操作监测将不可用。");
        output->analyser->analyseReg = false;
    }
    if(!ui->allNet->isChecked()){
        QMessageBox::warning(this, "警告", "您没有打开所有网络操作API选项，网络相关操作监测将不可用。");
        output->analyser->analyseNet = false;
    }
    std::wstring filename = ui->targetFile->text().toStdWString();
    output->analyser->exeFileName = ui->targetFile->text();
    if(filename != L"......"){

        fstream fileTest(filename);
        if(!fileTest.good()){
            QMessageBox::warning(this, "警告", "选择的文件不存在！请检查该文件是否已经被删除。");
            fileTest.close();
            return;
        }
        fileTest.close();

        output->show();

        injectThread inject;
        output->injThread = &inject;
        inject.inputEXE = filename.c_str();
        connect(&inject, &QThread::finished, this, [](){
            QMessageBox::information(nullptr, "提示", "分析程序已经执行完毕。");
        });
        inject.start();

        while(!inject.isFinished()) QCoreApplication::processEvents();

        output->injThread = nullptr;
        output->updateLog();

        if(output->analyser->logList.size() < MAXIMUM_HOOK){
            QString suspectedLastHook = getLastHookBeforeCall();
            bool repeat = output->analyser->appendRecord(suspectedLastHook, nullptr, 0, true);
            if(!repeat){
                output->trimExeInfo(suspectedLastHook);
                output->appendLog(suspectedLastHook + "Return Value: (UNKNOWN) 0xDEADBEEFCAFEBABE / <THIS_API_CAUSES_CRUSH>\n"
                                                      "----------------------------------------------------\n");
            }
        }

        if(output->analyser->analyseHeap){
            if(output->analyser->chunksExpl->empty())
                QMessageBox::information(this, "堆监控总结", "程序结束前，所有堆块都已被释放");
            else{
                QString info = "程序结束前，有";
                int unleashed = std::count_if(output->analyser->chunksExpl->begin(),
                                              output->analyser->chunksExpl->end(),
                                              [](pair<uint64_t, std::map<unsigned, bool>> p)->bool{
                                                  return p.second.rbegin()->second;
                                              });
                info += to_string(unleashed).c_str();
                info += "个堆块没有被释放。";
                if(unleashed > 50){
                    info += "未被释放的堆块过多，无法展示，请移步堆监控查看，此时这里保存有程序执行最后一刻的堆空间布局监控情况";
                }else{
                    if(unleashed){
                        info += "分别为：";
                        for(const auto &address : *output->analyser->chunksExpl){
                            if(address.second.rbegin()->second){
                                info += ull2a(address.first);
                                info += ", ";
                            }
                        }
                    }
                    info += "有关堆块更加详细的信息，请移步堆监控进行查看。";
                }
                QMessageBox::information(this, "堆监控总结", info);
            }
        }
        output->executing = false;
    }
    else
        QMessageBox::warning(this, "未指定文件", "你还没有指定要分析的可执行文件，请先指定可执行文件！");
}

void Widget::on_startAnalyseHistory_clicked()
{
    if(output->uselessFlag)
        output = new Output();
    output->loadHistory = true;
    output->executing = false;
    output->tracer->initialize();
    output->updateLogCount();
    output->analyser->getModules();
}

std::vector<bool> Widget::getAllChoices(){
    std::vector<bool> choices;
    choices.push_back(ui->InjMessageBoxA->isChecked());
    choices.push_back(ui->InjMessageBoxW->isChecked());
    choices.push_back(ui->InjHeapCreate->isChecked());
    choices.push_back(ui->InjHeapDestroy->isChecked());
    choices.push_back(ui->InjHeapAlloc->isChecked());
    choices.push_back(ui->InjHeapFree->isChecked());
    choices.push_back(ui->InjCreateFile->isChecked());
    choices.push_back(ui->InjReadFile->isChecked());
    choices.push_back(ui->InjWriteFile->isChecked());
    choices.push_back(ui->InjCloseHandle->isChecked());
    choices.push_back(ui->InjRegCreateKeyEx->isChecked());
    choices.push_back(ui->InjRegSetValueEx->isChecked());
    choices.push_back(ui->InjRegDeleteValue->isChecked());
    choices.push_back(ui->InjRegCloseKey->isChecked());
    choices.push_back(ui->InjRegOpenKeyEx->isChecked());
    choices.push_back(ui->InjRegDeleteKeyEx->isChecked());
    choices.push_back(ui->InjSend->isChecked());
    choices.push_back(ui->InjRecv->isChecked());
    choices.push_back(ui->InjBind->isChecked());
    choices.push_back(ui->InjConnect->isChecked());
    choices.push_back(ui->InjSocket->isChecked());
    choices.push_back(ui->InjAccept->isChecked());
    return choices;
}

void Widget::setMessageBoxAInj(){
    bool choice = ui->InjMessageBoxA->isChecked();
    if(choice && ui->InjMessageBoxW->isChecked())
        ui->allMessageBox->setChecked(true);
    else
        ui->allMessageBox->setChecked(false);
    output->analyser->injMessageBoxA = choice;
    InjMessageBoxAFunc(choice);
}

void Widget::setMessageBoxWInj(){
    bool choice = ui->InjMessageBoxW->isChecked();
    if(choice && ui->InjMessageBoxA->isChecked())
        ui->allMessageBox->setChecked(true);
    else
        ui->allMessageBox->setChecked(false);
    output->analyser->injMessageBoxW = choice;
    InjMessageBoxWFunc(choice);
}

void Widget::setHeapCreateInj(){
    bool choice = ui->InjHeapCreate->isChecked();
    if(ui->InjHeapCreate->isChecked() && ui->InjHeapAlloc->isChecked() &&
            ui->InjHeapFree->isChecked() && ui->InjHeapDestroy->isChecked())
        ui->allHeap->setChecked(true);
    else
        ui->allHeap->setChecked(false);
    output->analyser->injHeapCreate = choice;
    InjHeapCreateFunc(choice);
}

void Widget::setHeapDestroyInj(){
    bool choice = ui->InjHeapDestroy->isChecked();
    if(ui->InjHeapCreate->isChecked() && ui->InjHeapAlloc->isChecked() &&
            ui->InjHeapFree->isChecked() && ui->InjHeapDestroy->isChecked())
        ui->allHeap->setChecked(true);
    else
        ui->allHeap->setChecked(false);
    output->analyser->injHeapDestroy = choice;
    InjHeapDestroyFunc(choice);
}

void Widget::setHeapAllocInj(){
    bool choice = ui->InjHeapAlloc->isChecked();
    if(ui->InjHeapCreate->isChecked() && ui->InjHeapAlloc->isChecked() &&
            ui->InjHeapFree->isChecked() && ui->InjHeapDestroy->isChecked())
        ui->allHeap->setChecked(true);
    else
        ui->allHeap->setChecked(false);
    output->analyser->injHeapAlloc = choice;
    InjHeapAllocFunc(choice);
}

void Widget::setHeapFreeInj(){
    bool choice = ui->InjHeapFree->isChecked();
    if(ui->InjHeapCreate->isChecked() && ui->InjHeapAlloc->isChecked() &&
            ui->InjHeapFree->isChecked() && ui->InjHeapDestroy->isChecked())
        ui->allHeap->setChecked(true);
    else
        ui->allHeap->setChecked(false);
    output->analyser->injHeapFree = choice;
    InjHeapFreeFunc(choice);
}

void Widget::setCreateFileInj(){
    bool choice = ui->InjCreateFile->isChecked();
    if(ui->InjCreateFile->isChecked() && ui->InjReadFile->isChecked() &&
            ui->InjWriteFile->isChecked() && ui->InjCloseHandle->isChecked())
        ui->allFile->setChecked(true);
    else
        ui->allFile->setChecked(false);
    output->analyser->injCreateFile = choice;
    InjCreateFileFunc(choice);
}

void Widget::setReadFileInj(){
    bool choice = ui->InjReadFile->isChecked();
    if(ui->InjCreateFile->isChecked() && ui->InjReadFile->isChecked() &&
            ui->InjWriteFile->isChecked() && ui->InjCloseHandle->isChecked())
        ui->allFile->setChecked(true);
    else
        ui->allFile->setChecked(false);
    output->analyser->injReadFile = choice;
    InjReadFileFunc(choice);
}

void Widget::setWriteFileInj(){
    bool choice = ui->InjWriteFile->isChecked();
    if(ui->InjCreateFile->isChecked() && ui->InjReadFile->isChecked() &&
            ui->InjWriteFile->isChecked() && ui->InjCloseHandle->isChecked())
        ui->allFile->setChecked(true);
    else
        ui->allFile->setChecked(false);
    output->analyser->injWriteFile = choice;
    InjWriteFileFunc(choice);
}

void Widget::setCloseHandleInj(){
    bool choice = ui->InjCloseHandle->isChecked();
    if(ui->InjCreateFile->isChecked() && ui->InjReadFile->isChecked() &&
            ui->InjWriteFile->isChecked() && ui->InjCloseHandle->isChecked())
        ui->allFile->setChecked(true);
    else
        ui->allFile->setChecked(false);
    output->analyser->injCloseHandle = choice;
    InjCloseHandleFunc(choice);
}

void Widget::setRegCreateKeyExInj(){
    bool choice = ui->InjRegCreateKeyEx->isChecked();
    if(ui->InjRegCreateKeyEx->isChecked() && ui->InjRegCloseKey->isChecked() &&
            ui->InjRegDeleteKeyEx->isChecked() && ui->InjRegDeleteValue->isChecked() &&
            ui->InjRegSetValueEx->isChecked() && ui->InjRegOpenKeyEx->isChecked())
        ui->allReg->setChecked(true);
    else
        ui->allReg->setChecked(false);
    output->analyser->injRegCreateKeyEx = choice;
    InjRegCreateKeyExFunc(choice);
}

void Widget::setRegSetValueExInj(){
    bool choice = ui->InjRegSetValueEx->isChecked();
    if(ui->InjRegCreateKeyEx->isChecked() && ui->InjRegCloseKey->isChecked() &&
            ui->InjRegDeleteKeyEx->isChecked() && ui->InjRegDeleteValue->isChecked() &&
            ui->InjRegSetValueEx->isChecked() && ui->InjRegOpenKeyEx->isChecked())
        ui->allReg->setChecked(true);
    else
        ui->allReg->setChecked(false);
    output->analyser->injRegSetValueEx = choice;
    InjRegSetValueExFunc(choice);
}

void Widget::setRegDeleteValueInj(){
    bool choice = ui->InjRegDeleteValue->isChecked();
    if(ui->InjRegCreateKeyEx->isChecked() && ui->InjRegCloseKey->isChecked() &&
            ui->InjRegDeleteKeyEx->isChecked() && ui->InjRegDeleteValue->isChecked() &&
            ui->InjRegSetValueEx->isChecked() && ui->InjRegOpenKeyEx->isChecked())
        ui->allReg->setChecked(true);
    else
        ui->allReg->setChecked(false);
    output->analyser->injRegDeleteValue = choice;
    InjRegDeleteValueFunc(choice);
}

void Widget::setRegCloseKeyInj(){
    bool choice = ui->InjRegCloseKey->isChecked();
    if(ui->InjRegCreateKeyEx->isChecked() && ui->InjRegCloseKey->isChecked() &&
            ui->InjRegDeleteKeyEx->isChecked() && ui->InjRegDeleteValue->isChecked() &&
            ui->InjRegSetValueEx->isChecked() && ui->InjRegOpenKeyEx->isChecked())
        ui->allReg->setChecked(true);
    else
        ui->allReg->setChecked(false);
    output->analyser->injRegCloseKey = choice;
    InjRegCloseKeyFunc(choice);
}

void Widget::setRegOpenKeyExInj(){
    bool choice = ui->InjRegOpenKeyEx->isChecked();
    if(ui->InjRegCreateKeyEx->isChecked() && ui->InjRegCloseKey->isChecked() &&
            ui->InjRegDeleteKeyEx->isChecked() && ui->InjRegDeleteValue->isChecked() &&
            ui->InjRegSetValueEx->isChecked() && ui->InjRegOpenKeyEx->isChecked())
        ui->allReg->setChecked(true);
    else
        ui->allReg->setChecked(false);
    output->analyser->injRegOpenKeyEx = choice;
    InjRegOpenKeyExFunc(choice);
}

void Widget::setRegDeleteKeyExInj(){
    bool choice = ui->InjRegDeleteKeyEx->isChecked();
    if(ui->InjRegCreateKeyEx->isChecked() && ui->InjRegCloseKey->isChecked() &&
            ui->InjRegDeleteKeyEx->isChecked() && ui->InjRegDeleteValue->isChecked() &&
            ui->InjRegSetValueEx->isChecked() && ui->InjRegOpenKeyEx->isChecked())
        ui->allReg->setChecked(true);
    else
        ui->allReg->setChecked(false);
    output->analyser->injRegDeleteKeyEx = choice;
    InjRegDeleteKeyExFunc(choice);
}

void Widget::setSendInj(){
    bool choice = ui->InjSend->isChecked();
    if(ui->InjSend->isChecked() && ui->InjBind->isChecked() && ui->InjRecv->isChecked() &&
            ui->InjConnect->isChecked() && ui->InjSocket->isChecked() && ui->InjAccept->isChecked())
        ui->allNet->setChecked(true);
    else
        ui->allNet->setChecked(false);
    output->analyser->injSend = choice;
    InjSendFunc(choice);
}

void Widget::setRecvInj(){
    bool choice = ui->InjRecv->isChecked();
    if(ui->InjSend->isChecked() && ui->InjBind->isChecked() && ui->InjRecv->isChecked() &&
            ui->InjConnect->isChecked() && ui->InjSocket->isChecked() && ui->InjAccept->isChecked())
        ui->allNet->setChecked(true);
    else
        ui->allNet->setChecked(false);
    output->analyser->injRecv = choice;
    InjRecvFunc(choice);
}

void Widget::setBindInj(){
    bool choice = ui->InjBind->isChecked();
    if(ui->InjSend->isChecked() && ui->InjBind->isChecked() && ui->InjRecv->isChecked() &&
            ui->InjConnect->isChecked() && ui->InjSocket->isChecked() && ui->InjAccept->isChecked())
        ui->allNet->setChecked(true);
    else
        ui->allNet->setChecked(false);
    output->analyser->injBind = choice;
    InjBindFunc(choice);
}

void Widget::setConnectInj(){
    bool choice = ui->InjConnect->isChecked();
    if(ui->InjSend->isChecked() && ui->InjBind->isChecked() && ui->InjRecv->isChecked() &&
            ui->InjConnect->isChecked() && ui->InjSocket->isChecked() && ui->InjAccept->isChecked())
        ui->allNet->setChecked(true);
    else
        ui->allNet->setChecked(false);
    output->analyser->injConnect = choice;
    InjConnectFunc(choice);
}

void Widget::setSocketInj(){
    bool choice = ui->InjSocket->isChecked();
    if(ui->InjSend->isChecked() && ui->InjBind->isChecked() && ui->InjRecv->isChecked() &&
            ui->InjConnect->isChecked() && ui->InjSocket->isChecked() && ui->InjAccept->isChecked())
        ui->allNet->setChecked(true);
    else
        ui->allNet->setChecked(false);
    output->analyser->injSocket = choice;
    InjSocketFunc(choice);
}

void Widget::setAcceptInj(){
    bool choice = ui->InjAccept->isChecked();
    if(ui->InjSend->isChecked() && ui->InjBind->isChecked() && ui->InjRecv->isChecked() &&
            ui->InjConnect->isChecked() && ui->InjSocket->isChecked() && ui->InjAccept->isChecked())
        ui->allNet->setChecked(true);
    else
        ui->allNet->setChecked(false);
    output->analyser->injAccept = choice;
    InjAcceptFunc(choice);
}

void Widget::on_allMessageBox_clicked()
{
    if(ui->allMessageBox->isChecked()){
        ui->InjMessageBoxA->setChecked(true);
        ui->InjMessageBoxW->setChecked(true);
    }else{
        ui->InjMessageBoxA->setChecked(false);
        ui->InjMessageBoxW->setChecked(false);
    }
}

void Widget::on_allHeap_clicked()
{
    if(ui->allHeap->isChecked()){
        ui->InjHeapAlloc->setChecked(true);
        ui->InjHeapCreate->setChecked(true);
        ui->InjHeapDestroy->setChecked(true);
        ui->InjHeapFree->setChecked(true);
    }else{
        ui->InjHeapAlloc->setChecked(false);
        ui->InjHeapCreate->setChecked(false);
        ui->InjHeapDestroy->setChecked(false);
        ui->InjHeapFree->setChecked(false);
    }
}


void Widget::on_allFile_clicked()
{
    if(ui->allFile->isChecked()){
        ui->InjCreateFile->setChecked(true);
        ui->InjReadFile->setChecked(true);
        ui->InjWriteFile->setChecked(true);
        ui->InjCloseHandle->setChecked(true);
    }else{
        ui->InjCreateFile->setChecked(false);
        ui->InjReadFile->setChecked(false);
        ui->InjWriteFile->setChecked(false);
        ui->InjCloseHandle->setChecked(false);
    }
}


void Widget::on_allReg_clicked()
{
    if(ui->allReg->isChecked()){
        ui->InjRegCreateKeyEx->setChecked(true);
        ui->InjRegCloseKey->setChecked(true);
        ui->InjRegDeleteKeyEx->setChecked(true);
        ui->InjRegDeleteValue->setChecked(true);
        ui->InjRegSetValueEx->setChecked(true);
        ui->InjRegOpenKeyEx->setChecked(true);
    }else{
        ui->InjRegCreateKeyEx->setChecked(false);
        ui->InjRegCloseKey->setChecked(false);
        ui->InjRegDeleteKeyEx->setChecked(false);
        ui->InjRegDeleteValue->setChecked(false);
        ui->InjRegSetValueEx->setChecked(false);
        ui->InjRegOpenKeyEx->setChecked(false);
    }
}


void Widget::on_allNet_clicked()
{
    if(ui->allNet->isChecked()){
        ui->InjSend->setChecked(true);
        ui->InjRecv->setChecked(true);
        ui->InjConnect->setChecked(true);
        ui->InjBind->setChecked(true);
        ui->InjSocket->setChecked(true);
        ui->InjAccept->setChecked(true);
    }else{
        ui->InjSend->setChecked(false);
        ui->InjRecv->setChecked(false);
        ui->InjConnect->setChecked(false);
        ui->InjBind->setChecked(false);
        ui->InjSocket->setChecked(false);
        ui->InjAccept->setChecked(false);
    }
}


