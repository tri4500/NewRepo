// sockclient.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include<stdio.h>
#include"Function.h"
#include<process.h>
#include"Menu.h"
#include <mutex>
#pragma comment(lib,"ws2_32.lib")
#pragma warning( disable : 4996 )
#define _WINSOCK_DEPRECATED_NO_WARNINGS
WSADATA wsaData;
int iResult;
sockaddr_in addr;
SOCKET sock;
mutex sock_access;
int thong_bao;
unsigned int __stdcall Nghe_Thong_bao(void* data);
int main()
{
	

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
	{
		recv(sock, reinterpret_cast<char*>(&thong_bao), sizeof(thong_bao), 0);
		sock_access.lock();
		menu.Get_List_file();
		sock_access.unlock();
		_beginthreadex(0, 0, Nghe_Thong_bao, (void*)&sock, 0, 0);
		int work_return = 1;
		while (work_return != 0) {
			menu.Print_list_file();
			cout << "\n\n\n\t Chon thao tac:" << endl;
			cout << "1. Download File" << endl;
			cout << "2. Upload File" << endl;
			cout << "3. Tai lai danh sach file" << endl;
			cout << "0. Thoat" << endl;
			int index;
			cin >> index;
			sock_access.lock();
			work_return = menu.work(index);
			sock_access.unlock();
		}
	}
	closesocket(sock);
	return 0;
}

unsigned int __stdcall Nghe_Thong_bao(void* data) {
	while (true) {
		int index = 4;
		sock_access.lock();
		send(sock, reinterpret_cast<char*>(&index), sizeof(index), 0);
		recv(sock, reinterpret_cast<char*>(&index), sizeof(index), 0);
		char* buffer;
		buffer = new char[index + 1];
		recv(sock, buffer, index, 0);
		buffer[index] = '\0';
		string temp = string(buffer);
		int size = temp.length();
		if (size > thong_bao) {
			temp = temp.substr(thong_bao);
			cout << temp;
		}
		thong_bao = size;
		sock_access.unlock();
		Sleep(5000);
	}
}