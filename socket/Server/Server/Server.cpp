// Server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include"winsock2.h"
#include"string.h"
#include<pthread.h>
#include<fstream>
#pragma comment(lib, "pthreadVC2.lib")
using namespace std;

#pragma comment(lib,"ws2_32.lib")

#define MAX_LISTEN_NUM 1
#define SEND_BUF_SIZE 100
#define RECV_BUF_SIZE 100
#define LISTEN_PORT 7800

//服务器监听socket
int listen_sock = 0;
//与客户端连接的socket
int client_sock = 0;
//UDPsocket
int UDP_sock = 0;
//本机的socket地址
struct sockaddr_in serveraddr;
//客户端的socket地址
struct sockaddr_in clientaddr;
//客户端地址数据大小
int socklen = sizeof(clientaddr);
//发送数据内存大小
char sendbuff[SEND_BUF_SIZE] = { 0 };
//接受数据内存大小
char recvbuff[RECV_BUF_SIZE] = { 0 };
int sendlen = 0;
int recvlen = 0;
//w
char file[32] = { 0 };


void *tranFile(void *sendSocket)
{
	int k = 0;
	sprintf(file, "1.dat");
	FILE *source=NULL;
	//
	int isendLEn = send(listen_sock, file, strlen(file),0);
	int ireadlength = recv(listen_sock, recvbuff, RECV_BUF_SIZE,0);
	while (k = fread(recvbuff,1,RECV_BUF_SIZE, source) > 0)
	{
		int iSendLen = send(listen_sock, recvbuff, k,0);
	}

	fclose(source);
	return 0;
}

int udp_sock() {
	//创建sock
	UDP_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (UDP_sock == INVALID_SOCKET)
	{
		printf("creat socket failed\n");
		exit(1);
	}
	//绑定sock
	if (bind(UDP_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
	{
		printf("bind socket failed\n");
		closesocket(listen_sock);
		exit(1);
	}
	int len = sizeof(serveraddr);
	
	//收发消息
	while (true)
	{
		recvfrom(UDP_sock, recvbuff, RECV_BUF_SIZE, 0,(SOCKADDR*)&serveraddr,&len);
		cout << "Client:" << recvbuff << endl;
		cout << "server:" << endl;
		cin >> sendbuff;
		sendto(UDP_sock, sendbuff, SEND_BUF_SIZE, 0, (SOCKADDR*)&serveraddr, len);
	}

	closesocket(UDP_sock);
	return 0;
}

int tcp_sock() {
	//创建socket
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		printf("creat socket failed\n");
		exit(1);
	}

	//绑定socket
	if (bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
	{
		printf("bind socket failed\n");
		closesocket(listen_sock);
		exit(1);
	}

	//监听socket
	if (listen(listen_sock, MAX_LISTEN_NUM) == SOCKET_ERROR)
	{
		printf("listen failed\n");
		closesocket(listen_sock);
		exit(1);
	}
	
	//线程定义
	pthread_t tid;
	while (true)
	{
	//接受客户端连接请求
	client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &socklen);
	if (client_sock == SOCKET_ERROR)
	{
		printf("accept failed\n");
		exit(1);
	}
	int ret = pthread_create(&tid, NULL, tranFile, (void*)&client_sock);
	
	

	}

	closesocket(client_sock);
	closesocket(listen_sock);
	return 0;
}

int main()
{
	
	memset((void*)&serveraddr, 0, sizeof(serveraddr));
	memset((void*)&clientaddr, 0, sizeof(clientaddr));


	//初始化socket
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata )!= 0)
	{
	cout << "WSAStarup error" << endl;
	exit(1);
	}

	//设置本机socket地址
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(LISTEN_PORT);
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	int word;
	while (1)
	{
		cout << "1.transWord" << endl;
		cout << "2.transFile" << endl;
		cin >> word;
		switch (word)
		{
		case 1:
			udp_sock();
		case 2:
			tcp_sock();

		}
	}
	
	//
	WSACleanup();
	cout << "close socket" << endl;
	return 0;



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
