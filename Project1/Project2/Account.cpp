#include "Account.h"

Account::Account()
{
	ifstream file;
	user temp;
	temp.is_login = false;
	file.open("listaccount.txt", std::ofstream::in);
	while (!file.eof()) {
		getline(file, temp.login_name, ' ');
		getline(file, temp.password);
		list.push_back(temp);
	}
	file.close();
}

Account::~Account()
{
}
void Account::save_list_user_file()
{
	mtx.lock();
	fstream file;
	file.open("listaccount.txt", std::fstream::app);
	file << list.back().login_name << " " << list.back().password << endl;
	file.close();
	mtx.unlock();
}
void Account::log_out(string name)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (name == list[i].login_name)
		{
			list[i].is_login = false;
			return;
		}
	}
}
int Account::check(string login,string pass)
{
	int check=0;
	for (int i = 0; i < list.size(); i++)
	{
		check = (login == list[i].login_name) && (pass == list[i].password);
		if (check == 1)
		{
			if (list[i].is_login == true)
				return -1;
			else {
				list[i].is_login = true;
				return check;
			}
		}
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
int login(SOCKET &Client,Account &a,string &name)
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
		name = login;
	}
	else
	{
		cout << "\nDang nhap that bai\n";
	}
	delete[]buffer;
	return check;
}
int sign_up(SOCKET& Client, Account &list) {
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



void send_all(vector<SOCKET>& list)
{
	for (int i = 0; i < list.size() - 1; i++)
	{
		send(list[i], "new one", 200, 0);
	}
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

//bool up_load(SOCKET sock, vector<file> list) {
//	int size;
//	recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
//	string path;
//	recv(sock, reinterpret_cast<char*>(&path), size, 0);
//	bool check = true;
//	int i;
//	for (i = 0; i < list.size(); i++) {
//		if (path.compare(list[i].name) == 0) {
//			check = false;
//			break;
//		}
//	}
//	if (check) {
//		file temp;
//		temp.name = path;
//		temp.count = 0;
//		fstream des;
//		des.open(path, ios::trunc | ios::out | ios::binary);
//		recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
//		char* buffer;
//		while (size > 0) {
//			int temp = (size > 512 * 8) ? 512 * 8 : size;
//			size -= 512 * 8;
//			buffer = new char[temp];
//			recv(sock, buffer, temp, 0);
//			des.write(buffer, temp);
//			delete[] buffer;
//		}
//		des.close();
//		list.push_back(temp);
//	}
//	else {
//		list[i].mutex_access.lock();
//		fstream des;
//		des.open(path, ios::trunc | ios::out | ios::binary);
//		recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
//		char* buffer;
//		while (size > 0) {
//			int temp = (size > 512 * 8) ? 512 * 8 : size;
//			size -= 512 * 8;
//			buffer = new char[temp];
//			recv(sock, buffer, temp, 0);
//			des.write(buffer, temp);
//			delete[] buffer;
//		}
//		des.close();
//		list[i].mutex_access.unlock();
//	}
//	return true;
//}

//bool load_list_file(vector<file>& v) {
//	fstream f;
//	f.open("list_file.txt", ios::in | ios::binary);
//	if (!f) {
//		cout << "khong mo dc file";
//		return false;
//	}
//	//v.clear();
//	while (!f.eof()) {
//		file temp;
//		getline(f, temp.name);
//		temp.count = 0;
//		v.push_back(temp);
//	}
//	v.pop_back();
//	return true;
//}

//bool down_load(SOCKET sock, vector<file>& list) {
//	int pos;
//	bool val_return = true;
//	recv(sock, reinterpret_cast<char*>(&pos), sizeof(pos), 0);
//	list[pos].mutex_val.lock();
//	list[pos].count++;
//	if (list[pos].count == 1) {
//		list[pos].mutex_access.lock();
//	}
//	list[pos].mutex_val.unlock();
//	fstream src;
//	src.open(list[pos].name, ios::in | ios::binary);
//	if (!src) {
//		val_return = false;
//	}
//	else {
//		int size;
//		src.seekg(0, ios::end);
//		size = src.tellg();
//		src.seekg(0, ios::beg);
//		send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
//		while (size > 0) {
//			int temp = (size > 512 * 8) ? 512 * 8 : size;
//			size -= 512 * 8;
//			char* buffer = new char[temp];
//			src.read(buffer, temp);
//			send(sock, buffer, temp, 0);
//			delete[] buffer;
//		}
//	}
//	list[pos].mutex_val.lock();
//	list[pos].count--;
//	if (list[pos].count == 0) {
//		list[pos].mutex_access.unlock();
//	}
//	list[pos].mutex_val.unlock();
//	return val_return;
//}