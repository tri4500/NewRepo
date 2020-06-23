#include "Menu.h"
#include"Function.h"
#include<stdio.h>
Menu::Menu()
{
    sock = INVALID_SOCKET;
}
Menu::Menu(SOCKET a)
{
    sock = a;
}
Menu::~Menu()
{
    delete[] list_file;
    closesocket(sock);
}


int Menu::start()
{
    cout << "1. Sign in\n";
    cout << "2. Sign up\n";
    cout << "3. Thoat\n";
    cout << "Moi ban chon: ";
    int i;
    cin >> i;
    send(sock, (char*)&i, sizeof(int), 0);
    if (i == 1)
    {
       
       return login(sock);
    }
    else if (i == 2)
    {
        
       return sign_up(sock);
    }
    else
        cout << "Tam biet";
    return 0;
}

void Menu::Print_list_file() {
    for (int i = 0; i < list_file_size; i++) {
        cout << list_file[i];
    }
}

int Menu::work() {
    this->list_file_size = get_list_file(sock, list_file);
    //system("clr");
    int index = 1;
    bool check;
    while (index != 0) {
        this->Print_list_file();
        cout << "\n\n\n\t Chon thao tac:" << endl;
        cout << "1. Download File" << endl;
        cout << "2. Upload File" << endl;
        cout << "3. Tai lai danh sach file" << endl;
        cout << "0. Thoat" << endl;
        cin >> index;
        send(this->sock, reinterpret_cast<char*>(&index), sizeof(index), 0);
        switch (index) {
        case 1:
            check = down_load(sock);
            if (check) {
                cout << "Download file thanh cong" << endl;
            }
            else {
                cout << "Download khong thanh cong" << endl;
            }
            cout << "Dang tai lai danh sach file va quay lai menu lam viec" << endl;
            index = 3;
            send(this->sock, reinterpret_cast<char*>(&index), sizeof(index), 0);
            this->list_file_size = get_list_file(sock, list_file);
            Sleep(2000);
            break;
        case 2:
            check = up_load(sock);
            if (check) {
                cout << "Upload file thanh cong" << endl;
            }
            else {
                cout << "Upload khong thanh cong" << endl;
            }
            cout << "Dang tai lai danh sach file va quay lai menu lam viec" << endl;
            index = 3;
            send(this->sock, reinterpret_cast<char*>(&index), sizeof(index), 0);
            this->list_file_size = get_list_file(sock, list_file);
            Sleep(2000);
            break;
        case 3:
            cout << "Dang tai lai danh sach file va cap nhat menu lam viec" << endl;
            this->list_file_size = get_list_file(sock, list_file);
            Sleep(2000);
            break;
        default:
            index = 0;
            break;
        }
    }
    return 0;
}
