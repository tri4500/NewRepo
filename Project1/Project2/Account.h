#pragma once
#include<iostream>
#include<string>
#include<winsock.h>
using namespace std;
class Account
{
	string login_name;
	string password;
public:
	Account();
	~Account();
	int check(string login, string pass);// neu sign =1 thi kiem tra login , neu sign=2 thi kiem tra password
};

int login(SOCKET Client,Account a);