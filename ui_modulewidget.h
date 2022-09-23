/********************************************************************************
** Form generated from reading UI file 'modulewidget.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODULEWIDGET_H
#define UI_MODULEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_moduleWidget
{
public:
    QGridLayout *gridLayout;
    QTableView *tableView;

    void setupUi(QWidget *moduleWidget)
    {
        if (moduleWidget->objectName().isEmpty())
            moduleWidget->setObjectName(QString::fromUtf8("moduleWidget"));
        moduleWidget->resize(800, 500);
        QFont font;
        font.setFamilies({QString::fromUtf8("Source Code Pro")});
        moduleWidget->setFont(font);
        gridLayout = new QGridLayout(moduleWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableView = new QTableView(moduleWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout->addWidget(tableView, 0, 0, 1, 1);


        retranslateUi(moduleWidget);

        QMetaObject::connectSlotsByName(moduleWidget);
    } // setupUi

    void retranslateUi(QWidget *moduleWidget)
    {
        moduleWidget->setWindowTitle(QCoreApplication::translate("moduleWidget", "\346\250\241\345\235\227\350\247\206\345\233\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class moduleWidget: public Ui_moduleWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODULEWIDGET_H
