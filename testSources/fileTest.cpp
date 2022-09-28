#include <iostream>
#include <cstdlib>
#include <direct.h>
#include "windows.h"
#include <fstream>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
HANDLE fileHandles[0x40];
bool debugMode = false;
using namespace std;
const char* menu = "--------------------------------------\n"
	"Choose what you want to test:\n"
	"0. exit\n"
	"1. file monitor efficiency test\n"
	"2. close any handle (you need to input)\n"
	"3. open an executable file and read/write\n"
	"4. close an invalid handle\n"
	"5. close a handle twice\n"
	"6. open file from multiple directories\n"
	"7. duplicate myself and send myself through TCP socket\n"
	"8. receive data from net and save into exe file\n";

int main() {
	puts(menu);
	int choice;

	while (true) {
		puts("Your choice: ");
		std::cin >> choice;
		switch (choice) {
		case 0:
			exit(0);
		case 1: {
			auto start_time = GetTickCount();
			srand(time(0));
			std::cout << "Ready to create and close handles with arbitrary options......";
			for (int i = 0; i < 0x40; i++) {
				HANDLE fileHandle = CreateFile(L".\\testfile.txt", i << 28, i & 7, nullptr, rand() % 5 + 1, 
					rand() % 8 + (rand() % 16 << 4) + (rand() % 16 << 8) + (rand() % 16 << 12) + (rand() % 16 << 16), nullptr);
				if(fileHandle != INVALID_HANDLE_VALUE)
					CloseHandle(fileHandle);
			}
			std::cout << "Ready to create, read, write file handles...... \n(You need to create a directory named 'test' first!)\n";
			for (int i = 0; i < 0x40; i++) {
				wchar_t fileName[0x30];
				wsprintf(fileName, L".\\test\\%d.txt", i);
				fileHandles[i] = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
				DWORD writtenBytes = 0;
				char buffer[0x1000];
				for (int i = 0; i < 0x1000; i++) {
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
					WriteFile(fileHandles[i], buffer, 0x1000, &writtenBytes, nullptr);
					SetFilePointer(fileHandles[i], NULL, NULL, FILE_BEGIN);
				}
				else
					puts("Invalid handle!");
			}
			for (int i = 0; i < 0x1000; i++) {
				bool read = rand() % 2;
				if (read) {
					int bytesToRead = 0x10;
					DWORD bytesRead = 0;
					int handleid = rand() % 0x40;
					char buffer[0x11];
					ReadFile(fileHandles[handleid], buffer, bytesToRead, &bytesRead, nullptr);
					buffer[0x10] = '\0';
					if (debugMode) {
						printf("Read: ");
						puts(buffer);
					}
				}else {
					int bytesToWrite = 0x10;
					DWORD bytesWritten = 0;
					int handleid = rand() % 0x40;
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
					if (debugMode) {
						printf("Write: ");
						puts(buffer);
					}
				}
			}
			for (int i = 0; i < 0x40; i++)
				CloseHandle(fileHandles[i]);
			auto end_time = GetTickCount();
			printf("Efficiency test completed, time cost: %d ms.\n", end_time - start_time);
			break;
		}
		case 2: {
			uint64_t handle;
			printf("input your handle: ");
			std::cin >> std::hex >> handle;
			CloseHandle((HANDLE)handle);
			break;
		}
		case 3: {
			HANDLE exeFileHandle = CreateFile(L".\\test\\exetest.dll", GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
			if (exeFileHandle == INVALID_HANDLE_VALUE) {
				puts("Invalid exe test file handle!");
				break;
			}
			char buffer[0x10] = "C01in f0r 7est.";
			char recvBuffer[0x10];
			DWORD finishLength = 0;
			WriteFile(exeFileHandle, buffer, 0x10, &finishLength, nullptr);
			SetFilePointer(exeFileHandle, 8, NULL, FILE_BEGIN);
			ReadFile(exeFileHandle, recvBuffer, 0x10, &finishLength, nullptr);	// finishLength should be 8
			SetFilePointer(exeFileHandle, NULL, NULL, FILE_BEGIN);
			WriteFile(exeFileHandle, buffer, 0x10, &finishLength, nullptr);
			ReadFile(exeFileHandle, recvBuffer, 0x10, &finishLength, nullptr);	// finishLength should be 0
			CloseHandle(exeFileHandle);
			break;
		}
		case 4: {
			CloseHandle((HANDLE)0xdeadbeef);
			break;
		}
		case 5: {
			HANDLE exeFileHandle = CreateFile(L".\\test\\exetest.dll", GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
			CloseHandle(exeFileHandle);
			CloseHandle(exeFileHandle);
			break;
		}
		case 6: {
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
				if(debugMode)
					wprintf(L"%ls\n", buffer);
				HANDLE h = CreateFile(buffer, GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
				CloseHandle(h);
			}
			for (int i = 0; i < 10; i++) {
				char filename[0x20];
				sprintf_s(filename, "rd /S /Q .\\test\\%d", i);
				system(filename);
			}
			break;
		}
		case 7: {
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);
			puts("net dll initialized.");
			SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			sockaddr_in sockAddr;
			memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
			sockAddr.sin_family = PF_INET;
			sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
			sockAddr.sin_port = htons(1234);
			// 连接服务器
			if(connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == -1)
				puts("invalid connect.");
			puts("Start copying myself......");
			HANDLE self = CreateFile(L".\\fileTest.exe", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
			if (self == INVALID_HANDLE_VALUE) {
				puts("Invalid file handle!");
				break;
			}
			HANDLE duplicate = CreateFile(L".\\fileTest_copy.exe", GENERIC_ALL, 0, nullptr, OPEN_ALWAYS, 0, nullptr);
			BYTE buffer[0x1000];
			DWORD bytesRead = 0x1000;
			DWORD bytesWritten = 0;
			while (bytesRead != 0) {
				ReadFile(self, buffer, 0x1000, &bytesRead, nullptr);
				send(sock, (char*)buffer, 0x1000, 0);
				WriteFile(duplicate, buffer, bytesRead, &bytesWritten, nullptr);
			}
			CloseHandle(self);
			CloseHandle(duplicate);
			puts("Copy completed."); 
			closesocket(sock);
			break;
		}
		case 8: {
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);
			puts("net dll initialized.");
			// 创建套接字
			SOCKET Socket = socket(AF_INET, SOCK_STREAM, 0);
			if (Socket == INVALID_SOCKET)
			{
				printf("Failed socket() \n");
				break;
			}

			// 服务器的ip地址为127.0.0.1，端口为1234
			struct sockaddr_in server_in;
			server_in.sin_family = AF_INET;
			server_in.sin_port = htons(4321);
			server_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
			bind(Socket, (SOCKADDR*)(&server_in), sizeof(SOCKADDR));
			// 连接服务器
			if (listen(Socket, 5) == SOCKET_ERROR)
			{
				std::cout << "Listen failed.\n";
				break;
			}
			SOCKADDR clntAddr;
			int nSize = sizeof(SOCKADDR);
			SOCKET accepted_remoteSockets = accept(Socket, (SOCKADDR*)&clntAddr, &nSize);
			puts("connected.");
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
	}
}