#include "Link.h"

int main()
{
	try
	{
		if (link->connect())
			link->class_interface();
	}
	catch (sql::SQLException exception)
	{
		cout << "\nException: " << exception.what() << "\n";
	}
	catch (std::exception exception)
	{
		cout << "\nException: " << exception.what() << "\n";
	}

	delete link;
	return 0;
}