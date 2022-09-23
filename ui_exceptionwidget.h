/********************************************************************************
** Form generated from reading UI file 'exceptionwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXCEPTIONWIDGET_H
#define UI_EXCEPTIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_exceptionWidget
{
public:
    QGridLayout *gridLayout;
    QTableView *tableView;

    void setupUi(QWidget *exceptionWidget)
    {
        if (exceptionWidget->objectName().isEmpty())
            exceptionWidget->setObjectName(QString::fromUtf8("exceptionWidget"));
        exceptionWidget->resize(600, 400);
        gridLayout = new QGridLayout(exceptionWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableView = new QTableView(exceptionWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout->addWidget(tableView, 0, 0, 1, 1);


        retranslateUi(exceptionWidget);

        QMetaObject::connectSlotsByName(exceptionWidget);
    } // setupUi

    void retranslateUi(QWidget *exceptionWidget)
    {
        exceptionWidget->setWindowTitle(QCoreApplication::translate("exceptionWidget", "\345\274\202\345\270\270\347\225\214\351\235\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class exceptionWidget: public Ui_exceptionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXCEPTIONWIDGET_H
