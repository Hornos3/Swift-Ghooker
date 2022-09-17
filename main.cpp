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
wchar_t currentDirectory[0x1000];

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
void (*InjCloseHandleFunc)(bool);
void (*InjRegCreateKeyExFunc)(bool);
void (*InjRegSetValueExFunc)(bool);
void (*InjRegDeleteValueFunc)(bool);
void (*InjRegCloseKeyFunc)(bool);
void (*InjRegOpenKeyExFunc)(bool);

bool (*getMutexSignal)(void);
void (*setMutexSignal)(void);

char* (*getLastHookBeforeCall)(void);

//bool (*MessageBoxAInjected)(void);
//bool (*MessageBoxWInjected)(void);
//bool (*HeapCreateInjected)(void);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationVersion("1.3.2");

    // 检查injectionDLL.dll是否存在
    if(!injectLib.load()){
        QMessageBox::critical(nullptr, "DLL缺失", "找不到injectionDLL.dll，程序无法执行！");
        exit(1);
    }

    GetCurrentDirectory(0x1000, currentDirectory);

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
    InjCloseHandleFunc = (void(*)(bool))injectLib.resolve("openInjectCloseHandle");
    InjRegCreateKeyExFunc = (void(*)(bool))injectLib.resolve("openInjectRegCreateKeyEx");
    InjRegSetValueExFunc = (void(*)(bool))injectLib.resolve("openInjectRegSetValueEx");
    InjRegDeleteValueFunc = (void(*)(bool))injectLib.resolve("openInjectRegDeleteValue");
    InjRegCloseKeyFunc = (void(*)(bool))injectLib.resolve("openInjectRegCloseKey");
    InjRegOpenKeyExFunc = (void(*)(bool))injectLib.resolve("openInjectRegOpenKeyEx");

    getMutexSignal = (bool(*)(void))injectLib.resolve("getMutexSignal");
    setMutexSignal = (void(*)(void))injectLib.resolve("setMutexSignal");

    getLastHookBeforeCall = (char*(*)(void))injectLib.resolve("getLastHookBeforeCall");
//    MessageBoxAInjected = (bool(*)())injectLib.resolve("isInjectMessageBoxA");
//    HeapCreateInjected = (bool(*)())injectLib.resolve("isInjectHeapCreate");

    Widget w;
    w.show();
    return a.exec();
}
