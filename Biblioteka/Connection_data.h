#pragma once
#include <iostream>
#include <string>
#include <mysql.h>
namespace conn
{
	class Connection_data
	{
	public:
		std::string host;
		std::string db_user;
		std::string db_password;
		std::string db_name;
		bool is_data_from_file;

		Connection_data();
		Connection_data(std::string file_path);

		bool add_data_from_file(std::string file_path);
	};

	bool mysql_connect(MYSQL* mysql, Connection_data connect);
	std::string mysql_code_sanitization(MYSQL* mysql, std::string string_from_user);
	std::ostream& operator << (std::ostream& os, const Connection_data& b);
}