#pragma once
#include<iostream>
#include<string>
#include<winsock.h>
#include<vector>
using namespace std;
struct user {
	string login_name;
	string password;
};
class Account
{
	vector<user> list;
public:
	Account();
	~Account();
	//kiem tra mat khau voi ten dang nhap co dung hay khong
	int check(string login, string pass);
	//Kiểm tra tên đăng nhập có hay khong
	int check_login(string login);
	void push_back(string login, string pass);
};

int login(SOCKET Client,Account a);
int sign_up(SOCKET Client, Account list);