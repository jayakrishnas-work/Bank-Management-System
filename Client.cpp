#include "Client.h"

void Client::main_interface()
{
    string choice;
    while (true)
    {
        cout << "\nChoose An Action\n\n'1' for SingIn\n'2' for SignUp\n'3' - Exit\n\nEnter here: ";

        // cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, choice);
        
        if (choice.length() != 1)
            cout << "\nInvalid Input - Try Again\n";
        else if (choice[0] == '1')
            signin();
        else if (choice[0] == '2')
            signup();
        else if (choice[0] == '3')
            break;
        else
            cout << "\nInvalid Input - Try Again\n";
    }
}

void Client::choice_interface()
{
    string choice;
    while (true)
    {
        cout << "\nChoose An Action\n\n'1' for Withdraw\n'2' for Deposit\n'3' for Transfer\n'4' for Show Balance\n'5' for Exit\n\nEnter here: ";

        // cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, choice);

        if (choice.length() != 1)
            cout << "\nInvalid Input - Try Again\n";
        else if (choice[0] == '1')
            withdraw();
        else if (choice[0] == '2')
            deposit();
        else if (choice[0] == '3')
            transfer();
        else if (choice[0] == '4')
            enquiry();
        else if (choice[0] == '5')
            break;
        else
            cout << "\nInvalid Input - Try Again\n";
    }
}

void Client::signin()
{
    auto connection = link->get_connection();
    auto statement = link->get_statement();
    sql::PreparedStatement* prepared = nullptr;
    sql::ResultSet* result = nullptr;

    string password;

    cout << "\nEnter Your Bank Username: ";
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, this->username);
    cout << "Enter Your Bank Password: ";
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, password);

    try
    {
        prepared = connection->prepareStatement("SELECT * FROM account WHERE username = ? AND password = ?");
        prepared->setString(1, this->username);
        prepared->setString(2, password);

        result = prepared->executeQuery();

        if (!result->next())
        {
            cout << "\nInvalid Username/Password - Try Again\n";

            delete prepared;
            delete result;
            
            return;
        }

        // this->username = username;
    }
    catch (sql::SQLException _exception)
    {
        delete prepared;
        delete result;

        cout << "\nException: " << _exception.what() << "\n";

        return;
    }

    delete result;
    delete prepared;

    choice_interface();
}

void Client::withdraw()
{
    auto connection = link->get_connection();
    auto statement = link->get_statement();
    sql::PreparedStatement* prepared = nullptr;
    sql::PreparedStatement* prepared2 = nullptr;
    sql::ResultSet* result = nullptr;

    string _amount;
    int balance, amount;

    try
    {
        prepared = connection->prepareStatement("SELECT balance FROM account WHERE username = ?");
        prepared->setString(1, this->username);

        result = prepared->executeQuery();

        result->next();
        balance = result->getInt("balance");

        cout << "\nEnter Withdraw Amount: ";
        // cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, _amount);

        amount = stoi(_amount);

        if (amount > balance)
        {
            delete prepared;
            delete prepared2;
            delete result;

            cout << "\nYour Available Balance Is: " << balance << " - Try Again\n";
            return;
        }
        else
        {
            balance -= amount;

            prepared2 = connection->prepareStatement("UPDATE account SET balance = ? WHERE username = ?");
            prepared2->setInt(1, balance);
            prepared2->setString(2, this->username);

            prepared2->executeUpdate();
        }
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

    cout << "\nSuccessfully Withdrawed Your Money\nYour Available Balance Is: " << balance << "\n";
}

void Client::deposit()
{
    auto connection = link->get_connection();
    auto statement = link->get_statement();
    sql::PreparedStatement* prepared = nullptr;
    sql::PreparedStatement* prepared2 = nullptr;
    sql::ResultSet* result = nullptr;

    string _amount;
    int balance, amount;

    try
    {
        prepared = connection->prepareStatement("SELECT balance FROM account WHERE username = ?");
        prepared->setString(1, username);

        result = prepared->executeQuery();
        result->next();
        balance = result->getInt("balance");

        cout << "\nEnter Deposit Amount: ";
        // cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, _amount);

        amount = stoi(_amount);

        if (amount <= 0)
            cout << "\nInvalid Input - Try Again\n";
        else if (INT_MAX - balance < amount)
            cout << "\nBank Deposit Capacity Reached - Contact Your Bank Manager\n";
        else
        {
            balance += amount;

            prepared2 = connection->prepareStatement("UPDATE account SET balance = ? WHERE username = ?");
            prepared2->setInt(1, balance);
            prepared2->setString(2, username);

            prepared2->executeUpdate();
        }
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

    cout << "\nSuccessfully Deposited Your Money\nYour Available Balance Is: " << balance << "\n";
}

void Client::enquiry()
{
    auto connection = link->get_connection();
    auto statement = link->get_statement();
    sql::PreparedStatement* prepared = nullptr;
    sql::ResultSet* result = nullptr;

    int balance = 0;

    try
    {
        prepared = connection->prepareStatement("SELECT balance FROM account WHERE username = ?");
        prepared->setString(1, username);
        result = prepared->executeQuery();

        result->next();
        balance = result->getInt("balance");
    }
    catch (sql::SQLException _exception)
    {
        delete prepared;
        delete result;

        cout << "\nException: " << _exception.what() << "\n";
        return;
    }

    delete prepared;
    delete result;

    cout << "\nYour Current Balance Is: " << balance << "\n";
}

void Client::transfer()
{
    auto connection = link->get_connection();
    auto statement = link->get_statement();
    sql::PreparedStatement* prepared = nullptr;
    sql::PreparedStatement* prepared2 = nullptr;
    sql::ResultSet* result = nullptr;

    string _amount, _accno;
    int balance, amount, account_number;

    try
    {
        prepared2 = connection->prepareStatement("SELECT account_number FROM account WHERE account_number = ?");

        cout << "\nEnter The Recipients Account Number: ";
        getline(cin, _accno);
        account_number = stoi(_accno);

        prepared2->setInt(1, account_number);
        result = prepared2->executeQuery();

        if (!result->next())
        {
            delete result;
            delete prepared;
            delete prepared2;

            cout << "\nThere Is No Account Under Account Number: " << account_number << " - Try Again\n";
            return;
        }

        delete result;
        delete prepared2;

        prepared = connection->prepareStatement("SELECT balance FROM account WHERE username = ?");
        prepared->setString(1, this->username);

        result = prepared->executeQuery();

        result->next();
        balance = result->getInt("balance");

        cout << "\nEnter Transfer Amount: ";
        // cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, _amount);

        amount = stoi(_amount);

        if (amount > balance)
        {
            delete prepared;
            delete prepared2;
            delete result;

            cout << "\nYour Available Balance Is: " << balance << " - Try Again\n";
            return;
        }
        else
        {
            delete prepared;
            delete result;

            prepared = connection->prepareStatement("SELECT balance FROM account WHERE account_number = ?");
            prepared->setInt(1, account_number);
            result = prepared->executeQuery();

            result->next();
            int balance2 = result->getInt("balance");

            balance -= amount;
            balance2 += amount;

            delete prepared;

            prepared = connection->prepareStatement("UPDATE account SET balance = ? WHERE username = ?");
            prepared->setInt(1, balance);
            prepared->setString(2, username);

            prepared->executeUpdate();

            prepared2 = connection->prepareStatement("UPDATE account SET balance = ? WHERE account_number = ?");
            prepared2->setInt(1, balance2);
            prepared2->setInt(2, account_number);

            prepared2->executeUpdate();
        }
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

    cout << "\nSuccessfully Transfered Money\nYour Available Balance Is: " << balance << "\n";
}

void Client::signup()
{
    auto connection = link->get_connection();
    auto statement = link->get_statement();
    sql::PreparedStatement* prepared = nullptr;
    sql::PreparedStatement* prepared2 = nullptr;
    sql::ResultSet* result = nullptr;

    string first_name, last_name, date_of_birth, gender, username, password, _bank_id;
    int bank_id;

    cout << "\nEnter Your First Name: ";
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, first_name);
    cout << "Enter Your Last Name : ";
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, last_name);
    cout << "Enter Your Date of Birth (YYYY-MM-DD): ";
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, date_of_birth);
    cout << "Enter Your Gender ('M' for Male - 'F' for Female - 'O' for Others): ";
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, gender); // assuming one enters only m, f, o, M, F, O
    cout << "Enter The Bank ID You Are Applying For: ";
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, _bank_id);
    cout << "Create Your Username: ";
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);
    cout << "Create Your Password: ";
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, password);

    bank_id = stoi(_bank_id);

    try
    {
        prepared = connection->prepareStatement("INSERT INTO request() VALUES(?, ?, ?, ?, ?, ?, ?)");
        prepared2 = connection->prepareStatement("SELECT id FROM bank WHERE id = ?");
        
        prepared2->setInt(1, bank_id);
        result = prepared2->executeQuery();

        if (!result->next())
        {
            delete result;
            delete prepared;
            delete prepared2;

            cout << "\nThere Is No Bank With ID - " << bank_id << " - Try Again\n";
            return;
        }

        prepared->setString(1, username);
        prepared->setString(2, password);
        prepared->setString(3, first_name);
        prepared->setString(4, last_name);
        prepared->setString(6, date_of_birth);
        prepared->setString(5, gender);
        prepared->setInt(7, bank_id);


        prepared->execute();
    }
    catch (sql::SQLException _exception)
    {
        delete result;
        delete prepared;
        delete prepared2;

        cout << "\nException: " << _exception.what() << "\n";

        return;
    }
    catch (std::exception _exception)
    {
        delete result;
        delete prepared;
        delete prepared2;

        cout << "\nException: " << _exception.what() << "\n";

        return;
    }

    delete result;
    delete prepared;
    delete prepared2;

    cout << "\nSuccessfully Signed Up - Please Wait For Manager Approval\n";
}

void Client::logout()
{
    return;
}