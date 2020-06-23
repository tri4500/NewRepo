// sockserver.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <process.h>
#include<stdio.h>
#include"Account.h"
#include <fstream>
#pragma comment(lib,"ws2_32.lib" )
#pragma warning( disable : 4996 )
unsigned int __stdcall  ServClient(void *data);
Account a;
vector<SOCKET> list_socket;
file* list_file;
mutex mutex_delete_socket;// dung de xoa cac socket khong con ket noi
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
	// khoi tao list file
	
	list_file = Create_List_file();

	
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
		list_socket.push_back(client);
		_beginthreadex(0,0,ServClient,(void*)&client,0,0);
		
	}
	//cap nhat list file vao file txt
	Update_txt_file(list_file);
	// xoa list file
	file* temp = list_file;
	while (temp) {
		file* temp1 = temp->next;
		delete[] temp;
		temp = temp1;
	}

	return 0;
}

unsigned int __stdcall ServClient(void* data)
{
	SOCKET* client = (SOCKET*)data;
	SOCKET Client = *client;
	string name;
	int check;
	printf("Client connected\n");
	int option;
	// Client chon dang nhap hoac dang ki
	recv(Client, (char*)&option, sizeof(int), 0);
	if (option == 1)
	{
		cout << "1 Client dang dang nhap\n";
		check = login(Client, a,name);
	}
	else if (option == 2)
	{
		check=sign_up(Client, a);
		a.save_list_user_file();
	}
	else
	{
		cout << "Client thoat\n";
		check = 0;
	}
	if (check == 1) {
		send_all(list_socket, 1, name);
		send_list_file(Client, list_file, name);
		int index = 1;
		while (index != 0) {
			recv(Client, reinterpret_cast<char*>(&index), sizeof(index), 0);
			// index=1 -> download file
			if (index == 1) {
				down_load(Client, list_file, name);
			}
			//index=2 -> upload file
			else if (index == 2) {
				up_load(Client, list_file, name);
			}
			//index=3 -> Tai lai list file
			else if (index == 3) {
				send_list_file(Client, list_file, name);
			}
			else {
				index = 0;
			}
		}
		cout << "nguoi dung " << name << " log out" << endl;
		erase_socket(list_socket, Client, mutex_delete_socket);
		send_all(list_socket, 2, name);
	}

	closesocket(Client);
	a.log_out(name);

	return 0;
}

