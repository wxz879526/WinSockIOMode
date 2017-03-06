// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	//初始化套接字库
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;

	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		printf("WSAStartup failed with err: %d\n", err);
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("Could not find a usable version of winsock.dll.\n");
		return -1;
	}
	else
	{
		printf("find a usable version of winsock.dll.\n");
	}

	// 创建套接字
	SOCKET connectSock = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == connectSock)
	{
		printf("Could not create connectSock,err: %d.\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// Connect
	sockaddr_in srvaddr;
	memset(&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(10010);
	inet_pton(AF_INET, "127.0.0.1", (PVOID)&(srvaddr.sin_addr));
	err = connect(connectSock, (sockaddr*)&srvaddr, sizeof(srvaddr));
	if (SOCKET_ERROR == err)
	{
		printf("Could not bind ListenSocket,err: %d.\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Connected server\n");

	err = send(connectSock, "wenxinzhou", strlen("wenxinzhou"), 0);
	if (SOCKET_ERROR == err)
	{
		printf("Could not send,err: %d.\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	char szBuffer[100] = { 0 };
	err = recv(connectSock, szBuffer, 100, 0);
	if (SOCKET_ERROR == err)
	{
		printf("Could not recv,err: %d.\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	//清理socket库
	closesocket(connectSock);
	WSACleanup();
	return 0;

	return 0;
}

