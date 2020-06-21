#include "Function.h"
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
int login(SOCKET &sock) {
	string login, pw;
	int sign;
	int len1, len2;
	cout << "Login: ";
	cin >> login;
	cout << "Password: ";
	pw = passwordInput(21);
	len1 = (int)login.length();
	len2 = (int)pw.length();
	// Gui kich thuoc cua ten dang nhap
	send(sock, (char*)&len1, sizeof(int), 0);
	//Gui ten dang nhap
	send(sock, login.c_str(), len1, 0);
	//Gui kich thuoc mat khau
	send(sock, (char*)&len2, sizeof(int), 0);
	//Gui mat khau
	send(sock, pw.c_str(), len2, 0);
	//Nhan tin nhan kiem tra cua server
	recv(sock, (char*)&sign, sizeof(int), 0);
	if (sign == 1)
		cout << "Ban dang nhap thanh cong";
	else
		cout << "Dang nhap that bai";
	return sign;
}
int sign_up(SOCKET sock) {
	string login, pw;
	int sign;
	int len1, len2;
	do
	{
		cout << "Login: ";
		cin >> login;
		len1 = (int)login.length();		
		// Gui kich thuoc cua ten dang nhap
		send(sock, (char*)&len1, sizeof(int), 0);
		//Gui ten dang nhap
		send(sock, login.c_str(), len1, 0);
		//Nhan tin nhan kiem tra cua server
		recv(sock, (char*)&sign, sizeof(int), 0);
		if (sign == 1)
			cout << "Xin loi ten da co nguoi su dung";
		else
			cout << "Ban co the su dung ten dang nhap nay";
	} while (sign == 1);


	string confirm;
	do {
		cout << "Password: ";
		pw = passwordInput(6);
		cout << "Moi ban xac nhan lai: ";
		confirm = passwordInput(6);
		if (pw.compare(confirm) != 0)
			cout << "Mat khau khong khop, moi ban nhap lai\n";
	} while (pw.compare(confirm) != 0);
	len2 = (int)pw.length();
	//Gui kich thuoc mat khau
	send(sock, (char*)&len2, sizeof(int), 0);
	//Gui mat khau
	send(sock, pw.c_str(), len2, 0);
	cout << "Dang ki thanh cong";
	Sleep(2000);
	return sign;
}

bool up_load(SOCKET sock) {
	cout << "Nhap ten file: ";
	string path;
	cin >> path;
	fstream src;
	src.open(path, ios::in | ios::binary);
	if (!src) {
		cout << "Khong mo duoc file";
		src.close();
		return false;
	}
	cout << "mo file thanh cong";
	src.seekg(0, ios::end);
	int size = src.tellg();
	src.seekg(0, ios::beg);
	send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	char* buffer;
	while (size > 0) {
		int temp = (size > 512 * 8) ? 512 * 8 : size;
		size -= 512 * 8;
		buffer = new char[temp];
		src.read(buffer, temp);
		send(sock, buffer, temp, 0);
		delete[] buffer;
	}
	cout << "Upload file thanh cong" << endl;
	src.close();
	return true;
}


bool down_load(SOCKET sock) {
	cout << "Nhap vi tri file: ";
	int pos;
	cin >> pos;
	send(sock, reinterpret_cast<char*>(&pos), sizeof(pos), 0);
	string path;
	cout << "Nhap ten file khi tai xuong:";
	cin >> path;
	fstream des;
	des.open(path, ios::trunc | ios::binary);
	if (!des) {
		cout << "Khong mo duoc file";
		des.close();
		return false;
	}
	cout << "mo file thanh cong";
	int size;
	recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	char* buffer;
	while (size > 0) {
		int temp = (size > 512 * 8) ? 512 * 8 : size;
		size -= 512 * 8;
		buffer = new char[temp];
		recv(sock, buffer, temp, 0);
		des.write(buffer, temp);
		delete[] buffer;
	}
	cout << "Download file thanh cong" << endl;
	des.close();
	return true;
}

bool get_list_file(SOCKET sock, char* buffer) {
	delete[] buffer;
	int size;
	recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	buffer = new char[size];
	recv(sock, buffer, size, 0);
	return true;
}


