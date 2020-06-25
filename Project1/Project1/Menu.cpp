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
    printf("%s", list_file);
}

int Menu::Get_List_file() {
    this->list_file_size = get_list_file(sock, list_file);
    if (this->list_file_size == -1)
        return -1;
    //system("clr");
    return 0;
}

int Menu::work(int index) {
    /*this->list_file_size = get_list_file(sock, list_file);
    //system("clr");*/
    int check;
    int iResult;

    iResult=send(this->sock, reinterpret_cast<char*>(&index), sizeof(index), 0);
    if (iResult == SOCKET_ERROR) {
        cout << "\nDisconnected to server\nKet thuc chuong trinh";
        Sleep(2000);
        return -1;
    }
    switch (index) {
    case 1:
        check = down_load(sock);
        if (check==1) {
            cout << "\nDownload file thanh cong" << endl;
        }
        else if(check==0){
            cout << "\nDownload khong thanh cong" << endl;
        }
        else
        {
            return -1;
        }
        cout << "Dang tai lai danh sach file va quay lai menu lam viec" << endl;
        index = 3;
        iResult=send(this->sock, reinterpret_cast<char*>(&index), sizeof(index), 0);
        if (iResult == SOCKET_ERROR) {
            cout << "\nDisconnected to server\nKet thuc chuong trinh";
            Sleep(2000);
            return -1;
        }  
        this->list_file_size = get_list_file(sock, list_file);
        if (this->list_file_size == -1)
            return -1;
        Sleep(2000);
        return 1;
        break;
    case 2:
        check = up_load(sock);
        if (check == 1) {
            cout << "Upload file thanh cong" << endl;
        }
        else if (check == 0) {
            cout << "Upload khong thanh cong" << endl;
        }
        else
            return -1;
        cout << "Dang tai lai danh sach file va quay lai menu lam viec" << endl;
        index = 3;
        iResult=send(this->sock, reinterpret_cast<char*>(&index), sizeof(index), 0);
        if (iResult == SOCKET_ERROR) {
            cout << "\nDisconnected to server\nKet thuc chuong trinh";
            Sleep(2000);
            return -1;
        }
        this->list_file_size = get_list_file(sock, list_file);
        if (this->list_file_size == -1)
            return -1;
        Sleep(2000);
        return 1;
        break;
    case 3:
        cout << "Dang tai lai danh sach file va cap nhat menu lam viec" << endl;
        this->list_file_size = get_list_file(sock, list_file);
        if (this->list_file_size == -1)
            return -1;   
        Sleep(2000);
        return 1;
        break;
    default:
        index = 0;
        break;
    }
    return 0;
}
