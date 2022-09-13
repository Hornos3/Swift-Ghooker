#ifndef INJECTTHREAD_H
#define INJECTTHREAD_H

#include <QThread>
#include "windows.h"


class injectThread : public QThread
{
public:
    explicit injectThread(QObject *parent = nullptr);
    const WCHAR* inputEXE;
protected:
    void run() override;
};

#endif // INJECTTHREAD_H
