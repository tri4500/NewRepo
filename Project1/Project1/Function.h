#pragma once
#include<iostream>
#include<string>
#include<conio.h>
#include<winsock.h>
using namespace std;
std::string passwordInput(unsigned maxLength);
int login(SOCKET &sock);
int sign_up(SOCKET sock);
unsigned int __stdcall  Listen(void* data);
bool up_load(SOCKET sock);
bool down_load(SOCKET sock);
int get_list_file(SOCKET sock, char*& buffer);


