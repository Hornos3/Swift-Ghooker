#ifndef NETWIDGET_H
#define NETWIDGET_H

#include "qstandarditemmodel.h"
#include <QWidget>

namespace Ui {
class netWidget;
}

class netWidget : public QWidget
{
    Q_OBJECT

public:
    explicit netWidget(QStandardItemModel* netModel, QWidget *parent = nullptr);
    ~netWidget();
    QStandardItemModel* model;

private:
    Ui::netWidget *ui;
};

#endif // NETWIDGET_H
