#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include "output.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget;}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Output* output = nullptr;
    Widget(QWidget *parent = nullptr);

    void setMessageBoxAInj();
    void setMessageBoxWInj();
    void setHeapCreateInj();
    void setHeapDestroyInj();
    void setHeapAllocInj();
    void setHeapFreeInj();
    void setOpenFileInj();
    void setCreateFileInj();
    void setReadFileInj();
    void setWriteFileInj();
    void setCloseHandleInj();
    void setRegCreateKeyExInj();
    void setRegSetValueExInj();
    void setRegDeleteValueInj();
    void setRegCloseKeyInj();
    void setRegOpenKeyExInj();
    void setRegDeleteKeyExInj();
    void setSendInj();
    void setRecvInj();
    void setBindInj();
    void setConnectInj();
    void setSocketInj();
    void setAcceptInj();

    std::vector<bool> getAllChoices();

    ~Widget();

private:
    Ui::Widget *ui;

private slots:
    void on_selectFile_clicked();
    void on_startAnalysis_clicked();
    void on_allMessageBox_clicked();
    void on_allHeap_clicked();
    void on_allFile_clicked();
    void on_allReg_clicked();
    void on_allNet_clicked();
    void on_startAnalyseHistory_clicked();
};
#endif // WIDGET_H
