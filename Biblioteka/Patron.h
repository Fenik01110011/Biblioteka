#pragma once
#include <iostream>
#include <string>
#include "Chrono.h"

namespace Patron_ns
{
	class Patron
	{
	private:
		int library_card_number;
		std::string user_name;
		std::string address;
		Chrono::Date registration_date;
		double library_fees;

	public:
		class invalid {};
		Patron(int klibrary_card_number, std::string kuser_name, std::string kaddress, Chrono::Date kregistration_date, double klibrary_fees);
		Patron();
		
		int slibrary_card_number() const { return library_card_number; }
		std::string suser_name() const { return user_name; }
		std::string saddress() const { return address; }
		Chrono::Date sregistration_date() const { return registration_date; }
		double slibrary_fees() const { return library_fees; }
		
		void add_library_card_number(int number);
		void add_user_name(std::string str);
		void add_address(std::string str);
		void add_registration_date(Chrono::Date date);
		void add_library_fees(double fee);

		bool add_user_name_interface();
		bool add_address_interface();
		bool approve_add_patron_interface();

		bool is_library_fees();
		void calculation_library_fees(Chrono::Date& return_date, Chrono::Date actually_date);
	
		void mysql_patron_sanitization(MYSQL* mysql);
	};

	bool is_library_card_number(int number);
	bool operator==(const Patron& a, const Patron& b);
	bool operator!=(const Patron& a, const Patron& b);
	std::ostream& operator << (std::ostream& os, const Patron& b);

	Patron MySQL_row_to_patron(MYSQL_ROW row);
	bool add_patron_to_MySQL_datebase(Patron patron);
	int find_next_free_library_card_number(MYSQL* mysql);
	bool if_exist_patron_in_mysql_database(MYSQL* mysql, int library_card_number, Patron& patron);
}