#include "Menu.h"
#include<stdio.h>
Menu::Menu()
{

}
int Menu::login()
{
    cout << "1. Sign in\n";
    cout << "2. Sign up\n";
    cout << "3. Thoat\n";
    cout << "Moi ban chon: ";
    int i;
    cin >> i;
    return i;
}
