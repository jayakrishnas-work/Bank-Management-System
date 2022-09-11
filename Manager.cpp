#include "Manager.h"

// have main interface to exit or login

void Manager::main_interface()
{
	string choice;
	while (true)
	{
		cout << "\nChoose An Action\n\n'1' for SingIn\n'2' - Exit\n\nEnter here: ";

		// cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

void Manager::choice_interface()
{
	string choice;
	while (true)
	{
		cout << "\nSelect Your Choice\n\n'1' for Validate Clients\n'2' for Remove Clients\n'3' for Exit\n\nEnter Here: ";
		// cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, choice);

		if (choice.length() != 1)
			cout << "\nInvalid Input - Try Again\n";
		else if (choice[0] == '1')
			validate_client();
		else if (choice[0] == '2')
			remove_client();
		else if (choice[0] == '3')
			break;
		else
			cout << "\nInvalid Input - Try Again\n";
	}
}

void Manager::signin()
{
	auto connection = link->get_connection();
	auto statement = link->get_statement();
	sql::PreparedStatement* prepared = nullptr;
	sql::ResultSet* result = nullptr;

	string _id, password;
	int id;

	try
	{
		prepared = connection->prepareStatement("SELECT id FROM manager WHERE id = ? AND password = ?");

		cout << "\nEnter Manager ID: ";
		// cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, _id);
		cout << "Enter Manager Password: ";
		// cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, password);

		id = stoi(_id);

		prepared->setInt(1, id);
		prepared->setString(2, password);
		result = prepared->executeQuery();

		if (!result->next())
		{
			delete prepared;
			delete result;

			cout << "\nInvalid ID/Username - Try Again\n";
			return;
		}
	}
	catch (sql::SQLException _exception)
	{
		delete prepared;
		delete result;

		cout << "\nException: " << _exception.what() << "\n";
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

void Manager::validate_client()
{
	auto connection = link->get_connection();
	auto statement = link->get_statement();
	sql::ResultSet* result = nullptr;

	string username;

	try
	{
		result = statement->executeQuery("SELECT * FROM request");

		while (result->next())
		{
			username = result->getString("username");
			cout << "\nFirst Name: " << result->getString("first_name") << "\n";
			cout << "Last Name: " << result->getString("last_name") << "\n";
			cout << "Gender: " << result->getString("gender") << "\n";
			cout << "Date Of Birth: " << result->getString("date_of_birth") << "\n";
			cout << "Bank ID: " << result->getInt("bank_id") << "\n";
			cout << "Username: " << username << "\n";

			string choice;

			while (true)
			{
				cout << "\nSelect Your Choice\n\n'1' for Admit\n'2' for Remove\n'3' for Move Next\n'4' Exit\n\nEnter Here: ";
				// cin.ignore(numeric_limits<streamsize>::max(), '\n');
				getline(cin, choice);

				if (choice.length() != 1)
					cout << "\nInvalid Input - Try Again\n";
				else if (choice[0] == '1')
				{
					statement->execute("INSERT INTO account(username, password, first_name, last_name, gender, date_of_birth, bank_id) SELECT * FROM request");
					statement->execute("DELETE FROM request WHERE username = '" + username + "'");
				}
				else if (choice[0] == '2')
					statement->execute("DELETE FROM request WHERE username = '" + username + "'");
				else if (choice[0] == '3')
					break;
				else if (choice[0] == '4')
				{
					delete result;
					return;
				}
				else
					cout << "\nInvalid Input - Try Again\n";
			}
		}
		if (!result)
			cout << "\nAll Users Are Handled\n";
	}
	catch (sql::SQLException _exception)
	{
		delete result;
		cout << "\nException: " << _exception.what() << "\n";
		return;
	}

	delete result;
	cout << "\nSuccessfully Added/Removed\n";
}

void Manager::remove_client()
{
	auto connection = link->get_connection();
	auto statement = link->get_statement();
	sql::ResultSet* result = nullptr;

	string _accno;
	int account_number;

	try
	{
		cout << "\nEnter Account Number: ";
		// cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, _accno);

		account_number = stoi(_accno);

		result = statement->executeQuery("SELECT * FROM account WHERE account_number = " + to_string(account_number));
		if (result->next())
		{
			cout << "\nFirst Name: " << result->getString("first_name") << "\n";
			cout << "Last Name: " << result->getString("last_name") << "\n";
			cout << "Gender: " << result->getString("gender") << "\n";
			cout << "Date Of Birth: " << result->getString("date_of_birth") << "\n";
			cout << "Bank ID: " << result->getInt("bank_id") << "\n";
			cout << "Username: " << result->getString("username") << "\n";
			while (true)
			{
				string choice;
				cout << "\nSelect Your Choice\n\n'1' for Remove\n'2' for Exit\n\nEnter Here: ";
				getline(cin, choice);
				if (choice.length() != 1)
					cout << "\nInvalid Choice - Try Again\n";
				else if (choice[0] == '1')
					break;
				else if (choice[0] == '2')
					return;
				else
					cout << "\nInvalid Choice - Try Again\n";
			}
		}
		else
		{
			delete result;
			cout << "There Is No Account With Account Number - " << account_number << " - Try Again\n";
			return;
		}
	}
	catch (sql::SQLException _exception)
	{
		delete result;
		cout << "\nException: " << _exception.what() << "\n";
		return;
	}
	catch (std::exception _exception)
	{
		delete result;
		cout << "\nException: " << _exception.what() << "\n";
		return;
	}

	statement->execute("DELETE FROM account WHERE account_number = " + to_string(account_number));
	delete result;
	cout << "\nSuccessfully Deleted\n";
}