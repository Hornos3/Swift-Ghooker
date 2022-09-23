#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H

#include <QWidget>
#include <QStringListModel>
#include "hookanalysis.h"

namespace Ui {
class memoryWidget;
}

class memoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit memoryWidget(std::map<uint64_t, std::map<int, memoryInfo>>* memories,
                          QStandardItemModel* memModel, QWidget *parent = nullptr);

    std::map<uint64_t, std::map<int, memoryInfo>>* memories;
    QStandardItemModel* memoryViewModel;
    QStandardItemModel* memoryContentModel = new QStandardItemModel();

    ~memoryWidget();

private slots:
    void on_memoryView_clicked(const QModelIndex &index);

private:
    Ui::memoryWidget *ui;
};

#endif // MEMORYWIDGET_H
