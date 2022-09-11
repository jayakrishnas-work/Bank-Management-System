#pragma once

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/exception.h>

#include <iostream>
#include <limits>

using namespace std;

class Link
{
	sql::Driver* driver = nullptr;
	sql::Connection* connection = nullptr;
	sql::Statement* statement = nullptr;
	const string address = "tcp://127.0.0.1:3306";
public:
	bool connect();
	void class_interface();
	sql::Connection* get_connection();
	sql::Statement* get_statement();
	~Link();
};

extern Link* link;