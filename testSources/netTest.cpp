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

void receiveExeData() {
    // 创建套接字
    SOCKET socket_client = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_client == INVALID_SOCKET)
    {
        printf("Failed socket() \n");
        return;
    }

    // 服务器的ip地址为127.0.0.1，端口为1234
    struct sockaddr_in server_in;
    server_in.sin_family = AF_INET;
    server_in.sin_port = htons(1234);
    server_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    // 连接服务器
    if (connect(socket_client, (struct sockaddr*)&server_in, sizeof(server_in)) == -1)
    {
        printf("Failed connect() \n");
        return;
    }
    // 接收exe文件内容
    ofstream o("./testexeRemote.exe", ios::trunc | ios::binary);
    int length = 0;
    char buffer[0x1000] = { 0 };
    while (true) {
        int ret = recv(socket_client, buffer, 0x1000, 0);
        if (ret == 0)
            break;
        o.write(buffer, ret);
        length += ret;
    }
    o.close();
    cout << "receive exe completed, length = " << length << endl;
}

void netTest() {
    SOCKET client_sockets[100];
    SOCKET server_sockets[100];
    SOCKET accepted_remoteSockets[100];
    int server_socket_ports[100];
    sockaddr_in server_config[100];

    // 与另一个程序的交互模式如下：
    // 两边各有100个服务端套接字和100个客户端套接字，两个程序都会首先进行服务端套接字的初始化工作，其中本程序首先启动
    // 然后两个程序的100个端口进入监听状态（注意程序阻塞是阻塞在accept函数）
    // 另外一个程序首先进行connect连接上本程序的100个端口让本程序accept完毕，然后本程序再connect到另一个程序的100个端口让令一个程序accept完毕。
    // 如此，两个程序的100个客户端端口都连接上了对方的100个服务器端口，然后就可以发送数据了。

    // 打开服务器套接字的监听并accept连接请求
    for (int i = 0; i < 100; i++) {
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
    for (int i = 0; i < 100; i++) {
        sockaddr_in sockAddr;
        memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
        sockAddr.sin_family = PF_INET;
        sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        sockAddr.sin_port = htons(50000 + i);
        connect(client_sockets[i], (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    }

    // 等待接入
    for (int i = 0; i < 100; i++) {
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        accepted_remoteSockets[i] = accept(server_sockets[i], (SOCKADDR*)&clntAddr, &nSize);
    }

    // 发送信息
    // 发送信息的内容均为随机，长度在10~4096之间，内容分为可打印内容和不可打印内容，均为随机生成
    // 本程序首先发送，然后对方接收后发送。
    ofstream oL("./Client_Local.dat", ios::binary | ios::trunc);
    ofstream oR("./Client_Remote.dat", ios::binary | ios::trunc);
    srand(time(0));
    for (int i = 0; i < 1000; i++) {
        char buffer[0x101] = {0};
        int len = 0x100;
        bool isPrintable = rand() & 1;
        char* content = isPrintable ? generateRandomPrintable(len) : generateRandomUnprintable(len);
        bool serverSocket = rand() & 1;
        int portIdx = i % 100;
        if (send(accepted_remoteSockets[portIdx], content, len, 0) <= 0)
            cout << "send error!\n";
        if (send(client_sockets[portIdx], content, len, 0) <= 0)
            cout << "send error!\n";
        oL.write(content, 0x100);
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
        oR.write(buffer, 0x100);
    }
    oL.close();
    oR.close();
}

int main()
{
    char recvData[200];
    char sendData[200];
    int ret;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to load Winsock.\n");
        return USER_ERROR;
    }

    puts("Choose what you want to test:\n"
        "\t0. exit\n"
        "\t1. receive exe data from remote address\n"
        "\t2. net monitor efficiency test\n");

    int choice;
    while (true) {
        puts("Your choice: ");
        std::cin >> choice;
        switch (choice) {
        case 0: {
            exit(0);
        }
        case 1: {
            receiveExeData();
            break;
        }
        case 2: {
            netTest();
            break;
        }
        }
    }
}