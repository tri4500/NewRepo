﻿#pragma once
#include<iostream>
#include<string>
#include<winsock.h>
#include<vector>
#include<mutex>
#include<fstream>
#include <mutex>
using namespace std;
struct user {
	string login_name;
	string password;
	bool is_login;
};
class Account
{
	vector<user> list;
	mutex mtx;
public:
	Account();
	~Account();
	//kiem tra mat khau voi ten dang nhap co dung hay khong
	int check(string login, string pass);
	//Kiểm tra tên đăng nhập có hay khong
	int check_login(string login);
	void push_back(string login, string pass);
	void save_list_user_file();
	void log_out(string name);
};

int login(SOCKET &Client,Account &a,string &name);
int sign_up(SOCKET&Client, Account &list,string &name);

struct file {
	mutex mutex_val;
	mutex mutex_access;
	int count;
	string name;
	file* next;
};
void send_all(vector<SOCKET>&list,int sign,string name);
file* Create_List_file();
void Update_txt_file(file* list_file);
bool send_list_file(SOCKET sock, file* list, string name);
bool up_load(SOCKET sock, file* list, string name);
bool down_load(SOCKET sock, file* list, string name);
void erase_socket(vector<SOCKET> list,SOCKET a,mutex& mtx);