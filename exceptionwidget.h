#ifndef EXCEPTIONWIDGET_H
#define EXCEPTIONWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class exceptionWidget;
}

class exceptionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit exceptionWidget(QStandardItemModel* exceptionModel, QWidget *parent = nullptr);
    ~exceptionWidget();

private:
    Ui::exceptionWidget *ui;
};

#endif // EXCEPTIONWIDGET_H
