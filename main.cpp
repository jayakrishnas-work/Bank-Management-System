// Author: Jaya Krishna Soganur
// Created On: December 17, 2021
// Last Edited: August 30, 2022

// including all the related header files
#include <iostream> // for input and output operations
// headers related to Connector/C++ 8.0 JDBC 4.0 based API
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/exception.h>
using namespace std;

// function declartions
void link();
void start();
void login();
void load_details();
void commit();
void show_balance();
void deposit();
void withdraw();
void logout();
void actions();
void signup();

// We are linking the server and application using JDBC 4.0 based API
// It is provided by Connector/C++ by MySQL DBMS
// Dependencies are as follows:
// 1. Microsoft Visual C++ Redistributable 2015 version or higher
// 2. Microsft Visual C++ (MSVC) compiler, same or higher than the MSVC used to compile JDBC library
// 3. Same build configuration, in this case 'Release' build for Windows x64 devices
// The JDBC 4.0 based API library is statically linked and MSVC++ is dynamically linked
// The target host must have MSVC++ 2015 or higher installed to smoothly run application

// defining all objects we need to connect and communicate with MySQL Server
sql::Driver* driver;
sql::Connection* connection;
sql::PreparedStatement* prepared;
sql::ResultSet* result;

// defining and initializing at which address we have our database
// in this case, the deault address is "tcp://127.0.0.1:3306"
const string& address = "tcp://127.0.0.1:3306";

// 'link()' connects our appliaction to the database
void link()
{
    cout << "\t\t\t...Connecting to the database...\n\n";

    // "username', 'password': username, password of the database user with sufficient rights
    string username, password;
    cout << "Enter the 'username' of DBMS user: ";
    cin >> username; // assuming one provides us with valid username
    cout << "Enter the 'password' of DBMS user: ";
    cin >> password; // assuming one provides us with valid password
    cout << "\n";

    // lets connect to our database
    try
    {
        // think driver as tool to establish the connection
        driver = get_driver_instance();
        // 'connect()' throws an exception if something goes wrong
        connection = driver->connect(address, username, password);
    }
    catch (sql::SQLException exception)
    {
        // stdout what went wrong and exit
        cout << exception.what() << endl;
        
        // we need to clear the memory allocated to the below variables before exiting
        delete result;
        delete prepared;
        delete connection;

        exit(1);
    }

    cout << "\t\t\t...Connected to the database...\n\n\n";
}

// defining all the variables needed for the client account access
// 'name', 'username', 'password': username, password of the bank client
string name, username, password;
// 'accno', 'balance': accno, balance of the bank client
int accno = 0, balance = 0;

// 'start()' is the entry point from which the client can use the application
// though in this application, one needs to enter the username and password
// of the DBMS user credentials but, as we grow, we shall let of that dependency
void start()
{
    cout << "\t\t\t...Bank Management System...\n\n\nSelect an action to perform\n\n";
    cout << "1 - Login\n2 - Register\n3 - Exit\n\nEnter here: ";
    int option = 0;
    cin >> option;

    // map the option to a set of actions
    switch (option) {
    case 1:
        login();
        break;
    case 2:
        signup();
        break;
    case 3:
        exit(0);
    default:
        cout << "\nInvalid option - Try again\n\n";
        start();
    }
}

// 'login()' authenticates and lets a client access ones account and lets one perform a set of operations
void login()
{
    // authentication of the client
    cout << "\nEnter the 'username' of the bank account: ";
    cin >> username;
    cout << "Enter the 'password' of the bank account: ";
    cin >> password;

    // lets access the account related to the username and password
    try
    {
        // prepared statements are derived from statements and has a few special feature
        // one is place holders '?' and lets us set his value later using member functions
        // unlike statements, which need query processing everytime, prepared statements need only one
        prepared = connection->prepareStatement("SELECT * FROM client WHERE username = ? AND password = ?");

        // set the ? values using indices, which start from 1 from left
        prepared->setString(1, username);
        prepared->setString(2, password);

        // executing the prepared query and storing the sql::ResultSet* at result 
        // result now contains pointer to the default loaction, above the 1st row
        result = prepared->executeQuery();

        // 'next()' moves 'result' cursor/pointer to next row and 
        // returns if the next row exists or not
        // if exists, one is a real client
        if (result->next())
        {
            // load the details to local memory/buffer
            load_details();
            // got to actions
            actions();
        }
        else
        {
            cout << "\nInvalid 'username' or 'password' - Try again\n\n\n";
            start();
        }
    }
    catch (sql::SQLException exception)
    {
        // stdout what went wrong and exit
        cout << "\n" << exception.what() << endl;

        // we need to clear the memory allocated to the below variables before exiting
        delete result;
        delete prepared;
        delete connection;

        exit(1);
    }

    // clear the memory
    delete result;
    delete prepared;
    cout << "\n";
}

// 'load_details()' loads the values/attributes of the row/client into variables of the application
void load_details()
{
    // as the username and password are already stored in their respective variables
    // we only need to load the rest into their respective variables;
    try
    {
        // result already contains the details
        name = result->getString("name");
        accno = result->getInt("accno");
        balance = result->getInt("balance");
    }
    catch (sql::SQLException exception)
    {
        // stdout what went wrong and exit
        cout << exception.what() << endl;

        // we need to clear the memory allocated to the below variables before exiting
        delete result;
        delete prepared;
        delete connection;

        exit(1);
    }
}

// 'commit()' changes the contents of the database permanently, in this case only 'balance'
void commit()
{
    try
    {
        prepared = connection->prepareStatement("UPDATE client SET balance = ? WHERE username = ?");
        prepared->setInt(1, balance);
        prepared->setString(2, username);

        // 'executeUpdate()' returns value is more than 0 if the qery executed perfectly
        if (prepared->executeUpdate())
            cout << "\nBalance is updated\n";
    }
    catch (sql::SQLException exception)
    {
        // stdout what went wrong and exit
        cout << exception.what() << endl;

        // we need to clear the memory allocated to the below variables before exiting
        delete result;
        delete prepared;
        delete connection;

        exit(1);
    }

    // clear the memory
    delete prepared;
}

// 'show_balance()' stdouts' the clients' current balance in the account
void show_balance()
{
    cout << "\nYour cuurent balance is: " << balance << "\n";
    actions();
}

// 'deposit()' lets the client deposit extra money into his account
void deposit()
{
    int amount = 0;
    cout << "\nEnter your deposit amount: ";
    cin >> amount;
    // if the client enters negative balance, report and try again
    if (amount < 0)
    {
        cout << "\nDeposit amount cannot be less than 0 - Try again\n";
        deposit();
    }

    // update balance
    balance += amount;

    //commmit the changes
    commit();
    // show his current balance
    show_balance();
    // go back to actions
    actions();
}

// 'withdraw()' lets the client take money from his account
void withdraw()
{
    int amount = 0;
    cout << "\nEnter your withdraw amount: ";
    cin >> amount;

    // if client enters more money than the balance, prompt and continue;
    if (amount > balance)
        cout << "\nCan't withdraw. You only have " << balance << " - Try again\n";
    else
    {
        balance -= amount;
        //commmit the changes
        commit();
        // show his current balance
        show_balance();
    }

    // go to actions
    actions();
}

// 'logout()' lets the client logout and 
void logout()
{
    // commit the changes;
    commit();
    cout << "\nSee you soon\n\n\n";

    start();
}

// a client who has logged in needs to perform a set of actions
// 'actions' lets the client choose which action to perform
void actions()
{
    cout << "\nSelect an action to perform\n\n";
    cout << "1 - Show Balance\n2 - Deposit\n3 - Withdraw\n4 - Logout\n\nEnter here: ";
    int option = 0;
    cin >> option;

    // map the actions using switch
    switch (option) {
    case 1:
        show_balance();
        break;
    case 2:
        deposit();
        break;
    case 3:
        withdraw();
        break;
    case 4:
        logout();
        break;
    default:
        cout << "\nInvalid option\n";
        actions();
    }
}


// 'singup()' lets a new client register himself into the database;
void signup()
{
    cout << "\nEnter your name (name) (single word): ";
    cin >> name;
    cout << "Enter your account number (accno): ";
    cin >> accno;
    cout << "Enter your username (username) (single word): ";
    cin >> username;
    cout << "Enter your password (password) (single word): ";
    cin >> password;
    balance = 0;

    try
    {
        prepared = connection->prepareStatement("INSERT INTO client VALUES(?, ?, ?, ?, ?)");
        prepared->setString(1, username);
        prepared->setString(2, password);
        prepared->setInt(3, accno);
        prepared->setString(4, name);
        prepared->setInt(5, balance);
        
        if (prepared->executeUpdate())
        {
            cout << "\nRegistered\n\n";
            // as the client registered, let him login again
            start();
        }
    }
    catch (sql::SQLException exception)
    {
        // stdout what went wrong and exit
        cout << "\n" << exception.what() << endl;

        // we need to clear the memory allocated to the below variables before exiting
        delete result;
        delete prepared;
        delete connection;

        exit(1);
    }

    // clear the memory
    delete prepared;
}

// 'main()' is the entry point into our program
int main()
{
    try
    {
        link();
        connection->setSchema("BankManagementSystem");
        start();
    }
    catch (sql::SQLException exception)
    {
        // stdout what went wrong and exit
        cout << exception.what() << endl;

        // we need to clear the memory allocated to the below variables before exiting
        delete result;
        delete prepared;
        delete connection;

        exit(1);
    }

    // we no longer need the connection between the application and DBMS
    // we shall clear the connections and related before we close the application
    delete result;
    delete prepared;
    delete connection;

    // driver is deleted by the Connctor/C++ 8.0 by itself
    return 0;
}