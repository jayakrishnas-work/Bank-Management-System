#include "Link.h"
#include "Client.h"
#include "Manager.h"
#include "Owner.h"

bool Link::connect()
{
	string username, password;

	cout << "Connecting To The Server\n\n";
	// cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "\nEnter DBMS username: ";
	getline(cin, username);
	// cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "Enter DBMS password: ";
	getline(cin, password);

	try
	{
		driver = get_driver_instance();
		connection = driver->connect(address, username, password);
	}
	catch (sql::SQLException exception)
	{
		cout << "\nException: " << exception.what() << "\n";
		return false;
	}

	statement = connection->createStatement();
	connection->setSchema("information_schema");

	sql::ResultSet* result = statement->executeQuery("SELECT * FROM schemata WHERE schema_name = \"bank_management_system\"");

	if (!result->next())
	{
		Owner owner;
		owner.create_database();

		cout << "\nServer Is Under Maintainance - Sorry For The Inconvinience\n";

		delete result;
		return false;
	}

	connection->setSchema("bank_management_system");
	cout << "\n\nConnected To The Server\n\n";

	delete result;
	return true;
}

void Link::class_interface()
{
	string user;

	while (true)
	{
		cout << "\nSelect Your Class\n\n'C' for Client\n'M' for Manager\n'O' for Owner\n'E' for Exit\n\nEnter Here: ";

		// cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, user);

		if (user.length() != 1)
			cout << "\nInvalid Input - Try Again\n";
		else if (user[0] == 'C' or user[0] == 'c')
		{
			Client client;
			client.main_interface();
		}
		else if (user[0] == 'M' or user[0] == 'm')
		{
			Manager manager;
			manager.main_interface();
		}
		else if (user[0] == 'O' or user[0] == 'o')
		{
			Owner owner;
			owner.main_interface();
		}
		else if (user[0] == 'E' or user[0] == 'e')
			break;
		else
			cout << "\nInvalid Input - Try Again\n";
	}
}

sql::Connection* Link::get_connection()
{
	return connection;
}

sql::Statement* Link::get_statement()
{
	return statement;
}

Link::~Link()
{
	delete connection;
	delete statement;

	cout << "\n\nDisconnected From The Server";
}

Link* link = new Link();