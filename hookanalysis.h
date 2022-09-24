#ifndef HOOKANALYSIS_H
#define HOOKANALYSIS_H

#include <map>
#include <set>
#include <vector>
#include <string>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <fstream>
#include <QStandardItemModel>
#include "Util.h"

//using namespace std;

const QString RegTypes[] = {"REG_NONE", "REG_SZ", "REG_EXPAND_SZ", "REG_BINARY", "REG_DWORD", "REG_DWORD_BIG_ENDIAN",
                        "REG_LINK", "REG_MULTI_SZ", "REG_RESOURCE_LIST", "REG_FULL_RESOURCE_DESCRIPTOR",
                        "REG_RESOURCE_REQUIREMENTS_LIST", "REG_QWORD"};
const QString af[] = {"AF_UNSPEC", "AF_UNIX", "AF_INET", "AF_IMPLINK", "AF_PUP", "AF_CHAOS", "AF_NS",
                     "AF_ISO", "AF_ECMA", "AF_DATAKIT", "AF_CCITT", "AF_SNA", "AF_DECnet", "AF_DLI", "AF_LAT",
                     "AF_HYLINK", "AF_APPLETALK", "AF_NETBIOS", "AF_VOICEVIEW", "AF_FIREFOX", "AF_UNKNOWN1",
                     "AF_BAN", "AF_ATM", "AF_INET6", "AF_CLUSTER", "AF_12844", "AF_IRDA", "<INVALID>", "AF_NETDES"};
const QString sockType[] = {"NONE", "SOCK_STREAM", "SOCK_DGRAM", "SOCK_RAW", "SOCK_RDM", "SOCK_SEQPACKET"};
const map<int, const QString> ipproto = {{0, "IPPROTO_HOPOPTS"}, {1, "IPPROTO_ICMP"}, {2, "IPPROTO_IGMP"},
                                   {3, "IPPROTO_GGP"}, {4, "IPPROTO_IPV4"}, {5, "IPPROTO_ST"},
                                   {6, "IPPROTO_TCP"}, {7, "IPPROTO_CBT"}, {8, "IPPROTO_EGP"},
                                   {9, "IPPROTO_IGP"}, {12, "IPPROTO_PUP"}, {17, "IPPROTO_UDP"},
                                   {22, "IPPROTO_IDP"}, {27, "IPPROTO_RDP"}, {41, "IPPROTO_IPV6"},
                                   {43, "IPPROTO_ROUTING"}, {44, "IPPROTO_FRAGMENT"}, {50, "IPPROTO_ESP"},
                                   {51, "IPPROTO_AH"}, {58, "IPPROTO_ICMPV6"}, {59, "IPPROTO_NONE"},
                                   {60, "IPPROTO_DSTOPTS"}, {77, "IPPROTO_ND"}, {78, "IPPROTO_ICLFXBM"},
                                   {103, "IPPROTO_PIM"}, {113, "IPPROTO_PGM"}, {115, "IPPROTO_L2TP"},
                                   {132, "IPPROTO_SCTP"}, {255, "IPPROTO_RAW"}, {256, "IPPROTO_MAX"},
                                   {257, "IPPROTO_RESERVED_RAW"}, {258, "IPPROTO_RESERVED_IPSEC"},
                                   {259, "IPPROTO_RESERVED_IPSECOFFLOAD"}, {260, "IPPROTO_RESERVED_WNV"},
                                   {261, "IPPROTO_RESERVED_MAX"}};

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
    QString originalTypeName;
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
    QList<QString> callChain;
    std::map<QString, realArg> args;
    std::map<QString, realArg> argsAfterCall;
    realArg retVal;
    friend bool operator<(const fullLog& self, const fullLog& other){return self.id < other.id;}
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
        return ret;
    }
}exeInfo;

extern wchar_t currentDirectory[0x1000];

/**
 * 这里列出了钩子可能截获到的所有异常情况，分别为：
 * （!为必然崩溃，?为可能崩溃，*为任务书中要求内容，-为未实现内容）
 * DoubleFree                 !*(0): 堆块被二次释放
 * AllocSameChunk               (1): 堆块被二次分配（指堆块还未被释放即再一次被重新分配）
 * InvalidDestroyHandle       ? (2): 删除的堆指针无效
 * DestroyFailed              ? (3): 删除堆失败
 * DoubleDestroy              ! (4): 堆被二次删除
 * AllocSameHandle              (5): 堆被二次分配（指堆还未被删除即再一次被重新分配）
 * OverlappingChunk            -(6): 地址相邻的两个堆块有重叠
 * ChunkCrossHandle            -(7): 属于一个堆的堆块与另外一个堆有重叠
 * InvalidFree                  (8): 释放的堆块指针无效（指监测界面没有找到堆块且释放失败）
 * FreeChunkNotFound            (9): 找不到释放的堆块指针（指监测界面没有找到堆块但释放成功）
 * AllocToDestroyedHandle     ! (10): 尝试从已删除的堆中分配堆块
 * AllocChunkFailed             (11): 分配堆块失败
 * AllocHandleFailed            (12): 分配堆失败
 * UntrackedHandleDestroyed     (13): 删除了没有被监测的堆且删除成功
 * FreeChunkFailed              (14): 释放堆块失败
 * UntrackedChunkFreed          (15): 释放了没有被监测的堆块且释放成功
 * CreateFileFailed             (16): 创建文件句柄失败
 * RepeatedFileHandle           (17): 重复的文件句柄（指文件句柄没有关闭就再一次被创建）
 * UntrackedFileHandle          (18): 没有监测的文件句柄（指对没有监测的文件句柄做操作）
 * RegCreateFail                (19): 创建注册表句柄失败
 * RegOpenFail                  (20): 打开注册表项句柄失败
 * RegDeleteFail                (21): 删除注册表项失败
 * RegSetFail                   (22): 设置注册表项失败
 * RegCloseFail                 (23): 关闭注册表项句柄失败
 * SameRegHandleGot             (24): 分配到相同的注册表句柄
 * UntrackedRegHandle           (25): 没有监测的注册表句柄
 * InvalidRegTypeValue          (26): 无效的注册表值类型
 * CloseRegKeyTwice             (27): 两次关闭注册表项句柄
 * VisitingStartupReg          *(28): 访问开机自启动项注册表
 * OpeningSelfExeHandle        *(29): 打开被执行的exe文件句柄
 * OpeningExeHandle            *(30): 打开可执行文件句柄
 * TryReadExecutableFile       *(31): 尝试从可执行文件中读取内容
 * TryWriteExecutableFile      *(32): 尝试向可执行文件中写入内容
 * TryDeleteStartupRegVal      *(33): 尝试删除注册表自启动键值
 * TrySetStartupRegVal         *(34): 尝试设置注册表自启动键值
 * TryCloseStdioHandle          (35): 尝试关闭标准输入输出句柄
 * CloseFileHandleFailed        (36): 关闭有效的文件句柄失败
 * CloseUntrackedFileHandle     (37): 关闭没有被监测的文件句柄且关闭成功
 * InvalidFileHandleToClose     (38): 关闭无效的文件句柄
 * DoubleCloseHandle            (39): 连续两次关闭文件句柄且第二次关闭成功
 * TryCloseClosedHandle         (40): 尝试关闭已经关闭的文件句柄且关闭失败
 * RelatingMultipleDirs        *(41): 文件操作涉及多个文件夹
 * TryDeleteStartupRegKey      *(42): 尝试删除整个开机自启动注册表项
 * DeleteRegKeyWithOpenHKey     (43): 在注册表项或其子项被打开的情况下删除该项
 * BindUntrackedSocket          (44): 绑定未被监测的SOCKET
 * UntrackedSocketForSend       (45): 未被监测的SOCKET被用于发送数据
 * UntrackedSocketForRecv       (46): 未被检测的SOCKET被用于接收数据
 * SendFailed                   (47): 网络发送错误
 * RecvFailed                   (48): 网络接收错误
 * SendExeContentToNet         *(49): 将可执行文件内容发送到远程端口
 * SaveExeContentFromNet       *(50): 接收远程发送的内容保存到可执行文件
 * UseAfterFree                -(51): 在堆块被释放后再一次使用（文件API、网络API）
 * DestroyBeforeFree            (52): 在堆块被释放前，其所在堆首先被删除
 */
typedef enum APIException{
    DoubleFree, AllocSameChunk, InvalidDestroyHandle, DestroyFailed, DoubleDestroy, AllocSameHandle,
    OverlappingChunk, ChunkCrossHandle, InvalidFree, FreeChunkNotFound,
    AllocToDestroyedHandle, AllocChunkFailed, AllocHandleFailed, UntrackedHandleDestroyed,
    FreeChunkFailed, UntrackedChunkFreed,
    CreateFileFailed, RepeatedFileHandle, UntrackedFileHandle,
    RegCreateFail, RegOpenFail, RegDeleteFail, RegSetFail, RegCloseFail,
    SameRegHandleGot, UntrackedRegHandle, InvalidRegTypeValue,
    CloseRegKeyTwice, VisitingStartupReg, OpeningSelfExeHandle, OpeningExeHandle,
    TryReadExecutableFile, TryWriteExecutableFile, TryDeleteStartupRegVal,
    TrySetStartupRegVal, TryCloseStdioHandle, CloseFileHandleFailed,
    CloseUntrackedFileHandle, InvalidFileHandleToClose, DoubleCloseHandle,
    TryCloseClosedHandle, RelatingMultipleDirs, TryDeleteStartupRegKey,
    DeleteRegKeyWithOpenHKey, BindUntrackedSocket, UntrackedSocketForSend,
    UntrackedSocketForRecv, SendFailed, RecvFailed, SendExeContentToNet,
    SaveExeContentFromNet, UseAfterFree, DestroyBeforeFree
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
    // 文件名，用于CreateFileFailed、VisitingStartupReg
    QString* fileName;
    // 错误码，用于5个注册表函数错误
    long errorCode;
}exceptionInfo;

typedef enum bufContent{
    ToBeCatagorized_TOFILE,    // 未分类
    PlainText_TOFILE,          // 可打印文本
    ExeFileContent_TOFILE,     // 可执行文件内容
    UnknownContent_TOFILE,     // 未知类型

    ToBeCatagorized_FROMFILE,    // 未分类
    PlainText_FROMFILE,          // 可打印文本
    ExeFileContent_FROMFILE,     // 可执行文件内容
    UnknownContent_FROMFILE,     // 未知类型

    ToBeCatagorized_FROMNET,    // 未分类
    PlainText_FROMNET,          // 可打印文本
    ExeFileContent_FROMNET,     // 可执行文件内容
    UnknownContent_FROMNET,     // 未知类型

    ToBeCatagorized_TONET,    // 未分类
    PlainText_TONET,          // 可打印文本
    ExeFileContent_TONET,     // 可执行文件内容
    UnknownContent_TONET,     // 未知类型
}bufContent;

const QString bufType[] = {"写入文件的未被分类的内容", "写入文件的文本", "写入文件的可执行文件片段", "写入文件的未知内容",
                          "来自文件的未被分类的内容", "来自文件的文本", "来自文件的可执行文件片段", "来自文件的未知内容",
                          "被发送的未被分类的内容", "被发送的文本", "被发送的可执行文件片段", "被发送的未知内容",
                          "接收到的未被分类的内容", "接收到的文本", "接收到的可执行文件片段", "接收到的未知内容"};

const QString memInstType[] = {"WRITE FILE", "READ FILE", "RECEIVE", "SEND"};

typedef struct memoryInfo{
    int length;
    char* content;      // 注：这里的content是本进程中保存的真实的内存内容（拷贝的）而不是目标进程中这块内存的地址
    bufContent type;
}memoryInfo;

class hookAnalysis
{
public:
    hookAnalysis(QStandardItemModel* tableModel, QStandardItemModel* fileViewModel,
                 QStandardItemModel* exceptionModel, QStandardItemModel* regeditModel,
                 QStandardItemModel* logWidgetModel, QStandardItemModel* netModel,
                 QStandardItemModel* memoryModel, QStandardItemModel* moduleModel);

    QString exeFileName;

    bool injMessageBoxA = false;
    bool injMessageBoxW = false;
    bool injHeapCreate = false;
    bool injHeapDestroy = false;
    bool injHeapAlloc = false;
    bool injHeapFree = false;
//    bool injOpenFile = false;     // deprecated
    bool injCreateFile = false;
    bool injReadFile = false;
    bool injWriteFile = false;
    bool injCloseHandle = false;
    bool injRegCreateKeyEx = false;
    bool injRegSetValueEx = false;
    bool injRegDeleteValue = false;
    bool injRegCloseKey = false;
    bool injRegOpenKeyEx = false;
    bool injRegDeleteKeyEx = false;
    bool injSend = false;
    bool injRecv = false;
    bool injConnect = false;
    bool injBind = false;
    bool injSocket = false;
    bool injAccept = false;

    bool exeInfoGot = false;
    bool analyseHeap = true;
    bool analyseFile = true;
    bool analyseReg = true;
    bool analyseNet = true;
    bool modulesGot = false;

    QStandardItemModel* heapViewModel;
    QStandardItemModel* fileViewModel;
    QStandardItemModel* exceptionModel;
    QStandardItemModel* regeditModel;
    QStandardItemModel* logWidgetModel;
    QStandardItemModel* netModel;
    QStandardItemModel* memoryModel;
    QStandardItemModel* moduleModel;

    int validAllocCount = 0, validFreeCount = 0;

    QString currentDir = QString::fromWCharArray(currentDirectory);
    std::set<QString> relatedDirs;

    std::vector<fullLog> logList;
    // 明确使用HeapCreate函数创建的所有堆句柄会保存在这里
    std::map<uint64_t, std::map<unsigned, bool>>* heapHandlesExpl = new std::map<uint64_t, std::map<unsigned, bool>>();
    // 使用HeapCreate函数创建的堆中的所有CHUNK保存在这里
    std::map<uint64_t, std::map<unsigned, bool>>* chunksExpl = new std::map<uint64_t, std::map<unsigned, bool>>();
    std::map<int, APIException> exceptions;     // 所有的异常保存在这里
    std::map<int, uint64_t> memoryLeakRisks;    // 当有CHUNK没有被释放而HANDLE首先被销毁时，将CHUNK地址保存到此处
    std::map<uint64_t, fileHandleAttr> fileHandles; // 这里保存所有的文件句柄及其属性
    std::multimap<QString, uint64_t> regHandles;   // 键是打开的注册表项的完整路径，值是打开的句柄
    // 用于读写文件、网络传输两个模块，保存文件内容
    // 关键内存块保存在这里，地址+内容+保存的内容类别，其中外层map的值也是map主要是考虑到内存空间中内容的变化，便于回溯
    // 其中内层map的键是截获的API编号，外层map的键是这块内存在目标进程的地址
    std::map<uint64_t, std::map<int, memoryInfo>> keyMemories;  // 这里保存所有监测的内存及其属性、内容
    std::map<uint64_t, uint64_t> socketPairs;   // SOCKET对，键是远程SOCKET，值是本地SOCKET
    std::map<uint64_t, QString> connectionInfo; // 连接信息，是本机连接到远程，键是本地SOCKET，值是远程IP+端口
    std::map<uint64_t, QString> remoteSockInfo; // 远程SOCKET信息，SOCKET与远程IP+端口相对应

    exeInfo exeInfo;    // 这里保存文件信息

    argType getType(QString input);
    void getModules();
    bool appendRecord(QString newRecord, char* binBuf = nullptr, int bufSize = 0, bool lastRecord = false);
    void diverter(fullLog latestLog, char *binBuf = nullptr);
    bool updateRecordBeauty(fullLog latestLog, bool lastRecord = false);

    void analyseNewHeapInst(fullLog newHeapLog);
    void analyseNewFileInst(fullLog newFileLog, char *binBuf);
    void analyseNewRegInst(fullLog newRegLog);

    bool addHandle(fullLog newHeapLog);
    bool destroyHandle(fullLog newHeapLog);
    bool addChunk(fullLog newHeapLog);
    bool freeChunk(fullLog newHeapLog);

    int findHandle(uint64_t handleAddr, QStandardItemModel *Model);
    int findChunk(uint64_t chunkAddr, QStandardItem* father);
    int findRegKey(uint64_t handleAddr);

    bool addFileHandle(fullLog newFileLog);
    bool addReadWriteFileRecord(fullLog newFileLog, char *binBuf, bool isRead);
    bool closeFileHandle(fullLog newFileLog);
    void extractDir(fullLog newFileLog);
    QString getAbsolutePath(QString relativePath);

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
    bool setRegVal(fullLog newRegLog);
    bool closeRegKey(fullLog newRegLog);
    bool openRegKey(fullLog newRegLog);
    bool deleteRegKey(fullLog newRegLog);
    bool deleteRegValue(fullLog newRegLog);

    QString getOpenRegKeyName(fullLog newRegLog, int findIdx = -1);

    bufContent addMemory(int logId, uint64_t targetMem, char* buf, int bufLen, bufContent type);

    bool addLocalUnbindedSocket(fullLog newNetLog);
    bool bindLocalSocket(fullLog newNetLog);
    bool newAcception(fullLog newNetLog);
    bool newConnection(fullLog newNetLog);
    bool newSend(fullLog newNetLog, char* buf);
    bool newRecv(fullLog newRecvLog, char* buf);

    bool stepBack(fullLog rewindLog);
    bool revokeHeapCreate(fullLog HeapCreateLog);
    bool revokeHeapDestroy(fullLog HeapDestroyLog);
    bool revokeHeapAlloc(fullLog HeapAllocLog);
    bool revokeHeapFree(fullLog HeapFreeLog);
};

#endif // HOOKANALYSIS_H
