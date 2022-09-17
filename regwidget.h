#ifndef REGWIDGET_H
#define REGWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class regWidget;
}

class regWidget : public QWidget
{
    Q_OBJECT

public:
    explicit regWidget(QStandardItemModel* regModel, QWidget *parent = nullptr);
    ~regWidget();

private:
    Ui::regWidget *ui;
};

#endif // REGWIDGET_H
