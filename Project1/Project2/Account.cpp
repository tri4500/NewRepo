#include "Account.h"

Account::Account()
{
	login_name = "admin";
	password = "123";
}

Account::~Account()
{
}

int Account::check(string login,string pass)
{
	return login_name == login && password == pass;
}
int login(SOCKET Client,Account a)
{
	string pw, login;
	int size;
	char* buffer;
	//Nhan kich thuoc cua ten dang nhap
	recv(Client, (char*)&size, sizeof(int), 0);
	buffer = new char[size + 1];
	//Nhan ten dang nhap
	recv(Client, buffer, size, 0);
	buffer[size] = '\0';
	login = string(buffer);
	//Nhan kich thuoc mat khau
	recv(Client, (char*)&size, sizeof(int), 0);
	buffer = new char[size + 1];
	//Nhan mat khau
	recv(Client, buffer, size, 0);
	buffer[size] = '\0';
	pw = string(buffer);
	//Kiem tra ten dang nhap va mat khau
	int check = a.check(login, pw);
	send(Client, (char*)&check, sizeof(int), 0);
	if (check == 1)
	{
		printf("\n\nClient %d dang nhap thanh cong\n", GetCurrentThreadId());
	}
	else
	{
		cout << "\nDang nhap that bai";
	}
	return check;
}
