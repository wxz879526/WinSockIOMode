// Srv_Select.cpp : 定义控制台应用程序的入口点。
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
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == listenSock)
	{
		printf("Could not create ListenSocket,err: %d.\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// Bind地址
	sockaddr_in srvaddr;
	memset(&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(10010);
	srvaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	err = bind(listenSock, (sockaddr*)&srvaddr, sizeof(srvaddr));
	if (SOCKET_ERROR == err)
	{
		printf("Could not bind ListenSocket,err: %d.\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// Listen
	err = listen(listenSock, SOMAXCONN);
	if (SOCKET_ERROR == err)
	{
		printf("Could not listen,err: %d.\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// Accept
	for (;;)
	{
		sockaddr_in clientAddr;
		memset(&clientAddr, 0, sizeof(clientAddr));
		clientAddr.sin_family = AF_INET;
		int clientaddrLen = sizeof(clientAddr); //这里赋值初始化是必须的 否则会 accept出错 返回 10014
		SOCKET connectSock = accept(listenSock, (sockaddr*)&clientAddr, &clientaddrLen);
		if (SOCKET_ERROR == connectSock)
		{
			printf("Could not accept,err: %d.\n", WSAGetLastError());
			WSACleanup();
			return -1;
		}
		else
		{
			char szBuffer[100] = { 0 };
			inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, szBuffer, 100);
			printf("A client connected: %s:%d.\n", szBuffer, clientAddr.sin_port);
		}

		for (;;)
		{
			char szBuffer[100] = { 0 };
			err = recv(connectSock, szBuffer, 100, 0);
			if (SOCKET_ERROR == err)
			{
				printf("Could not recv,err: %d.\n", WSAGetLastError());
				WSACleanup();
				break;
			}
			else
			{
				char szClientAddr[100] = { 0 };
				inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, szClientAddr, 100);
				printf("recv %s from %d:%d: %s:%d.\n", szBuffer, szClientAddr, clientAddr.sin_port);
			}
		}
	}

	//清理socket库
	closesocket(listenSock);
	WSACleanup();

	return 0;
}

