#include "widget.h"
#include <QApplication>
#include <windows.h>
#include <fstream>
#include <QDebug>
#include <cassert>
#include <QLibrary>
#include <QMessageBox>
#include <QReadWriteLock>
using namespace std;

#pragma comment(lib, "D:/SSdesign/detours/lib.X64/detours.lib")

QLibrary injectLib("injectionDLL");
void (*InjMessageBoxAFunc)(bool);
void (*InjMessageBoxWFunc)(bool);
void (*InjHeapCreateFunc)(bool);
void (*InjHeapDestroyFunc)(bool);
void (*InjHeapAllocFunc)(bool);
void (*InjHeapFreeFunc)(bool);
void (*InjOpenFileFunc)(bool);
void (*InjCreateFileFunc)(bool);
void (*InjReadFileFunc)(bool);
void (*InjWriteFileFunc)(bool);
bool (*getMutexSignal)(void);
void (*setMutexSignal)(void);
QReadWriteLock* (*getLock)(void);

QReadWriteLock* lock;
//bool (*MessageBoxAInjected)(void);
//bool (*MessageBoxWInjected)(void);
//bool (*HeapCreateInjected)(void);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationVersion("1.1.0");

    // 检查injectionDLL.dll是否存在
    if(!injectLib.load()){
        QMessageBox::critical(nullptr, "DLL缺失", "找不到injectionDLL.dll，程序无法执行！");
        exit(1);
    }
    InjMessageBoxAFunc = (void(*)(bool))injectLib.resolve("openInjectMessageBoxA");
    InjMessageBoxWFunc = (void(*)(bool))injectLib.resolve("openInjectMessageBoxW");
    InjHeapCreateFunc = (void(*)(bool))injectLib.resolve("openInjectHeapCreate");
    InjHeapDestroyFunc = (void(*)(bool))injectLib.resolve("openInjectHeapDestroy");
    InjHeapAllocFunc = (void(*)(bool))injectLib.resolve("openInjectHeapAlloc");
    InjHeapFreeFunc = (void(*)(bool))injectLib.resolve("openInjectHeapFree");
    InjOpenFileFunc = (void(*)(bool))injectLib.resolve("openInjectOpenFile");
    InjCreateFileFunc = (void(*)(bool))injectLib.resolve("openInjectCreateFile");
    InjReadFileFunc = (void(*)(bool))injectLib.resolve("openInjectReadFile");
    InjWriteFileFunc = (void(*)(bool))injectLib.resolve("openInjectWriteFile");
    getMutexSignal = (bool(*)(void))injectLib.resolve("getMutexSignal");
    setMutexSignal = (void(*)(void))injectLib.resolve("setMutexSignal");
    getLock = (QReadWriteLock*(*)(void))injectLib.resolve("getLock");
    lock = getLock();
//    MessageBoxAInjected = (bool(*)())injectLib.resolve("isInjectMessageBoxA");
//    HeapCreateInjected = (bool(*)())injectLib.resolve("isInjectHeapCreate");

    Widget w;
    w.show();
    return a.exec();
}
