#pragma once
#include<iostream>
#include<string>
using namespace std;
class Account
{
	string login_name;
	string password;
public:
	Account();
	~Account();
	int check(int sign, string a);// neu sign =1 thi kiem tra login , neu sign=2 thi kiem tra password
};

