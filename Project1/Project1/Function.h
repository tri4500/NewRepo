#pragma once
#include<iostream>
#include<string>
#include<conio.h>
#include<winsock.h>
using namespace std;
std::string passwordInput(unsigned maxLength);
string create_password();
int login(SOCKET &sock);
int sign_up(SOCKET sock);
unsigned int __stdcall  Listen(void* data);

