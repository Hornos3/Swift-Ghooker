#include "hookanalysis.h"
#include "qdebug.h"

#define ADDRESS 0
#define TYPE 1
#define SIZE 2
#define HEAPSTATUS 3

#define FILENAME 1
#define FILEHANDLESTATUS 2

hookAnalysis::hookAnalysis(QStandardItemModel* tableModel, QStandardItemModel* fileViewModel){
    this->heapViewModel = tableModel;
    this->fileViewModel = fileViewModel;
}

/**
 * @brief hookAnalysis::appendRecord 记录分析主函数
 * @param newRecord 新一条记录
 * @return 处理是否产生异常
 */
bool hookAnalysis::appendRecord(QString newRecord){
    fullLog latestLog;
    size_t idxptr;

    // 获取该条记录的编号（此编号唯一确定一条记录，避免重复记录）
    idxptr = newRecord.indexOf("ID: ");
    idxptr += 4;
    unsigned id = newRecord.mid(idxptr, newRecord.indexOf("\n") - idxptr).toUInt();
    latestLog.id = id;

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
        thisArg.type = getType(argType);

        // 获取参数名
        QString argName = elements[1];

        // 获取参数值
        QString val = elements[3];
        if((int)thisArg.type <= 8)
            thisArg.value.imm = val.toLongLong();
        else
            thisArg.value.imm = val.mid(2).toLongLong(nullptr, 16);

        // 当类型是字符串时，获取字符串的内容
        QString str;
        if(thisArg.type == charptr || thisArg.type == wcharptr){
            assert(elements.size() >= 6);
            str = line.split("\"").at(1);
        }
        if(!str.isEmpty())
            thisArg.str = new QString(str);

        // 保存参数
        latestLog.args.insert({argName, thisArg});
    }

    // 获取exe文件信息——进程名
    idxeptr = newRecord.indexOf("\n", idxptr);
    QString line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    if(!exeInfoGot)
        exeInfo.processName = line.mid(22);

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
    realArg retVal;
    idxeptr = newRecord.indexOf("\n", idxptr);
    line = newRecord.mid(idxptr, idxeptr - idxptr);
    idxptr = idxeptr + 1;
    QStringList elements = line.split(" ");
    argType returnType = getType(elements[2].mid(1, elements[2].length() - 2));
    retVal.type = returnType;

    // 保存返回值的整数值
    QString val = elements[3];
    if((int)retVal.type <= 8)
        retVal.value.imm = val.toLongLong();
    else
        retVal.value.imm = val.mid(2).toLongLong(nullptr, 16);

    // 保存返回值的字符串内容
    QString str;
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
        // 获取需要获取执行后结果的参数值
        while(line.startsWith("\t")){
            realArg thisArg;

            idxeptr = newRecord.indexOf("\n", idxptr);
            QString line = newRecord.mid(idxptr + 1, idxeptr - idxptr);
            idxptr = idxeptr + 1;
            QStringList elements = line.split(" ");

            // 获取参数类型
            QString argType = elements[0];
            thisArg.type = getType(argType);

            // 获取参数名
            QString argName = elements[1];

            // 获取参数值
            QString val = elements[3];
            thisArg.value.imm = val.toLongLong();

            // 保存参数
            latestLog.argsAfterCall.insert({argName, thisArg});
        }
    }

    // 与前一个记录进行比较，对于HeapAlloc/HeapCreate这一类函数，两次连续分配不可能获得同一个地址，据此判断是否有重复的现象出现。
    if(logList.size() != 0){
        fullLog lastLog = *(logList.rbegin());
        if(latestLog == lastLog)   // 发现重复
            return true;
    }

    // 如果这个函数与堆操作有关系，则进入堆分析模块进行进一步行为分析。
    // 如果这个函数与文件操作有关系，则进入文件分析模块进行进一步行为分析。
    logList.insert(latestLog);
    if(latestLog.funcName.contains("Heap") && analyseHeap)
        analyseNewHeapInst(latestLog);
    else if(latestLog.funcName.contains("File") && analyseFile)
        analyseNewFileInst(latestLog);

    return false;
}

argType hookAnalysis::getType(QString input){
    argType type = others;
    if(input == "HANDLE" || input == "LPVOID")
        type = voidptr;
    else if(input == "DWORD" || input == "UINT")
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
    }else{
        exit(1);
    }
}

/**
 * @brief hookAnalysis::findHandleInspos 查找新的handle地址应该插入到树图的哪一个索引位置
 * @param newHandleAddr 新handle地址
 * @return 应该插入的位置，在插入前需要将该位置及其后所有item后移一位。如果handle已经存在且处于释放状态，则返回-1*位置，如果处于正在使用状态，则返回0xdeadbeef（错误）
 */
int hookAnalysis::findHandleInspos(uint64_t newHandleAddr){
    int rowCount = heapViewModel->rowCount();
    for(int i=0; i<rowCount; i++){
        auto head = heapViewModel->item(i);
        uint64_t address = head->text().toULongLong(nullptr, 16);
        if(address > newHandleAddr)
            return i;
        else if(address == newHandleAddr && heapViewModel->item(i, 3)->text() == "正在使用")
            return 0xdeadbeef;
        else if(address == newHandleAddr)
            return -1 * i;
    }
    return rowCount;
}

bool hookAnalysis::insertNewHandle(fullLog newHandleLog, int insPos){
    int rowCount = heapViewModel->rowCount();
    if(insPos > rowCount || insPos < 0)
        return false;
//    newItem->appendColumn(QList<QStandardItem*>() << size << status);

    if(insPos == rowCount){
        heapViewModel->insertRow(rowCount);
        setRow(rowCount, QStringList() << ull2a(newHandleLog.retVal.value.imm) <<
               "HANDLE" << ull2a(calculateHandleSize(newHandleLog.retVal.value.imm)) << "正在使用");
    }else{
        heapViewModel->insertRow(insPos);
        setRow(insPos, QStringList() << ull2a(newHandleLog.retVal.value.imm) <<
               "HANDLE" << ull2a(calculateHandleSize(newHandleLog.retVal.value.imm)) << "正在使用");
    }
    return true;
}

bool hookAnalysis::disableHandle(uint64_t destroyAddress){
    int rowCount = heapViewModel->rowCount();
    for(int i=0; i<rowCount; i++){
        auto head = heapViewModel->item(i);
        if(head->text().toULongLong(nullptr, 16) == destroyAddress){
            heapViewModel->item(i, 3)->setText("已被销毁");
            if(head->hasChildren()){
                int j = 0;
                QStandardItem* child = head->child(j);
                while(child != nullptr){
                    head->child(j, 3)->setText("无效");
                    child = head->child(++j);
                }
            }
            break;
        }
    }
    return true;
}

inline size_t hookAnalysis::calculateHandleSize(uint64_t startAddr){
    return ((startAddr & (-1 * PAGE_SIZE)) + PAGE_SIZE) - startAddr;
}

int hookAnalysis::getAffiliatedHandle(uint64_t chunkAddr){
    int rowCount = heapViewModel->rowCount();
    for(int i=0; i<rowCount; i++){
        uint64_t handleAddress = heapViewModel->item(i)->text().toULongLong(nullptr, 16);
        uint64_t handleSize = heapViewModel->item(i, 2)->text().toULongLong(nullptr, 16);
        if(handleAddress <= chunkAddr && handleAddress + handleSize > chunkAddr && heapViewModel->item(i, 1)->text() == "HANDLE")
            return i;
        else if(handleAddress > chunkAddr)
            return -i;
    }
    return -rowCount;
}

/**
 * @brief hookAnalysis::addHandle 添加一个新HANDLE
 * @param newHeapLog 最新一条截获日志
 * @return 是否插入成功
 */
bool hookAnalysis::addHandle(fullLog newHeapLog){
    uint64_t allocatedHandle = newHeapLog.retVal.value.imm; // 获取该函数的返回值，即分配到的堆起始地址
    if(allocatedHandle == 0){       // 如果返回值为0，说明分配失败，不需要进行后续操作
        exceptions.insert({newHeapLog.id, AllocHandleFailed});
        return false;
    }

    int findHandleIdx = findHandle(allocatedHandle, heapViewModel);                 // 查找这个HANDLE是否已经被监视
    if(findHandleIdx < 0){      // 没有找到这个HANDLE，在表中新增
        insertNewHeapHandle(allocatedHandle, -findHandleIdx - 1);
    }else{
        // 首先判断这个HANDLE目前是否正在使用，如果之前监测到明确的HeapDestroy函数将其销毁，那么再一次分配到这个HANDLE就是异常的行为。
        if(heapViewModel->item(findHandleIdx, HEAPSTATUS)->text() == "已被销毁")
            exceptions.insert({newHeapLog.id, AllocToDestroyedHandle});
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
    bool destroyFinished = newHeapLog.retVal.value.imm;     // 获取该函数的返回值，即是否销毁成功
    uint64_t handleToDestroy = newHeapLog.args["hHeap"].value.imm;
    int findHandleIdx = findHandle(handleToDestroy, heapViewModel);
    if(!destroyFinished){
        // 如果返回值为false，说明销毁失败，查找销毁失败原因，如果要销毁的HANDLE在监视范围，则产生未知错误，如果不在则认定为无效参数
        if(findHandleIdx < 0)       // 在监视列表中没有找到这个HANDLE，视为无效参数
            exceptions.insert({newHeapLog.id, InvalidDestroyHandle});
        else
            exceptions.insert({newHeapLog.id, DestroyFailed});
        return false;
    }

    // 如果返回值为true，而且要销毁的HANDLE不在监视范围内，则说明该HANDLE在创建时没有被跟踪到，不添加该HANDLE，且新增异常信息
    if(findHandleIdx < 0){
        exceptions.insert({newHeapLog.id, UntrackedHandleDestroyed});
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
        exceptions.insert({newHeapLog.id, AllocChunkFailed});
        return false;
    }

    uint64_t handleAddr = newHeapLog.args["hHeap"].value.imm;       // 获取该CHUNK所属的handle地址
    int findHandleIdx = findHandle(handleAddr, heapViewModel);                 // 查找这个HANDLE是否已经被监视
    if(findHandleIdx < 0)      // 没有找到这个HANDLE，在表中新增
        insertNewHeapHandle(handleAddr, -findHandleIdx - 1);
    else{
        // 首先判断这个HANDLE目前是否正在使用，如果之前监测到明确的HeapDestroy函数将其销毁，那么再一次分配到这个HANDLE就是异常的行为。
        // 但即使存在这种异常行为，程序依然会记录这个CHUNK，除非其返回值为nullptr。
        if(heapViewModel->item(findHandleIdx, HEAPSTATUS)->text() == "已被销毁")
            exceptions.insert({newHeapLog.id, AllocToDestroyedHandle});
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
            if(father->child(findChunkIdx, HEAPSTATUS)->text() == "正在使用")
                exceptions.insert({newHeapLog.id, AllocSameChunk});
            father->child(findChunkIdx, SIZE)->setText(ull2a(chunkSize));       // 修改size
            father->child(findChunkIdx, HEAPSTATUS)->setText("正在使用");            // 修改状态
        }
    }
    return true;
}

bool hookAnalysis::freeChunk(fullLog newHeapLog){

    bool success = newHeapLog.retVal.value.imm;     // 本次释放是否成功
    if(!success){       // 不成功，返回异常信息
        exceptions.insert({newHeapLog.id, FreeChunkFailed});
        return false;
    }

    uint64_t victim = newHeapLog.args["lpMem"].value.imm;
    uint64_t handleAddr = newHeapLog.args["hHeap"].value.imm;

    // 首先根据HANDLE地址查找HANDLE，如果没有找到，说明这个CHUNK也没有被追踪，那么其大小就未知。
    int findHandleIdx = findHandle(handleAddr, heapViewModel);                 // 查找这个HANDLE是否已经被监视
    if(findHandleIdx < 0){      // 没有找到这个HANDLE，在表中新增，同时增加这个CHUNK，新增异常
        insertNewHeapHandle(handleAddr, -findHandleIdx - 1);
        insertUnknownSizeChunk(victim, heapViewModel->item(-findHandleIdx - 1), 0);
        exceptions.insert({newHeapLog.id, UntrackedChunkFreed});
        return true;
    }

    // 如果HANDLE能够被找到，再查询这个CHUNK是否被监视，如果没有则新增。
    QStandardItem* father = heapViewModel->item(findHandleIdx);
    int findChunkIdx = findChunk(victim, father);
    if(findChunkIdx < 0){       // 没有找到这个CHUNK
        insertUnknownSizeChunk(victim, father, -findChunkIdx - 1);
        exceptions.insert({newHeapLog.id, UntrackedChunkFreed});
    }else{  // 找到这个CHUNK
        // 判断这个CHUNK是否正在被使用，如果已经释放，说明发现了Double Free
        if(father->child(findChunkIdx, HEAPSTATUS)->text() == "已被释放")
            exceptions.insert({newHeapLog.id, DoubleFree});
        else
            father->child(findChunkIdx, HEAPSTATUS)->setText("已被释放");
    }

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

pair<int, int> hookAnalysis::findChunk(uint64_t chunkAddr){
    int rowCount = heapViewModel->rowCount();
    int findRow = 0;
    for(; findRow<rowCount; findRow++){
        if(heapViewModel->item(findRow)->text().toULongLong(nullptr, 16) >= chunkAddr)
            break;
    }
    if(findRow == rowCount){
        return {-1, -1};
    }else if(heapViewModel->item(findRow)->text().toULongLong(nullptr, 16) == chunkAddr){
        if(heapViewModel->item(findRow, 1)->text() == "CHUNK")
            return {findRow, -1};
        else if(heapViewModel->item(findRow)->child(0) == nullptr ||
                heapViewModel->item(findRow)->child(0)->text().toULongLong(nullptr, 16) != chunkAddr)
            return {-1, -1};
        else
            return {findRow, 0};
    }else if(findRow == 0 && heapViewModel->item(findRow)->text().toULongLong(nullptr, 16) > chunkAddr){
        return {-1, -1};
    }else{
        findRow--;
        int findChild = 0;
        QStandardItem* parent = heapViewModel->item(findRow);
        QStandardItem* child = parent->child(findChild);
        while(child != nullptr){
            if(child->text().toULongLong(nullptr, 16) == chunkAddr)
                return {findRow, findChild};
        }
        return {-1, -1};
    }
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
    int rowPtr = 0;
    for(; rowPtr < rowCount; rowPtr++){
        uint64_t findHandle = Model->item(rowPtr)->text().toULongLong(nullptr, 16);
        if(findHandle == handleAddr)
            return rowPtr;
        else if(findHandle > handleAddr)
            return -rowPtr - 1;
    }
    return -rowCount - 1;
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
    int rowPtr = 0;
    for(; rowPtr < rowCount; rowPtr++){
        uint64_t findChunk = father->child(rowPtr, 0)->text().toULongLong(nullptr, 16);
        if(findChunk == chunkAddr)
            return rowPtr;
        else if(findChunk > chunkAddr)
            return -rowPtr - 1;
    }
    return -rowCount - 1;
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
    // 如果句柄无效
    if(returnHandle == (uint64_t)INVALID_HANDLE_VALUE){
        exceptions.insert({newFileLog.id, CreateFileFailed});
        return false;
    }

    // 如果句柄有效，首先查找表中是否已经存在这个句柄
    int findHandleIdx = findHandle(returnHandle, fileViewModel);
    if(findHandleIdx >= 0){      // 如果查找到了这个HANDLE，输出异常信息
        exceptions.insert({newFileLog.id, RepeatedFileHandle});
        return false;
    }

    // 如果没有这个句柄，则插入这个句柄，展示相关信息
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
        exceptions.insert({newFileLog.id, UntrackedFileHandle});
        return false;
    }

    // 要读取的字节数
    uint64_t requiredBytes = newFileLog.args["nNumberOfBytesToRead"].value.imm;
    // 实际读取的字节数
    uint64_t actualBytes = newFileLog.argsAfterCall["lpNumberOfBytesRead"].value.imm;

    auto father = fileViewModel->item(handle);
    if(isRead)
        father->insertRow(father->rowCount(), QList<QStandardItem*>() <<
                          new QStandardItem("READ") <<
                          new QStandardItem(ull2a(requiredBytes)) <<
                          new QStandardItem(ull2a(actualBytes)) <<
                          new QStandardItem(success ? "SUCCESS" : "FAILED"));
    else
        father->insertRow(father->rowCount(), QList<QStandardItem*>() <<
                          new QStandardItem("WRITE") <<
                          new QStandardItem(ull2a(requiredBytes)) <<
                          new QStandardItem(ull2a(actualBytes)) <<
                          new QStandardItem(success ? "SUCCESS" : "FAILED"));
    return true;
}

void hookAnalysis::insertNewFileHandle(fullLog log, int insPos){
    assert(log.args["lpFileName"].str != nullptr);
    QString fileName = *log.args["lpFileName"].str;
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

std::list<int> hookAnalysis::getGenericAccess(unsigned access){
    std::list<int> ret = {};
    if(access & 0x10000000 || access & 0x20000000)
        ret.push_back(1);
    if(access & 0x10000000 || access & 0x40000000)
        ret.push_back(2);
    if(access & 0x10000000 || access & 0x80000000)
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
