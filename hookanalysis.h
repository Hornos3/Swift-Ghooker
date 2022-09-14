#ifndef HOOKANALYSIS_H
#define HOOKANALYSIS_H

#include <map>
#include <set>
#include <vector>
#include <string>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QStandardItemModel>
#include "Util.h"

//using namespace std;

const QString RegTypes[12] = {"REG_NONE", "REG_SZ", "REG_EXPAND_SZ", "REG_BINARY", "REG_DWORD", "REG_DWORD_BIG_ENDIAN",
                        "REG_LINK", "REG_MULTI_SZ", "REG_RESOURCE_LIST", "REG_FULL_RESOURCE_DESCRIPTOR",
                        "REG_RESOURCE_REQUIREMENTS_LIST", "REG_QWORD"};

typedef void* heapHandle;
typedef struct chunk{
    heapHandle affiliatedHandle = nullptr;
    void* chunkAddr;
    friend bool operator<(const chunk& self, const chunk& others){return self.chunkAddr < others.chunkAddr;}
}chunk;

typedef enum argType{
    int8, int16, int32, int64, uint8, uint16, uint32, uint64,
    Bool, charptr, wcharptr, voidptr, others
}argType;

typedef struct fileHandleAttr{
    uint64_t handleAddr;
    QString fileName;
    unsigned access;
    unsigned shareMode;
    unsigned createDisp;
    unsigned flagAttr;
    bool isUsing;
    friend bool operator<(const fileHandleAttr& self, const fileHandleAttr& other){
        return self.handleAddr < other.handleAddr;
    }
    friend bool operator==(const fileHandleAttr& self, const fileHandleAttr& other){
        return self.handleAddr == other.handleAddr;
    }
}fileHandleAttr;

typedef struct realArg{
    argType type;
    union{
        uint64_t imm;
        void* ptr;
    }value;
    QString* str = nullptr;
    friend bool operator==(const realArg& self, const realArg& other){
        if(!(self.type == other.type && self.value.imm == other.value.imm))
            return false;
        if(self.str == nullptr && other.str == nullptr)
            return true;
        if((self.str == nullptr) ^ (other.str == nullptr))
            return false;
        return *(self.str) == *(other.str);
    }
}realArg;

typedef struct fullLog{
    unsigned id;
    QString time;
    QString funcName;
    std::map<QString, realArg> args;
    std::map<QString, realArg> argsAfterCall;
    realArg retVal;
    friend bool operator<(const fullLog& self, const fullLog& other){return self.time < other.time;}
    friend bool operator==(const fullLog& self, const fullLog& other){return self.id == other.id;}
}fullLog;

typedef struct exeInfo{
    uint64_t STDIN;
    uint64_t STDOUT;
    uint64_t STDERR;
    QString processName;
    QString fileDescription;
    QString fileVersion;
    QString internalName;
    QString companyName;
    QString legalCopyright;
    QString originalFileName;
    QString productVersion;
    QString to_string(){
        QString ret;
        ret += "stdin handle: ";
        ret += ull2a(STDIN);
        ret += "\nstdout handle: ";
        ret += ull2a(STDOUT);
        ret += "\nstderr handle: ";
        ret += ull2a(STDERR);
        ret += "\nProcess Name: ";
        ret += processName.isEmpty() ? "<EMPTY>\n" : (processName + "\n");
        ret += "File Description: ";
        ret += fileDescription.isEmpty() ? "<EMPTY>\n" : (fileDescription + "\n");
        ret += "File Version: ";
        ret += fileVersion.isEmpty() ? "<EMPTY>\n" : (fileVersion + "\n");
        ret += "Internal Name: ";
        ret += internalName.isEmpty() ? "<EMPTY>\n" : (internalName + "\n");
        ret += "Company Name: ";
        ret += companyName.isEmpty() ? "<EMPTY>\n" : (companyName + "\n");
        ret += "Legal Copyright: ";
        ret += legalCopyright.isEmpty() ? "<EMPTY>\n" : (legalCopyright + "\n");
        ret += "Original File Name: ";
        ret += originalFileName.isEmpty() ? "<EMPTY>\n" : (originalFileName + "\n");
        ret += "Production Version: ";
        ret += productVersion.isEmpty() ? "<EMPTY>\n" : (productVersion + "\n");
        ret += "*****************************************************\n";
        return ret;
    }
}exeInfo;

typedef enum APIException{
    DoubleFree, AllocSameChunk, InvalidDestroyHandle, DestroyFailed, AllocSameHandle,
    OverlappingChunk, ChunkCrossHandle, InvalidFree, FreeChunkNotFound,
    AllocToDestroyedHandle, AllocChunkFailed, AllocHandleFailed, UntrackedHandleDestroyed,
    FreeChunkFailed, UntrackedChunkFreed,
    CreateFileFailed, RepeatedFileHandle, UntrackedFileHandle,
    RegCreateFail, RegOpenFail, RegDeleteFail, RegSetFail, RegCloseFail,
    SameRegHandleGot, UntrackedRegHandle, InvalidRegTypeValue,
    CloseRegKeyTwice,
}APIException;

typedef union exceptionInfo{
    // 存在异常的地址，用于DoubleFree、AllocSameChunk、InvalidDestroyHandle、DestroyFailed、AllocSameHandle、
    // InvalidFree、FreeChunkNotFound、UntrackedHandleDestroyed、RepeatedFileHandle、UntrackedFileHandle、
    // AllocHandleFailed、AllocToDestroyedHandle、SameRegHandleGot、UntrackedRegHandle、InvalidRegTypeValue、
    // CloseRegKeyTwice
    uint64_t addressWithException;
    // 堆块重叠，用于OverlappingChunk
    struct overlapChunkInfo{
        uint64_t prevChunk;
        uint64_t nextChunk;
        uint64_t prevSize;
    }overlapChunkInfo;
    // 堆块跨句柄，用于ChunkCrossHandle
    struct overlapHandleInfo{
        uint64_t chunk;
        uint64_t nextHandle;
        uint64_t chunkSize;
    }overlapHandleInfo;
    // 分配堆块失败，用于AllocChunkFailed
    struct allocFail{
        uint64_t handle;
        uint64_t requiredSize;
    }allocFail;
    // 用于FreeChunkFailed、UntrackedChunkFreed
    struct chunkAndHandle{
        uint64_t chunk;
        uint64_t handle;
    }chunkAndHandle;
    // 文件名，用于CreateFileFailed
    QString* fileName;
    // 错误码，用于5个注册表函数错误
    long errorCode;
}exceptionInfo;

class hookAnalysis
{
public:
    hookAnalysis(QStandardItemModel* tableModel, QStandardItemModel* fileViewModel,
                 QStandardItemModel* exceptionModel, QStandardItemModel* regeditModel);

    bool injMessageBoxA = false;
    bool injMessageBoxW = false;
    bool injHeapCreate = false;
    bool injHeapDestroy = false;
    bool injHeapAlloc = false;
    bool injHeapFree = false;
    bool injOpenFile = false;
    bool injCreateFile = false;
    bool injReadFile = false;
    bool injWriteFile = false;
    bool injRegCreateKeyEx = false;
    bool injRegSetValueEx = false;
    bool injRegDeleteValue = false;
    bool injRegCloseKey = false;
    bool injRegOpenKeyEx = false;

    bool exeInfoGot = false;
    bool analyseHeap = true;
    bool analyseFile = true;
    bool analyseReg = true;

    QStandardItemModel* heapViewModel;
    QStandardItemModel* fileViewModel;
    QStandardItemModel* exceptionModel;
    QStandardItemModel* regeditModel;

    std::set<fullLog> logList;
    std::set<heapHandle>* heapHandles = nullptr;
    std::set<chunk>* chunks = nullptr;
    std::map<int, APIException> exceptions;
    std::map<int, uint64_t> memoryLeakRisks;    // 当有CHUNK没有被释放而HANDLE首先被销毁时，将CHUNK地址保存到此处
    std::map<uint64_t, fileHandleAttr> fileHandles;

    exeInfo exeInfo;

    argType getType(QString input);
    bool appendRecord(QString newRecord);

    void analyseNewHeapInst(fullLog newHeapLog);
    void analyseNewFileInst(fullLog newFileLog);
    void analyseNewRegInst(fullLog newRegLog);

    bool addHandle(fullLog newHeapLog);
    bool destroyHandle(fullLog newHeapLog);
    bool addChunk(fullLog newHeapLog);
    bool freeChunk(fullLog newHeapLog);

    int findHandle(uint64_t handleAddr, QStandardItemModel *Model);
    int findChunk(uint64_t chunkAddr, QStandardItem* father);
    int findRegKey(uint64_t handleAddr);

    bool addFileHandle(fullLog newFileLog);
    bool addReadWriteFileRecord(fullLog newFileLog, bool isRead);

    void insertNewHeapHandle(uint64_t handleAddr, int insPos);
    void insertNewHeapChild(uint64_t chunkAddr, uint64_t size, QStandardItem* father, int insPos);
    void insertNewFileHandle(fullLog log, int insPos);
    void insertNewRegHandle(fullLog log);
    void appendNewRegSet(fullLog log, QStandardItem* father);

    static std::list<int> getGenericAccess(unsigned access);
    static std::list<int> getShareMode(unsigned shareMode);
    static std::list<int> getCreateDisp(unsigned disposition);
    static std::list<int> getFileAttr(unsigned fileAttr);

    void setRow(int row, QStringList elements);
    void insertUnknownSizeChunk(uint64_t chunkAddr, QStandardItem* father, int insPos);
    void setChildRow(QStandardItem* parent, int row, QStringList elements);

    void handleException(pair<int, APIException> latestException, exceptionInfo* info);

    bool addRegKey(fullLog newRegLog);
    bool setRegKey(fullLog newRegLog);
    bool closeRegKey(fullLog newRegLog);
    QString getOpenRegKeyName(fullLog newRegLog, int findIdx = -1);
};

#endif // HOOKANALYSIS_H
