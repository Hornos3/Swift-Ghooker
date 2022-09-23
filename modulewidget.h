#ifndef MODULEWIDGET_H
#define MODULEWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class moduleWidget;
}

class moduleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit moduleWidget(QStandardItemModel* moduleModel, QWidget *parent = nullptr);
    ~moduleWidget();

private:
    Ui::moduleWidget *ui;
};

#endif // MODULEWIDGET_H
