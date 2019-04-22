// Client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include"winsock2.h"
#include"string.h"
#include <WS2tcpip.h>
#include"pthread.h"
#include<io.h>
#include<fstream>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

#define MAX_LISTEN_NUM 1
#define SEND_BUF_SIZE 100
#define RECV_BUF_SIZE 100
#define LISTEN_PORT 7800



//定义socket
int sock_fd = 0;
//发送数据内存大小
char sendbuff[SEND_BUF_SIZE] = { 0 };
//接受数据内存大小
char recvbuff[RECV_BUF_SIZE] = { 0 };
//定义服务器socket通信地址
struct sockaddr_in serveraddr;
char file[32] = { 0 }; 

int TransWord() {
	//创建socket
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	int len = sizeof(serveraddr);

	//收发消息
	while (true)
	{
		cout << "client:" << endl;
		cin >> sendbuff;
		if (strcmp(sendbuff, "close") == 0)
		{
			cout << "client requests close the connect" << endl;
			break;
		}
		sendto(sock_fd, sendbuff, SEND_BUF_SIZE, 0, (SOCKADDR*)&serveraddr, len);
		recvfrom(sock_fd, recvbuff, RECV_BUF_SIZE, 0, (SOCKADDR*)&serveraddr, &len);


		memset(sendbuff, 0, sizeof(sendbuff));
		memset(recvbuff, 0, sizeof(recvbuff));
	}

	closesocket(sock_fd);
	
	cout << "socket close" << endl;
	return 0;
}
int TransFile() 
{
	//创建socket
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == INVALID_SOCKET)
	{
		printf("creat socket failed\n");
		exit(1);
	}

	//连接到服务器
	if (connect(sock_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
	{
		cout << "fail to connect" << endl;
		exit(1);
	}

	//接受文件名称
	int re = 0;
	while (re <= 0)
	{
		re = recv(sock_fd, file, sizeof(file),0);
	}
	//创建文件
	FILE *target;
	target = fopen(recvbuff, "wb");
	//通知服务器已准备好
	sprintf(sendbuff, "ready");
	send(sock_fd, sendbuff, strlen(sendbuff),0);

	while (re = recv(sock_fd, recvbuff, RECV_BUF_SIZE,0) > 0)
	{
		fwrite(recvbuff, 1, re, target);
	}

	fclose(target);
	closesocket(sock_fd);


	
	
	cout << "socket close" << endl;
	return 0;
	
}
int main()
{
	
	int word=0;
	//初始化socket
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		cout << "WSAStarup error" << endl;
		exit(1);
	}

	//设置服务器socket参数
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(LISTEN_PORT);
	//serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr.S_un.S_addr);

	while (1)
	{
		cout << "1.transWord" << endl;
		cout << "2.transFile" << endl;
		cin >> word;
		switch (word)
		{
		case 1:
			TransWord();
		case 2:
			TransFile();
		
		}
	}
	WSACleanup();
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
