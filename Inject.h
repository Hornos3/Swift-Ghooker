#ifndef INJECT_H
#define INJECT_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <include/detours.h>

#define DLL_DIRECTORY (L"D:\\SSdesign\\UI\\designMain")
#define DLL_ADDRESS ("D:\\SSdesign\\UI\\designMain\\injectionDLL.dll")

void dllInject(const WCHAR* inputEXE);

#endif // INJECT_H
