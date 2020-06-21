// sockserver.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <process.h>
#include<stdio.h>
#include"Account.h"
#pragma comment(lib,"ws2_32.lib" )

unsigned int __stdcall  ServClient(void *data);
Account a;
int main()
{

	WSADATA wsaData;
	int iResult;
	sockaddr_in addr;
	SOCKET sock,client;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(2222);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	iResult = WSAStartup(MAKEWORD(2,2),&wsaData);//2.2

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

	iResult = bind(sock,(sockaddr*)&addr,sizeof(sockaddr_in ));
	
	if(iResult)
	{
		
		printf("bind failed %u",GetLastError());
		
		return 0;
	}

	iResult = listen(sock,SOMAXCONN);
	
	if(iResult)
	{
		
		printf("iResult failed %u",GetLastError());
		
		return 0;
	}

	while(client = accept(sock,0,0))
	{
		if(client == INVALID_SOCKET)
		{
			printf("invalid client socket: %s",(char*)GetLastError());
			continue;
		}	
		_beginthreadex(0,0,ServClient,(void*)&client,0,0);
		
	}


	return 0;
}

unsigned int __stdcall ServClient(void* data)
{
	SOCKET* client = (SOCKET*)data;
	SOCKET Client = *client;
	int check;
	printf("Client connected\n");
	int option;
	// Client chon dang nhap hoac dang ki
	recv(Client, (char*)&option, sizeof(int), 0);
	if (option == 1)
	{
		"Client dang dang nhap";
		check = login(Client, a);
	}
	else if (option == 2)
	{
		check=sign_up(Client, a);
	}
	else
	{
		cout << "Client thoat\n";
		check = 0;
	}
	if (check == 1)
		cout << "Dang cap nhat\n";
	closesocket(Client);
	return 0;
}

