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
			printf("invalid client socket: %s",GetLastError());
			continue;
		}	
		_beginthreadex(0,0,ServClient,(void*)&client,0,0);
		
	}


	return 0;
}

unsigned int __stdcall ServClient(void *data)
{
	SOCKET* client =(SOCKET*)data;
    SOCKET Client = *client;
	printf("Client connected\n");
	string pw, login;

	int size;
	for (int i = 0; i < 3; i++) {
		//Nhan kich thuoc ten dang nhap
		recv(Client, (char*)&size, sizeof(int), 0);
		char* chunk = new char[size + 1];
		//Nhap ten dang nhap
		recv(Client, chunk, size, 0);
		chunk[size] = '\0';
		login = string(chunk);
		delete[]chunk;
		//Nhan kich thuoc mat khau
		recv(Client, (char*)&size, sizeof(int), 0);
		char * chunk2 = new char[21];
		//Nhan mat khau
		recv(Client, chunk2, size, 0);
		chunk2[size] = '\0';
		pw = string(chunk2);
		delete[]chunk2;
		cout << pw;
		//Gui xac nhan mat khau
		int check = a.check(1, login);
		check = a.check(2, pw);
		send(Client, (char*)&check, sizeof(int), 0);
		if (check == 1)
			break;
	}
	printf("Client %d dang nhap thanh cong\n",GetCurrentThreadId());
return 0;
}