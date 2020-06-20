﻿#include "Function.h"

std::string passwordInput(unsigned maxLength)
{
	std::string pw;
	for (char c; (c = _getch()); )
	{
		if (c == '\n' || c == '\r') { // enter keyboard
			std::cout << "\n";
			break;
		}
		else if (c == '\b') { // backspace keyboard
			if (!pw.empty())
				std::cout << "\b \b";
			if (!pw.empty()) pw.erase(pw.size() - 1);
		}
		else if (isprint(c) && pw.size() < maxLength) { //isprint la chi nhan nhung ky tu in ra đuoc (co tinh khoang trang)
			std::cout << '*';
			pw += c;
		}
	}
	return pw;
}

string create_password()
{
	string pw;
	string confirm;
	do {
		cout << "Moi ban nhap mat khau(Toi da 5 ky tu): ";
		pw = passwordInput(6);
		cout << "Moi ban xac nhan lai: ";
		confirm = passwordInput(6);
		if (pw.compare(confirm) != 0)
			cout << "Moi ban nhap lai: \n";
	} while (pw.compare(confirm) != 0);
	return pw;
}
int login(SOCKET sock) {
	string login, pw;
	int sign;
	do{
		cout << "Login: ";
		cin >> login;
		// Gui do dai ten dang nhap
		send(sock, (char*)login.length(), sizeof(int), 0);
		// Gui ten dang nhap
		send(sock, login.c_str(), sizeof(login), 0);
		//Nhan lai xac nhan cua server
		recv(sock, (char*)&sign, sizeof(int), 0);
		if (sign == 0)
		{
			cout << "Ten dang nhap cua ban khong dung\n";
			cout << "1. Nhap lai\n";
			cout << "2. Thoat\n";
			int choice;
			cin >> choice;
			if (choice == 2)
				return -1;
		}
	} while (sign == 0);
	do {
		cout << "Password: ";
		pw = passwordInput(21);
		// Gui do dai mat khau
		send(sock, (char*)login.length(), sizeof(int), 0);
		// Gui mat khau
		send(sock, pw.c_str(), sizeof(pw), 0);
		//Nhan lai xac nhan cua server
		recv(sock, (char*)&sign, sizeof(int), 0);
		if (sign == 0)
		{
			cout << "Mat khau cua ban khong dung\n";
			cout << "Nhap lai\n";
		}

	} while (sign == 0);
	cout << "Ban dang nhap thanh cong";
}