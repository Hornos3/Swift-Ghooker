#ifndef OUTPUT_H
#define OUTPUT_H

#include <QWidget>
#include <QCloseEvent>
#include <QTableView>
#include <QStandardItemModel>
#include <QFileSystemWatcher>
#include <QStringListModel>
#include "hookanalysis.h"
#include "colorfulmodel.h"

#define FILEACCESS_COUNT 3
#define FILESHAREMODE_COUNT 3
#define FILECREATEDISP_COUNT 5
#define FILEFLAGATTR 12

namespace Ui {
class Output;
}

class Output : public QWidget
{
    Q_OBJECT

public:
    QFileSystemWatcher* watcher = new QFileSystemWatcher(this);
    QStandardItemModel* heapViewModel = new QStandardItemModel(this);
    QStandardItemModel* fileViewModel = new QStandardItemModel(this);
    QStandardItemModel* exceptionModel = new QStandardItemModel(this);
    QStandardItemModel* regeditModel = new QStandardItemModel(this);
    colorfulModel* fileAccessModel;
    colorfulModel* fileShareModeModel;
    colorfulModel* fileCreateDispModel;
    colorfulModel* fileFlagAttrModel;
    hookAnalysis* analyser = new hookAnalysis(heapViewModel, fileViewModel, exceptionModel, regeditModel);

    explicit Output(QWidget *parent = nullptr);
    void updateLog();
    void showExeInfo();
    void closeEvent(QCloseEvent * event);
    void trimExeInfo(QString& fullInfo);
    ~Output();

private slots:
    void on_fileView_clicked(const QModelIndex &index);

private:
    Ui::Output *ui;
    QStandardItemModel* model = new QStandardItemModel();
};

#endif // OUTPUT_H
