#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTreeView>
#include "colorfulmodel.h"
#include "hookanalysis.h"

namespace Ui {
class fileWidget;
}

class fileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit fileWidget(QStandardItemModel* fileModel,
                        colorfulModel* access,
                        colorfulModel* shareMode,
                        colorfulModel* createDisp,
                        colorfulModel* flagAttr,
                        hookAnalysis* analyser,
                        QWidget *parent = nullptr);
    ~fileWidget();
    QStandardItemModel* fileModel;
    colorfulModel* access;
    colorfulModel* shareMode;
    colorfulModel* createDisp;
    colorfulModel* flagAttr;
    hookAnalysis* analyser;

private slots:
    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::fileWidget *ui;
};

#endif // FILEWIDGET_H
