#pragma once
#include<iostream>
#include<string>
#include<conio.h>
#include<winsock.h>
using namespace std;
std::string passwordInput(unsigned maxLength);
int login(SOCKET &sock);
int sign_up(SOCKET sock);
int up_load(SOCKET sock);
int down_load(SOCKET sock);
int get_list_file(SOCKET sock, char*& buffer);


