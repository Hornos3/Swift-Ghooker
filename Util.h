#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include "Inject.h"

using namespace std;
#define PAGE_SIZE (size_t)4096

QString getFilenameAndShow(QLabel* label, const QString& filter = "", const QString& capture = "选择文件", const QString& dir = ".");
wstring stringTowstring(string str);
string wstring2string(wstring wstr);
char* ull2a(uint64_t value, bool containsPrefix = true);
QString ip_int2str(unsigned ip);

#endif // UTIL_H
