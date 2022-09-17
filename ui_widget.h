/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *selectFile;
    QLabel *targetFile;
    QPushButton *startAnalysis;
    QLabel *label_4;
    QCheckBox *InjMessageBoxA;
    QCheckBox *InjMessageBoxW;
    QCheckBox *InjHeapCreate;
    QCheckBox *InjHeapDestroy;
    QCheckBox *InjHeapAlloc;
    QCheckBox *InjHeapFree;
    QCheckBox *InjOpenFile;
    QCheckBox *InjCreateFile;
    QCheckBox *InjReadFile;
    QCheckBox *InjWriteFile;
    QCheckBox *InjRegCreateKeyEx;
    QCheckBox *InjRegSetValueEx;
    QCheckBox *InjRegDeleteValue;
    QCheckBox *InjRegCloseKey;
    QCheckBox *InjRegOpenKeyEx;
    QCheckBox *InjCloseHandle;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1200, 600);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Widget->sizePolicy().hasHeightForWidth());
        Widget->setSizePolicy(sizePolicy);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 1200, 600));
        label->setPixmap(QPixmap(QString::fromUtf8(":/background/back.png")));
        label->setScaledContents(true);
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(300, 30, 600, 50));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        label_2->setPalette(palette);
        QFont font;
        font.setFamilies({QString::fromUtf8("\346\261\211\344\273\252\347\221\236\350\231\216\345\256\213W")});
        font.setPointSize(22);
        label_2->setFont(font);
        label_2->setTextFormat(Qt::RichText);
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 110, 211, 31));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        label_3->setPalette(palette1);
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\346\261\211\344\273\252\347\221\236\350\231\216\345\256\213W")});
        label_3->setFont(font1);
        selectFile = new QPushButton(Widget);
        selectFile->setObjectName(QString::fromUtf8("selectFile"));
        selectFile->setGeometry(QRect(240, 110, 93, 31));
        selectFile->setFont(font1);
        targetFile = new QLabel(Widget);
        targetFile->setObjectName(QString::fromUtf8("targetFile"));
        targetFile->setGeometry(QRect(340, 110, 841, 31));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        targetFile->setPalette(palette2);
        targetFile->setFont(font1);
        startAnalysis = new QPushButton(Widget);
        startAnalysis->setObjectName(QString::fromUtf8("startAnalysis"));
        startAnalysis->setGeometry(QRect(500, 500, 200, 50));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("\346\261\211\344\273\252\347\221\236\350\231\216\345\256\213W")});
        font2.setPointSize(16);
        startAnalysis->setFont(font2);
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(40, 150, 261, 31));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        label_4->setPalette(palette3);
        label_4->setFont(font1);
        InjMessageBoxA = new QCheckBox(Widget);
        InjMessageBoxA->setObjectName(QString::fromUtf8("InjMessageBoxA"));
        InjMessageBoxA->setGeometry(QRect(40, 200, 131, 26));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush3(QColor(231, 231, 231, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        QBrush brush4(QColor(255, 255, 255, 127));
        brush4.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjMessageBoxA->setPalette(palette4);
        InjMessageBoxW = new QCheckBox(Widget);
        InjMessageBoxW->setObjectName(QString::fromUtf8("InjMessageBoxW"));
        InjMessageBoxW->setGeometry(QRect(40, 230, 131, 26));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjMessageBoxW->setPalette(palette5);
        InjHeapCreate = new QCheckBox(Widget);
        InjHeapCreate->setObjectName(QString::fromUtf8("InjHeapCreate"));
        InjHeapCreate->setGeometry(QRect(40, 260, 131, 26));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjHeapCreate->setPalette(palette6);
        InjHeapDestroy = new QCheckBox(Widget);
        InjHeapDestroy->setObjectName(QString::fromUtf8("InjHeapDestroy"));
        InjHeapDestroy->setGeometry(QRect(40, 290, 131, 26));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjHeapDestroy->setPalette(palette7);
        InjHeapAlloc = new QCheckBox(Widget);
        InjHeapAlloc->setObjectName(QString::fromUtf8("InjHeapAlloc"));
        InjHeapAlloc->setGeometry(QRect(40, 320, 131, 26));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjHeapAlloc->setPalette(palette8);
        InjHeapFree = new QCheckBox(Widget);
        InjHeapFree->setObjectName(QString::fromUtf8("InjHeapFree"));
        InjHeapFree->setGeometry(QRect(40, 350, 131, 26));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjHeapFree->setPalette(palette9);
        InjOpenFile = new QCheckBox(Widget);
        InjOpenFile->setObjectName(QString::fromUtf8("InjOpenFile"));
        InjOpenFile->setGeometry(QRect(40, 380, 131, 26));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjOpenFile->setPalette(palette10);
        InjCreateFile = new QCheckBox(Widget);
        InjCreateFile->setObjectName(QString::fromUtf8("InjCreateFile"));
        InjCreateFile->setGeometry(QRect(40, 410, 131, 26));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjCreateFile->setPalette(palette11);
        InjReadFile = new QCheckBox(Widget);
        InjReadFile->setObjectName(QString::fromUtf8("InjReadFile"));
        InjReadFile->setGeometry(QRect(40, 440, 131, 26));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjReadFile->setPalette(palette12);
        InjWriteFile = new QCheckBox(Widget);
        InjWriteFile->setObjectName(QString::fromUtf8("InjWriteFile"));
        InjWriteFile->setGeometry(QRect(40, 470, 131, 26));
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjWriteFile->setPalette(palette13);
        InjRegCreateKeyEx = new QCheckBox(Widget);
        InjRegCreateKeyEx->setObjectName(QString::fromUtf8("InjRegCreateKeyEx"));
        InjRegCreateKeyEx->setGeometry(QRect(250, 200, 151, 26));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette14.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjRegCreateKeyEx->setPalette(palette14);
        InjRegSetValueEx = new QCheckBox(Widget);
        InjRegSetValueEx->setObjectName(QString::fromUtf8("InjRegSetValueEx"));
        InjRegSetValueEx->setGeometry(QRect(250, 230, 151, 26));
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette15.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjRegSetValueEx->setPalette(palette15);
        InjRegDeleteValue = new QCheckBox(Widget);
        InjRegDeleteValue->setObjectName(QString::fromUtf8("InjRegDeleteValue"));
        InjRegDeleteValue->setGeometry(QRect(250, 260, 151, 26));
        QPalette palette16;
        palette16.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette16.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjRegDeleteValue->setPalette(palette16);
        InjRegCloseKey = new QCheckBox(Widget);
        InjRegCloseKey->setObjectName(QString::fromUtf8("InjRegCloseKey"));
        InjRegCloseKey->setGeometry(QRect(250, 290, 151, 26));
        QPalette palette17;
        palette17.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette17.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette17.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjRegCloseKey->setPalette(palette17);
        InjRegOpenKeyEx = new QCheckBox(Widget);
        InjRegOpenKeyEx->setObjectName(QString::fromUtf8("InjRegOpenKeyEx"));
        InjRegOpenKeyEx->setGeometry(QRect(250, 320, 151, 26));
        QPalette palette18;
        palette18.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette18.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette18.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjRegOpenKeyEx->setPalette(palette18);
        InjCloseHandle = new QCheckBox(Widget);
        InjCloseHandle->setObjectName(QString::fromUtf8("InjCloseHandle"));
        InjCloseHandle->setGeometry(QRect(40, 500, 131, 26));
        QPalette palette19;
        palette19.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette19.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        InjCloseHandle->setPalette(palette19);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("Widget", "\345\237\272\344\272\216DLL\346\263\250\345\205\245\347\232\204\345\217\257\346\211\247\350\241\214\346\226\207\344\273\266\345\210\206\346\236\220\351\241\271\347\233\256", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\350\257\267\350\276\223\345\205\245\351\234\200\350\246\201\345\210\206\346\236\220\347\232\204\345\217\257\346\211\247\350\241\214\346\226\207\344\273\266\357\274\232", nullptr));
        selectFile->setText(QCoreApplication::translate("Widget", "\346\265\217\350\247\210......", nullptr));
        targetFile->setText(QCoreApplication::translate("Widget", "......", nullptr));
        startAnalysis->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213\345\210\206\346\236\220", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\350\257\267\345\234\250\344\270\213\346\226\271\347\202\271\351\200\211\351\234\200\350\246\201\344\270\213\351\222\251\345\255\220\347\233\221\346\216\247\347\232\204\345\207\275\346\225\260\357\274\232", nullptr));
        InjMessageBoxA->setText(QCoreApplication::translate("Widget", "MessageBoxA", nullptr));
        InjMessageBoxW->setText(QCoreApplication::translate("Widget", "MessageBoxW", nullptr));
        InjHeapCreate->setText(QCoreApplication::translate("Widget", "HeapCreate", nullptr));
        InjHeapDestroy->setText(QCoreApplication::translate("Widget", "HeapDestroy", nullptr));
        InjHeapAlloc->setText(QCoreApplication::translate("Widget", "HeapAlloc", nullptr));
        InjHeapFree->setText(QCoreApplication::translate("Widget", "HeapFree", nullptr));
        InjOpenFile->setText(QCoreApplication::translate("Widget", "OpenFile", nullptr));
        InjCreateFile->setText(QCoreApplication::translate("Widget", "CreateFile", nullptr));
        InjReadFile->setText(QCoreApplication::translate("Widget", "ReadFile", nullptr));
        InjWriteFile->setText(QCoreApplication::translate("Widget", "WriteFile", nullptr));
        InjRegCreateKeyEx->setText(QCoreApplication::translate("Widget", "RegCreateKeyEx", nullptr));
        InjRegSetValueEx->setText(QCoreApplication::translate("Widget", "RegSetValueEx", nullptr));
        InjRegDeleteValue->setText(QCoreApplication::translate("Widget", "RegDeleteValue", nullptr));
        InjRegCloseKey->setText(QCoreApplication::translate("Widget", "RegCloseKey", nullptr));
        InjRegOpenKeyEx->setText(QCoreApplication::translate("Widget", "RegOpenKeyEx", nullptr));
        InjCloseHandle->setText(QCoreApplication::translate("Widget", "CloseHandle", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
