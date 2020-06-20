#include "Function.h"

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
