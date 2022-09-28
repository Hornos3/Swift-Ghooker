#include <winsock2.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>     
#include <iostream>
#include <fstream>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

#define USER_ERROR -1
using namespace std;

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

void netTest() {
    SOCKET client_sockets[10];
    SOCKET server_sockets[10];
    SOCKET accepted_remoteSockets[10];
    int server_socket_ports[10];
    sockaddr_in server_config[10];

    // ����һ������Ľ���ģʽ���£�
    // ���߸���100��������׽��ֺ�100���ͻ����׽��֣��������򶼻����Ƚ��з�����׽��ֵĳ�ʼ�����������б�������������
    // Ȼ�����������100���˿ڽ������״̬��ע�����������������accept������
    // ����һ���������Ƚ���connect�����ϱ������100���˿��ñ�����accept��ϣ�Ȼ�󱾳�����connect����һ�������100���˿�����һ������accept��ϡ�
    // ��ˣ����������100���ͻ��˶˿ڶ��������˶Է���100���������˿ڣ�Ȼ��Ϳ��Է��������ˡ�

    // �򿪷������׽��ֵļ�����accept��������
    for (int i = 0; i < 10; i++) {
        server_sockets[i] = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        client_sockets[i] = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        server_config[i].sin_family = AF_INET;
        server_config[i].sin_addr.s_addr = inet_addr("127.0.0.1");
        while (true) {
            server_config[i].sin_port = htons(i + 50000);
            bind(server_sockets[i], (SOCKADDR*)(server_config + i), sizeof(SOCKADDR));
            if (listen(server_sockets[i], 5) == SOCKET_ERROR) {
                cout << "Listen in port " << (i + 50000) << " failed.\n";
                break;
            }
            server_socket_ports[i] = i + 50000;
            break;
        }
    }

    // �ȴ�����
    cout << "wait for connection...\n";
    for (int i = 0; i < 10; i++) {
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        accepted_remoteSockets[i] = accept(server_sockets[i], (SOCKADDR*)&clntAddr, &nSize);
    }

    cout << "connect to net...\n";
    // ��������
    for (int i = 0; i < 10; i++) {
        sockaddr_in sockAddr;
        memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
        sockAddr.sin_family = PF_INET;
        sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        sockAddr.sin_port = htons(60000 + i);
        connect(client_sockets[i], (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    }

    // ������Ϣ
    // ������Ϣ�����ݾ�Ϊ���������Ϊ256�����ݷ�Ϊ�ɴ�ӡ���ݺͲ��ɴ�ӡ���ݣ���Ϊ�������
    // ���������Ƚ��գ����յ�һ�κ��ٷ���
    // ����Ὣ����������ɵ����ݱ��浽�ļ��б���
    srand(time(0));
    for (int i = 0; i < 100; i++) {
        char buffer[0x101] = { 0 };
        int len = 0x100;
        bool isPrintable = rand() & 1;
        char* content = isPrintable ? generateRandomPrintable(len) : generateRandomUnprintable(len);
        bool serverSocket = rand() & 1;
        int portIdx = i % 10;
        recv(client_sockets[portIdx], buffer, 256, 0);
        recv(accepted_remoteSockets[portIdx], buffer, 256, 0);
        if (send(accepted_remoteSockets[portIdx], content, len, 0) <= 0)
            cout << "send error!\n";
        if (send(client_sockets[portIdx], content, len, 0) <= 0)
            cout << "send error!\n";
    }
}

void receiveExeData() {
    // �����׽���
    SOCKET Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (Socket == INVALID_SOCKET)
    {
        printf("Failed socket() \n");
        return;
    }

    // ��������ip��ַΪ127.0.0.1���˿�Ϊ1234
    struct sockaddr_in server_in;
    server_in.sin_family = AF_INET;
    server_in.sin_port = htons(1234);
    server_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    bind(Socket, (SOCKADDR*)(&server_in), sizeof(SOCKADDR));
    // ���ӷ�����
    if (listen(Socket, 5) == SOCKET_ERROR)
    {
        cout << "Listen failed.\n";
        return;
    }
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET accepted_remoteSockets = accept(Socket, (SOCKADDR*)&clntAddr, &nSize);
    cout << "connection accepted.(exe file)\n";
    puts("connected.");
    // ����exe�ļ�����
    ofstream o("./testexeRemote.exe", ios::trunc | ios::binary);
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
    cout << "receive exe completed, length = " << length << endl;
}

void simulate_spread() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    puts("net dll initialized.");
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(4321);    // Զ�̶˿�4321
    // ���ӷ�����
    if (connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == -1)
        puts("invalid connect.");
    puts("Start copying myself......");
    HANDLE self = CreateFile(L".\\finalTestPartner.exe", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
    if (self == INVALID_HANDLE_VALUE) {
        puts("Invalid file handle!");
        return;
    }
    HANDLE duplicate = CreateFile(L".\\finalTestPartner_copy.exe", GENERIC_ALL, 0, nullptr, OPEN_ALWAYS, 0, nullptr);
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
    return;
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to load Winsock.\n");
        return USER_ERROR;
    }
    netTest();
    receiveExeData();
    Sleep(1);
    simulate_spread();
}