/********************************************************************************
** Form generated from reading UI file 'regwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGWIDGET_H
#define UI_REGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_regWidget
{
public:
    QGridLayout *gridLayout;
    QTreeView *treeView;

    void setupUi(QWidget *regWidget)
    {
        if (regWidget->objectName().isEmpty())
            regWidget->setObjectName(QString::fromUtf8("regWidget"));
        regWidget->resize(600, 400);
        gridLayout = new QGridLayout(regWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        treeView = new QTreeView(regWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        gridLayout->addWidget(treeView, 0, 0, 1, 1);


        retranslateUi(regWidget);

        QMetaObject::connectSlotsByName(regWidget);
    } // setupUi

    void retranslateUi(QWidget *regWidget)
    {
        regWidget->setWindowTitle(QCoreApplication::translate("regWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class regWidget: public Ui_regWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGWIDGET_H
