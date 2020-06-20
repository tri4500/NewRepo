#pragma once
#include<iostream>
#include<string>
#include<conio.h>
#include<winsock.h>
using namespace std;
std::string passwordInput(unsigned maxLength);
string create_password();
int login(SOCKET sock);