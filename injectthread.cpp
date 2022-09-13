#include "injectthread.h"
#include "Inject.h"

injectThread::injectThread(QObject *parent)
    : QThread{parent}
{

}

void injectThread::run(){
    dllInject(inputEXE);
}
