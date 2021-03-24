#include <fstream>
#include "Polish_characters.h"
#include "Connection_data.h"

using namespace std;
using namespace PL;

namespace conn
{
	Connection_data::Connection_data()
		:host{ "localhost" },
		db_user{ "root" },
		db_password{ "" },
		db_name{ "default" },
		is_data_from_file{ false }
	{

	}

	Connection_data::Connection_data(string file_path)
	{
		add_data_from_file(file_path);
	}

	bool Connection_data::add_data_from_file(string file_path)
	{
		fstream file;
		file.open(file_path, ios::in);
		int line = 0;
		if (file.good())
		{
			is_data_from_file = true;
			string buffer;
			while (getline(file, buffer) && line < 4)
			{
				buffer = PL::pl_from_file(buffer);
				++line;
				switch (line)
				{
				case 1: if (buffer.length() >= 7) host = buffer.erase(0, 7);
						else is_data_from_file = false; break;
				case 2: if (buffer.length() >= 10) db_user = buffer.erase(0, 10);
						else is_data_from_file = false; break;
				case 3: if (buffer.length() >= 14) db_password = buffer.erase(0, 14);
						else is_data_from_file = false; break;
				case 4: if (buffer.length() >= 10) db_name = buffer.erase(0, 10);
						else is_data_from_file = false; break;
				}
			}
			file.close();
		}
		else
			is_data_from_file = false;
		if (line < 4)
			is_data_from_file = false;

		if (is_data_from_file == false)
			* this = Connection_data();

		return is_data_from_file;
	}

	bool mysql_connect(MYSQL* mysql, Connection_data connect)
	{
		mysql_init(mysql);
		if (!connect.is_data_from_file)
			cout << pl("B³¹d odczytu danych potrzebnych do po³¹czenia z baz¹.") << endl;
		else if (mysql_real_connect(mysql, connect.host.c_str(), connect.db_user.c_str(), connect.db_password.c_str(), connect.db_name.c_str(), 3306, NULL, 0))
			return true;
		else
		{
			cout << pl("Nie uda³o siê po³¹czyæ z baz¹.") << endl;
			cout << "Error code: " << mysql_errno(mysql) << endl;
			cout << pl_from_mysql(mysql_error(mysql)) << endl;
		}
		return false;
	}

	string mysql_code_sanitization(MYSQL* mysql, string string_from_user)
	{
		string new_string = "";
		char* buffer = new char[string_from_user.length() * 2 + 1];
		mysql_real_escape_string(mysql, buffer, string_from_user.c_str(), (int)string_from_user.length());
		new_string = buffer;
		delete[] buffer;
		return new_string;
	}

	ostream& operator << (ostream& os, const Connection_data& b)
	{
		os << "host = \"" << b.host << "\"" << endl;
		os << "db_user = \"" << b.db_user << "\"" << endl;
		os << "db_password = \"" << b.db_password << "\"" << endl;
		os << "db_name = \"" << b.db_name << "\"" << endl;
		return os;
	}
}