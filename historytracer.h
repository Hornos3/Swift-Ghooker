#ifndef HISTORYTRACER_H
#define HISTORYTRACER_H

#include "hookanalysis.h"

class historyTracer
{
public:
    historyTracer(QStandardItemModel* heapViewModel, QStandardItemModel* fileViewModel,
                  QStandardItemModel* exceptionModel, QStandardItemModel* regeditModel,
                  QStandardItemModel* logWidgetModel, QStandardItemModel* netModel,
                  QStandardItemModel* memoryModel, QStandardItemModel* moduleModel);
    historyTracer(hookAnalysis* analyser);
    std::vector<fullLog>* logList;
    hookAnalysis* mainAnalyser;
    int totalStep = 0;
    int currentStep = 0;

    bool initialize();
    bool stepBack();
    bool stepFront();
    bool jumpTo(int logId);
};

#endif // HISTORYTRACER_H
