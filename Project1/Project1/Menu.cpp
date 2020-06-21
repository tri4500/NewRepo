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
