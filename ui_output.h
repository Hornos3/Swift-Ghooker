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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
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
    QPushButton *showLogWidget;
    QPushButton *showHeapWidget;
    QPushButton *showFileWidget;
    QPushButton *showRegWidget;
    QPushButton *showExceptionWidget;
    QPushButton *showNetWidget;
    QLabel *label_8;
    QTreeView *netView;
    QPushButton *showMemoryWidget;
    QPushButton *showModulesWidget;
    QPushButton *prevStep;
    QPushButton *nextStep;
    QLabel *label_9;
    QSpinBox *currentStep;
    QLabel *label_10;
    QSpinBox *stepCount;
    QLabel *lastHookTime;

    void setupUi(QWidget *Output)
    {
        if (Output->objectName().isEmpty())
            Output->setObjectName(QString::fromUtf8("Output"));
        Output->resize(1500, 800);
        label = new QLabel(Output);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 1500, 800));
        label->setPixmap(QPixmap(QString::fromUtf8(":/background/disco_elysium_four_Moment.jpg")));
        label->setScaledContents(true);
        logInfo = new QPlainTextEdit(Output);
        logInfo->setObjectName(QString::fromUtf8("logInfo"));
        logInfo->setGeometry(QRect(70, 130, 410, 401));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 0));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush2(QColor(0, 0, 0, 255));
        brush2.setStyle(Qt::NoBrush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        QBrush brush3(QColor(231, 231, 231, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush3);
        QBrush brush4(QColor(0, 0, 0, 255));
        brush4.setStyle(Qt::NoBrush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        QBrush brush5(QColor(255, 255, 255, 127));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush5);
        QBrush brush6(QColor(0, 0, 0, 255));
        brush6.setStyle(Qt::NoBrush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        logInfo->setPalette(palette);
        QFont font;
        font.setFamilies({QString::fromUtf8("Source Code Pro")});
        font.setBold(false);
        logInfo->setFont(font);
        logInfo->setStyleSheet(QString::fromUtf8("background-color:rgba(0,0,0,0)"));
        title = new QLabel(Output);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(140, 20, 1220, 60));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush7(QColor(0, 0, 0, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        QBrush brush8(QColor(120, 120, 120, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush8);
        title->setPalette(palette1);
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\346\261\211\344\273\252\347\221\236\350\231\216\345\256\213W")});
        font1.setPointSize(20);
        title->setFont(font1);
        title->setStyleSheet(QString::fromUtf8(""));
        title->setAlignment(Qt::AlignCenter);
        processInfo = new QTableView(Output);
        processInfo->setObjectName(QString::fromUtf8("processInfo"));
        processInfo->setGeometry(QRect(529, 130, 440, 100));
        processInfo->setStyleSheet(QString::fromUtf8(""));
        heapView = new QTreeView(Output);
        heapView->setObjectName(QString::fromUtf8("heapView"));
        heapView->setGeometry(QRect(1020, 130, 410, 200));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Source Code Pro")});
        heapView->setFont(font2);
        heapView->setStyleSheet(QString::fromUtf8(""));
        label_2 = new QLabel(Output);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(1180, 100, 81, 31));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        label_2->setPalette(palette2);
        QFont font3;
        font3.setFamilies({QString::fromUtf8("\346\261\211\344\273\252\345\224\220\347\276\216\344\272\272 95W")});
        label_2->setFont(font3);
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(Output);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(710, 100, 81, 31));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        label_3->setPalette(palette3);
        label_3->setFont(font3);
        label_3->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(Output);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(230, 100, 81, 31));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        label_4->setPalette(palette4);
        label_4->setFont(font3);
        label_4->setAlignment(Qt::AlignCenter);
        label_5 = new QLabel(Output);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(710, 230, 81, 31));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        label_5->setPalette(palette5);
        label_5->setFont(font3);
        label_5->setAlignment(Qt::AlignCenter);
        exceptionWindow = new QTableView(Output);
        exceptionWindow->setObjectName(QString::fromUtf8("exceptionWindow"));
        exceptionWindow->setGeometry(QRect(529, 259, 440, 270));
        exceptionWindow->setStyleSheet(QString::fromUtf8(""));
        label_6 = new QLabel(Output);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(930, 530, 80, 30));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        label_6->setPalette(palette6);
        label_6->setFont(font3);
        label_6->setAlignment(Qt::AlignCenter);
        fileView = new QTreeView(Output);
        fileView->setObjectName(QString::fromUtf8("fileView"));
        fileView->setGeometry(QRect(529, 560, 410, 200));
        fileView->setStyleSheet(QString::fromUtf8(""));
        fileAccess = new QListView(Output);
        fileAccess->setObjectName(QString::fromUtf8("fileAccess"));
        fileAccess->setGeometry(QRect(939, 560, 70, 200));
        fileAccess->setStyleSheet(QString::fromUtf8(""));
        fileShareMode = new QListView(Output);
        fileShareMode->setObjectName(QString::fromUtf8("fileShareMode"));
        fileShareMode->setGeometry(QRect(1010, 560, 100, 200));
        fileShareMode->setStyleSheet(QString::fromUtf8(""));
        fileCreationDisposition = new QListView(Output);
        fileCreationDisposition->setObjectName(QString::fromUtf8("fileCreationDisposition"));
        fileCreationDisposition->setGeometry(QRect(1110, 560, 150, 200));
        fileCreationDisposition->setStyleSheet(QString::fromUtf8(""));
        fileFlagsAndAttributes = new QListView(Output);
        fileFlagsAndAttributes->setObjectName(QString::fromUtf8("fileFlagsAndAttributes"));
        fileFlagsAndAttributes->setGeometry(QRect(1260, 560, 170, 200));
        fileFlagsAndAttributes->setStyleSheet(QString::fromUtf8(""));
        regeditView = new QTreeView(Output);
        regeditView->setObjectName(QString::fromUtf8("regeditView"));
        regeditView->setGeometry(QRect(70, 560, 410, 200));
        regeditView->setStyleSheet(QString::fromUtf8(""));
        label_7 = new QLabel(Output);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(230, 530, 80, 30));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        label_7->setPalette(palette7);
        label_7->setFont(font3);
        label_7->setAlignment(Qt::AlignCenter);
        showLogWidget = new QPushButton(Output);
        showLogWidget->setObjectName(QString::fromUtf8("showLogWidget"));
        showLogWidget->setGeometry(QRect(950, 20, 120, 30));
        showLogWidget->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image:url(:/background/unclicked.png)\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image:url(:/background/clicked.png)\n"
"}"));
        showHeapWidget = new QPushButton(Output);
        showHeapWidget->setObjectName(QString::fromUtf8("showHeapWidget"));
        showHeapWidget->setGeometry(QRect(950, 60, 120, 30));
        showHeapWidget->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image:url(:/background/unclicked.png)\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image:url(:/background/clicked.png)\n"
"}"));
        showFileWidget = new QPushButton(Output);
        showFileWidget->setObjectName(QString::fromUtf8("showFileWidget"));
        showFileWidget->setGeometry(QRect(1080, 20, 120, 30));
        showFileWidget->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image:url(:/background/unclicked.png)\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image:url(:/background/clicked.png)\n"
"}"));
        showRegWidget = new QPushButton(Output);
        showRegWidget->setObjectName(QString::fromUtf8("showRegWidget"));
        showRegWidget->setGeometry(QRect(1080, 60, 120, 30));
        showRegWidget->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image:url(:/background/unclicked.png)\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image:url(:/background/clicked.png)\n"
"}"));
        showExceptionWidget = new QPushButton(Output);
        showExceptionWidget->setObjectName(QString::fromUtf8("showExceptionWidget"));
        showExceptionWidget->setGeometry(QRect(1210, 20, 120, 30));
        showExceptionWidget->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image:url(:/background/unclicked.png)\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image:url(:/background/clicked.png)\n"
"}"));
        showNetWidget = new QPushButton(Output);
        showNetWidget->setObjectName(QString::fromUtf8("showNetWidget"));
        showNetWidget->setGeometry(QRect(1210, 60, 120, 30));
        showNetWidget->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image:url(:/background/unclicked.png)\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image:url(:/background/clicked.png)\n"
"}"));
        label_8 = new QLabel(Output);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(1180, 330, 81, 31));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        label_8->setPalette(palette8);
        label_8->setFont(font3);
        label_8->setAlignment(Qt::AlignCenter);
        netView = new QTreeView(Output);
        netView->setObjectName(QString::fromUtf8("netView"));
        netView->setGeometry(QRect(1020, 360, 410, 170));
        netView->setFont(font2);
        netView->setStyleSheet(QString::fromUtf8(""));
        showMemoryWidget = new QPushButton(Output);
        showMemoryWidget->setObjectName(QString::fromUtf8("showMemoryWidget"));
        showMemoryWidget->setGeometry(QRect(1340, 20, 120, 30));
        showMemoryWidget->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image:url(:/background/unclicked.png)\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image:url(:/background/clicked.png)\n"
"}"));
        showModulesWidget = new QPushButton(Output);
        showModulesWidget->setObjectName(QString::fromUtf8("showModulesWidget"));
        showModulesWidget->setGeometry(QRect(1340, 60, 120, 30));
        showModulesWidget->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image:url(:/background/unclicked.png)\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image:url(:/background/clicked.png)\n"
"}"));
        prevStep = new QPushButton(Output);
        prevStep->setObjectName(QString::fromUtf8("prevStep"));
        prevStep->setGeometry(QRect(70, 50, 51, 51));
        prevStep->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image: url(:/background/LGbutton.png)\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image: url(:/background/LRbutton.png)\n"
"}"));
        nextStep = new QPushButton(Output);
        nextStep->setObjectName(QString::fromUtf8("nextStep"));
        nextStep->setGeometry(QRect(430, 50, 51, 51));
        nextStep->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image: url(:/background/RGbutton.png)\n"
"}\n"
"QPushButton:pressed{\n"
"	border-image: url(:/background/RRbutton.png)\n"
"}"));
        label_9 = new QLabel(Output);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(230, 60, 81, 31));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        label_9->setPalette(palette9);
        label_9->setFont(font3);
        label_9->setAlignment(Qt::AlignCenter);
        currentStep = new QSpinBox(Output);
        currentStep->setObjectName(QString::fromUtf8("currentStep"));
        currentStep->setGeometry(QRect(170, 50, 81, 51));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Source Code Pro Black")});
        font4.setPointSize(16);
        font4.setBold(true);
        currentStep->setFont(font4);
        currentStep->setStyleSheet(QString::fromUtf8("QSpinBox::up-button,QSpinBox::down-button\n"
"{\n"
"	width:0px;\n"
"}\n"
""));
        currentStep->setMaximum(0);
        label_10 = new QLabel(Output);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(250, 60, 51, 31));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        label_10->setPalette(palette10);
        QFont font5;
        font5.setFamilies({QString::fromUtf8("\346\261\211\344\273\252\345\224\220\347\276\216\344\272\272 95W")});
        font5.setPointSize(18);
        label_10->setFont(font5);
        label_10->setAlignment(Qt::AlignCenter);
        stepCount = new QSpinBox(Output);
        stepCount->setObjectName(QString::fromUtf8("stepCount"));
        stepCount->setEnabled(false);
        stepCount->setGeometry(QRect(300, 50, 81, 51));
        stepCount->setFont(font4);
        stepCount->setStyleSheet(QString::fromUtf8("QSpinBox::up-button,QSpinBox::down-button\n"
"{\n"
"	width:0px;\n"
"}"));
        stepCount->setFrame(false);
        stepCount->setKeyboardTracking(true);
        stepCount->setMaximum(0);
        lastHookTime = new QLabel(Output);
        lastHookTime->setObjectName(QString::fromUtf8("lastHookTime"));
        lastHookTime->setGeometry(QRect(170, 10, 211, 41));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        lastHookTime->setPalette(palette11);
        QFont font6;
        font6.setFamilies({QString::fromUtf8("Source Code Pro Black")});
        font6.setPointSize(11);
        font6.setBold(true);
        lastHookTime->setFont(font6);

        retranslateUi(Output);

        QMetaObject::connectSlotsByName(Output);
    } // setupUi

    void retranslateUi(QWidget *Output)
    {
        Output->setWindowTitle(QCoreApplication::translate("Output", "\347\233\221\346\216\247\347\225\214\351\235\242", nullptr));
        label->setText(QString());
        logInfo->setPlainText(QString());
        title->setText(QCoreApplication::translate("Output", "\351\222\251\345\255\220\346\210\252\350\216\267\344\277\241\346\201\257", nullptr));
        label_2->setText(QCoreApplication::translate("Output", "\345\240\206\347\233\221\346\216\247", nullptr));
        label_3->setText(QCoreApplication::translate("Output", "\346\226\207\344\273\266\344\277\241\346\201\257", nullptr));
        label_4->setText(QCoreApplication::translate("Output", "\346\227\245\345\277\227", nullptr));
        label_5->setText(QCoreApplication::translate("Output", "\345\274\202\345\270\270\344\277\241\346\201\257", nullptr));
        label_6->setText(QCoreApplication::translate("Output", "\346\226\207\344\273\266\347\233\221\346\216\247", nullptr));
        label_7->setText(QCoreApplication::translate("Output", "\346\263\250\345\206\214\350\241\250\347\233\221\346\216\247", nullptr));
        showLogWidget->setText(QCoreApplication::translate("Output", "\346\211\223\345\274\200\346\227\245\345\277\227\350\247\206\345\233\276", nullptr));
        showHeapWidget->setText(QCoreApplication::translate("Output", "\346\211\223\345\274\200\345\240\206\350\247\206\345\233\276", nullptr));
        showFileWidget->setText(QCoreApplication::translate("Output", "\346\211\223\345\274\200\346\226\207\344\273\266\350\247\206\345\233\276", nullptr));
        showRegWidget->setText(QCoreApplication::translate("Output", "\346\211\223\345\274\200\346\263\250\345\206\214\350\241\250\350\247\206\345\233\276", nullptr));
        showExceptionWidget->setText(QCoreApplication::translate("Output", "\346\211\223\345\274\200\345\274\202\345\270\270\350\247\206\345\233\276", nullptr));
        showNetWidget->setText(QCoreApplication::translate("Output", "\346\211\223\345\274\200\347\275\221\347\273\234\350\247\206\345\233\276", nullptr));
        label_8->setText(QCoreApplication::translate("Output", "\347\275\221\347\273\234\347\233\221\346\216\247", nullptr));
        showMemoryWidget->setText(QCoreApplication::translate("Output", "\346\211\223\345\274\200\345\206\205\345\255\230\350\247\206\345\233\276", nullptr));
        showModulesWidget->setText(QCoreApplication::translate("Output", "\346\211\223\345\274\200\346\250\241\345\235\227\350\247\206\345\233\276", nullptr));
        prevStep->setText(QString());
        nextStep->setText(QString());
        label_9->setText(QString());
        label_10->setText(QCoreApplication::translate("Output", "/", nullptr));
        lastHookTime->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Output: public Ui_Output {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTPUT_H
