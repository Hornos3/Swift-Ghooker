#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStringListModel>
#include <vector>
#include "hookanalysis.h"

namespace Ui {
class logWidget;
}

class logWidget : public QWidget
{
    Q_OBJECT

public:
    explicit logWidget(std::vector<fullLog>* allLog, QStandardItemModel* model, QWidget *parent = nullptr);
    ~logWidget();

    std::vector<fullLog>* allLog;

private:
    Ui::logWidget *ui;
};

#endif // LOGWIDGET_H
