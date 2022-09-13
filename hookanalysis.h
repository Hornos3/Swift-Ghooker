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
    CreateFileFailed, RepeatedFileHandle, UntrackedFileHandle
}APIException;

class hookAnalysis
{
public:
    hookAnalysis(QStandardItemModel* tableModel, QStandardItemModel *fileViewModel);

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

    bool exeInfoGot = false;
    bool analyseHeap = true;
    bool analyseFile = true;

    QStandardItemModel* heapViewModel;
    QStandardItemModel* fileViewModel;


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

    int findHandleInspos(uint64_t newHandleAddr);
    bool insertNewHandle(fullLog newHandleLog, int insPos);
    bool disableHandle(uint64_t destroyAddress);
    inline static size_t calculateHandleSize(uint64_t startAddr);
    int getAffiliatedHandle(uint64_t chunkAddr);
    pair<int, int> findChunk(uint64_t chunkAddr);

    bool addHandle(fullLog newHeapLog);
    bool destroyHandle(fullLog newHeapLog);
    bool addChunk(fullLog newHeapLog);
    bool freeChunk(fullLog newHeapLog);

    int findHandle(uint64_t handleAddr, QStandardItemModel *Model);
    int findChunk(uint64_t chunkAddr, QStandardItem* father);

    bool addFileHandle(fullLog newFileLog);
    bool addReadWriteFileRecord(fullLog newFileLog, bool isRead);
    bool addWriteFileRecord(fullLog newFileLog);

    void insertNewHeapHandle(uint64_t handleAddr, int insPos);
    void insertNewHeapChild(uint64_t chunkAddr, uint64_t size, QStandardItem* father, int insPos);
    void insertNewFileHandle(fullLog log, int insPos);

    static std::list<int> getGenericAccess(unsigned access);
    static std::list<int> getShareMode(unsigned shareMode);
    static std::list<int> getCreateDisp(unsigned disposition);
    static std::list<int> getFileAttr(unsigned fileAttr);

    void setRow(int row, QStringList elements);
    void insertUnknownSizeChunk(uint64_t chunkAddr, QStandardItem* father, int insPos);
    void setChildRow(QStandardItem* parent, int row, QStringList elements);
};

#endif // HOOKANALYSIS_H
