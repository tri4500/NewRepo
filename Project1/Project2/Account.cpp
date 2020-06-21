#include "Account.h"

Account::Account()
{
	login_name = "admin";
	password = "123";
}

Account::~Account()
{
}

int Account::check(string login,string pass)
{
	return login_name == login && password == pass;
}
