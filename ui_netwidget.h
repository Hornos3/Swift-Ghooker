/********************************************************************************
** Form generated from reading UI file 'netwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETWIDGET_H
#define UI_NETWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_netWidget
{
public:
    QGridLayout *gridLayout;
    QTreeView *treeView;

    void setupUi(QWidget *netWidget)
    {
        if (netWidget->objectName().isEmpty())
            netWidget->setObjectName(QString::fromUtf8("netWidget"));
        netWidget->resize(880, 574);
        gridLayout = new QGridLayout(netWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        treeView = new QTreeView(netWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        gridLayout->addWidget(treeView, 0, 0, 1, 1);


        retranslateUi(netWidget);

        QMetaObject::connectSlotsByName(netWidget);
    } // setupUi

    void retranslateUi(QWidget *netWidget)
    {
        netWidget->setWindowTitle(QCoreApplication::translate("netWidget", "\347\275\221\347\273\234\350\247\206\345\233\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class netWidget: public Ui_netWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETWIDGET_H
