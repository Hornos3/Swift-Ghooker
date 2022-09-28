#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <time.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <direct.h>

#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

using namespace std;

#define HEAPHANDLE_COUNT 50
#define HEAPCHUNK_COUNT 500
#define FILEHANDLE_COUNT 50
#define FILERWTIME_COUNT 500
#define REGKEY_COUNT 50
#define REGVALUE_COUNT 500

void* chunks[HEAPCHUNK_COUNT];
int heapId[HEAPCHUNK_COUNT];
HANDLE heapHandles[HEAPHANDLE_COUNT];
HANDLE fileHandles[FILEHANDLE_COUNT];
HKEY regHandles[REGKEY_COUNT];
wchar_t keys[REGVALUE_COUNT][0x11];
HANDLE undestroyedHandle;
void* unfreedChunk;

char* generateRandomPrintable(int length) {
	char* ret = (char*)calloc(length, 1);
	for (int i = 0; i < length; i++) {
		char nextChar = rand() % 62;
		if (nextChar < 10)
			nextChar += 0x30;
		else if (nextChar < 36)
			nextChar += 0x41 - 10;
		else
			nextChar += 0x61 - 36;
		ret[i] = nextChar;
	}
	return ret;
}

char* generateRandomUnprintable(int length) {
	char* ret = (char*)calloc(length, 1);
	for (int i = 0; i < length; i++) {
		char nextChar = rand() & 0xFF;
		while (!nextChar || nextChar == '\x1A')
			nextChar = rand() & 0xFF;
		ret[i] = nextChar;
	}
	return ret;
}

void heapTest() {
	puts("*************** Heap API Test ***************");
	puts("Start timing......");
	auto start_time = GetTickCount();
	for (int i = 0; i < HEAPHANDLE_COUNT; i++)
		heapHandles[i] = HeapCreate(0, rand() % 0xFF000 + 0x1000, 0);
	for (int i = 0; i < HEAPCHUNK_COUNT; i++) {
		int chunksize = rand() % 0xFA0 + 0x60;
		heapId[i] = rand() % HEAPHANDLE_COUNT;
		chunks[i] = HeapAlloc(heapHandles[heapId[i]], HEAP_ZERO_MEMORY, chunksize);
	}
	for (int i = 0; i < HEAPCHUNK_COUNT; i++) {
		HeapFree(heapHandles[heapId[i]], 0, chunks[i]);
		if (i == HEAPCHUNK_COUNT / 2) {
			undestroyedHandle = heapHandles[heapId[i]];
			unfreedChunk = chunks[i];
		}
	}
	for (int i = 0; i < HEAPHANDLE_COUNT; i++)
		if(heapHandles[i] != undestroyedHandle)
			HeapDestroy(heapHandles[i]);
	auto end_time = GetTickCount();
	std::cout << "Efficiency test completed.\n"
		"HeapCreate and HeapDestroy called individually for 50 times.\n"
		"HeapAlloc and HeapFree called individually for 500 times.\n"
		"time cost: " << (end_time - start_time) << " ms.\n";
}

void fileTest() {
	puts("*************** File API Test ***************");
	auto start_time = GetTickCount();
	std::cout << "Ready to create and close handles with arbitrary options......\n";
	for (int i = 0; i < 0x10; i++) {
		HANDLE fileHandle = CreateFile(L".\\testfile.txt", i << 28, i & 7, nullptr, rand() % 5 + 1,
			rand() % 8 + (rand() % 16 << 4) + (rand() % 16 << 8) + (rand() % 16 << 12) + (rand() % 16 << 16), nullptr);
		if (fileHandle != INVALID_HANDLE_VALUE)
			CloseHandle(fileHandle);
	}
	std::cout << "Ready to create, read, write file handles...... \n(You need to create a directory named 'test' first!)\n";
	for (int i = 0; i < FILEHANDLE_COUNT; i++) {
		wchar_t fileName[0x30];
		wsprintf(fileName, L".\\test\\%d.txt", i);
		fileHandles[i] = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
		DWORD writtenBytes = 0;
		char buffer[0x100];
		for (int i = 0; i < 0x100; i++) {
			char nextChar = rand() % 62;
			if (nextChar < 10)
				nextChar += 0x30;
			else if (nextChar < 36)
				nextChar += 0x41 - 10;
			else
				nextChar += 0x61 - 36;
			buffer[i] = nextChar;
		}
		if (fileHandles[i] != INVALID_HANDLE_VALUE) {
			WriteFile(fileHandles[i], buffer, 0x100, &writtenBytes, nullptr);
			SetFilePointer(fileHandles[i], NULL, NULL, FILE_BEGIN);
		}
		else
			puts("Invalid handle!");
	}
	for (int i = 0; i < FILERWTIME_COUNT; i++) {
		bool read = rand() % 2;
		if (read) {
			int bytesToRead = 0x10;
			DWORD bytesRead = 0;
			int handleid = rand() % FILEHANDLE_COUNT;
			char buffer[0x11];
			ReadFile(fileHandles[handleid], buffer, bytesToRead, &bytesRead, nullptr);
			buffer[0x10] = '\0';
		}
		else {
			int bytesToWrite = 0x10;
			DWORD bytesWritten = 0;
			int handleid = rand() % FILEHANDLE_COUNT;
			char buffer[0x11];
			for (int i = 0; i < 0x10; i++) {
				char nextChar = rand() % 62;
				if (nextChar < 10)
					nextChar += 0x30;
				else if (nextChar < 36)
					nextChar += 0x41 - 10;
				else
					nextChar += 0x61 - 36;
				buffer[i] = nextChar;
			}
			WriteFile(fileHandles[handleid], buffer, bytesToWrite, &bytesWritten, nullptr);
			buffer[0x10] = '\0';
		}
	}
	for (int i = 0; i < FILEHANDLE_COUNT; i++)
		CloseHandle(fileHandles[i]);
	auto end_time = GetTickCount();
	std::cout << "Efficiency test completed.\n"
		"CreateFile and DestroyFile called individually for 50 times.\n"
		"ReadFile and WriteFile called for 500 times.\n"
		"time cost: " << (end_time - start_time) << " ms.\n";
}

void regTest() {
	puts("*************** Regedit API Test ***************");
	puts("Start timing......");
	auto start_time = GetTickCount();
	// 创建REGVALUE_COUNT个子key
	for (int idx = 0; idx < REGVALUE_COUNT; idx++) {
		for (int i = 0; i < 0x10; i++) {
			char nextChar = rand() % 62;
			if (nextChar < 10)
				nextChar += 0x30;
			else if (nextChar < 36)
				nextChar += 0x41 - 10;
			else
				nextChar += 0x61 - 36;
			keys[idx][i] = nextChar;
		}
		keys[idx][0x11] = '\0';
	}
	// 创建REGKEY_COUNT个key
	for (int i = 0; i < REGKEY_COUNT; i++) {
		wchar_t keyname[0x20];
		wsprintf(keyname, L"a_test_key\\key_%02d", i);
		RegCreateKeyEx(HKEY_CURRENT_USER, keyname, 0, NULL, REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, regHandles + i, NULL);
	}
	for (int i = 0; i < REGVALUE_COUNT; i++) {
		TCHAR Data[254];
		memcpy(Data, keys[i], 0x10);
		memset(Data, 0, sizeof(Data));
		size_t iLen = wcslen(Data);
		RegSetValueEx(regHandles[i % REGKEY_COUNT], keys[i], 0, REG_SZ, (CONST BYTE*)Data, sizeof(TCHAR) * iLen);
	}
	for (int i = 0; i < REGVALUE_COUNT; i++)
		RegDeleteValue(regHandles[i % REGKEY_COUNT], keys[i]);
	for (int i = 0; i < REGKEY_COUNT; i++) {
		wchar_t keyname[0x20];
		wsprintf(keyname, L"a_test_key\\key_%02d", i);
		RegCloseKey(regHandles[i]);
		RegDeleteKeyEx(HKEY_CURRENT_USER, keyname, KEY_WOW64_64KEY, 0);
	}
	RegDeleteKeyEx(HKEY_CURRENT_USER, L"a_test_key", KEY_WOW64_64KEY, 0);
	auto end_time = GetTickCount();
	std::cout << "Efficiency test completed.\n"
		"RegCreateKeyEx and RegDeleteKeyEx called individually for 50 times.\n"
		"RegSetValueEx and RegDeleteValue called individually for 500 times.\n"
		"time cost: " << (end_time - start_time) << " ms.\n";
}

void messageBoxTest() {
	MessageBoxA(NULL, "I'm MessageBoxA", "I'm MessageBoxA's title", MB_OK);
	MessageBoxW(NULL, L"I'm MessageBoxW", L"I'm MessageBoxW's title", MB_OK);
}

void netTest() {
	puts("*************** Net API Test ***************");
	puts("Start timing......");
	auto start_time = GetTickCount();
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	puts("net dll initialized.");
	SOCKET client_sockets[10];
	SOCKET server_sockets[10];
	SOCKET accepted_remoteSockets[10];
	int server_socket_ports[10];
	sockaddr_in server_config[10];

	// 与另一个程序的交互模式如下：
	// 两边各有10个服务端套接字和10个客户端套接字，两个程序都会首先进行服务端套接字的初始化工作，其中本程序首先启动
	// 然后两个程序的10个端口进入监听状态（注意程序阻塞是阻塞在accept函数）
	// 另外一个程序首先进行connect连接上本程序的10个端口让本程序accept完毕，然后本程序再connect到另一个程序的10个端口让令一个程序accept完毕。
	// 如此，两个程序的10个客户端端口都连接上了对方的10个服务器端口，然后就可以发送数据了。

	// 打开服务器套接字的监听并listen
	for (int i = 0; i < 10; i++) {
		server_sockets[i] = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		client_sockets[i] = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		server_config[i].sin_family = AF_INET;
		server_config[i].sin_addr.s_addr = inet_addr("127.0.0.1");
		while (true) {
			server_config[i].sin_port = htons(i + 60000);
			bind(server_sockets[i], (SOCKADDR*)(server_config + i), sizeof(SOCKADDR));
			if (listen(server_sockets[i], 5) == SOCKET_ERROR) {
				cout << "Listen in port " << (i + 60000) << " failed.\n";
				break;
			}
			server_socket_ports[i] = i + 60000;
			break;
		}
	}

	// 尝试连接
	for (int i = 0; i < 10; i++) {
		sockaddr_in sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
		sockAddr.sin_family = PF_INET;
		sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		sockAddr.sin_port = htons(50000 + i);
		connect(client_sockets[i], (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	}
	cout << "connection to 127.0.0.1 50000~50010 completed...\n";

	// 等待接入
	for (int i = 0; i < 10; i++) {
		SOCKADDR clntAddr;
		int nSize = sizeof(SOCKADDR);
		accepted_remoteSockets[i] = accept(server_sockets[i], (SOCKADDR*)&clntAddr, &nSize);
	}
	cout << "connection accepted...\n";

	// 发送信息
	// 发送信息的内容均为随机，长度在10~4096之间，内容分为可打印内容和不可打印内容，均为随机生成
	// 本程序首先发送，然后对方接收后发送。
	puts("Start to send and recv......");
	for (int i = 0; i < 100; i++) {
		char buffer[0x101] = { 0 };
		int len = 0x100;
		bool isPrintable = rand() & 1;
		char* content = isPrintable ? generateRandomPrintable(len) : generateRandomUnprintable(len);
		bool serverSocket = rand() & 1;
		int portIdx = i % 10;
		if (send(accepted_remoteSockets[portIdx], content, len, 0) <= 0)
			cout << "send error!\n";
		if (send(client_sockets[portIdx], content, len, 0) <= 0)
			cout << "send error!\n";
		while (true) {
			int bytesRemaining = 256;
			bytesRemaining -= recv(accepted_remoteSockets[portIdx], buffer + (256 - bytesRemaining), 256, 0);
			if (!bytesRemaining)
				break;
		}
		while (true) {
			int bytesRemaining = 256;
			bytesRemaining -= recv(client_sockets[portIdx], buffer + (256 - bytesRemaining), 256, 0);;
			if (!bytesRemaining)
				break;
		}
	}
	cout << "send and recv completed...\n";
	auto end_time = GetTickCount();
	std::cout << "Efficiency test completed.\n"
		"open 20 sockets, 10 for server and 10 for client.\n"
		"accept 10 connections and connect to 10 remote sockets.\n"
		"send and recv for individually 200 times.\n"
		"time cost: " << (end_time - start_time) << " ms.\n";
}

void exeRelatedTest() {
	{
		puts("*************** Send myself to net ***************");
		// 发送自身exe到网络
		SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		sockaddr_in sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
		sockAddr.sin_family = PF_INET;
		sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		sockAddr.sin_port = htons(1234);
		// 连接服务器
		if (connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == -1)
			puts("invalid connect.");
		puts("Start copying and sending myself......");
		HANDLE self = CreateFile(L".\\finalTest.exe", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
		if (self == INVALID_HANDLE_VALUE) {
			puts("Invalid file handle!");
			return;
		}
		HANDLE duplicate = CreateFile(L".\\finalTest_copy.exe", GENERIC_ALL, 0, nullptr, OPEN_ALWAYS, 0, nullptr);
		BYTE buffer[0x1000];
		DWORD bytesRead = 0x1000;
		DWORD bytesWritten = 0;
		while (bytesRead != 0) {
			ReadFile(self, buffer, 0x1000, &bytesRead, nullptr);
			send(sock, (char*)buffer, bytesRead, 0);
			WriteFile(duplicate, buffer, bytesRead, &bytesWritten, nullptr);
		}
		CloseHandle(self);
		CloseHandle(duplicate);
		puts("Copy and send myself completed...");
		closesocket(sock);
	}

	{
		puts("*************** Save exe from net ***************");
		// 接收网络数据到exe文件
		SOCKET Socket = socket(AF_INET, SOCK_STREAM, 0);
		if (Socket == INVALID_SOCKET)
		{
			printf("Failed socket() \n");
			return;
		}
		// 服务器的ip地址为127.0.0.1，端口为4321
		struct sockaddr_in server_in;
		server_in.sin_family = AF_INET;
		server_in.sin_port = htons(4321);
		server_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		bind(Socket, (SOCKADDR*)(&server_in), sizeof(SOCKADDR));
		// 连接服务器
		if (listen(Socket, 5) == SOCKET_ERROR)
		{
			std::cout << "Listen failed.\n";
			return;
		}
		SOCKADDR clntAddr;
		int nSize = sizeof(SOCKADDR);
		SOCKET accepted_remoteSockets = accept(Socket, (SOCKADDR*)&clntAddr, &nSize);
		puts("remote socket connected, start to receive and save data......");
		// 接收exe文件内容
		ofstream o("./testexeRemote.exe", std::ios::trunc | std::ios::binary);
		int length = 0;
		char buffer[0x1000] = { 0 };
		while (true) {
			int ret = recv(accepted_remoteSockets, buffer, 0x1000, 0);
			if (ret == 0)
				break;
			o.write(buffer, ret);
			length += ret;
		}
		closesocket(Socket);
		o.close();
		std::cout << "receive exe completed, length = " << length << std::endl;
	}
}

void startupReg() {
	puts("*************** Change startup registry ***************");
	HKEY hkey;
	RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey);
	BYTE* value = (BYTE*)L"C:\\Program Files (x86)\\Tencent\\QQ\\Bin\\QQ.exe";
	RegSetValueEx(hkey, L"QQ", 0, REG_SZ, value, lstrlen((LPCWSTR)value) * sizeof(WCHAR));
	RegDeleteValue(hkey, L"QQ");
	RegCloseKey(hkey);
	puts("Change completed.");
}

void multipleFolder() {
	puts("*************** Create file in multiple folders ***************");
	for (int i = 0; i < 10; i++) {
		char filename[0x10];
		sprintf_s(filename, ".\\test\\%d", i);
		_mkdir(filename);
	}
	for (int i = 0; i < 100; i++) {
		wchar_t buffer[0x20];
		wsprintf(buffer, L".\\test\\%d\\", i % 10);
		for (int j = 0; j < 0x10; j++) {
			char nextChar = rand() % 62;
			if (nextChar < 10)
				nextChar += 0x30;
			else if (nextChar < 36)
				nextChar += 0x41 - 10;
			else
				nextChar += 0x61 - 36;
			buffer[j + 9] = nextChar;
		}
		buffer[0x19] = L'.';
		buffer[0x1a] = L't';
		buffer[0x1b] = L'x';
		buffer[0x1c] = L't';
		buffer[0x1d] = L'\0';
		HANDLE h = CreateFile(buffer, GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
		CloseHandle(h);
	}
	puts("Create 10 folders and 100 files completed.");
	for (int i = 0; i < 10; i++) {
		char filename[0x20];
		sprintf_s(filename, "rd /S /Q .\\test\\%d", i);
		system(filename);
	}
	puts("Delete completed.");
}

void allocWithoutFree() {
	puts("*************** Alloc without Free ***************");
	HANDLE handle = HeapCreate(0, 0x10000, 0);
	for (int i = 0; i < 10; i++)
		HeapAlloc(handle, 0, 0x38);
}

void doubleFree() {
	puts("*************** Double free ***************");
	HeapFree(undestroyedHandle, 0, unfreedChunk);		// free twice, will crush
}

int main() {
	srand(time(0));

	heapTest();
	fileTest();
	regTest();
	messageBoxTest();
	netTest();

	exeRelatedTest();
	startupReg();
	multipleFolder();
	allocWithoutFree();
	doubleFree();
	abort();
}