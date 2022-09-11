#include "Owner.h"

void Owner::main_interface()
{
	string choice;
	while (true)
	{
		cout << "\nChoose An Action\n\n'1' for SingIn\n'2' - Exit\n\nEnter here: ";

		// cin.ignore(numeric_limits<streamsize>::max(), '\n')
		getline(cin, choice);

		if (choice.length() != 1)
			cout << "\nInvalid Input - Try Again\n";
		else if (choice[0] == '1')
			signin();
		else if (choice[0] == '2')
			break;
		else
			cout << "\nInvalid Input - Try Again\n";
	}
}

void Owner::choice_interface()
{
	string choice;
	while (true)
	{
		cout << "\nChoose An Action\n\n'1' for Add Bank\n'2' for Add Manager\n'3' for Delete Bank\n'4' for Delete Manager\n'5' for Exit\n\nEnter here: ";

		// cin.ignore(numeric_limits<streamsize>::max(), '\n')
		getline(cin, choice);

		if (choice.length() != 1)
			cout << "\nInvalid Input - Try Again\n";
		else if (choice[0] == '1')
			add_bank();
		else if (choice[0] == '2')
			add_manager();
		else if (choice[0] == '3')
			delete_bank();
		else if (choice[0] == '4')
			delete_manager();
		else if (choice[0] == '5')
			break;
		else
			cout << "\nInvalid Input - Try Again\n";
	}
}

void Owner::create_database()
{
	auto statement = link->get_statement();

	statement->execute("CREATE DATABASE bank_management_system");

	statement->execute("USE bank_management_system");

	statement->execute("CREATE TABLE owner(id INT AUTO_INCREMENT, password VARCHAR(32) NOT NULL, PRIMARY KEY(id))");
	statement->execute("CREATE TABLE manager(id INT AUTO_INCREMENT, password VARCHAR(32) NOT NULL, first_name VARCHAR(32) NOT NULL, last_name VARCHAR(32) NOT NULL, PRIMARY KEY(id))");
	statement->execute("CREATE TABLE bank(id INT AUTO_INCREMENT, name VARCHAR(32) NOT NULL, city VARCHAR(32) NOT NULL, manager_id INT NOT NULL, PRIMARY KEY(id), FOREIGN KEY(manager_id) REFERENCES manager(id) ON UPDATE CASCADE)");
	// statement->execute("ALTER TABLE manager ADD COLUMN bank_id INT NOT NULL REFERENCES bank(id) ON UPDATE CASCADE");
	statement->execute("CREATE TABLE account(account_number INT AUTO_INCREMENT, username VARCHAR(32) NOT NULL UNIQUE, password VARCHAR(32) NOT NULL, balance INT NOT NULL DEFAULT 0, first_name VARCHAR(32) NOT NULL, last_name VARCHAR(32) NOT NULL, gender VARCHAR(1) NOT NULL, date_of_birth DATE NOT NULL, bank_id INT NOT NULL, PRIMARY KEY(account_number), FOREIGN KEY(bank_id) REFERENCES bank(id) ON UPDATE CASCADE)");
	statement->execute("CREATE TABLE request(username VARCHAR(32), password VARCHAR(32) NOT NULL, first_name VARCHAR(32) NOT NULL, last_name VARCHAR(32) NOT NULL, gender VARCHAR(1) NOT NULL, date_of_birth DATE NOT NULL, bank_id INT NOT NULL, PRIMARY KEY(username), FOREIGN KEY(bank_id) REFERENCES bank(id) ON UPDATE CASCADE ON DELETE CASCADE)");
}

void Owner::add_bank()
{
	auto connection = link->get_connection();
	auto statement = link->get_statement();
	sql::PreparedStatement* prepared = nullptr;
	sql::PreparedStatement* prepared2 = nullptr;
	sql::ResultSet* result = nullptr;

	int id;

	try
	{
		prepared = connection->prepareStatement("INSERT INTO bank(name, city, manager_id) VALUES(?, ?, ?)");
		prepared2 = connection->prepareStatement("SELECT id FROM manager WHERE id = ?");

		string bank_name, bank_city, _manager_id;
		int manager_id;

		cout << "\nEnter Bank Name: ";
		// cin.ignore(numeric_limits<streamsize>::max(), '\n')
		getline(cin, bank_name);
		cout << "Enter Bank City: ";
		// cin.ignore(numeric_limits<streamsize>::max(), '\n')
		getline(cin, bank_city);
		cout << "Enter Manager ID: ";
		// cin.ignore(numeric_limits<streamsize>::max(), '\n')
		getline(cin, _manager_id);

		manager_id = stoi(_manager_id);

		prepared2->setInt(1, manager_id);
		result = prepared2->executeQuery();

		if (!result->next())
		{
			delete prepared;
			delete prepared2;
			delete result;

			cout << "\nManager Does Not Exist - Try Again\n";
			return;
		}

		prepared->setString(1, bank_name);
		prepared->setString(2, bank_city);
		prepared->setInt(3, manager_id);

		prepared->execute();

		//delete result;
		//result = statement->executeQuery("SELECT id FROM bank ORDER BY id DESC LIMIT 1");
		//id = result->getInt("id");
	}
	catch (sql::SQLException _exception)
	{
		delete prepared;
		delete prepared2;
		delete result;

		cout << "\nException: " << _exception.what() << "\n";

		return;
	}
	catch (std::exception _exception)
	{
		delete prepared;
		delete prepared2;
		delete result;

		cout << "\nException: " << _exception.what() << "\n";

		return;
	}

	delete prepared;
	delete prepared2;
	delete result;

	cout << "\nBank Successfully Added\n";// - ID: " << id << "\n";
}

void Owner::add_manager()
{
	auto connection = link->get_connection();
	auto statement = link->get_statement();
	sql::PreparedStatement* prepared = nullptr;
	sql::ResultSet* result = nullptr;

	int id;

	try
	{
		sql::PreparedStatement* prepared = connection->prepareStatement("INSERT INTO manager(password, first_name, last_name) VALUES(?, ?, ?)");

		string password, first_name, last_name;

		cout << "\nEnter Manager First Name: ";
		// cin.ignore(numeric_limits<streamsize>::max(), '\n')
		getline(cin, first_name);
		cout << "Enter Manager Last Name: ";
		// cin.ignore(numeric_limits<streamsize>::max(), '\n')
		getline(cin, last_name);
		cout << "Create Password: ";
		// cin.ignore(numeric_limits<streamsize>::max(), '\n')
		getline(cin, password);

		prepared->setString(1, password);
		prepared->setString(2, first_name);
		prepared->setString(3, last_name);

		prepared->execute();

		//result = statement->executeQuery("SELECT id FROM manager ORDER BY id DESC LIMIT 1");
		//id = result->getInt("id");
	}
	catch (sql::SQLException _exception)
	{
		delete prepared;
		delete result;
		cout << "\nException: " << _exception.what() << "\n";
		return; // exit(1);
	}

	delete prepared;
	delete result;

	cout << "\nSuccessfully Added The Manager\n";// -ID: " << id << "\n";
}

void Owner::delete_bank()
{
	auto connection = link->get_connection();
	auto statement = link->get_statement();
	sql::PreparedStatement* prepared = nullptr, * prepared2 = nullptr, * prepared3 = nullptr, * prepared4 = nullptr;
	sql::ResultSet* result = nullptr;

	string _id, _id2;
	int id, id2;

	cout << "\nEnter The Bank ID That Needs To Be Deleted: ";
	// cin.ignore(numeric_limits<streamsize>::max(), '\n')
	getline(cin, _id);
	id = stoi(_id);

	try
	{
		prepared = connection->prepareStatement("DELETE FROM bank WHERE id = ?");
		prepared2 = connection->prepareStatement("SELECT username FROM account WHERE bank_id = ? UNION SELECT username FROM request WHERE bank_id = ?");
		prepared3 = connection->prepareStatement("UPDATE account SET bank_id = ? WHERE bank_id = ?");
		prepared4 = connection->prepareStatement("UPDATE request SET bank_id = ? WHERE bank_id = ?");

		prepared2->setInt(1, id);
		prepared2->setInt(2, id);
		result = prepared2->executeQuery();

		if (result->next())
		{
			cout << "\nEnter New Manager ID: ";
			// cin.ignore(numeric_limits<streamsize>::max(), '\n')
			getline(cin, _id2);
			id2 = stoi(_id2);

			prepared3->setInt(1, id2);
			prepared3->setInt(2, id);
			prepared3->executeUpdate();

			prepared4->setInt(1, id2);
			prepared4->setInt(2, id);
			prepared4->executeUpdate();
		}
		else
		{
			prepared->setInt(1, id);
			prepared->execute();
		}
	}
	catch (sql::SQLException _exception)
	{
		delete prepared;
		delete prepared2;
		delete prepared3;
		delete prepared4;
		delete result;

		cout << "\nException: " << _exception.what() << "\n";
		return; // exit(1);
	}
	catch (std::exception _exception)
	{
		delete prepared;
		delete prepared2;
		delete prepared3;
		delete prepared4;
		delete result;

		cout << "\nException: " << _exception.what() << "\n";
		return;
	}

	delete prepared;
	delete prepared2;
	delete prepared3;
	delete prepared4;
	delete result;

	cout << "\nSuccessfully Deleted The Bank - ID: " << id << "\n";
}

void Owner::delete_manager()
{
	auto connection = link->get_connection();
	auto statement = link->get_statement();
	sql::PreparedStatement* prepared = nullptr, * prepared2 = nullptr, * prepared3 = nullptr;
	sql::ResultSet* result = nullptr;

	string _id, _id2;
	int id, id2;

	cout << "\nEnter The Manager ID That Needs To Be Deleted: ";
	// cin.ignore(numeric_limits<streamsize>::max(), '\n')
	getline(cin, _id);
	id = stoi(_id);

	try
	{
		prepared = connection->prepareStatement("DELETE FROM manager WHERE id = ?");
		prepared2 = connection->prepareStatement("SELECT id FROM bank WHERE manager_id = ?");
		prepared3 = connection->prepareStatement("UPDATE bank SET manager_id = ? WHERE manager_id = ?");

		prepared2->setInt(1, id);
		result = prepared2->executeQuery();

		if (result->next())
		{
			cout << "\nEnter New Manager ID: ";
			// cin.ignore(numeric_limits<streamsize>::max(), '\n')
			getline(cin, _id2);
			id2 = stoi(_id2);

			prepared3->setInt(1, id2);
			prepared3->setInt(2, id);
			prepared3->executeUpdate();
		}

		prepared->setInt(1, id);
		prepared->execute();
	}
	catch (sql::SQLException _exception)
	{
		delete prepared;
		delete prepared2;
		delete prepared3;
		delete result;

		cout << "\nException: " << _exception.what() << "\n";
		return; // exit(1);
	}
	catch (std::exception _exception)
	{
		delete prepared;
		delete prepared2;
		delete prepared3;
		delete result;

		cout << "\nException: " << _exception.what() << "\n";
		return;
	}

	delete prepared;
	delete prepared2;
	delete prepared3;
	delete result;

	cout << "\nSuccessfully Deleted The Manager - ID: " << id << "\n";
}

void Owner::signin()
{
	auto connection = link->get_connection();
	auto statement = link->get_statement();
	sql::PreparedStatement* prepared = nullptr;
	sql::ResultSet* result = nullptr;

	string _id, password;
	int id;

	try
	{
		prepared = connection->prepareStatement("SELECT id FROM owner WHERE id = ? AND password = ?");

		cout << "\nEnter Owner ID: ";
		// cin.ignore(numeric_limits<streamsize>::max(), '\n')
		getline(cin, _id);
		cout << "Enter Owner Password: ";
		// cin.ignore(numeric_limits<streamsize>::max(), '\n')
		getline(cin, password);

		id = stoi(_id);

		prepared->setInt(1, id);
		prepared->setString(2, password);

		result = prepared->executeQuery();

		if (!result->next())
		{
			cout << "\nInvalid ID/Password - Try Again\n";

			delete prepared;
			delete result;
			return;
		}
	}
	catch (sql::SQLException exception)
	{
		delete prepared;
		delete result;

		cout << "\nException: " << exception.what() << "\n";
		return;
	}
	catch (std::exception _exception)
	{
		delete prepared;
		delete result;

		cout << "\nException: " << _exception.what() << "\n";
		return;
	}

	delete prepared;
	delete result;

	choice_interface();
}