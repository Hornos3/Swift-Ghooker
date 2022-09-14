/********************************************************************************
** Form generated from reading UI file 'output.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTPUT_H
#define UI_OUTPUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Output
{
public:
    QLabel *label;
    QPlainTextEdit *logInfo;
    QLabel *title;
    QTableView *processInfo;
    QTreeView *heapView;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QTableView *exceptionWindow;
    QLabel *label_6;
    QTreeView *fileView;
    QListView *fileAccess;
    QListView *fileShareMode;
    QListView *fileCreationDisposition;
    QListView *fileFlagsAndAttributes;
    QTreeView *regeditView;
    QLabel *label_7;

    void setupUi(QWidget *Output)
    {
        if (Output->objectName().isEmpty())
            Output->setObjectName(QString::fromUtf8("Output"));
        Output->resize(1500, 800);
        label = new QLabel(Output);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 1500, 800));
        label->setPixmap(QPixmap(QString::fromUtf8(":/background/back.png")));
        label->setScaledContents(true);
        logInfo = new QPlainTextEdit(Output);
        logInfo->setObjectName(QString::fromUtf8("logInfo"));
        logInfo->setGeometry(QRect(70, 130, 410, 401));
        QFont font;
        font.setFamilies({QString::fromUtf8("Source Code Pro")});
        font.setBold(false);
        logInfo->setFont(font);
        title = new QLabel(Output);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(140, 20, 1220, 60));
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
        title->setPalette(palette);
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\346\261\211\344\273\252\347\221\236\350\231\216\345\256\213W")});
        font1.setPointSize(20);
        title->setFont(font1);
        title->setAlignment(Qt::AlignCenter);
        processInfo = new QTableView(Output);
        processInfo->setObjectName(QString::fromUtf8("processInfo"));
        processInfo->setGeometry(QRect(529, 130, 440, 60));
        heapView = new QTreeView(Output);
        heapView->setObjectName(QString::fromUtf8("heapView"));
        heapView->setGeometry(QRect(1020, 130, 410, 400));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Source Code Pro")});
        heapView->setFont(font2);
        label_2 = new QLabel(Output);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(1190, 100, 81, 31));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush3(QColor(231, 231, 231, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        QBrush brush4(QColor(255, 255, 255, 127));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        label_2->setPalette(palette1);
        QFont font3;
        font3.setFamilies({QString::fromUtf8("\346\261\211\344\273\252\345\224\220\347\276\216\344\272\272 95W")});
        label_2->setFont(font3);
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(Output);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(710, 100, 81, 31));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        label_3->setPalette(palette2);
        label_3->setFont(font3);
        label_3->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(Output);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(230, 100, 81, 31));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        label_4->setPalette(palette3);
        label_4->setFont(font3);
        label_4->setAlignment(Qt::AlignCenter);
        label_5 = new QLabel(Output);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(710, 190, 81, 31));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        label_5->setPalette(palette4);
        label_5->setFont(font3);
        label_5->setAlignment(Qt::AlignCenter);
        exceptionWindow = new QTableView(Output);
        exceptionWindow->setObjectName(QString::fromUtf8("exceptionWindow"));
        exceptionWindow->setGeometry(QRect(529, 220, 440, 310));
        label_6 = new QLabel(Output);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(890, 530, 80, 30));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        label_6->setPalette(palette5);
        label_6->setFont(font3);
        label_6->setAlignment(Qt::AlignCenter);
        fileView = new QTreeView(Output);
        fileView->setObjectName(QString::fromUtf8("fileView"));
        fileView->setGeometry(QRect(529, 560, 410, 200));
        fileAccess = new QListView(Output);
        fileAccess->setObjectName(QString::fromUtf8("fileAccess"));
        fileAccess->setGeometry(QRect(939, 560, 70, 200));
        fileShareMode = new QListView(Output);
        fileShareMode->setObjectName(QString::fromUtf8("fileShareMode"));
        fileShareMode->setGeometry(QRect(1010, 560, 100, 200));
        fileCreationDisposition = new QListView(Output);
        fileCreationDisposition->setObjectName(QString::fromUtf8("fileCreationDisposition"));
        fileCreationDisposition->setGeometry(QRect(1110, 560, 150, 200));
        fileFlagsAndAttributes = new QListView(Output);
        fileFlagsAndAttributes->setObjectName(QString::fromUtf8("fileFlagsAndAttributes"));
        fileFlagsAndAttributes->setGeometry(QRect(1260, 560, 170, 200));
        regeditView = new QTreeView(Output);
        regeditView->setObjectName(QString::fromUtf8("regeditView"));
        regeditView->setGeometry(QRect(70, 560, 410, 200));
        label_7 = new QLabel(Output);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(230, 530, 80, 30));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        label_7->setPalette(palette6);
        label_7->setFont(font3);
        label_7->setAlignment(Qt::AlignCenter);

        retranslateUi(Output);

        QMetaObject::connectSlotsByName(Output);
    } // setupUi

    void retranslateUi(QWidget *Output)
    {
        Output->setWindowTitle(QCoreApplication::translate("Output", "Form", nullptr));
        label->setText(QString());
        logInfo->setPlainText(QString());
        title->setText(QCoreApplication::translate("Output", "\351\222\251\345\255\220\346\210\252\350\216\267\344\277\241\346\201\257", nullptr));
        label_2->setText(QCoreApplication::translate("Output", "\345\240\206\347\233\221\346\216\247", nullptr));
        label_3->setText(QCoreApplication::translate("Output", "\346\226\207\344\273\266\344\277\241\346\201\257", nullptr));
        label_4->setText(QCoreApplication::translate("Output", "\346\227\245\345\277\227", nullptr));
        label_5->setText(QCoreApplication::translate("Output", "\345\274\202\345\270\270\344\277\241\346\201\257", nullptr));
        label_6->setText(QCoreApplication::translate("Output", "\346\226\207\344\273\266\347\233\221\346\216\247", nullptr));
        label_7->setText(QCoreApplication::translate("Output", "\346\263\250\345\206\214\350\241\250\347\233\221\346\216\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Output: public Ui_Output {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTPUT_H
