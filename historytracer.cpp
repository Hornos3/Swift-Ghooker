#include "historytracer.h"

historyTracer::historyTracer(QStandardItemModel *heapViewModel, QStandardItemModel *fileViewModel,
                             QStandardItemModel *exceptionModel, QStandardItemModel *regeditModel,
                             QStandardItemModel *logWidgetModel, QStandardItemModel *netModel,
                             QStandardItemModel *memoryModel, QStandardItemModel *moduleModel)
{
    this->mainAnalyser = new hookAnalysis(heapViewModel, fileViewModel, exceptionModel,
                                          regeditModel, logWidgetModel, netModel,
                                          memoryModel, moduleModel);
}

historyTracer::historyTracer(hookAnalysis* analyser){
    this->mainAnalyser = analyser;
    this->logList = &analyser->logList;
}

//bool historyTracer::initialize(){
//    ofstream
//}

bool historyTracer::stepFront(){
    fullLog nextLog = (*logList)[currentStep];
    // this->mainAnalyser->diverter(nextLog);
    // TODO
    return true;
}
