# Swift-Ghooker
——“可作为调试与测试工具发布的软件”（某位老师所言）

## 0x01. 简介 Introduction
Swift-Ghooker，华中科技大学网络空间安全学院2022年软件安全课程设计项目的主UI部分，完全由本人自主编写。可对目标进程调用的涉及堆、文件、注册表、网络等的共22个windows API进行截获与简单分析。成功满足任务书中所有必做要求与部分选做要求。

注：本项目需要与SSDesign-InjectionDLL项目配合使用，其与本项目均使用Qt6环境+MSVC 2019编译器开发。

## 0x02. 主要功能概述 Main Functions
### A. 对API直接截获信息的分析
这里的直接截获信息指的是API的参数，返回值即某些用于输出的参数在执行结束后的值。截获的API如下：
- 消息框：MessageBoxA，MessageBoxW
- 堆：HeapCreate，HeapDestroy，HeapAlloc，HeapFree
- 文件：CreateFile，ReadFile（会获取lpNumberOfBytesRead在执行结束后的解引用值），WriteFile（会获取lpNumberOfBytesWritten在执行结束后的解引用值），CloseHandle
- 注册表：RegCreateKeyEx，RegSetValueEx，RegDeleteValue，RegCloseKey，RegOpenKeyEx，RegDeleteKeyEx
- 网络：send，recv，accept，bind，connect，socket（对于有具体套接字信息的会获取IP地址和端口号）

### B. 堆相关分析
任务书中要求需要分析程序结束时分配与释放是否一致，以及是否有多次释放一块内存空间的情况。本项目除实现这两项检测外还实现了其他一些异常情况的检测，源码中共枚举出50余种可能出现的异常，其中绝大多数都已被实现能够监测。细目请参见hookanalysis.h的APIException枚举类型。

本程序使用双层map结构保存堆操作信息，外层map的键是堆块/HANDLE地址，值为内层map结构，内层map的键是操作id，值为bool变量，表示该操作后该内存块/HANDLE是否为使用状态：
```cpp
std::map<uint64_t, std::map<unsigned, bool>>* heapHandlesExpl;
std::map<uint64_t, std::map<unsigned, bool>>* chunksExpl;
```

需要注意的是，当检测到HeapAlloc、HeapFree、HeapDestroy三个API的HANDLE操作对象在之前并未通过HeapCreate返回时，本程序不作处理，在堆监控视图中也不会进行显示。

在堆视图中，保存有最后一次API截获后程序的堆内存情况，使用QTreeView表示，每一行均为HANDLE，每一行的子项均为该HANDLE下分配的内存块。虽然内存块的实际大小与HeapAlloc传入的表示申请大小的参数值并不一定相等，但仍然将其视作该内存块的大小。

堆视图中：
- 正在使用的HANDLE以<font color=green>绿色</font>作为背景色
- 已被删除的HANDLE以<font color=red>红色</font>作为背景色
- 正在使用的HANDLE下：
	- 正在使用的堆块以<font color=green>绿色</font>作为背景色
	- 被释放的堆块以<font color=red>红色</font>作为背景色
- 已被删除的HANDLE下：
	- 所有堆块均以<font color=gray>灰色</font>作为背景色
	- 但有一种情况除外：当该HANDLE中存在有正在使用的堆块时该HANDLE被删除，则该HANDLE删除后其内部所有未被释放的堆块均以<font color=orange>橙色</font>作为背景色以表示此操作可能存在风险。

### C. 文件相关分析
任务书中要求检测操作范围是否有多个文件夹、是否自我复制、是否修改其他可执行文件代码、是否将文件内容读取后发送到网络。与堆操作检测相同，除实现以上功能外，还能够进行其他一些异常情况的检测。

同堆监控，也是使用双层map的结构保存文件打开和关闭的状态，便于查找、灵活回溯与推演。

所有与文件读写相关的内存空间都会被读取（DLL中的工作）并保存到一个单独的文件中，在实时监测模式下由UI实时读取并显示在内存视图中，在历史分析模式下由UI将文件读取完毕后再进行显示。

```cpp
std::map<uint64_t, std::map<unsigned, fileHandleAttr>> fileHandles;
```

本程序对文件打开时的各种选项参数进行了解析，并能够在文件视图点击某行时使用<font color=green>绿色</font>高亮显示该文件句柄的打开选项。

在文件视图中，保存有文件句柄及与该句柄相关的读写操作。其中：
- 正在使用的文件句柄以<font color=green>绿色</font>作为背景色
- 已被关闭的文件句柄以<font color=red>红色</font>作为背景色
- 文件句柄下：
	- 读操作以<font color=C8C8FF>浅紫色</font>作为背景色
	- 写操作以<font color=cccc00>黄色</font>作为背景色
	- 关闭操作以<font color=red>红色</font>作为背景色
	- 打开操作以<font color=green>绿色</font>作为背景色。

### D. 注册表相关分析
任务书中要求检测是否新增注册表项并判断是否为开机自启项，是否修改注册表，输出所有注册表操作项。与堆操作检测相同，除实现以上功能外，还能够进行其他一些异常情况的检测。

为查询注册表打开项方便，使用multimap结构保存注册表项的路径和句柄对应信息（因为一个路径可能有多个句柄被打开，所以不使用map），应回溯和推演的需要，使用与堆分析相似的结构保存打开和关闭信息。

```cpp
std::multimap<QString, uint64_t> regHandles;
std::map<uint64_t, std::map<unsigned, pair<QString, bool>>> regHandlesExpl;
```

在注册表视图中，保存有注册表相关操作，父项为打开的注册表句柄，是注册表项，子项为相关的注册表操作，可能涉及项内子项的创建与删除，以及键值对的创建、设置与删除。
- 正在使用的注册表项句柄以<font color=green>绿色</font>作为背景色
- 已经关闭的注册表项句柄以<font color=red>红色</font>作为背景色
- 已经被删除的注册表项句柄无论在删除前是否打开，都将以<font color=purple>紫色</font>作为背景色
- 开机自启注册表项以<font color=red>大红色</font>作为背景色
- 注册表句柄父项下：
	- 设置键值操作以<font color=blue>蓝色</font>作为背景色（其中对于开机自启项的设置以<font color=red>大红色</font>作为背景色）
	- 删除键值及删除子项操作以<font color=green>绿色</font>作为背景色（其中对于开机自启项的设置以<font color=red>大红色</font>作为背景色）
	- 关闭该句柄以<font color=red>红色</font>作为背景色

### E. 网络相关分析
任务书中要求对基本的网络API进行截获，打印参数，并判断发送内容是否是明文。

与文件操作相同，所有发送或接收网络内容的缓冲区内容都会被DLL读取并保存到文件中，然后被UI读取保存到内存视图中显示。

由于网络连接的特殊性，本程序使用了较多的数据结构以不同的形式保存套接字对应信息，以最大程度地保证时间上的执行效率。

```cpp
std::map<uint64_t, uint64_t> socketPairs;
std::map<uint64_t, QString> connectionInfo;
std::map<uint64_t, QString> remoteSockInfo;
std::map<uint64_t, std::map<unsigned, pair<QString, QString>>> universalSocketInfo;
```

在网络视图中，保存有套接字信息及与之相关的网络操作。

- 新创建的还未绑定IP和端口，还未连接远程的SOCKET以<font color=green>绿色</font>作为背景色
- 已经绑定IP和端口但还未接受远程连接的SOCKET以<font color=cccc00>黄色</font>作为背景色
- 已经接受远程连接的SOCKET以<font color=blue>蓝色</font>作为背景色
- 已经连接远程的SOCKET以<font color=C8C8FF>浅紫色</font>作为背景色
- 对于SOCKET操作：
	- 发送操作以<font color=orange>橙色</font>作为背景色
	- 接受操作以<font color=99cc00>黄绿色</font>作为背景色
	- 绑定IP地址和端口操作以<font color=cccc00>黄色</font>作为背景色
	- 接受远程连接以<font color=C8C8FF>浅紫色</font>作为背景色
	- 连接远程以<font color=blue>蓝色</font>作为背景色

### F. 内存相关分析

本程序配有内存视图，其中保存有目标进程中已知与文件读写或网络收发有关的内存空间，由于内存空间在不同时间段的内容可能不同，在内存视图中能够显示每一次相关API执行后相应内存的内容，内存的显示格式仿照winhex进行设计。其中每一块内存都有相应的标签，表示这一块内存中可能保存的内容，如读取到的exe文件片段等，据此来判断目标进程是否将exe文件读取并发送到网络。

```cpp
std::map<uint64_t, std::map<int, memoryInfo>> keyMemories;
```

### G. 自由截获
本程序使用了共享内存，可在目标进程开始执行之前自由选择是否截获某个API，从而更有针对性地进行某一方面的检测与分析。

### H. 历史截获记录重演
本程序支持通过历史截获记录对进程的截获进行重演，便于用户在进程结束后进行复盘再分析。

### I. 回溯与推演
本程序支持在目标进程结束后或重演历史进程时对所有的检测进行一定程度的回溯，如一次截获共获取到5000次截获的记录，用户可以自由选择将监控状态调整至截获到第几条记录之后，便于逐API截获记录的分析与重演，提升调试与检查效率，使用户能够更为清楚地获取到进程执行过程中某一时刻的状态。

## 0x03. 不足之处 Drawbacks
虽然本项目在任务书的基础上做出了很大的扩展，但在很多方面仍然存在不足，有较大的改善和提升空间。
### A. 截获卡顿
当目标进程短时间内调用多次API时，UI会出现卡顿的现象。因此为了UI的鲁棒性考虑，设置一次进程执行截获API次数最多为5000次。

### B. 源代码耦合性高
在开发初期没有充分考虑耦合性问题导致新增功能需要修改很多地方的代码，代码封装不充分，不利于项目的进一步扩展开发。

### C. 依然存在未被处理的一些bug
如回溯时对行背景色的处理、第二次进行截获导致程序崩溃、release版本中出现的一些debug版本未出现的bug等问题依然没有解决。另外由于测试的样例不足，可能还会存在别的未发现的bug。

### D. 截获效率有待提高
在电脑充电状态，即满性能状态下，1s平均可以截获约200个API，速度有待提升。
- 效率测试结果：在release版本下进行测试
	- 1100次堆相关操作：平均约6s，约180 hooks/s
	- 1100次文件相关操作：平均约3.5s，约315 hooks/s
	- 1100次注册表相关操作：平均约6s，约180 hooks/s
	- 网络相关操作：由于网络环境的影响，多次测试发现时间花费差距较大，但一般不超过5s。
	- 历史记录读取：平均约1000 records/s（debug慢4倍，约200 records/s）
