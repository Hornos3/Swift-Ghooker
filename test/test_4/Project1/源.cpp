#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include <stdlib.h>

#include <WinSock2.h>
#include<windows.h>
#pragma comment(lib, "ws2_32.lib")  //╪сть ws2_32.dll

int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	auto a = INVALID_SOCKET;
	printf("%zx", sock);
}