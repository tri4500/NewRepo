﻿#include "Function.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
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

int login(SOCKET &sock) {
	string login, pw;
	int sign;
	int len1, len2;
	int iResult;
	cout << "Login: ";
	cin >> login;
	cout << "Password: ";
	pw = passwordInput(21);
	len1 = (int)login.length();
	len2 = (int)pw.length();
	// Gui kich thuoc cua ten dang nhap
	iResult=send(sock, (char*)&len1, sizeof(int), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		exit(1);
	}
	//Gui ten dang nhap
	iResult=send(sock, login.c_str(), len1, 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		exit(1);
	}
	//Gui kich thuoc mat khau
	iResult=send(sock, (char*)&len2, sizeof(int), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		exit(1);
	}
	//Gui mat khau
	iResult=send(sock, pw.c_str(), len2, 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		exit(1);
	}
	//Nhan tin nhan kiem tra cua server
	iResult=recv(sock, (char*)&sign, sizeof(int), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		exit(1);
	}
	cout << sign;
	if (sign == 1)
		cout << "Ban dang nhap thanh cong\n";
	else if (sign == -1)
		cout << "Tai khoan nay hien dang co nguoi dung\n";
	else
		cout << "Dang nhap that bai\n";
	return sign;
}
int sign_up(SOCKET sock) {
	string login, pw;
	int sign;
	int len1, len2;
	int iResult;
	do
	{
		cout << "Login: ";
		cin >> login;
		len1 = (int)login.length();		
		// Gui kich thuoc cua ten dang nhap
		iResult=send(sock, (char*)&len1, sizeof(int), 0);
		if (iResult == SOCKET_ERROR) {
			cout << "\nDisconnected to server\nKet thuc chuong trinh";
			Sleep(2000);
			exit(1);
		}
		//Gui ten dang nhap
		iResult=send(sock, login.c_str(), len1, 0);
		if (iResult == SOCKET_ERROR) {
			cout << "\nDisconnected to server\nKet thuc chuong trinh";
			Sleep(2000);
			exit(1);
		}
		//Nhan tin nhan kiem tra cua server
		iResult=recv(sock, (char*)&sign, sizeof(int), 0);
		if (iResult == SOCKET_ERROR) {
			cout << "\nDisconnected to server\nKet thuc chuong trinh";
			Sleep(2000);
			exit(1);
		}
		if (sign == 1)
			cout << "Xin loi ten da co nguoi su dung\n";
		else
			cout << "Ban co the su dung ten dang nhap nay\n";
	} while (sign == 1);


	string confirm;
	do {
		cout << "Password: ";
		pw = passwordInput(21);
		cout << "Moi ban xac nhan lai: ";
		confirm = passwordInput(21);
		if (pw.compare(confirm) != 0)
			cout << "Mat khau khong khop, moi ban nhap lai\n";
	} while (pw.compare(confirm) != 0);
	len2 = (int)pw.length();
	//Gui kich thuoc mat khau
	iResult=send(sock, (char*)&len2, sizeof(int), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		exit(1);
	}
	//Gui mat khau
	iResult=send(sock, pw.c_str(), len2, 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		exit(1);
	}
	cout << "Dang ki thanh cong\n";
	sign = (sign == 0) ? 1 : 0;
	Sleep(2000);
	return sign;
}

int up_load(SOCKET sock) {
	cout << "Nhap ten file: ";
	int iResult;
	string path;
		cin >> path;
	fstream src;
	src.open(path, ios::in | ios::binary);
	if (!src) {
		cout << "Khong mo duoc file";
		src.close();
		return 0;
	}
	cout << "mo file thanh cong";
	int size;
	size = path.length();
	iResult=send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		src.close();
		return -1;
	}
	iResult=send(sock, path.c_str(), size, 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		src.close();
		return -1;
	}
	src.seekg(0, ios::end);
	size = src.tellg();
	src.seekg(0, ios::beg);
	iResult=send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		src.close();
		return -1;
	}
	char* buffer;
	buffer = new char[size];
	src.read(buffer, size);
	iResult=send(sock, buffer, size, 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		delete[] buffer;
		src.close();
		return -1;
	}
	delete[] buffer;
	cout << "Upload file thanh cong" << endl;
	src.close();
	return 1;
}


int down_load(SOCKET sock) {
	cout << "Nhap vi tri file: ";
	int pos;
	cin >> pos;
	int iResult;
	iResult=send(sock, reinterpret_cast<char*>(&pos), sizeof(pos), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		return -1;
	}
	string path;
	cout << "Nhap ten file khi tai xuong:";
	cin >> path;
	fstream des;
	des.open(path, ios::out | ios::trunc | ios::binary);
	if (!des) {
		cout << "Khong mo duoc file output";
		des.close();
		return 0;
	}
	int size;
	iResult=recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		des.close();
		Sleep(2000);
		return -1;
	}
	if (size < 0) {
		des.close();
		cout << "Loi tu server" << endl;
		return 0;
	}
	char* buffer;
	buffer = new char[size];
	iResult=recv(sock, buffer, size, 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		delete[]buffer;
		des.close();
		return -1;
	}
	des.write(buffer, size);
	delete[] buffer;
	cout << "Download file thanh cong" << endl;
	des.close();
	return 1;
}

int get_list_file(SOCKET sock, char*& buffer) {
	int size;
	delete[] buffer;
	int iResult;
	iResult=recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		return -1;
	}
	buffer = new char[size+1];
	iResult=recv(sock, buffer, size, 0);
	if (iResult == SOCKET_ERROR) {
		cout << "\nDisconnected to server\nKet thuc chuong trinh";
		Sleep(2000);
		delete[]buffer;
		return -1;
	}
	buffer[size] = '\0';
	return size;
}
