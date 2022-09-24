#include "historytracer.h"
#include "qcoreapplication.h"

historyTracer::historyTracer(QStandardItemModel *heapViewModel, QStandardItemModel *fileViewModel,
                             QStandardItemModel *exceptionModel, QStandardItemModel *regeditModel,
                             QStandardItemModel *logWidgetModel, QStandardItemModel *netModel,
                             QStandardItemModel *memoryModel, QStandardItemModel *moduleModel)
{
    this->mainAnalyser = new hookAnalysis(heapViewModel, fileViewModel, exceptionModel,
                                          regeditModel, logWidgetModel, netModel,
                                          memoryModel, moduleModel);
}

historyTracer::historyTracer(hookAnalysis* analyser, QWidget* parent){
    this->mainAnalyser = analyser;
    this->logList = &analyser->logList;
    this->parent = parent;
}

bool historyTracer::initialize(){
    ifstream logFile("./hookLog/hookLog_complete.log");

    std::string line;
    QString qline;

    // 获取总的日志数量
    getline(logFile, line);
    qline = QString::fromStdString(line);
    qline = qline.mid(12);
    totalStep = qline.toInt();

    // 获取标准输入句柄
    getline(logFile, line);
    qline = QString::fromStdString(line);
    qline = qline.mid(14);
    uint64_t STDIN = qline.toULongLong(nullptr, 16);
    mainAnalyser->exeInfo.STDIN = STDIN;

    // 获取标准输出句柄
    getline(logFile, line);
    qline = QString::fromStdString(line);
    qline = qline.mid(15);
    uint64_t STDOUT = qline.toULongLong(nullptr, 16);
    mainAnalyser->exeInfo.STDOUT = STDOUT;

    // 获取标准错误句柄
    getline(logFile, line);
    qline = QString::fromStdString(line);
    qline = qline.mid(15);
    uint64_t STDERR = qline.toULongLong(nullptr, 16);
    mainAnalyser->exeInfo.STDERR = STDERR;

    // 获取进程名
    getline(logFile, line);
    qline = QString::fromStdString(line);
    qline = qline.mid(14);
    mainAnalyser->exeInfo.processName = qline;

    // 获取文件描述
    getline(logFile, line);
    qline = QString::fromStdString(line);
    qline = qline.mid(18);
    mainAnalyser->exeInfo.fileDescription = qline;

    // 获取文件版本
    getline(logFile, line);
    qline = QString::fromStdString(line);
    qline = qline.mid(14);
    mainAnalyser->exeInfo.fileVersion = qline;

    // 获取文件内部名
    getline(logFile, line);
    qline = QString::fromStdString(line);
    qline = qline.mid(15);
    mainAnalyser->exeInfo.internalName = qline;

    // 获取公司名
    getline(logFile, line);
    qline = QString::fromStdString(line);
    qline = qline.mid(14);
    mainAnalyser->exeInfo.companyName = qline;

    // 获取公司名
    getline(logFile, line);
    qline = QString::fromStdString(line);
    qline = qline.mid(17);
    mainAnalyser->exeInfo.legalCopyright = qline;

    // 获取公司名
    getline(logFile, line);
    qline = QString::fromStdString(line);
    qline = qline.mid(20);
    mainAnalyser->exeInfo.originalFileName = qline;

    // 获取公司名
    getline(logFile, line);
    qline = QString::fromStdString(line);
    qline = qline.mid(20);
    mainAnalyser->exeInfo.productVersion = qline;

    getline(logFile, line);
    // 下面获取所有被截获的API
    while(getline(logFile, line)){
        if(line.starts_with("*"))
            break;
        if(line == "\tMessageBoxA")
            mainAnalyser->injMessageBoxA = true;
        else if(line == "\tMessageBoxW")
            mainAnalyser->injMessageBoxW = true;
        else if(line == "\tHeapCreate")
            mainAnalyser->injHeapCreate = true;
        else if(line == "\tHeapDestroy")
            mainAnalyser->injHeapDestroy = true;
        else if(line == "\tHeapAlloc")
            mainAnalyser->injHeapAlloc = true;
        else if(line == "\tHeapFree")
            mainAnalyser->injHeapFree = true;
        else if(line == "\tCreateFile")
            mainAnalyser->injCreateFile = true;
        else if(line == "\tReadFile")
            mainAnalyser->injReadFile = true;
        else if(line == "\tWriteFile")
            mainAnalyser->injWriteFile = true;
        else if(line == "\tCloseHandle")
            mainAnalyser->injCloseHandle = true;
        else if(line == "\tRegCreateKeyEx")
            mainAnalyser->injRegCreateKeyEx = true;
        else if(line == "\tRegSetValueEx")
            mainAnalyser->injRegSetValueEx = true;
        else if(line == "\tRegDeleteValue")
            mainAnalyser->injRegDeleteValue = true;
        else if(line == "\tRegCloseKey")
            mainAnalyser->injRegCloseKey = true;
        else if(line == "\tRegOpenKeyEx")
            mainAnalyser->injRegOpenKeyEx = true;
        else if(line == "\tRegDeleteKeyEx")
            mainAnalyser->injRegDeleteKeyEx = true;
        else if(line == "\tSend")
            mainAnalyser->injSend = true;
        else if(line == "\tRecv")
            mainAnalyser->injRecv = true;
        else if(line == "\tBind")
            mainAnalyser->injBind = true;
        else if(line == "\tConnect")
            mainAnalyser->injConnect = true;
        else if(line == "\tSocket")
            mainAnalyser->injSocket = true;
        else if(line == "\tAccept")
            mainAnalyser->injAccept = true;
    }

    mainAnalyser->analyseHeap = mainAnalyser->injHeapAlloc && mainAnalyser->injHeapDestroy &&
            mainAnalyser->injHeapAlloc && mainAnalyser->injHeapFree;
    mainAnalyser->analyseFile = mainAnalyser->injCreateFile && mainAnalyser->injReadFile &&
            mainAnalyser->injWriteFile && mainAnalyser->injCloseHandle;
    mainAnalyser->analyseReg = mainAnalyser->injRegCreateKeyEx && mainAnalyser->injRegSetValueEx &&
            mainAnalyser->injRegDeleteValue && mainAnalyser->injRegCloseKey &&
            mainAnalyser->injRegOpenKeyEx && mainAnalyser->injRegDeleteKeyEx;
    mainAnalyser->analyseNet = mainAnalyser->injSend && mainAnalyser->injRecv && mainAnalyser->injBind &&
            mainAnalyser->injConnect && mainAnalyser->injSocket && mainAnalyser->injAccept;

    QProgressDialog pd(parent);
    pd.setWindowTitle("历史记录导入进度");
    pd.setLabelText("正在读取内存截获文件.\\hookLog\\memoryCaps.mcp\n已读取0字节");
    pd.setModal(true);
    pd.setRange(0, totalStep);
    pd.showNormal();

    // 读取内存文件中的内容
    ifstream memCapFile("./hookLog/memoryCaps.mcp", ios::binary);
    int bytesRead = 0;
    while(true){
        uint64_t memAddr = 0;
        map<int, memoryInfo> memMap;
        memoryInfo info;
        memCapFile.read(reinterpret_cast<char*>(&memAddr), 8);
        if(memCapFile.eof())
            break;
        int hookId = 0;
        memCapFile.read(reinterpret_cast<char*>(&hookId), 4);
        int bufLen = 0;
        memCapFile.read(reinterpret_cast<char*>(&bufLen), 4);
        int type = 0;
        memCapFile.read(reinterpret_cast<char*>(&type), 4);
        bufContent btype = (bufContent)type;
        info.length = bufLen;
        info.type = btype;
        char* buffer = (char*)calloc(bufLen, 1);
        memCapFile.read(buffer, bufLen);
        info.content = buffer;
        auto f = mainAnalyser->keyMemories.find(memAddr);
        if(f == mainAnalyser->keyMemories.end()){
            memMap.insert({hookId, info});
            mainAnalyser->keyMemories.insert({memAddr, memMap});
        }else
            f->second.insert({hookId, info});
        bytesRead += bufLen + 20;
        pd.setLabelText(("正在读取内存截获文件.\\hookLog\\memoryCaps.mcp\n已读取" + to_string(bytesRead) + "字节").c_str());
        QCoreApplication::processEvents();
    }

    // 读取日志文件记录
    for(int i=0; i<totalStep; i++){
        getline(logFile, line);
        getline(logFile, line);
        fullLog log;
        qline = QString::fromStdString(line);
        int id = qline.mid(4).toInt();
        assert(id == i);
        log.id = id;

        // 获取时间
        getline(logFile, line);
        qline = QString::fromStdString(line);
        log.time = qline.mid(23);

        // 获取函数名
        getline(logFile, line);
        qline = QString::fromStdString(line);
        log.funcName = qline.split(" ")[0];

        // 获取参数
        getline(logFile, line);
        bool useVar = false;
        while(true){
            getline(logFile, line);
            if(!line.starts_with("\t"))
                break;
            realArg thisArg;

            qline = QString::fromStdString(line).mid(1);
            QStringList elements = qline.split(" ");

            // 获取参数类型
            QString argType = elements[0];
            thisArg.originalTypeName = argType;
            thisArg.type = mainAnalyser->getType(argType);

            // 获取参数名
            QString argName = elements[1];

            // 获取参数值
            QString val = elements[3];
            if((int)thisArg.type <= 8)
                thisArg.value.imm = val.toULongLong();
            else
                thisArg.value.imm = val.mid(2).toULongLong(nullptr, 16);

            if(argName == "dwType" && (thisArg.value.imm == REG_SZ ||
                                       thisArg.value.imm == REG_EXPAND_SZ ||
                                       thisArg.value.imm == REG_MULTI_SZ))
                useVar = true;

            // 当类型是字符串时，获取字符串的内容
            QString str;
            if(thisArg.type == charptr || thisArg.type == wcharptr || (useVar && argName == "lpData")){
                if(elements.size() >= 6){
                    str = qline.split("\"").at(1);
                    thisArg.str = new QString(str);
                }
            }

            // 保存参数
            log.args.insert({argName, thisArg});
        }

        // 获取模块调用链
        qline = QString::fromStdString(line).mid(19);
        auto chain = qline.split("->");
        log.callChain = chain;

        // 获取返回值
        getline(logFile, line);
        qline = QString::fromStdString(line);
        auto elements = qline.split(" ");
        realArg retVal;

        retVal.originalTypeName = elements[2].mid(1, elements[2].length() - 2);
        auto returnType = mainAnalyser->getType(retVal.originalTypeName);
        retVal.type = returnType;

        // 保存返回值的整数值
        auto val = elements[3];
        if((int)retVal.type <= 8)
            retVal.value.imm = val.toLongLong();
        else
            retVal.value.imm = val.toULongLong(nullptr, 16);

        // 保存返回值的字符串内容
        QString str;
        if(retVal.type == charptr || retVal.type == wcharptr){
            if(elements.size() >= 6)
                str = qline.split("\"").at(1);
        }
        if(!str.isEmpty())
            retVal.str = new QString(str);

        log.retVal = retVal;

        // 获取指针参数解引用的值
        getline(logFile, line);
        if(!line.starts_with("-")){
            getline(logFile, line);
            qline = QString::fromStdString(line);

            // 获取需要获取执行后结果的参数值
            while(qline.startsWith("\t")){

                qline = qline.mid(1);
                realArg thisArg;

                QStringList elements = qline.split(" ");

                // 获取参数类型
                QString argType = elements[0];
                thisArg.originalTypeName = argType;
                thisArg.type = mainAnalyser->getType(argType);

                // 获取参数名
                QString argName = elements[1];

                // 获取参数值
                QString val = elements[3];
                if(val.startsWith("0x"))
                    thisArg.value.imm = val.toULongLong(nullptr, 16);
                else
                    thisArg.value.imm = val.toULongLong();

                // 保存参数
                log.argsAfterCall.insert({argName, thisArg});

                getline(logFile, line);
                qline = QString::fromStdString(line);
            }
        }

        char* relatedBuf;
        if(log.funcName == "ReadFile" || log.funcName == "WriteFile" || log.funcName == "send" || log.funcName == "recv"){
            uint64_t memAddr;
            if(log.funcName == "ReadFile" || log.funcName == "WriteFile")
                memAddr = log.args["lpBuffer"].value.imm;
            else
                memAddr = log.args["buf"].value.imm;
            auto f = mainAnalyser->keyMemories.find(memAddr);
            if(f == mainAnalyser->keyMemories.end()){
                QMessageBox::critical(parent, "解析错误", "日志文件内容匹配失败，读取历史记录失败！");
                return false;
            }
            auto fi = f->second.find(log.id);
            if(fi == f->second.end()){
                QMessageBox::critical(parent, "解析错误", "日志文件内容匹配失败，读取历史记录失败！");
                return false;
            }
            relatedBuf = fi->second.content;
            mainAnalyser->diverter(log, relatedBuf);
        }else
            mainAnalyser->diverter(log);

        ++currentStep;
        pd.setValue(i + 1);
        pd.setLabelText(("正在读取截获日志文件.\\hookLog\\hookLog_complete.log\n已读取" + to_string(i) + "/" +
                         to_string(totalStep) + "条记录").c_str());

        mainAnalyser->updateRecordBeauty(log, i == totalStep - 1);
        mainAnalyser->logList.push_back(log);
        QCoreApplication::processEvents();
    }

    parent->show();

    return true;
}

char* historyTracer::getRelatedBuffer(fullLog log){
    char* relatedBuf = nullptr;
    if(log.funcName == "ReadFile" || log.funcName == "WriteFile" || log.funcName == "send" || log.funcName == "recv"){
        uint64_t memAddr;
        if(log.funcName == "ReadFile" || log.funcName == "WriteFile")
            memAddr = log.args["lpBuffer"].value.imm;
        else
            memAddr = log.args["buf"].value.imm;
        auto f = mainAnalyser->keyMemories.find(memAddr);
        if(f == mainAnalyser->keyMemories.end()){
            QMessageBox::critical(parent, "解析错误", "日志文件内容匹配失败，读取历史记录失败！");
            return (char*)-1;
        }
        auto fi = f->second.find(log.id);
        if(fi == f->second.end()){
            QMessageBox::critical(parent, "解析错误", "日志文件内容匹配失败，读取历史记录失败！");
            return (char*)-1;
        }
        relatedBuf = fi->second.content;
    }
    return relatedBuf;
}

bool historyTracer::stepFront(){
    if(currentStep == totalStep)
        return false;
    fullLog nextLog = (*logList)[currentStep];
    this->mainAnalyser->diverter(nextLog, getRelatedBuffer(nextLog));
    currentStep++;
    return true;
}

bool historyTracer::stepBack(){
    if(currentStep == 0)
        return false;
    fullLog lastLog = (*logList)[currentStep - 1];
    mainAnalyser->stepBack(lastLog);
    currentStep--;
    return true;
}
