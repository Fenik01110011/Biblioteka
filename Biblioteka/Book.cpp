#include <iostream>
#include <vector>
#include <conio.h>
#include "Book.h"
#include "Polish_characters.h"
#include "Connection_data.h"
#include "Global.h"

using namespace std;
using namespace Chrono;
using namespace PL;

namespace Book_ns
{
	Book::Book(string kISBN, string ktitle, string kauthor, string kgenre, int kcopyright_date, bool kchecked_out)
		:title{ ktitle },
		author{ kauthor },
		genre{ kgenre },
		copyright_date{ kcopyright_date },
		checked_out{ kchecked_out }
		
	{
		int ISBN_length = (int)kISBN.length();
		if (ISBN_length > 0 && ISBN_length < 13)
		{
			string new_ISBN = "";
			for (int i = ISBN_length; i < 13; ++i)
				new_ISBN += '0';
			kISBN = new_ISBN + kISBN;
		}

		if (!is_ISBN(kISBN)) throw invalid{};
		else ISBN = kISBN;
	}

	const Book& default_book()
	{
		static Book b("1111111111111", "Tytu³ zagadka", "Gall Anonim", "Fikcja", 1970, false);
		return b;
	}

	Book::Book()
		:ISBN{ default_book().sISBN() },
		title{ default_book().stitle() },
		author{ default_book().sauthor() },
		copyright_date{ default_book().scopyright_date() },
		checked_out{ default_book().schecked_out() },
		genre{ default_book().sgenre() }
	{

	}

	bool is_ISBN(string ISBN)
	{
		if (ISBN.size() != 13) return false;
		for (int i = 0; i < 13; ++i)
			if (!(ISBN[i] >= 48 && ISBN[i] <= 57)) return false;
		return true;
	}

	void Book::add_ISBN(string str)
	{
		int ISBN_length = (int)str.length();
		if (ISBN_length > 0 && ISBN_length < 13)
		{
			string new_ISBN = "";
			for (int i = ISBN_length; i < 13; ++i)
				new_ISBN += '0';
			str = new_ISBN + str;
		}

		if (!is_ISBN(str)) throw invalid{};
		else ISBN = str;
	}

	void Book::add_title(string str)
	{
		title = str;
	}

	void Book::add_author(string str)
	{
		author = str;
	}

	void Book::add_copyright_date(int year)
	{
		copyright_date = year;
	}

	void Book::add_checked_out(bool b)
	{
		checked_out = b;
	}

	void Book::add_genre(string g)
	{
		genre = g;
	}
//##### Book interface #####################################
	bool Book::add_ISBN_interface()
	{
		cout << pl("ISBN: ");

		string ISBN_buffer;
		bool wrong_data = true;

		while (wrong_data)
		{
			getline(cin, ISBN_buffer);
			ISBN_buffer = delete_spaces_and_dashes(ISBN_buffer);
			if (is_ISBN(ISBN_buffer))
				wrong_data = false;
			else
			{
				cout << pl("Wprowadzono nieprawid³owe dane. ISBN sk³ada siê z 13 cyfr.") << endl;
				cout << pl("Podaj poprawny ISBN: ");
			}
		}

		//##### MySQL connection #########################################################
		MYSQL mysql;
		MYSQL_RES* result;
		bool error = true;
		if (conn::mysql_connect(&mysql, conn::Connection_data("connect.txt")))
		{
			string query = "SELECT * FROM books WHERE ISBN = " + ISBN_buffer;
			if (!mysql_query(&mysql, query.c_str()))
			{
				result = mysql_store_result(&mysql);

				if (mysql_num_rows(result) > 0)
				{
					cout << endl << pl("Ksi¹¿ka z podanym ISBN ju¿ istnieje:") << endl << endl;
					try
					{
						cout << MySQL_row_to_book(mysql_fetch_row(result));
					}
					catch (...)
					{
						cout << endl << pl("B³¹d funkcji \"MySQL_row_to_book()\".") << endl;
					}
				}
				else
					error = false;

				mysql_free_result(result);
			}
			else
			{
				cout << "Error code: " << mysql_errno(&mysql) << endl;
				cout << "Query failed: " << pl_from_mysql(mysql_error(&mysql)) << endl;
			}
			mysql_close(&mysql);
		}
		//#END# MySQL connection #########################################################

		if(!error) ISBN = ISBN_buffer;
		return !error;
	}
	bool Book::add_title_interface()
	{
		cout << pl("Tytu³ ksi¹¿ki: ");
		getline(cin, title);
		return true;
	}
	bool Book::add_author_interface()
	{
		cout << pl("Autor: ");
		getline(cin, author);
		return true;
	}
	bool Book::add_genre_interface()
	{
		vector <string> genres;
		//##### MySQL connection #########################################################
		MYSQL mysql;
		MYSQL_ROW row;
		MYSQL_RES* result;
		bool error = true;

		if (conn::mysql_connect(&mysql, conn::Connection_data("connect.txt")))
		{
			string query = "SELECT genre FROM genres";
			if (!mysql_query(&mysql, query.c_str()))
			{
				result = mysql_store_result(&mysql);
				while (row = mysql_fetch_row(result))
					genres.push_back(pl_from_mysql(row[0]));

				mysql_free_result(result);
				error = false;
			}
			else
			{
				cout << "Error code: " << mysql_errno(&mysql) << endl;
				cout << "Query failed: " << pl_from_mysql(mysql_error(&mysql)) << endl;
			}
			mysql_close(&mysql);
		}

		if (error) return false;
		//#END# MySQL connection #########################################################

		cout << pl("Wybierz jeden z podanych gatunków:") << endl;
		for (int i = 0; i < genres.size(); ++i)
			cout << "+ " << genres[i] << endl;

		cout << pl("Gatunek: ");
		string genre_buffer;
		bool wrong_data = true;
		while (wrong_data)
		{
			getline(cin, genre_buffer);
			if (genre_buffer.length() > 0)
			{
				for (int i = 0; i < genres.size(); ++i)
					if (genres[i].find(genre_buffer) != string::npos)
					{
						genre_buffer = genres[i];
						wrong_data = false;
						break;
					}
				if (wrong_data)
					cout << pl("Nie ma takiego gatunku. Wpisz gatunek ponownie: ");
			}
			else
				cout << pl("Wpisz nazwê gatunku: ");
		}
		genre = genre_buffer;
		return true;
	}
	bool Book::add_copyright_date_interface()
	{
		cout << pl("Rok wydania: ");
		int copyright_date_buffer;
		while (!(cin >> copyright_date_buffer))
		{
			cout << pl("B³êdne wpisanie roku wydania ksi¹¿ki. Poprawny format to: yyyy") << endl;
			cout << pl("Wpisz poprawnie rok wydania ksi¹¿ki: ");
			cin.clear();
			cin.ignore(999999, '\n');
		}
		cin.ignore(999999, '\n');
		copyright_date = copyright_date_buffer;
		return true;
	}
	bool Book::approve_add_book_interface()
	{
		system("cls");
		cout << pl("Czy chcesz dodaæ ksi¹¿kê z nastêpuj¹cymi danymi?") << endl << endl;
		cout << *this << endl;
		cout << pl("Wybór (y/n): ");
		char c;
		for (c = ' '; c != 'y' && c != 'n';)
			c = _getch();

		if (c == 'y')
		{
			if (add_book_to_MySQL_datebase(*this))
			{
				cout << pl("Nowa ksi¹¿ka zosta³a dodana!") << endl << endl;
				return true;
			}
		}
		else if (c != 'n')
			cout << pl("Wyst¹pi³ b³¹d przy wyborze czy dodaæ ksi¹¿kê.") << endl;

		cout << pl("Ksi¹¿ka nie zosta³a dodana.") << endl << endl;
		return false;
	}
//#END# Book interface #####################################

	void Book::mysql_book_sanitization(MYSQL* mysql)
	{
		title = conn::mysql_code_sanitization(mysql,title);
		author = conn::mysql_code_sanitization(mysql, author);
		genre = conn::mysql_code_sanitization(mysql, genre);
	}


	bool operator==(const Book& a, const Book& b)
	{
		return a.sISBN() == b.sISBN();
	}
	bool operator!=(const Book& a, const Book& b)
	{
		return !(a == b);
	}

	ostream& operator << (ostream& os, const Book& b)
	{
		os << pl("ISBN:        ") << b.sISBN() << endl;
		os << pl("Tytu³:       ") << b.stitle() << endl;
		os << pl("Autor:       ") << b.sauthor() << endl;
		os << pl("Rok wydania: ") << b.scopyright_date() << endl;
		os << pl("Gatunek:     ") << b.sgenre() << endl;
		os << pl("Wypo¿yczona: ");
		if (b.schecked_out()) cout << "Tak";
		else cout << "Nie";
		cout << endl;
		return os;
	}

	string delete_spaces_and_dashes(string text)
	{
		string new_text = "";
		for (int i = 0; i < text.length(); ++i)
			if (text[i] != ' ' && text[i] != '-') new_text += text[i];
		return new_text;
	}
	
	Book MySQL_row_to_book(MYSQL_ROW row)
	{
		return Book(row[0], pl_from_mysql(row[1]), pl_from_mysql(row[2]), pl_from_mysql(row[3]), atoi(row[4]), atoi(row[5]));
	}
	
	bool add_book_to_MySQL_datebase(Book book)
	{
		//##### MySQL connection #########################################################
		MYSQL mysql;
		bool error = true;

		if (conn::mysql_connect(&mysql, conn::Connection_data("connect.txt")))
		{
			book.mysql_book_sanitization(&mysql);
			string query = "INSERT INTO `books` (`ISBN`, `title`, `author`, `genre`, `copyright_date`, `checked_out`) VALUES('";
			query += book.sISBN() + "', '";
			query += book.stitle() + "', '";
			query += book.sauthor() + "', '";
			query += book.sgenre() + "', '";
			query += to_string(book.scopyright_date()) + "', '0')";

			if (!mysql_query(&mysql, pl_to_mysql(query).c_str()))
				error = false;
			else
			{
				cout << "Error code: " << mysql_errno(&mysql) << endl;
				cout << "Query failed: " << pl_from_mysql(mysql_error(&mysql)) << endl;
			}
			mysql_close(&mysql);
		}
		return !error;
		//#END# MySQL connection #########################################################
	}

	bool delete_book_from_MySQL_datebase(string ISBN)
	{
		//##### MySQL connection #########################################################
		MYSQL mysql;
		bool error = true;

		if (conn::mysql_connect(&mysql, conn::Connection_data("connect.txt")))
		{
			ISBN = conn::mysql_code_sanitization(&mysql, ISBN);
			string query = "DELETE FROM `books` WHERE `books`.`ISBN` = ";
			query += ISBN;

			if (!mysql_query(&mysql, pl_to_mysql(query).c_str()))
				error = false;
			else
			{
				cout << "Error code: " << mysql_errno(&mysql) << endl;
				cout << "Query failed: " << pl_from_mysql(mysql_error(&mysql)) << endl;
			}
			mysql_close(&mysql);
		}
		return !error;
		//#END# MySQL connection #########################################################
	}

	bool if_exist_book_in_mysql_database(MYSQL* mysql, string ISBN, Book& book)
	{
		ISBN = conn::mysql_code_sanitization(mysql, ISBN);
		MYSQL_RES* result;
		bool error = true;
		string query = "SELECT * FROM books WHERE ISBN = " + ISBN;
		if (!mysql_query(mysql, query.c_str()))
		{
			result = mysql_store_result(mysql);

			if (mysql_num_rows(result) > 0)
			{
				try
				{
					book = MySQL_row_to_book(mysql_fetch_row(result));
					error = false;
				}
				catch (...)
				{
					cout << endl << pl("B³¹d funkcji \"MySQL_row_to_book()\".") << endl;
				}
			}
			else
				cout << endl << pl("Ksi¹¿ki z podanym ISBN nie ma w bazie.") << endl;


			mysql_free_result(result);
		}
		else
		{
			cout << "Error code: " << mysql_errno(mysql) << endl;
			cout << "Query failed: " << pl_from_mysql(mysql_error(mysql)) << endl;
		}

		return !error;
	}

	bool change_book_checked_out_in_mysql_database(MYSQL* mysql, string ISBN, bool checked_out)
	{
		string query = "UPDATE `books` SET `checked_out` = '";
		if (checked_out) query += "1";
		else query += "0";
		query += "' WHERE `books`.`ISBN` = " + ISBN;

		if (!mysql_query(mysql, pl_to_mysql(query).c_str()))
			return true;
		else
		{
			cout << "Error code: " << mysql_errno(mysql) << endl;
			cout << "Query failed: " << pl_from_mysql(mysql_error(mysql)) << endl;
		}
		return false;
	}
}