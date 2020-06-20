#include "Account.h"

Account::Account()
{
	login_name = "admin";
	password = "123";
}

Account::~Account()
{
}

int Account::check(int sign,string a)
{
	if (sign == 1)
		return a == login_name;
	else
		return a == password;
}
