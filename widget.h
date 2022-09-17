#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "output.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
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

    ~Widget();

private:
    Ui::Widget *ui;

private slots:
    void on_selectFile_clicked();
    void on_startAnalysis_clicked();
};
#endif // WIDGET_H
