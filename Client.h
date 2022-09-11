#pragma once
#include "Link.h"

class Client
{
	string username;
public:
	void main_interface();
	void choice_interface();
	void signin();
	void withdraw();
	void deposit();
	void enquiry();
	void transfer();
	void signup();
	void logout();
};

