// sockclient.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include<stdio.h>
#include"Function.h"
#include"Menu.h"
#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

int main()
{
	WSADATA wsaData;
	int iResult;
	sockaddr_in addr;
	SOCKET sock;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(2222);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	iResult = WSAStartup(MAKEWORD(2,2),&wsaData);

	if(iResult)
	{
		printf("WSA startup failed");
		return 0;
	}


	sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if(sock == INVALID_SOCKET)
	{
		printf("Invalid socket");
		return 0;
	}

	iResult = connect(sock,(SOCKADDR*)&addr,sizeof(sockaddr_in ));

	if(iResult)
	{
		printf("Connect failed %u",WSAGetLastError());
		return 0;
	}
	printf("Connect Server Success\n");
	Menu menu(sock);
	if (menu.start() == 1)
		cout << "cac dich vu khac chua viet code";

	closesocket(sock);
	return 0;
}