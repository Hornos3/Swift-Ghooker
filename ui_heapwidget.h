/********************************************************************************
** Form generated from reading UI file 'heapwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEAPWIDGET_H
#define UI_HEAPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_heapWidget
{
public:
    QGridLayout *gridLayout;
    QTreeView *treeView;

    void setupUi(QWidget *heapWidget)
    {
        if (heapWidget->objectName().isEmpty())
            heapWidget->setObjectName(QString::fromUtf8("heapWidget"));
        heapWidget->resize(600, 400);
        gridLayout = new QGridLayout(heapWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        treeView = new QTreeView(heapWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        gridLayout->addWidget(treeView, 0, 0, 1, 1);


        retranslateUi(heapWidget);

        QMetaObject::connectSlotsByName(heapWidget);
    } // setupUi

    void retranslateUi(QWidget *heapWidget)
    {
        heapWidget->setWindowTitle(QCoreApplication::translate("heapWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class heapWidget: public Ui_heapWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEAPWIDGET_H
