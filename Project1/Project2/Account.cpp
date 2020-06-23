#include "Account.h"

Account::Account()
{
	ifstream file;
	user temp;
	temp.is_login = false;
	file.open("Server\\listaccount.txt", std::ofstream::in);
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
	file.open("Server\\listaccount.txt", std::fstream::app);
	file << list.back().login_name << " " << list.back().password << endl;
	file.close();
	mtx.unlock();
}
//logount
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



void send_all(vector<SOCKET>& list, int sign,string name)
{
	char buffer[200];
	if (sign == 1)
	{
		sprintf_s(buffer, "%s login\n", name);
	}
	else
		sprintf_s(buffer, "%s dang xuat\n", name);

	for (int i = 0; i < list.size() - 1; i++)
	{
		send(list[i], buffer, 200, 0);
	}

}

file* Create_List_file() {
	file* result = new file;
	file* temp = result;
	fstream f;
	f.open("Server\\list_file.txt", ios::in | ios::binary);
	if (!f.eof()) {
		string s;
		getline(f, s);
		s.pop_back();
		if (s.length() > 0) {
			result->name = s;
			result->count = 0;
			result->next = NULL;
		}
	}
	while (!f.eof()) {
		string s;
		getline(f, s);
		s.pop_back();
		if (s.length() > 0) {
			file* temp1 = new file;
			temp1->name = s;
			temp1->count = 0;
			temp1->next = NULL;
			temp->next = temp1;
			temp = temp1;
		}
	}
	f.close();
	return result;
}

bool send_list_file(SOCKET sock, file* list,string name) {
	string s;
	file* temp = list;
	int i = 1;
	while (temp) {
		s += to_string(i) + "   ";
		i++;
		s += temp->name;
		s += '\n';
		temp = temp->next;
	}
	cout << "Nguoi dung: " << name << "dang nhan list file" << endl;
	int size = s.length() + 1;
	send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	send(sock, s.c_str(), size, 0);
	return true;
}
//123
bool up_load(SOCKET sock, file* list,string name) {
	int size;
	recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	string path;
	recv(sock, reinterpret_cast<char*>(&path), size, 0);
	bool check = true;
	file* temp = list;
	while (temp) {
		if (temp->name.compare(path) == 0) {
			check = false;
			break;
		}
	}
	if (check) {
		fstream des;
		des.open(path, ios::trunc | ios::out | ios::binary);
		cout << "nguoi dung: " << name << " dang Upload file: " << path << endl;
		recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		char* buffer;
		buffer = new char[size];
		recv(sock, buffer, size, 0);
		des.write(buffer, size);
		delete[] buffer;
		des.close();
		file* new_file = new file;
		new_file->name = path;
		new_file->count = 0;
		new_file->next = NULL;
		temp = list;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = new_file;
		return true;
	}
	else {
		temp->mutex_access.lock();
		cout << "nguoi dung: " << name << " dang cap nhat file: " << path << endl;
		fstream des;
		des.open(path, ios::trunc | ios::out | ios::binary);
		recv(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		char* buffer;
		buffer = new char[size];
		recv(sock, buffer, size, 0);
		des.write(buffer, size);
		des.close();
		temp->mutex_access.unlock();
		return true;
	}
	return true;
}

bool down_load(SOCKET sock, file* list,string name) {
	int pos;
	bool val_return = true;
	recv(sock, reinterpret_cast<char*>(&pos), sizeof(pos), 0);
	file* temp = list;
	for (int i = 1; i < pos; i++) {
		temp = temp->next;
	}
	temp->mutex_val.lock();
	temp->count++;
	if (temp->count == 1) {
		temp->mutex_access.lock();
	}
	temp->mutex_val.unlock();
	fstream src;
	src.open("Server\\"+temp->name, ios::in | ios::binary);
	if (!src) {
		val_return = false;
		int size = -1;
		send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	}
	else {
		cout << "Nguoi dung: " << name << " dang dowload file: " << temp->name << endl;
		int size;
		src.seekg(0, ios::end);
		size = src.tellg();
		src.seekg(0, ios::beg);
		send(sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		char* buffer = new char[size];
		src.read(buffer, size);
		send(sock, buffer, size, 0);
	}
	temp->mutex_val.lock();
	temp->count--;
	if (temp->count == 0) {
		temp->mutex_access.unlock();
	}
	temp->mutex_val.unlock();
	return val_return;
}

void erase_socket(vector<SOCKET> list,SOCKET a,mutex& mtx)
{
	mtx.lock();
	for (int i = 0; i < list.size(); i++)
	{
		if (a == list[i])
		{
			list.erase(list.begin()+i);
			break;
		}
	}
	mtx.unlock();
}
