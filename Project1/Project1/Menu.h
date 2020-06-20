#pragma once
#define WIN32_LEAN_AND_MEAN
#include<iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
using namespace std;
class Menu
{
private:
	SOCKET	sock;
public:
	Menu();
	~Menu();
	int login();
};

