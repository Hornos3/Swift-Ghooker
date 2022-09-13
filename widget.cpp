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
//    connect(ui->selectFile, &QPushButton::clicked, this, &Widget::on_selectFile_clicked);
//    connect(ui->startAnalysis, &QPushButton::clicked, this, &Widget::on_startAnalysis_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_selectFile_clicked(){
    getFilenameAndShow(ui->targetFile, "可执行文件(*.exe)");
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
