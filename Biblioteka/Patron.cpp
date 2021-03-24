#include <iostream>
#include <conio.h>
#include <cmath>
#include "Polish_characters.h"
#include "Connection_data.h"
#include "Book.h"
#include "Patron.h"
#include "Global.h"

using namespace std;
using namespace Chrono;
using namespace PL;

namespace Patron_ns
{
	Patron::Patron(int klibrary_card_number, string kuser_name, string kaddress, Date kregistration_date, double klibrary_fees)
		:library_card_number{ klibrary_card_number },
		user_name{ kuser_name },
		address{ kaddress },
		registration_date{ kregistration_date },
		library_fees{ round(klibrary_fees*100)/100 }
	{
		if (!is_library_card_number(klibrary_card_number))
			throw invalid{};
	}

	const Patron& default_patron()
	{
		static Patron p(100000, "Gallus Anonymus", "Narnia", Date(1, Month(1), 2000), 0);
		return p;
	}

	Patron::Patron()
		: library_card_number{ default_patron().slibrary_card_number() },
		user_name{ default_patron().suser_name() },
		address{ default_patron().saddress() },
		registration_date{ default_patron().sregistration_date() },
		library_fees{ default_patron().slibrary_fees() }
	{}


	void Patron::add_library_card_number(int number)
	{
		if (is_library_card_number(number))
			library_card_number = number;
		else
			throw invalid{};
	}
	void Patron::add_user_name(string str)
	{
		user_name = str;
	}
	void Patron::add_address(string str)
	{
		address = str;
	}
	void Patron::add_registration_date(Date date)
	{
		registration_date = date;
	}
	void Patron::add_library_fees(double fee)
	{
		library_fees = round(fee*100)/100;
	}

	//##### Patron interface #######################################
	bool Patron::add_user_name_interface()
	{
		cout << pl("Imiê i nazwisko: ");
		getline(cin, user_name);
		return true;
	}
	bool Patron::add_address_interface()
	{
		cout << pl("Adres: ");
		getline(cin, address);
		return true;
	}
	bool Patron::approve_add_patron_interface()
	{
		system("cls");
		cout << pl("Czy chcesz dodaæ klienta z nastêpuj¹cymi danymi?") << endl << endl;

		cout << pl("Imiê i nazwisko:  ") << user_name << endl;
		cout << pl("Adres:            ") << address << endl;
		cout << pl("Data rejestracji: ") << registration_date << endl;
		cout << pl("Naliczona kara:   ") << library_fees << pl(" z³") << endl;

		cout << endl << pl("Wybór (y/n): ");
		char c;
		for (c = ' '; c != 'y' && c != 'n';)
			c = _getch();

		if (c == 'y')
		{
			if (add_patron_to_MySQL_datebase(*this))
				return true;
		}
		else if (c != 'n')
			cout << pl("Wyst¹pi³ b³¹d przy wyborze czy dodaæ klienta.") << endl;

		cout << pl("Klient nie zosta³ dodany.") << endl << endl;
		return false;
	}
	//#END# Patron interface #######################################


	bool Patron::is_library_fees()
	{
		if (library_fees >= 0.01)
			return true;
		else
			return false;
	}

	void Patron::calculation_library_fees(Date& return_date, Date actually_date)
	{
		int roznica_dni = date_to_days(actually_date) - date_to_days(return_date);
		if (roznica_dni > 0)
		{
			library_fees += roznica_dni * 0.1;
			return_date = actually_date;
		}
	}

	void Patron::mysql_patron_sanitization(MYSQL* mysql)
	{
		user_name = conn::mysql_code_sanitization(mysql, user_name);
		address = conn::mysql_code_sanitization(mysql, address);
	}

	bool is_library_card_number(int number)
	{
		if (number >= 100000 && number <= 2140000000)
			return true;
		else
			return false;
	}

	bool operator==(const Patron& a, const Patron& b)
	{
		return a.slibrary_card_number() == b.slibrary_card_number();
	}

	bool operator!=(const Patron& a, const Patron& b)
	{
		return !(a == b);
	}

	ostream& operator << (ostream& os, const Patron& p)
	{
		os << pl("Numer karty bibliotecznej: ") << p.slibrary_card_number() << endl;
		os << pl("Imiê i nazwisko:  ") << p.suser_name() << endl;
		os << pl("Adres:            ") << p.saddress() << endl;
		os << pl("Data rejestracji: ") << p.sregistration_date() << endl;
		os << pl("Naliczona kara:   ") << p.slibrary_fees() << pl(" z³") << endl;
		return os;
	}


	Patron MySQL_row_to_patron(MYSQL_ROW row)
	{
		return Patron(atoi(row[0]), pl_from_mysql(row[1]), pl_from_mysql(row[2]), date_from_mysql(row[3]), stod(row[4]));
	}

	bool add_patron_to_MySQL_datebase(Patron patron)
	{
		//##### MySQL connection #########################################################
		MYSQL mysql;
		bool error = true;

		if (conn::mysql_connect(&mysql, conn::Connection_data("connect.txt")))
		{
			int library_card_number = find_next_free_library_card_number(&mysql);
			if (library_card_number == 0)
				;
			else if (is_library_card_number(library_card_number))
			{
				patron.mysql_patron_sanitization(&mysql);
				string query = "INSERT INTO `patrons` (`library_card_number`, `user_name`, `address`, `registration_date`, `library_fees`) VALUES (";
				query += "NULL, '";
				query += patron.suser_name() + "', '";
				query += patron.saddress() + "', '";
				query += date_to_mysql(patron.sregistration_date()) + "', '0')";

				if (!mysql_query(&mysql, pl_to_mysql(query).c_str()))
				{
					std::system("cls");
					try
					{
						patron.add_library_card_number((int)mysql_insert_id(&mysql));
						cout << pl("Nowy klient zosta³ dodany!") << endl;
						cout << pl("Dane nowego klienta:") << endl << endl;
						cout << patron;
						error = false;
					}
					catch (...)
					{
						cout << pl("Wyst¹pi³ b³¹d z numerem karty bibliotecznej.") << endl;

						query = "DELETE FROM `patrons` WHERE library_card_number = ";
						query += to_string(mysql_insert_id(&mysql));
						if (mysql_query(&mysql, query.c_str()))
						{
							cout << "Error code: " << mysql_errno(&mysql) << endl;
							cout << "Query failed: " << pl_from_mysql(mysql_error(&mysql)) << endl;
							cout << pl("Dodany b³êdny wpis nie zosta³ usuniêty z bazy.") << endl;
						}
					}
				}
				else
				{
					cout << "Error code: " << mysql_errno(&mysql) << endl;
					cout << "Query failed: " << pl_from_mysql(mysql_error(&mysql)) << endl;
				}
			}
			else
			{
				cout << pl("Wyst¹pi³ b³¹d.") << endl;
				cout << pl("Nowy numer karty bibliotecznej jest niepoprawny.") << endl;
			}
			mysql_close(&mysql);
		}
		return !error;
		//#END# MySQL connection #########################################################
	}

	int find_next_free_library_card_number(MYSQL* mysql)
	{
		MYSQL_ROW row;
		MYSQL_RES* result;

		string query = "SHOW TABLE STATUS LIKE 'patrons'";
		if (!mysql_query(mysql, query.c_str()))
		{
			result = mysql_store_result(mysql);
			row = mysql_fetch_row(result);
			mysql_free_result(result);
			
			if (is_library_card_number(atoi(row[11]))) //row[11] powiniene zawierac wartosc AUTO_INCREMENT
				return atoi(row[11]);
			else if (atoi(row[11]) < 100000)
			{
				query = "ALTER TABLE `patrons` AUTO_INCREMENT = 100000";
				if (!mysql_query(mysql, query.c_str()))
					return 100000;
				else
				{
					cout << "Error code: " << mysql_errno(mysql) << endl;
					cout << "Query failed: " << pl_from_mysql(mysql_error(mysql)) << endl;
				}
			}
			else
				cout << pl("Znaleziony numer karty bibliotecznej wykracza poza zakres.") << endl;
		}
		else
		{
			cout << "Error code: " << mysql_errno(mysql) << endl;
			cout << "Query failed: " << pl_from_mysql(mysql_error(mysql)) << endl;
		}
		return 0;
	}

	bool if_exist_patron_in_mysql_database(MYSQL* mysql, int library_card_number, Patron& patron)
	{
		MYSQL_RES* result;
		bool error = true;

		string query = "SELECT * FROM patrons WHERE library_card_number = ";
		query += to_string(library_card_number);

		if (!mysql_query(mysql, query.c_str()))
		{
			result = mysql_store_result(mysql);

			if (mysql_num_rows(result) > 0)
			{
				try
				{
					patron = MySQL_row_to_patron(mysql_fetch_row(result));
					error = false;
				}
				catch (...)
				{
					cout << endl << pl("B³¹d funkcji \"MySQL_row_to_patron()\".") << endl;
				}
			}
			else
				cout << endl << pl("Karta biblioteczna z podanym numerem nie jest zarejestrowana.") << endl;

			mysql_free_result(result);
		}
		else
		{
			cout << "Error code: " << mysql_errno(mysql) << endl;
			cout << "Query failed: " << pl_from_mysql(mysql_error(mysql)) << endl;
		}
		return !error;
	}
}