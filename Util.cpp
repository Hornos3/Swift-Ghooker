#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include "Inject.h"

using namespace std;

QString getFilenameAndShow(QLabel* label, const QString& filter = "", const QString& capture = "选择文件", const QString& dir = "."){
    QString filename = QFileDialog::getOpenFileName(nullptr, capture, dir, filter);
    if(filename.isNull())
        label->setText("...");
    else
        label->setText(filename);
    return filename;
}

wstring stringTowstring(string str)
{
    wstring result;
    //获取缓冲区大小，并申请空间，缓冲区大小按字符计算
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    TCHAR* buffer = new TCHAR[len + 1];
    //多字节编码转换成宽字节编码
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
    buffer[len] = '\0';             //添加字符串结尾
    //删除缓冲区并返回值
    result.append(buffer);
    delete[] buffer;
    buffer = NULL;
    return result;
}

//将wstring转换成string
string wstring2string(wstring wstr)
{
    string result;
    //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    char* buffer = new char[len + 1];
    //宽字节编码转换成多字节编码
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    //删除缓冲区并返回值
    result.append(buffer);
    delete[] buffer;
    return result;
}

char* ull2a(uint64_t value, bool containsPrefix){
    char* ret = (char*)calloc(19, 1);
    if(containsPrefix)
        sprintf_s(ret, 19, "%#zx", value);
    else
        sprintf_s(ret, 19, "%zx", value);
    return ret;
}

QString ip_int2str(unsigned ip){
    QString ret;
    ret += to_string(ip & 0xFF).c_str();
    ret += ".";
    ret += to_string((ip >> 8) & 0xFF).c_str();
    ret += ".";
    ret += to_string((ip >> 16) & 0xFF).c_str();
    ret += ".";
    ret += to_string(ip >> 24).c_str();
    return ret;
}
