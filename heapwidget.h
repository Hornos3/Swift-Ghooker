#ifndef HEAPWIDGET_H
#define HEAPWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class heapWidget;
}

class heapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit heapWidget(QStandardItemModel* heapModel, QWidget *parent = nullptr);
    ~heapWidget();

private:
    Ui::heapWidget *ui;
};

#endif // HEAPWIDGET_H
