/********************************************************************************
** Form generated from reading UI file 'memorywidget.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEMORYWIDGET_H
#define UI_MEMORYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_memoryWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QTreeView *memoryView;
    QTableView *memoryContent;

    void setupUi(QWidget *memoryWidget)
    {
        if (memoryWidget->objectName().isEmpty())
            memoryWidget->setObjectName(QString::fromUtf8("memoryWidget"));
        memoryWidget->resize(900, 500);
        horizontalLayout = new QHBoxLayout(memoryWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        memoryView = new QTreeView(memoryWidget);
        memoryView->setObjectName(QString::fromUtf8("memoryView"));

        horizontalLayout->addWidget(memoryView);

        memoryContent = new QTableView(memoryWidget);
        memoryContent->setObjectName(QString::fromUtf8("memoryContent"));
        QFont font;
        font.setFamilies({QString::fromUtf8("Source Code Pro")});
        memoryContent->setFont(font);

        horizontalLayout->addWidget(memoryContent);


        retranslateUi(memoryWidget);

        QMetaObject::connectSlotsByName(memoryWidget);
    } // setupUi

    void retranslateUi(QWidget *memoryWidget)
    {
        memoryWidget->setWindowTitle(QCoreApplication::translate("memoryWidget", "\345\206\205\345\255\230\350\247\206\345\233\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class memoryWidget: public Ui_memoryWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEMORYWIDGET_H
