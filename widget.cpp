#include "widget.h"
#include "ui_widget.h"
#include "Util.h"
#include "injectthread.h"
#include <QDebug>

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

extern char* (*getLastHookBeforeCall)(void);

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->output = new Output(nullptr);
    connect(ui->InjMessageBoxA, &QCheckBox::stateChanged, this, &Widget::setMessageBoxAInj);
    connect(ui->InjMessageBoxW, &QCheckBox::stateChanged, this, &Widget::setMessageBoxWInj);
    connect(ui->InjHeapCreate, &QCheckBox::stateChanged, this, &Widget::setHeapCreateInj);
    connect(ui->InjHeapDestroy, &QCheckBox::stateChanged, this, &Widget::setHeapDestroyInj);
    connect(ui->InjHeapAlloc, &QCheckBox::stateChanged, this, &Widget::setHeapAllocInj);
    connect(ui->InjHeapFree, &QCheckBox::stateChanged, this, &Widget::setHeapFreeInj);
    connect(ui->InjOpenFile, &QCheckBox::stateChanged, this, &Widget::setOpenFileInj);
    connect(ui->InjCreateFile, &QCheckBox::stateChanged, this, &Widget::setCreateFileInj);
    connect(ui->InjReadFile, &QCheckBox::stateChanged, this, &Widget::setReadFileInj);
    connect(ui->InjWriteFile, &QCheckBox::stateChanged, this, &Widget::setWriteFileInj);
    connect(ui->InjCloseHandle, &QCheckBox::stateChanged, this, &Widget::setCloseHandleInj);
    connect(ui->InjRegCreateKeyEx, &QCheckBox::stateChanged, this, &Widget::setRegCreateKeyExInj);
    connect(ui->InjRegSetValueEx, &QCheckBox::stateChanged, this, &Widget::setRegSetValueExInj);
    connect(ui->InjRegDeleteValue, &QCheckBox::stateChanged, this, &Widget::setRegDeleteValueInj);
    connect(ui->InjRegCloseKey, &QCheckBox::stateChanged, this, &Widget::setRegCloseKeyInj);
    connect(ui->InjRegOpenKeyEx, &QCheckBox::stateChanged, this, &Widget::setRegOpenKeyExInj);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_selectFile_clicked(){
    getFilenameAndShow(ui->targetFile, "executable file(*.exe)");
}

void Widget::on_startAnalysis_clicked(){
    if(!ui->InjHeapAlloc->isChecked() || !ui->InjHeapCreate->isChecked() ||
            !ui->InjHeapDestroy->isChecked() || !ui->InjHeapFree->isChecked()){
        QMessageBox::warning(this, "警告", "您没有打开所有堆操作API选项，堆空间实时监测将不可用。");
        output->analyser->analyseHeap = false;
    }
    if(!ui->InjOpenFile->isChecked() || !ui->InjCreateFile->isChecked() ||
            !ui->InjReadFile->isChecked() || !ui->InjWriteFile->isChecked()){
        QMessageBox::warning(this, "警告", "您没有打开所有文件操作API选项，文件相关操作监测将不可用。");
        output->analyser->analyseFile = false;
    }
    std::wstring filename = ui->targetFile->text().toStdWString();
    output->analyser->exeFileName = ui->targetFile->text();
    if(filename != L"......"){
        output->show();

        injectThread inject;
        inject.inputEXE = filename.c_str();
        connect(&inject, &QThread::finished, this, [](){
            QMessageBox::information(nullptr, "提示", "分析程序已经执行完毕。");
        });
        inject.start();

        while(!inject.isFinished())
            QCoreApplication::processEvents();

        output->updateLog();

        QString suspectedLastHook = getLastHookBeforeCall();
        output->analyser->appendRecord(suspectedLastHook, true);

        if(output->analyser->analyseHeap){
            if(output->analyser->chunksExpl->empty())
                QMessageBox::information(this, "堆监控总结", "程序结束前，所有堆块都已被释放");
            else{
                QString info = "程序结束前，有";
                info += to_string(output->analyser->chunksExpl->size()).c_str();
                info += "个堆块没有被释放。";
                if(output->analyser->chunksExpl->size() > 50){
                    info += "未被释放的堆块过多，无法展示，请移步堆监控查看，此时这里保存有程序执行最后一刻的堆空间布局监控情况";
                }else{
                    info += "分别为：";
                    for(uint64_t address : *output->analyser->chunksExpl){
                        info += ull2a(address);
                        info += ", ";
                    }
                    info += "有关堆块更加详细的信息，请移步堆监控进行查看。";
                }
                QMessageBox::information(this, "堆监控总结", info);
            }
        }
    }
    else
        QMessageBox::warning(this, "未指定文件", "你还没有指定要分析的可执行文件，请先指定可执行文件！");
}

void Widget::setMessageBoxAInj(){
    bool choice = ui->InjMessageBoxA->isChecked();
    output->analyser->injMessageBoxA = choice;
    InjMessageBoxAFunc(choice);
}

void Widget::setMessageBoxWInj(){
    bool choice = ui->InjMessageBoxW->isChecked();
    output->analyser->injMessageBoxW = choice;
    InjMessageBoxWFunc(choice);
}

void Widget::setHeapCreateInj(){
    bool choice = ui->InjHeapCreate->isChecked();
    output->analyser->injHeapCreate = choice;
    InjHeapCreateFunc(choice);
}

void Widget::setHeapDestroyInj(){
    bool choice = ui->InjHeapDestroy->isChecked();
    output->analyser->injHeapDestroy = choice;
    InjHeapDestroyFunc(choice);
}

void Widget::setHeapAllocInj(){
    bool choice = ui->InjHeapAlloc->isChecked();
    output->analyser->injHeapAlloc = choice;
    InjHeapAllocFunc(choice);
}

void Widget::setHeapFreeInj(){
    bool choice = ui->InjHeapFree->isChecked();
    output->analyser->injHeapFree = choice;
    InjHeapFreeFunc(choice);
}

void Widget::setOpenFileInj(){
    bool choice = ui->InjOpenFile->isChecked();
    output->analyser->injOpenFile = choice;
    InjOpenFileFunc(choice);
}

void Widget::setCreateFileInj(){
    bool choice = ui->InjCreateFile->isChecked();
    output->analyser->injCreateFile = choice;
    InjCreateFileFunc(choice);
}

void Widget::setReadFileInj(){
    bool choice = ui->InjReadFile->isChecked();
    output->analyser->injReadFile = choice;
    InjReadFileFunc(choice);
}

void Widget::setWriteFileInj(){
    bool choice = ui->InjWriteFile->isChecked();
    output->analyser->injWriteFile = choice;
    InjWriteFileFunc(choice);
}

void Widget::setCloseHandleInj(){
    bool choice = ui->InjCloseHandle->isChecked();
    output->analyser->injCloseHandle = choice;
    InjCloseHandleFunc(choice);
}

void Widget::setRegCreateKeyExInj(){
    bool choice = ui->InjRegCreateKeyEx->isChecked();
    output->analyser->injRegCreateKeyEx = choice;
    InjRegCreateKeyExFunc(choice);
}

void Widget::setRegSetValueExInj(){
    bool choice = ui->InjRegSetValueEx->isChecked();
    output->analyser->injRegSetValueEx = choice;
    InjRegSetValueExFunc(choice);
}

void Widget::setRegDeleteValueInj(){
    bool choice = ui->InjRegDeleteValue->isChecked();
    output->analyser->injRegDeleteValue = choice;
    InjRegDeleteValueFunc(choice);
}

void Widget::setRegCloseKeyInj(){
    bool choice = ui->InjRegCloseKey->isChecked();
    output->analyser->injRegCloseKey = choice;
    InjRegCloseKeyFunc(choice);
}

void Widget::setRegOpenKeyExInj(){
    bool choice = ui->InjRegOpenKeyEx->isChecked();
    output->analyser->injRegOpenKeyEx = choice;
    InjRegOpenKeyExFunc(choice);
}
