﻿#include "hookanalysis.h"

#define ADDRESS 0
#define TYPE 1
#define SIZE 2
#define HEAPSTATUS 3

#define FILENAME 1
#define FILEHANDLESTATUS 2

#define REGHANDLE 0
#define REGKEYVAL 1
#define REGSTATUS 2

hookAnalysis::hookAnalysis(QStandardItemModel* tableModel, QStandardItemModel* fileViewModel,
                           QStandardItemModel* exceptionModel, QStandardItemModel* regeditModel,
                           QStandardItemModel* logWidgetModel){
    this->heapViewModel = tableModel;
    this->fileViewModel = fileViewModel;
    this->exceptionModel = exceptionModel;
    this->regeditModel = regeditModel;
    this->logWidgetModel = logWidgetModel;
}

/**
 * @brief hookAnalysis::appendRecord 记录分析主函数
 * @param newRecord 新一条记录
 * @return 处理是否产生异常
 */
bool hookAnalysis::appendRecord(QString newRecord, bool lastRecord){
    fullLog latestLog;
    size_t idxptr;
    bool useVar = false;
    QString str;
    QStringList elements;
    QString val;
    argType returnType;
    realArg retVal;
    QString line;

    // 获取该条记录的编号（此编号唯一确定一条记录，避免重复记录）
    idxptr = newRecord.indexOf("ID: ");
    idxptr += 4;
    unsigned id = newRecord.mid(idxptr, newRecord.indexOf("\n") - idxptr).toUInt();
    latestLog.id = id;

    // 与前一个记录进行比较，判断是否有重复的现象出现。
    if(logList.size() != 0 && !lastRecord){
        fullLog lastLog = *(logList.rbegin());
        if(latestLog < lastLog || latestLog == lastLog)   // 发现重复
            return true;
    }

    // 获取该条记录的时间（函数调用的时间，精确到毫秒）
    idxptr = newRecord.indexOf("DLL log output: ");
    idxptr += 16;
    QString logTime = newRecord.mid(idxptr, 23);
    latestLog.time = logTime;

    // 获取该条记录调用的函数名
    idxptr = newRecord.indexOf("\n", idxptr) + 1;
    size_t idxeptr = newRecord.indexOf(" ", idxptr);
    QString funcName = newRecord.mid(idxptr, idxeptr - idxptr);
    latestLog.funcName = funcName;

    idxptr = newRecord.indexOf("Arguments:");
    idxptr += 11;

    // 获取所有参数
    while(newRecord.mid(idxptr, 1) == "\t"){
        realArg thisArg;

        idxeptr = newRecord.indexOf("\n", idxptr);
        QString line = newRecord.mid(idxptr + 1, idxeptr - idxptr);
        idxptr = idxeptr + 1;
        QStringList elements = line.split(" ");

        // 获取参数类型
        QString argType = elements[0];
        thisArg.originalTypeName = argType;
        thisArg.type = getType(argType);

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
            assert(elements.size() >= 6);
            str = line.split("\"").at(1);
        }
        if(!str.isEmpty())
            thisArg.str = new QString(str);

        // 保存参数
        latestLog.args.insert({argName, thisArg});
    }

    if(lastRecord)
        goto analyse;

    // 获取exe文件信息——进程名
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    if(!exeInfoGot){
        exeInfo.processName = line.mid(22);
        exeFileName = exeInfo.processName;
    }

    // 获取exe加载信息——stdin句柄的值
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    if(!exeInfoGot)
        exeInfo.STDIN = line.mid(14).toULongLong(nullptr, 16);

    // 获取exe加载信息——stdout句柄的值
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    if(!exeInfoGot)
        exeInfo.STDOUT = line.mid(15).toULongLong(nullptr, 16);

    // 获取exe加载信息——stderr句柄的值
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    if(!exeInfoGot)
        exeInfo.STDERR = line.mid(15).toULongLong(nullptr, 16);

    // 获取exe文件信息——文件描述
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    if(!exeInfoGot)
        exeInfo.fileDescription = line.mid(18);

    // 获取exe文件信息——文件版本
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    if(!exeInfoGot)
        exeInfo.fileVersion = line.mid(14);

    // 获取exe文件信息——内部名
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    if(!exeInfoGot)
        exeInfo.internalName = line.mid(15);

    // 获取exe文件信息——公司名
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    if(!exeInfoGot)
        exeInfo.companyName = line.mid(14);

    // 获取exe文件信息——版权
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    if(!exeInfoGot)
        exeInfo.legalCopyright = line.mid(17);

    // 获取exe文件信息——原始名
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    if(!exeInfoGot)
        exeInfo.originalFileName = line.mid(20);

    // 获取exe文件信息——产品版本
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    if(!exeInfoGot)
        exeInfo.productVersion = line.mid(17);

    // 获取函数返回值，如果该返回值是一个字符串指针，则会同时保存字符串的值，所有返回值都会保存其值（整数值或指针值）
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    elements = line.split(" ");
    returnType = getType(elements[2].mid(1, elements[2].length() - 2));
    retVal.type = returnType;

    // 保存返回值的整数值
    val = elements[3];
    if((int)retVal.type <= 8)
        retVal.value.imm = val.toLongLong();
    else
        retVal.value.imm = val.toULongLong(nullptr, 16);

    // 保存返回值的字符串内容
    if(retVal.type == charptr || retVal.type == wcharptr){
        assert(elements.size() >= 6);
        str = line.split("\"").at(1);
    }
    if(!str.isEmpty())
        retVal.str = new QString(str);

    latestLog.retVal = retVal;
    exeInfoGot = true;

    // 有些函数会要求传入指针参数，将一些函数的执行结果保存到参数中的指针，如ReadFile和WriteFile就是将成功读取/写入的字符数量保存到传入的指针参数中。
    // 为了全面体现函数的执行结果，这些指针在函数执行后解引用的值也应该被获取，以便及时发现任何异常情况。
    if(newRecord[idxptr] != '-'){
        idxeptr = newRecord.indexOf("\n", idxptr);
        idxptr = idxeptr + 1;
        idxeptr = newRecord.indexOf("\n", idxptr);
        line = newRecord.mid(idxptr, idxeptr - idxptr);
        idxptr = idxeptr + 1;
        // 获取需要获取执行后结果的参数值
        while(line.startsWith("\t")){
            line = line.mid(1);
            realArg thisArg;

            QStringList elements = line.split(" ");

            // 获取参数类型
            QString argType = elements[0];
            thisArg.originalTypeName = argType;
            thisArg.type = getType(argType);

            // 获取参数名
            QString argName = elements[1];

            // 获取参数值
            QString val = elements[3];
            if(val.startsWith("0x"))
                thisArg.value.imm = val.toULongLong(nullptr, 16);
            else
                thisArg.value.imm = val.toULongLong();

            // 保存参数
            latestLog.argsAfterCall.insert({argName, thisArg});

            idxeptr = newRecord.indexOf("\n", idxptr);
            line = newRecord.mid(idxptr + 1, idxeptr - idxptr);
            idxptr = idxeptr + 1;
        }
    }

    // 如果这个函数与堆操作有关系，则进入堆分析模块进行进一步行为分析。
    // 如果这个函数与文件操作有关系，则进入文件分析模块进行进一步行为分析。
    // 如果这个函数与注册表操作有关系，则进入注册表分析模块进行进一步行为分析。
    analyse:
    if(latestLog.id == logList.size() - 1 && lastRecord)
        return false;
    else if(lastRecord)
        latestLog.retVal.value.imm = 0xdeadbeefcafebabe;
    logList.push_back(latestLog);
    updateRecordBeauty(latestLog);
    if(latestLog.funcName.contains("Heap") && analyseHeap)
        analyseNewHeapInst(latestLog);
    else if((latestLog.funcName.contains("File") || latestLog.funcName == "CloseHandle") && analyseFile)
        analyseNewFileInst(latestLog);
    else if(latestLog.funcName.contains("Reg") && analyseReg)
        analyseNewRegInst(latestLog);
    return false;
}

bool hookAnalysis::updateRecordBeauty(fullLog latestLog){
    int rowCount = logWidgetModel->rowCount();
    logWidgetModel->insertRow(rowCount, QList<QStandardItem*>() <<
                              new QStandardItem(to_string(latestLog.id).c_str()) <<
                              new QStandardItem(latestLog.funcName));
    auto father = logWidgetModel->item(rowCount);
    for(const auto &arg : latestLog.args){
        if(arg.second.str == nullptr)
            father->appendRow(QList<QStandardItem*>() <<
                              new QStandardItem(arg.first) <<
                              new QStandardItem(arg.second.originalTypeName) <<
                              new QStandardItem(ull2a(arg.second.value.imm)));
        else
            father->appendRow(QList<QStandardItem*>() <<
                              new QStandardItem(arg.first) <<
                              new QStandardItem(arg.second.originalTypeName) <<
                              new QStandardItem(ull2a(arg.second.value.imm)) <<
                              new QStandardItem(*arg.second.str));
    }
    return true;
}

argType hookAnalysis::getType(QString input){
    argType type = others;
    if(input == "HANDLE" || input == "LPVOID" || input == "HKEY")
        type = voidptr;
    else if(input == "DWORD" || input == "UINT" || input == "REGSAM")
        type = uint32;
    else if(input == "int" || input == "HFILE")
        type = int32;
    else if(input == "SIZE_T")
        type = int64;
    else if(input == "LPCSTR")
        type = charptr;
    else if(input == "LPCWSTR")
        type = wcharptr;
    else if(input == "BOOL")
        type = Bool;
    return type;
}

// 堆结构分析入口函数，对HeapCreate、HeapDestroy、HeapAlloc、HeapFree相关HANDLE和CHUNK进行跟踪分析。
// 由于在调试过程中发现有一部分HANDLE没有被HeapCreate追踪到，因此这里需要根据HeapAlloc中传入的HANDLE类型参数判断每一个CHUNK所在的HANDLE。
// 如果这个HANDLE在之前并没有追踪到被HeapCreate所生成，那么将自动添加。由于HANDLE的大小不固定，所以HANDLE一栏不填大小，只有起始地址。
void hookAnalysis::analyseNewHeapInst(fullLog newHeapLog){
    if(newHeapLog.funcName == "HeapCreate"){
        // 分析HeapCreate行为
        addHandle(newHeapLog);
    }else if(newHeapLog.funcName == "HeapDestroy"){
        // 分析HeapDestroy行为
        destroyHandle(newHeapLog);
    }else if(newHeapLog.funcName == "HeapAlloc"){
        // 分析HeapAlloc行为
        addChunk(newHeapLog);
    }else if(newHeapLog.funcName == "HeapFree"){
        // 分析HeapFree行为
        freeChunk(newHeapLog);
    }else{
        exit(1);
    }
}

// 文件操作分析入口函数，对OpenFile、CreateFile、ReadFile、WriteFile相关文件进行跟踪分析。
void hookAnalysis::analyseNewFileInst(fullLog newFileLog){
    if(newFileLog.funcName == "OpenFile"){

    }else if(newFileLog.funcName == "CreateFile"){
        addFileHandle(newFileLog);
    }else if(newFileLog.funcName == "ReadFile"){
        addReadWriteFileRecord(newFileLog, true);
    }else if(newFileLog.funcName == "WriteFile"){
        addReadWriteFileRecord(newFileLog, false);
    }else if(newFileLog.funcName == "CloseHandle"){
        closeFileHandle(newFileLog);
    }else{
        exit(1);
    }
}

// 注册表操作分析入口函数
void hookAnalysis::analyseNewRegInst(fullLog newRegLog){
    if(newRegLog.funcName == "RegCreateKeyEx"){
        addRegKey(newRegLog);
    }else if(newRegLog.funcName == "RegSetValueEx"){
        setRegKey(newRegLog);
    }else if(newRegLog.funcName == "RegDeleteValue"){
        deleteRegKey(newRegLog);
    }else if(newRegLog.funcName == "RegCloseKey"){
        closeRegKey(newRegLog);
    }else if(newRegLog.funcName == "RegOpenKeyEx"){
        openRegKey(newRegLog);
    }else
        exit(1);
}

/**
 * @brief hookAnalysis::addHandle 添加一个新HANDLE
 * @param newHeapLog 最新一条截获日志
 * @return 是否插入成功
 */
bool hookAnalysis::addHandle(fullLog newHeapLog){
    uint64_t allocatedHandle = newHeapLog.retVal.value.imm; // 获取该函数的返回值，即分配到的堆起始地址
    if((void*)allocatedHandle == INVALID_HANDLE_VALUE || !allocatedHandle){       // 如果返回值为0，说明分配失败，不需要进行后续操作
        handleException({newHeapLog.id, AllocHandleFailed},
                        new exceptionInfo{.addressWithException = newHeapLog.args["dwInitialSize"].value.imm});
        return false;
    }

    int findHandleIdx = findHandle(allocatedHandle, heapViewModel);                 // 查找这个HANDLE是否已经被监视
    if(findHandleIdx < 0){      // 没有找到这个HANDLE，在表中新增
        insertNewHeapHandle(allocatedHandle, -findHandleIdx - 1);
        heapHandlesExpl->insert(allocatedHandle);
    }else{
        // 首先判断这个HANDLE目前是否正在使用，如果之前监测到明确的HeapDestroy函数将其销毁，那么再一次分配到这个HANDLE就是异常的行为。
        if(heapViewModel->item(findHandleIdx, HEAPSTATUS)->text() == "已被销毁"){
            handleException({newHeapLog.id, AllocSameHandle},
                            new exceptionInfo{.addressWithException = allocatedHandle});
        }
        else
            heapViewModel->item(findHandleIdx, HEAPSTATUS)->setText("正在使用");   // 修改HANDLE状态
    }
    return true;
}

/**
 * @brief hookAnalysis::destroyHandle 销毁一个HANDLE
 * @param newHeapLog 最新一条截获日志
 * @return 是否销毁成功
 */
bool hookAnalysis::destroyHandle(fullLog newHeapLog){
    uint64_t destroyFinished = newHeapLog.retVal.value.imm;     // 获取该函数的返回值，即是否销毁成功
    uint64_t handleToDestroy = newHeapLog.args["hHeap"].value.imm;
    int findHandleIdx = findHandle(handleToDestroy, heapViewModel);

    if(findHandleIdx >= 0 && heapViewModel->item(findHandleIdx, HEAPSTATUS)->text() == "已被销毁"){
        handleException({newHeapLog.id, DoubleDestroy},
                        new exceptionInfo{.addressWithException = handleToDestroy});
        return false;
    }

    if(!destroyFinished || destroyFinished == 0xdeadbeefcafebabe){
        // 如果返回值为false，说明销毁失败，查找销毁失败原因，如果要销毁的HANDLE在监视范围，则产生未知错误，如果不在则认定为无效参数
        if(findHandleIdx < 0){       // 在监视列表中没有找到这个HANDLE，视为无效参数
            handleException({newHeapLog.id, InvalidDestroyHandle},
                            new exceptionInfo{.addressWithException = handleToDestroy});
        }else{
            handleException({newHeapLog.id, DestroyFailed},
                            new exceptionInfo{.addressWithException = handleToDestroy});
        }
        return false;
    }

    // 如果返回值为true，而且要销毁的HANDLE不在监视范围内，则说明该HANDLE在创建时没有被跟踪到，不添加该HANDLE，且新增异常信息
    if(findHandleIdx < 0){
        handleException({newHeapLog.id, UntrackedHandleDestroyed},
                        new exceptionInfo{.addressWithException = handleToDestroy});
        return false;
    }

    // 将HANDLE设置为销毁状态，同时将其内部所有CHUNK均设置为无效状态
    // 会对其内部的CHUNK进行检查，如果有CHUNK正在被使用而这个HANDLE先被销毁，则目标程序再次访问这个CHUNK就会产生SIGSEGV错误。
    heapViewModel->item(findHandleIdx, HEAPSTATUS)->setText("已被销毁");
    QStandardItem* father = heapViewModel->item(findHandleIdx);
    for(int i=0; i<father->rowCount(); i++){
        // 判断CHUNK的使用状态
        if(father->child(i, HEAPSTATUS)->text() == "正在使用")      // 正在使用的CHUNK所在的HANDLE被销毁
            memoryLeakRisks.insert({newHeapLog.id, father->child(i)->text().toULongLong(nullptr, 16)});
        father->child(i, HEAPSTATUS)->setText("无效");
    }
    heapHandlesExpl->erase(heapHandlesExpl->find(handleToDestroy));
    return true;
}

/**
 * @brief hookAnalysis::addChunk 添加一个新CHUNK，当这个CHUNK所在的HANDLE没有被追踪时，添加该HANDLE。
 * @param newHeapLog 最新一条截获日志
 * @param handleIdx
 * @return 是否插入成功
 */
bool hookAnalysis::addChunk(fullLog newHeapLog){

    uint64_t allocatedChunk = newHeapLog.retVal.value.imm;      // 获取该函数的返回值，即分配到的地址
    if(allocatedChunk == 0){            // 如果返回值为0，说明分配失败，不需要进行后续操作
        handleException({newHeapLog.id, AllocChunkFailed},
                        new exceptionInfo{
                            .allocFail = {
                                          .handle = newHeapLog.args["hHeap"].value.imm,
                                          .requiredSize = newHeapLog.args["dwBytes"].value.imm
                            }
                        });
        return false;
    }

    uint64_t handleAddr = newHeapLog.args["hHeap"].value.imm;       // 获取该CHUNK所属的handle地址
    int findHandleIdx = findHandle(handleAddr, heapViewModel);                 // 查找这个HANDLE是否已经被监视
    if(findHandleIdx < 0)      // 没有找到这个HANDLE，在表中新增
        insertNewHeapHandle(handleAddr, -findHandleIdx - 1);
    else{
        // 首先判断这个HANDLE目前是否正在使用，如果之前监测到明确的HeapDestroy函数将其销毁，那么再一次分配到这个HANDLE就是异常的行为。
        // 但即使存在这种异常行为，程序依然会记录这个CHUNK，除非其返回值为nullptr。
        if(heapViewModel->item(findHandleIdx, HEAPSTATUS)->text() == "已被销毁"){
            handleException({newHeapLog.id, AllocToDestroyedHandle},
                            new exceptionInfo{.addressWithException = handleAddr});
        }
        // 下面将这个CHUNK保存到对应HANDLE的子项中。
        // 首先需要找到这个CHUNK应该插入到哪一行。
        QStandardItem* father = heapViewModel->item(findHandleIdx);
        uint64_t chunkSize = newHeapLog.args["dwBytes"].value.imm;
        int findChunkIdx = findChunk(allocatedChunk, father);
        if(findChunkIdx < 0)        // 没有找到这个CHUNK，在子表中新增
            insertNewHeapChild(allocatedChunk, chunkSize, father, -findChunkIdx - 1);
        else{                       // 找到这个CHUNK，只需要修改其size和使用状态即可。
            // 首先判断这个找到的CHUNK是否正在被使用，如果正在被使用，那么产生异常
            // （这种异常经常会产生，因为有的CHUNK并不是程序本身调用malloc等堆相关函数来产生的，因此有的CHUNK被释放的操作可能不能跟踪到）
            // 但这里只会对这种情况记录异常，如果size有改动还是会继续进行修改。
            if(father->child(findChunkIdx, HEAPSTATUS)->text() == "正在使用"){
                handleException({newHeapLog.id, AllocSameChunk},
                                new exceptionInfo{.addressWithException = allocatedChunk});
            }
            father->child(findChunkIdx, SIZE)->setText(ull2a(chunkSize));       // 修改size
            father->child(findChunkIdx, HEAPSTATUS)->setText("正在使用");            // 修改状态
        }
        // 如果一个堆句柄没有检测到其被HeapCreate分配而是根据HeapAlloc监测到，那么这样的堆中分配和释放堆块均不计数
        // 如果有一个堆块被成功分配到了已经被删除的堆句柄，且这个堆句柄明确使用HeapCreate创建，这样的堆块也不会被计数
        if(heapHandlesExpl->find(handleAddr) != heapHandlesExpl->end())
            chunksExpl->insert(allocatedChunk);
    }
    return true;
}

bool hookAnalysis::freeChunk(fullLog newHeapLog){

    uint64_t success = newHeapLog.retVal.value.imm;     // 本次释放是否成功
    if(!success){       // 不成功，返回异常信息
        handleException({newHeapLog.id, FreeChunkFailed},
                        new exceptionInfo{.addressWithException = newHeapLog.args["lpMem"].value.imm});
        return false;
    }

    uint64_t victim = newHeapLog.args["lpMem"].value.imm;
    uint64_t handleAddr = newHeapLog.args["hHeap"].value.imm;

    // 首先根据HANDLE地址查找HANDLE，如果没有找到，说明这个CHUNK也没有被追踪，那么其大小就未知。
    int findHandleIdx = findHandle(handleAddr, heapViewModel);                 // 查找这个HANDLE是否已经被监视

    if(success == 0xdeadbeefcafebabe){        // 程序崩溃，找出原因
        if(findHandleIdx < 0)   // 这个堆的句柄都找不到，认定为无效CHUNK
            handleException({newHeapLog.id, InvalidFree},
                            new exceptionInfo{.addressWithException = victim});
        else{
            int findChunkIdx = findChunk(victim, heapViewModel->item(findHandleIdx));
            if(findChunkIdx < 0)    // 能够找到堆，但是找不到这个CHUNK
                handleException({newHeapLog.id, InvalidFree},
                                new exceptionInfo{.addressWithException = victim});
            else if(heapViewModel->item(findHandleIdx)->child(findChunkIdx, HEAPSTATUS)->text() == "正在使用")        // 能够找到这个CHUNK，但是释放失败而且程序还崩溃了
                handleException({newHeapLog.id, FreeChunkFailed},
                                new exceptionInfo{.addressWithException = victim});
            else
                handleException({newHeapLog.id, DoubleFree},
                                new exceptionInfo{.addressWithException = victim});
        }
        return false;
    }

    if(findHandleIdx < 0){      // 没有找到这个HANDLE，在表中新增，同时增加这个CHUNK，新增异常
        insertNewHeapHandle(handleAddr, -findHandleIdx - 1);
        insertUnknownSizeChunk(victim, heapViewModel->item(-findHandleIdx - 1), 0);
        handleException({newHeapLog.id, UntrackedChunkFreed},
                        new exceptionInfo{.addressWithException = victim});
        validFreeCount++;
        return true;
    }

    // 如果HANDLE能够被找到，再查询这个CHUNK是否被监视，如果没有则新增。
    QStandardItem* father = heapViewModel->item(findHandleIdx);
    int findChunkIdx = findChunk(victim, father);
    if(findChunkIdx < 0){       // 没有找到这个CHUNK
        insertUnknownSizeChunk(victim, father, -findChunkIdx - 1);
        handleException({newHeapLog.id, UntrackedChunkFreed},
                        new exceptionInfo{.addressWithException = victim});
    }else{  // 找到这个CHUNK
        // 判断这个CHUNK是否正在被使用，如果已经释放，说明发现了Double Free
        if(father->child(findChunkIdx, HEAPSTATUS)->text() == "已被释放"){
            handleException({newHeapLog.id, DoubleFree},
                            new exceptionInfo{.addressWithException = victim});
        }else{
            father->child(findChunkIdx, HEAPSTATUS)->setText("已被释放");
            auto removeIter = chunksExpl->find(victim);
            assert(removeIter != chunksExpl->end());
            chunksExpl->erase(removeIter);
        }
    }
    validFreeCount++;
    return true;
}

void hookAnalysis::setChildRow(QStandardItem* parent, int row, QStringList elements){
    for(int i=0; i<4; i++)
        parent->setChild(row, i, new QStandardItem(elements[i]));
}

void hookAnalysis::setRow(int row, QStringList elements){
    for(int i=0; i<4; i++)
        heapViewModel->setItem(row, i, new QStandardItem(elements[i]));
}

/**
 * @brief hookAnalysis::findHandle 根据参数的HANDLE地址从堆信息中找到对应的HANDLE位置索引
 *                                 需要注意的是，一个HANDLE需要插入到索引为0的位置和一个HANDLE需要加入到空表中都会返回0
 *                                 因此当CHUNK没有找到时，其返回值为(需要插入的位置 * -1) - 1，以防止歧义发生。
 * @param handleAddr 需要查找的地址
 * @return 索引，如果能够找到这个HANDLE，返回这个HANDLE位于哪一行，如果找不到，返回这个HANDLE应该插入在哪一行 * -1。
 */
int hookAnalysis::findHandle(uint64_t handleAddr, QStandardItemModel *Model){
    int rowCount = Model->rowCount();
    if(rowCount == 0)
        return -1;
//    int rowPtr = 0;

    int floor = 0, ceil = rowCount;
    if(rowCount == 0)
        return -1;
    if(Model->item(0)->text().toULongLong(nullptr, 16) > handleAddr)
        return -1;
    else if(Model->item(0)->text().toULongLong(nullptr, 16) == handleAddr)
        return 0;
    if(Model->item(rowCount-1)->text().toULongLong(nullptr, 16) < handleAddr)
        return -rowCount-1;
    else if(Model->item(rowCount-1)->text().toULongLong(nullptr, 16) == handleAddr)
        return rowCount - 1;
    while(floor != ceil - 1){
        auto findHandle = Model->item((floor + ceil) / 2)->text().toULongLong(nullptr, 16);
        if(findHandle == handleAddr)
            return (floor + ceil) / 2;
        else if(findHandle > handleAddr)
            ceil = (floor + ceil) / 2;
        else
            floor = (floor + ceil) / 2;
    }
    return -floor-2;

//    for(; rowPtr < rowCount; rowPtr++){
//        uint64_t findHandle = Model->item(rowPtr)->text().toULongLong(nullptr, 16);
//        if(findHandle == handleAddr)
//            return rowPtr;
//        else if(findHandle > handleAddr)
//            return -rowPtr - 1;
//    }
//    return -rowCount - 1;
}

/**
 * @brief hookAnalysis::findChunk 根据参数的chunk地址从一个HANDLE中找到对应的HANDLE位置索引
 *                                需要注意的是，一个CHUNK需要插入到索引为0的位置和一个CHUNK需要加入到空子表中都会返回0
 *                                因此当CHUNK没有找到时，其返回值为(需要插入的位置 * -1) - 1，以防止歧义发生。
 * @param chunkAddr 需要查找的地址
 * @param father HANDLE父项对象
 * @return 索引，如果能够找到这个CHUNK，返回这个CHUNK位于哪一行，如果找不到，返回这个CHUNK应该插入在哪一行 * -1。
 */
int hookAnalysis::findChunk(uint64_t chunkAddr, QStandardItem* father){
    int rowCount = father->rowCount();
//    int rowPtr = 0;
    char* handlecmp = ull2a(chunkAddr);

    int floor = 0, ceil = rowCount;
    if(rowCount == 0)
        return -1;
    if(father->child(0)->text() > handlecmp)
        return -1;
    else if(father->child(0)->text() == handlecmp)
        return 0;
    if(father->child(rowCount-1)->text() < handlecmp)
        return -rowCount-1;
    else if(father->child(rowCount-1)->text() == handlecmp)
        return rowCount - 1;
    while(floor != ceil - 1){
        auto findHandle = father->child((floor + ceil) / 2)->text();
        if(findHandle == handlecmp)
            return (floor + ceil) / 2;
        else if(findHandle > handlecmp)
            ceil = (floor + ceil) / 2;
        else
            floor = (floor + ceil) / 2;
    }
    return -floor-2;

//    for(; rowPtr < rowCount; rowPtr++){
//        uint64_t findChunk = father->child(rowPtr, 0)->text().toULongLong(nullptr, 16);
//        if(findChunk == chunkAddr)
//            return rowPtr;
//        else if(findChunk > chunkAddr)
//            return -rowPtr - 1;
//    }
//    return -rowCount - 1;
}

/**
 * @brief hookAnalysis::findChunk 根据参数的handle地址从列表中找到对应注册表句柄位置索引
 * @param chunkAddr 需要查找的地址
 * @param father HANDLE父项对象
 * @return 索引，如果能够找到这个handle，返回这个CHUNK位于哪一行，如果找不到，返回这个handle应该插入在哪一行 * -1。
 */
int hookAnalysis::findRegKey(uint64_t handleAddr){
    return findHandle(handleAddr, regeditModel);
//    int rowCount = regeditModel->rowCount();
//    int rowPtr = 0;
//    for(; rowPtr < rowCount; rowPtr++){
//        uint64_t findHandle = regeditModel->item(rowPtr, REGHANDLE)->text().toULongLong(nullptr, 16);
//        if(findHandle == handleAddr)
//            return rowPtr;
//        else if(findHandle > handleAddr)
//            return -rowPtr - 1;
//    }
//    return -rowCount - 1;
}

void hookAnalysis::insertNewHeapHandle(uint64_t handleAddr, int insPos){
    heapViewModel->insertRow(insPos);
    setRow(insPos, QStringList() << ull2a(handleAddr) << "HANDLE" << "" << "正在使用");
}

void hookAnalysis::insertNewHeapChild(uint64_t chunkAddr, uint64_t size, QStandardItem* father, int insPos){
    father->insertRow(insPos, QList<QStandardItem*>() << new QStandardItem(ull2a(chunkAddr)) <<
                      new QStandardItem("CHUNK") <<
                      new QStandardItem(ull2a(size)) <<
                      new QStandardItem("正在使用"));
}

void hookAnalysis::insertUnknownSizeChunk(uint64_t chunkAddr, QStandardItem* father, int insPos){
    father->insertRow(insPos, QList<QStandardItem*>() << new QStandardItem(ull2a(chunkAddr)) <<
                      new QStandardItem("CHUNK") <<
                      new QStandardItem("???") <<
                      new QStandardItem("已被释放"));
}

/**
 * @brief hookAnalysis::addFileHandle 添加一个新的文件句柄跟踪
 * @param newFileLog 最新操作日志
 * @return 是否添加成功
 */
bool hookAnalysis::addFileHandle(fullLog newFileLog){
    // 首先判断文件句柄是否成功获取
    uint64_t returnHandle = newFileLog.retVal.value.imm;
    QString fileName = *newFileLog.args["lpFileName"].str;
    // 如果句柄无效
    if(returnHandle == (uint64_t)INVALID_HANDLE_VALUE){
        handleException({newFileLog.id, CreateFileFailed},
                        new exceptionInfo{.fileName = newFileLog.args["lpFileName"].str});
        return false;
    }

    // 如果句柄有效，首先查找表中是否已经存在这个句柄
    int findHandleIdx = findHandle(returnHandle, fileViewModel);
    if(findHandleIdx >= 0 && fileViewModel->item(findHandleIdx, FILEHANDLESTATUS)->text() == "正在使用"){      // 如果查找到了这个HANDLE，输出异常信息
        handleException({newFileLog.id, RepeatedFileHandle},
                        new exceptionInfo{.addressWithException = returnHandle});
        return false;
    }else if(findHandleIdx >= 0 && fileViewModel->item(findHandleIdx, FILEHANDLESTATUS)->text() == "已被关闭"){
        // 句柄存在，但已经被关闭，则重新分配这个句柄
        extractDir(newFileLog);
        fileViewModel->item(findHandleIdx, FILENAME)->setText(fileName);
        fileViewModel->item(findHandleIdx, FILEHANDLESTATUS)->setText("正在使用");
        fileViewModel->item(findHandleIdx)->appendRow(QList<QStandardItem*>() <<
                                                      new QStandardItem("OPEN") <<
                                                      new QStandardItem(fileName) << nullptr <<
                                                      new QStandardItem("SUCCESS"));
        return true;
    }

    // 如果没有这个句柄，则插入这个句柄，展示相关信息
    extractDir(newFileLog);
    insertNewFileHandle(newFileLog, findHandleIdx);
    return true;
}

/**
 * @brief hookAnalysis::addReadFileRecord 添加一个对于指定文件的读取操作的记录
 * @param newFileLog 最新操作日志
 * @return 是否添加成功
 */
bool hookAnalysis::addReadWriteFileRecord(fullLog newFileLog, bool isRead){
    // 文件是否读取成功
    bool success = newFileLog.retVal.value.imm;
    // 操作的文件句柄
    uint64_t handle = newFileLog.args["hFile"].value.imm;
    // 如果这个文件句柄是标准输入输出句柄，则不进行分析
    if(handle == exeInfo.STDIN || handle == exeInfo.STDOUT || handle == exeInfo.STDERR)
        return true;
    // 检查这个文件句柄是否正在被监控，如果没有则放弃本次添加
    int findHandleIdx = findHandle(handle, fileViewModel);
    if(findHandleIdx < 0){      // 如果没有查找到这个HANDLE，输出异常信息
        handleException({newFileLog.id, UntrackedFileHandle},
                        new exceptionInfo{.addressWithException = handle});
        return false;
    }

    auto father = fileViewModel->item(findHandleIdx);
    QString fName = fileViewModel->item(findHandleIdx, FILENAME)->text();
    QString fileSuffix = fName.split(".").last();
    if(fileSuffix == "exe" || fileSuffix == "dll" || fileSuffix == "ocx"){
        if(isRead){
            handleException({newFileLog.id, TryReadExecutableFile},
                            new exceptionInfo{.fileName = new QString(fName)});
            exeFileContentPtr.insert({newFileLog.args["lpBuffer"].value.imm,
                                         newFileLog.args["nNumberOfBytesToRead"].value.imm});
        }
        else{
            handleException({newFileLog.id, TryWriteExecutableFile},
                            new exceptionInfo{.fileName = new QString(fName)});
        }
    }
    if(isRead){
        // 要读取的字节数
        uint64_t requiredBytes = newFileLog.args["nNumberOfBytesToRead"].value.imm;
        // 实际读取的字节数
        uint64_t actualBytes = newFileLog.argsAfterCall["lpNumberOfBytesRead"].value.imm;
        father->insertRow(father->rowCount(), QList<QStandardItem*>() <<
                          new QStandardItem("READ") <<
                          new QStandardItem(ull2a(requiredBytes)) <<
                          new QStandardItem(ull2a(actualBytes)) <<
                          new QStandardItem(success ? "SUCCESS" : "FAILED"));
    }
    else{
        // 要读取的字节数
        uint64_t requiredBytes = newFileLog.args["nNumberOfBytesToWrite"].value.imm;
        // 实际读取的字节数
        uint64_t actualBytes = newFileLog.argsAfterCall["lpNumberOfBytesWritten"].value.imm;
        father->insertRow(father->rowCount(), QList<QStandardItem*>() <<
                          new QStandardItem("WRITE") <<
                          new QStandardItem(ull2a(requiredBytes)) <<
                          new QStandardItem(ull2a(actualBytes)) <<
                          new QStandardItem(success ? "SUCCESS" : "FAILED"));
    }
    return true;
}

bool hookAnalysis::closeFileHandle(fullLog newFileLog){
    uint64_t success = newFileLog.retVal.value.imm;
    uint64_t handle = newFileLog.args["hObject"].value.imm;

    // 尝试关闭标准输入输出句柄，直接产生异常信息
    if(handle == exeInfo.STDIN || handle == exeInfo.STDOUT || handle == exeInfo.STDERR){
        handleException({newFileLog.id, TryCloseStdioHandle},
                new exceptionInfo{.addressWithException = handle});
        return false;
    }

    int findHandleIdx = findHandle(handle, fileViewModel);
    if(!success || success == 0xdeadbeefcafebabe){  // 文件句柄关闭失败，查找原因
        if(findHandleIdx >= 0 && fileViewModel->item(findHandleIdx, FILEHANDLESTATUS)->text() == "正在使用"){
            QString fileName = fileViewModel->item(findHandleIdx, 1)->text();
            handleException({newFileLog.id, CloseFileHandleFailed},
                 new exceptionInfo{.addressWithException = handle});
            fileViewModel->item(findHandleIdx)->appendRow(QList<QStandardItem*>() <<
                                                          new QStandardItem("CLOSE") <<
                                                          new QStandardItem(fileName) << nullptr <<
                                                          new QStandardItem("FAILED"));
        }else if(findHandleIdx >= 0 && fileViewModel->item(findHandleIdx, FILEHANDLESTATUS)->text() == "已被关闭"){
            QString fileName = fileViewModel->item(findHandleIdx, 1)->text();
            handleException({newFileLog.id, TryCloseClosedHandle},
                 new exceptionInfo{.addressWithException = handle});
            fileViewModel->item(findHandleIdx)->appendRow(QList<QStandardItem*>() <<
                                                          new QStandardItem("CLOSE") <<
                                                          new QStandardItem(fileName) << nullptr <<
                                                          new QStandardItem("FAILED"));
        }else
            handleException({newFileLog.id, InvalidFileHandleToClose},
                 new exceptionInfo{.addressWithException = handle});
        return false;
    }

    if(findHandleIdx >= 0 && fileViewModel->item(findHandleIdx, FILEHANDLESTATUS)->text() == "已被关闭")
        handleException({newFileLog.id, DoubleCloseHandle},
             new exceptionInfo{.addressWithException = handle});
    else if(findHandleIdx < 0){
        handleException({newFileLog.id, CloseUntrackedFileHandle},
             new exceptionInfo{.addressWithException = handle});
        return false;
    }

    QString fileName = fileViewModel->item(findHandleIdx, 1)->text();
    fileViewModel->item(findHandleIdx, FILEHANDLESTATUS)->setText("已被关闭");
    fileViewModel->item(findHandleIdx)->appendRow(QList<QStandardItem*>() <<
                                                  new QStandardItem("CLOSE") <<
                                                  new QStandardItem(fileName) << nullptr <<
                                                  new QStandardItem("SUCCESS"));
    return true;
}

void hookAnalysis::extractDir(fullLog newFileLog){
    if(newFileLog.funcName != "CreateFile" || ((HANDLE)newFileLog.retVal.value.imm == INVALID_HANDLE_VALUE))
        return;
    auto relatedDir = newFileLog.args["lpFileName"].str->split("\\");
    relatedDir.erase(relatedDir.end() - 1);
    auto currentDirLayers = currentDir.split("\\");
    currentDirLayers.append(relatedDir);
    for(auto iter = currentDirLayers.begin(); iter != currentDirLayers.end();){
        if(*iter == ".")
            iter = currentDirLayers.erase(iter);
        else if(*iter == ".."){
            iter = currentDirLayers.erase(iter);
            iter--;
            if(iter != currentDirLayers.begin())
                iter = currentDirLayers.erase(iter);
            iter++;
        }else
            iter++;
    }
    QString newdir = currentDirLayers.join("\\");
    if(!relatedDirs.contains(newdir) && !relatedDirs.empty())
        handleException({newFileLog.id, RelatingMultipleDirs},
                        new exceptionInfo{.fileName = new QString(newdir)});
    relatedDirs.insert(newdir);

}

/**
 * @brief getAbsolutePath 通过相对路径和当前路径获取绝对路径
 * @param relativePath 相对路径
 * @return 绝对路径
 */
QString hookAnalysis::getAbsolutePath(QString relativePath){
    auto relativeLayers = relativePath.split("\\");
    auto currentDirLayers = currentDir.split("\\");
    currentDirLayers.append(relativeLayers);
    for(auto iter = currentDirLayers.begin(); iter != currentDirLayers.end();){
        if(*iter == ".")
            iter = currentDirLayers.erase(iter);
        else if(*iter == ".."){
            iter = currentDirLayers.erase(iter);
            iter--;
            if(iter != currentDirLayers.begin())
                iter = currentDirLayers.erase(iter);
            iter++;
        }else
            iter++;
    }
    return currentDirLayers.join("\\");
}

void hookAnalysis::insertNewFileHandle(fullLog log, int insPos){
    assert(log.args["lpFileName"].str != nullptr);
    QString fileName = *log.args["lpFileName"].str;
    // 如果文件名等于其本身exe文件名，报出异常
    if(getAbsolutePath(fileName) == exeFileName)
        handleException({log.id, OpeningSelfExeHandle}, new exceptionInfo{});
    QString fileSuffix = fileName.split(".").last();
    if(fileSuffix == "exe" || fileSuffix == "dll" || fileSuffix == "ocx")
        handleException({log.id, OpeningExeHandle}, new exceptionInfo{});
    fileHandleAttr newHandleAttr;
    newHandleAttr.handleAddr = log.retVal.value.imm;
    newHandleAttr.fileName = *log.args["lpFileName"].str;
    newHandleAttr.access = log.args["dwDesiredAccess"].value.imm;
    newHandleAttr.shareMode = log.args["dwShareMode"].value.imm;
    newHandleAttr.createDisp = log.args["dwCreationDisposition"].value.imm;
    newHandleAttr.flagAttr = log.args["dwFlagsAndAttributes"].value.imm;
    fileHandles.insert({newHandleAttr.handleAddr, newHandleAttr});

    fileViewModel->insertRow(-insPos - 1, QList<QStandardItem*>() << new QStandardItem(ull2a(newHandleAttr.handleAddr))
                             << new QStandardItem(fileName) << new QStandardItem("正在使用"));
}

void hookAnalysis::insertNewRegHandle(fullLog log){
    uint64_t hKeyGot = log.argsAfterCall["phkResult"].value.imm;
    int insIdx = findRegKey(hKeyGot);
    // 如果找到了这个句柄，且这个句柄正在使用，说明分配到了两个相同的句柄，出现异常。
    if(insIdx > 0 && regeditModel->item(insIdx, REGSTATUS)->text() == "正在使用"){
        handleException({log.id, SameRegHandleGot},
                        new exceptionInfo{.addressWithException = hKeyGot});
    }else if(insIdx > 0){
        // 找到了这个句柄，但这个句柄已经被关闭，则重启这个句柄
        QString directory = getOpenRegKeyName(log) + "\\" + *log.args["lpSubKey"].str;
        regeditModel->item(insIdx, REGSTATUS)->setText("正在使用");
        regeditModel->item(insIdx, REGKEYVAL)->setText(directory);
        regeditModel->item(insIdx)->appendRow(QList<QStandardItem*>() <<
                                              new QStandardItem("OPEN") <<
                                              new QStandardItem("打开此句柄（键值为" + directory + "）") <<
                                              new QStandardItem(ull2a(log.retVal.value.imm)));
        // 判断是否是开机自启动注册表项
        if((getOpenRegKeyName(log) == "HKEY_CURRENT_USER" || getOpenRegKeyName(log) == "HKEY_LOCAL_MACHINE")
                && *log.args["lpSubKey"].str == "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run")
            handleException({log.id, VisitingStartupReg},
                            new exceptionInfo{.fileName = new QString(directory)});
    }else{
        // 没有找到这个句柄，则进行插入
        QString directory = getOpenRegKeyName(log) + "\\" + *log.args["lpSubKey"].str;
        // 判断是否是开机自启动注册表项
        if((getOpenRegKeyName(log) == "HKEY_CURRENT_USER" || getOpenRegKeyName(log) == "HKEY_LOCAL_MACHINE")
                && *log.args["lpSubKey"].str == "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run")
            handleException({log.id, VisitingStartupReg},
                            new exceptionInfo{.fileName = new QString(directory)});
        if(directory == "*")   // 没有找到键值，无法进行插入
            return;
        // 正式插入这个键值
        regeditModel->insertRow(-insIdx - 1, QList<QStandardItem*>() <<
                                new QStandardItem(ull2a(hKeyGot)) <<
                                new QStandardItem(directory) <<
                                new QStandardItem("正在使用"));
    }
}

void hookAnalysis::appendNewRegSet(fullLog log, QStandardItem* father){
    if(log.args["dwType"].value.imm > 11){
        handleException({log.id, InvalidRegTypeValue},
                        new exceptionInfo{.addressWithException = log.args["dwType"].value.imm});
        return;
    }
    // 获取到注册表项的类型
    QString regType = RegTypes[log.args["dwType"].value.imm];

    QString value;
    switch(log.args["dwType"].value.imm){
    case REG_SZ:
    case REG_EXPAND_SZ:
    case REG_MULTI_SZ:{
        value = *log.args["lpData"].str;
        break;
    }
    default:
        value = ull2a(log.args["lpData"].value.imm);
        break;
    }

    QString keyName = *log.args["lpValueName"].str;
    QString detail = "将键" + keyName + "设置为" + regType + "类型，值为" + value;

    father->insertRow(father->rowCount(), QList<QStandardItem*>() <<
                      new QStandardItem("SET") <<
                      new QStandardItem(detail) <<
                      new QStandardItem(ull2a(log.retVal.value.imm)));
}

std::list<int> hookAnalysis::getGenericAccess(unsigned access){
    std::list<int> ret = {};
    if(access & 0x10000000 || access & 0x80000000)
        ret.push_back(1);
    if(access & 0x10000000 || access & 0x40000000)
        ret.push_back(2);
    if(access & 0x10000000 || access & 0x20000000)
        ret.push_back(3);
    return ret;
}

std::list<int> hookAnalysis::getShareMode(unsigned shareMode){
    std::list<int> ret = {};
    if(shareMode & FILE_SHARE_READ)
        ret.push_back(1);
    if(shareMode & FILE_SHARE_WRITE)
        ret.push_back(2);
    if(shareMode & FILE_SHARE_DELETE)
        ret.push_back(3);
    return ret;
}

std::list<int> hookAnalysis::getCreateDisp(unsigned disposition){
    std::list<int> ret = {};
    if(disposition == CREATE_NEW)
        ret.push_back(1);
    if(disposition == CREATE_ALWAYS)
        ret.push_back(2);
    if(disposition == OPEN_EXISTING)
        ret.push_back(3);
    if(disposition == OPEN_ALWAYS)
        ret.push_back(4);
    if(disposition == TRUNCATE_EXISTING)
        ret.push_back(5);
    return ret;
}

std::list<int> hookAnalysis::getFileAttr(unsigned fileAttr){
    std::list<int> ret = {};
    if(fileAttr & FILE_ATTRIBUTE_ARCHIVE)
        ret.push_back(1);
    if(fileAttr & FILE_ATTRIBUTE_COMPRESSED)
        ret.push_back(2);
    if(fileAttr & FILE_ATTRIBUTE_NORMAL)
        ret.push_back(3);
    if(fileAttr & FILE_ATTRIBUTE_HIDDEN)
        ret.push_back(4);
    if(fileAttr & FILE_ATTRIBUTE_READONLY)
        ret.push_back(5);
    if(fileAttr & FILE_ATTRIBUTE_SYSTEM)
        ret.push_back(6);
    if(fileAttr & FILE_FLAG_WRITE_THROUGH)
        ret.push_back(7);
    if(fileAttr & FILE_FLAG_OVERLAPPED)
        ret.push_back(8);
    if(fileAttr & FILE_FLAG_NO_BUFFERING)
        ret.push_back(9);
    if(fileAttr & FILE_FLAG_RANDOM_ACCESS)
        ret.push_back(10);
    if(fileAttr & FILE_FLAG_SEQUENTIAL_SCAN)
        ret.push_back(11);
    if(fileAttr & FILE_FLAG_DELETE_ON_CLOSE)
        ret.push_back(12);
    return ret;
}

void hookAnalysis::handleException(pair<int, APIException> latestException, exceptionInfo* info){
    assert(info != nullptr);

    exceptions.insert(latestException);
    int row = exceptionModel->rowCount();

    QString detail;
    QString exceptionType;

    switch(latestException.second){
    case DoubleFree:{
        detail = "重复释放了";
        detail += ull2a(info->addressWithException);
        detail += "这块内存空间。";
        exceptionType = "DoubleFree";
        break;
    }
    case AllocSameChunk:{
        detail = "重复申请到";
        detail += ull2a(info->addressWithException);
        detail += "这块内存空间。";
        exceptionType = "AllocSameChunk";
        break;
    }
    case InvalidDestroyHandle:{
        detail = "销毁无效的堆：";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "InvalidDestroyHandle";
        break;
    }
    case DestroyFailed:{
        detail = "销毁堆";
        detail += ull2a(info->addressWithException);
        detail += "失败。";
        exceptionType = "DestroyFailed";
        break;
    }
    case DoubleDestroy:{
        detail = "堆句柄";
        detail += ull2a(info->addressWithException);
        detail += "被多次销毁。";
        exceptionType = "DoubleDestroy";
        break;
    }
    case AllocSameHandle:{
        detail = "申请到重复的堆";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "AllocSameHandle";
        break;
    }
    case OverlappingChunk:{
        detail = "内存块";
        detail += ull2a(info->overlapChunkInfo.prevChunk);
        detail += "大小为";
        detail += ull2a(info->overlapChunkInfo.prevSize);
        detail += "，与其后一个内存块";
        detail += ull2a(info->overlapChunkInfo.nextChunk);
        detail += "发生重叠。";
        exceptionType = "OverlappingChunk";
        break;
    }
    case ChunkCrossHandle:{
        detail = "内存块";
        detail += ull2a(info->overlapHandleInfo.chunk);
        detail += "大小为";
        detail += ull2a(info->overlapHandleInfo.chunkSize);
        detail += "，与其后一个堆句柄";
        detail += ull2a(info->overlapHandleInfo.nextHandle);
        detail += "发生重叠。";
        exceptionType = "ChunkCrossHandle";
        break;
    }
    case InvalidFree:{
        detail = "释放无效的内存块空间";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "InvalidFree";
        break;
    }
    case FreeChunkNotFound:{
        detail = "释放未找到的内存块空间";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "FreeChunkNotFound";
        break;
    }
    case AllocToDestroyedHandle:{
        detail = "尝试从已销毁的堆";
        detail += ull2a(info->addressWithException);
        detail += "中分配空间。";
        exceptionType = "AllocToDestroyedHandle";
        break;
    }
    case AllocChunkFailed:{
        detail = "从堆";
        detail += ull2a(info->allocFail.handle);
        detail += "中分配大小";
        detail += ull2a(info->allocFail.requiredSize);
        detail += "的空间失败。";
        exceptionType = "AllocChunkFailed";
        break;
    }
    case AllocHandleFailed:{
        detail = "尝试分配大小为";
        detail += ull2a(info->addressWithException);
        detail += "的堆失败。";
        exceptionType = "AllocHandleFailed";
        break;
    }
    case UntrackedHandleDestroyed:{
        detail = "销毁未被监视的堆";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "UntrackedHandleDestroyed";
        break;
    }
    case FreeChunkFailed:{
        detail = "释放内存块";
        detail += ull2a(info->addressWithException);
        detail += "失败。";
        exceptionType = "FreeChunkFailed";
        break;
    }
    case UntrackedChunkFreed:{
        detail = "释放未被监视的内存块";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "UntrackedChunkFreed";
        break;
    }
    case CreateFileFailed:{
        detail = "打开文件";
        detail += info->fileName->toStdWString().c_str();
        detail += "失败。";
        exceptionType = "CreateFileFailed";
        break;
    }
    case RepeatedFileHandle:{
        detail = "发现重复的文件句柄";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "RepeatedFileHandle";
        break;
    }
    case UntrackedFileHandle:{
        detail = "发现未被监视的文件句柄";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "UntrackedFileHandle";
        break;
    }
    case RegCreateFail:{
        detail = "创建注册表项失败，错误码";
        detail += ull2a(info->errorCode);
        detail += "。";
        exceptionType = "RegCreateFail";
        break;
    }
    case RegOpenFail:{
        detail = "打开注册表项失败，错误码";
        detail += ull2a(info->errorCode);
        detail += "。";
        exceptionType = "RegOpenFail";
        break;
    }
    case RegDeleteFail:{
        detail = "删除注册表项失败，错误码";
        detail += ull2a(info->errorCode);
        detail += "。";
        exceptionType = "RegDeleteFail";
        break;
    }
    case RegCloseFail:{
        detail = "关闭注册表项失败，错误码";
        detail += ull2a(info->errorCode);
        detail += "。";
        exceptionType = "RegCloseFail";
        break;
    }
    case RegSetFail:{
        detail = "设置注册表项值失败，错误码";
        detail += ull2a(info->errorCode);
        detail += "。";
        exceptionType = "RegSetFail";
        break;
    }
    case SameRegHandleGot:{
        detail = "注册表句柄";
        detail += ull2a(info->addressWithException);
        detail += "还未关闭就被再一次成功分配。";
        exceptionType = "SameRegHandleGot";
        break;
    }
    case UntrackedRegHandle:{
        detail = "未被监测的注册表句柄";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "UntrackedRegHandle";
        break;
    }
    case CloseRegKeyTwice:{
        detail = "多次关闭注册表句柄";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "CloseRegKeyTwice";
        break;
    }
    case VisitingStartupReg:{
        detail = "访问系统自启动注册表项";
        detail += *info->fileName;
        detail += "。";
        exceptionType = "VisitingStartupReg";
        break;
    }
    case InvalidRegTypeValue:{
        detail = "无效的注册表值类型：";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "InvalidRegTypeValue";
        break;
    }
    case OpeningSelfExeHandle:{
        detail = "打开自身exe文件句柄。";
        exceptionType = "OpeningSelfExeHandle";
        break;
    }
    case OpeningExeHandle:{
        detail = "打开可执行文件句柄。";
        exceptionType = "OpeningExeHandle";
        break;
    }
    case TryReadExecutableFile:{
        detail = "尝试读取可执行文件";
        detail += *info->fileName;
        detail += "内容。";
        exceptionType = "TryReadExecutableFile";
        break;
    }
    case TryWriteExecutableFile:{
        detail = "尝试写入可执行文件";
        detail += *info->fileName;
        detail += "内容。";
        exceptionType = "TryReadExecutableFile";
        break;
    }
    case TryDeleteStartupRegVal:{
        detail = "尝试删除自启动项注册表中名为";
        detail += *info->fileName;
        detail += "的键。";
        exceptionType = "TryDeleteStartupRegVal";
        break;
    }
    case TrySetStartupRegVal:{
        detail = "尝试设置自启动项注册表中名为";
        detail += *info->fileName;
        detail += "的键。";
        exceptionType = "TrySetStartupRegVal";
        break;
    }
    case TryCloseStdioHandle:{
        detail = "尝试关闭标准输入或输出句柄";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "TryCloseStdioHandle";
        break;
    }
    case CloseFileHandleFailed:{
        detail = "关闭正在使用的文件句柄";
        detail += ull2a(info->addressWithException);
        detail += "失败。";
        exceptionType = "CloseFileHandleFailed";
        break;
    }
    case CloseUntrackedFileHandle:{
        detail = "关闭未被监测的文件句柄";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "CloseUntrackedFileHandle";
        break;
    }
    case InvalidFileHandleToClose:{
        detail = "关闭无效的文件句柄";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "InvalidFileHandleToClose";
        break;
    }
    case DoubleCloseHandle:{
        detail = "多次成功关闭同一个文件句柄";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "DoubleCloseHandle";
        break;
    }
    case TryCloseClosedHandle:{
        detail = "尝试关闭已经被关闭的文件句柄";
        detail += ull2a(info->addressWithException);
        detail += "。";
        exceptionType = "TryCloseClosedHandle";
        break;
    }
    case RelatingMultipleDirs:{
        detail = "文件操作涉及多个文件夹，新发现涉及文件夹：";
        detail += *info->fileName;
        detail += "。";
        exceptionType = "RelatingMultipleDirs";
        break;
    }
    }

    exceptionModel->insertRow(row, QList<QStandardItem*>() <<
                              new QStandardItem(to_string(latestException.first).c_str()) <<
                              new QStandardItem(exceptionType) <<
                              new QStandardItem(detail));
}

bool hookAnalysis::addRegKey(fullLog newRegLog){
    // 首先获得操作的返回值
    long retVal = (long)newRegLog.retVal.value.imm;
    if(retVal != 0){        // 如果返回值不为0，说明操作没有正常完成。
        handleException({newRegLog.id, RegCreateFail},
                        new exceptionInfo{.errorCode = retVal});
        return false;
    }

    // 进行插入，所有的判断都交给下面函数来实现
    insertNewRegHandle(newRegLog);
    return true;
}

bool hookAnalysis::setRegKey(fullLog newRegLog){
    // 首先获得操作的返回值
    long retVal = (long)newRegLog.retVal.value.imm;
    if(retVal != 0){        // 如果返回值不为0，说明操作没有正常完成。
        handleException({newRegLog.id, RegSetFail},
                        new exceptionInfo{.errorCode = retVal});
    }

    // 查找要修改的注册表项
    uint64_t regHandle = newRegLog.args["hKey"].value.imm;
    int keyIdx = findRegKey(regHandle);
    if(keyIdx < 0){ // 如果没有找到这个句柄，无法进行后续分析
        handleException({newRegLog.id, UntrackedRegHandle},
                        new exceptionInfo{.addressWithException = regHandle});
        return false;
    }

    // 如果设置的是自启动项句柄中的键值
    if((getOpenRegKeyName(newRegLog) == "HKEY_CURRENT_USER" || getOpenRegKeyName(newRegLog) == "HKEY_LOCAL_MACHINE")
            && *newRegLog.args["lpSubKey"].str == "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run")
        handleException({newRegLog.id, TrySetStartupRegVal},
                        new exceptionInfo{.fileName = newRegLog.args["lpSubKey"].str});

    // 找到了这个注册表项，插入子项
    appendNewRegSet(newRegLog, regeditModel->item(keyIdx));
    return true;
}

bool hookAnalysis::closeRegKey(fullLog newRegLog){
    // 首先获得操作的返回值
    long retVal = (long)newRegLog.retVal.value.imm;
    if(retVal != 0){        // 如果返回值不为0，说明操作没有正常完成。
        handleException({newRegLog.id, RegCloseFail},
                        new exceptionInfo{.errorCode = retVal});
    }

    // 查找要修改的注册表项
    uint64_t regHandle = newRegLog.args["hKey"].value.imm;
    int keyIdx = findRegKey(regHandle);
    if(keyIdx < 0){ // 如果没有找到这个句柄，无法进行后续分析
        handleException({newRegLog.id, UntrackedRegHandle},
                        new exceptionInfo{.addressWithException = regHandle});
        return false;
    }
    // 如果这个注册表项已经被关闭，则这个注册表项就被成功关闭了两次，产生异常，
    if(regeditModel->item(keyIdx, REGSTATUS)->text() == "已被关闭"){
        handleException({newRegLog.id, CloseRegKeyTwice},
                        new exceptionInfo{.addressWithException = regHandle});
    }
    // 找到这个注册表项，关闭这个注册表项
    regeditModel->item(keyIdx, REGSTATUS)->setText("已被关闭");
    regeditModel->item(keyIdx)->insertRow(regeditModel->item(keyIdx)->rowCount(),
                                          QList<QStandardItem*>() <<
                                          new QStandardItem("CLOSE") <<
                                          new QStandardItem("关闭该句柄（键值为" + regeditModel->item(keyIdx, 1)->text() + "）") <<
                                          new QStandardItem(ull2a(retVal)));
    return true;
}

bool hookAnalysis::openRegKey(fullLog newRegLog){
    // 首先获得操作的返回值
    long retVal = (long)newRegLog.retVal.value.imm;
    if(retVal != 0){        // 如果返回值不为0，说明操作没有正常完成。
        handleException({newRegLog.id, RegOpenFail},
                        new exceptionInfo{.errorCode = retVal});
        return false;
    }

    // 进行插入，所有的判断都交给下面函数来实现
    insertNewRegHandle(newRegLog);
    return true;
}

bool hookAnalysis::deleteRegKey(fullLog newRegLog){
    // 首先获得操作的返回值
    long retVal = (long)newRegLog.retVal.value.imm;
    if(retVal != 0){        // 如果返回值不为0，说明操作没有正常完成。
        handleException({newRegLog.id, RegOpenFail},
                        new exceptionInfo{.errorCode = retVal});
    }

    // 查找要修改的注册表项
    uint64_t regHandle = newRegLog.args["hKey"].value.imm;
    int keyIdx = findRegKey(regHandle);
    if(keyIdx < 0){ // 如果没有找到这个句柄，无法进行后续分析
        handleException({newRegLog.id, UntrackedRegHandle},
                        new exceptionInfo{.addressWithException = regHandle});
        return false;
    }

    // 找到这个注册表项，删除指定键值
    QString keyVal = *newRegLog.args["lpValueName"].str;

    // 如果删除的是自启动项句柄中的键值
    if((getOpenRegKeyName(newRegLog) == "HKEY_CURRENT_USER" || getOpenRegKeyName(newRegLog) == "HKEY_LOCAL_MACHINE")
            && *newRegLog.args["lpSubKey"].str == "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run")
        handleException({newRegLog.id, TryDeleteStartupRegVal},
                        new exceptionInfo{.fileName = newRegLog.args["lpSubKey"].str});

    regeditModel->item(keyIdx)->insertRow(regeditModel->item(keyIdx)->rowCount(),
                                          QList<QStandardItem*>() <<
                                          new QStandardItem("DELETE") <<
                                          new QStandardItem("删除该句柄下的\"" + keyVal + "\"键值") <<
                                          new QStandardItem(ull2a(retVal)));
    return true;
}

QString hookAnalysis::getOpenRegKeyName(fullLog newRegLog, int findIdx){
    uint64_t hKey = newRegLog.args["hKey"].value.imm;
    if(findIdx == -1)
        findIdx = findRegKey(hKey);
    // 如果没有找到句柄，说明这个句柄没有被监视，无法进行后续分析
    if(findIdx < 0){
        handleException({newRegLog.id, UntrackedRegHandle},
                        new exceptionInfo{.addressWithException = hKey});
        return "*";
    }
    // 如果找到句柄，就能够找到键值
    return regeditModel->item(findIdx, REGKEYVAL)->text();
}