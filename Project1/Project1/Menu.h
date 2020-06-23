#pragma once
#define WIN32_LEAN_AND_MEAN
#include<iostream>
#include <winsock2.h>
#include <string>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
using namespace std;
class Menu
{
private:
	SOCKET	sock;
	char* list_file;
	int list_file_size;
public:
	Menu();
	Menu(SOCKET a);
	~Menu();
	void Print_list_file();
	int start();
	int work();
};

