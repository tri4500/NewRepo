#include "Account.h"

Account::Account()
{
	user b;
	b.login_name = "admin";
	b.password = "123";
	list.push_back(b);
}

Account::~Account()
{
}

int Account::check(string login,string pass)
{
	int check=0;
	for (int i = 0; i < list.size(); i++)
	{
		check = (login==list[i].login_name) && (pass==list[i].password);
		if (check == 1)
			return check;
	}
	return check;
}
int Account::check_login(string login)
{
	int check = 0;
	for (int i = 0; i < list.size(); i++)
	{
		check = (login == list[i].login_name);
		if (check == 1)
			return check;
	}
	return check;
}
void Account::push_back(string login, string pass)
{
	user a;
	a.login_name = login;
	a.password = pass;
	list.push_back(a);
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
	delete[]buffer;
	return check;
}
int sign_up(SOCKET Client, Account list) {
	string pw, login;
	int size;
	char* buffer;
	do
	{
		//Nhan kich thuoc cua ten dang nhap
		recv(Client, (char*)&size, sizeof(int), 0);
		buffer = new char[size + 1];
		//Nhan ten dang nhap
		recv(Client, buffer, size, 0);
		buffer[size] = '\0';
		login = string(buffer);
		int check = list.check_login(login);
		send(Client, (char*)&check, sizeof(int), 0);
	} while (list.check_login(login) == 1);
	//Nhan kich thuoc mat khau
	recv(Client, (char*)&size, sizeof(int), 0);
	buffer = new char[size + 1];
	//Nhan mat khau
	recv(Client, buffer, size, 0);
	buffer[size] = '\0';
	pw = string(buffer);
	list.push_back(login, pw);
	delete[]buffer;

	return list.check_login(login);
}


bool send_list_file(SOCKET sock, vector<string> list) {
	string s;
	for (int i = 0; i < list.size(); i++) {
		s += to_string(i) + "   ";
		s += list[i];
		s += '\n';
	}
	int size = s.length() + 1;
	send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	send(sock, s.c_str(), size, 0);
	return true;
}