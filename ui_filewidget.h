/********************************************************************************
** Form generated from reading UI file 'filewidget.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEWIDGET_H
#define UI_FILEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_fileWidget
{
public:
    QListView *access;
    QListView *shareMode;
    QListView *createDisp;
    QListView *flagAttr;
    QTreeView *treeView;

    void setupUi(QWidget *fileWidget)
    {
        if (fileWidget->objectName().isEmpty())
            fileWidget->setObjectName(QString::fromUtf8("fileWidget"));
        fileWidget->resize(850, 500);
        access = new QListView(fileWidget);
        access->setObjectName(QString::fromUtf8("access"));
        access->setGeometry(QRect(11, 11, 160, 231));
        shareMode = new QListView(fileWidget);
        shareMode->setObjectName(QString::fromUtf8("shareMode"));
        shareMode->setGeometry(QRect(10, 261, 160, 231));
        createDisp = new QListView(fileWidget);
        createDisp->setObjectName(QString::fromUtf8("createDisp"));
        createDisp->setGeometry(QRect(590, 11, 250, 231));
        flagAttr = new QListView(fileWidget);
        flagAttr->setObjectName(QString::fromUtf8("flagAttr"));
        flagAttr->setGeometry(QRect(590, 261, 250, 231));
        treeView = new QTreeView(fileWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setGeometry(QRect(176, 11, 410, 481));

        retranslateUi(fileWidget);

        QMetaObject::connectSlotsByName(fileWidget);
    } // setupUi

    void retranslateUi(QWidget *fileWidget)
    {
        fileWidget->setWindowTitle(QCoreApplication::translate("fileWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class fileWidget: public Ui_fileWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEWIDGET_H
