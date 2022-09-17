/********************************************************************************
** Form generated from reading UI file 'logwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGWIDGET_H
#define UI_LOGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_logWidget
{
public:
    QGridLayout *gridLayout;
    QTreeView *treeView;

    void setupUi(QWidget *logWidget)
    {
        if (logWidget->objectName().isEmpty())
            logWidget->setObjectName(QString::fromUtf8("logWidget"));
        logWidget->resize(600, 400);
        gridLayout = new QGridLayout(logWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        treeView = new QTreeView(logWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        gridLayout->addWidget(treeView, 0, 0, 1, 1);


        retranslateUi(logWidget);

        QMetaObject::connectSlotsByName(logWidget);
    } // setupUi

    void retranslateUi(QWidget *logWidget)
    {
        logWidget->setWindowTitle(QCoreApplication::translate("logWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class logWidget: public Ui_logWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGWIDGET_H
