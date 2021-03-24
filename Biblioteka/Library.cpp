#include <string>
#include <conio.h>
#include <time.h>
#include <fstream>
#include "Polish_characters.h"
#include "Connection_data.h"
#include "Library.h"

using namespace std;
using namespace Chrono;
using namespace Book_ns;
using namespace Patron_ns;
using namespace PL;

namespace Library_ns
{
//##### Transaction ##########################################################
	Transaction::Transaction(Patron kpatron, Book kbook, Date kdate, Date kreturn_date)
		:patron{ kpatron },
		book{ kbook },
		date{ kdate },
		return_date{ kreturn_date }
	{}

	Transaction::Transaction(Patron kpatron, Book kbook, Date kdate, Date kreturn_date, bool for_create_static)
		:patron{ kpatron },
		book{ kbook },
		date{ kdate },
		return_date{ kreturn_date }
	{}

	const Transaction& default_transaction()
	{
		static Transaction t(Patron(), Book(), Date(), Date(), true);
		return t;
	}

	Transaction::Transaction()
		:patron{ default_transaction().patron },
		book{ default_transaction().book },
		date{ default_transaction().date },
		return_date{ default_transaction().return_date }
	{}

	bool Transaction::add_library_card_number_interface()
	{
		cout << pl("Numer karty bibliotecznej klienta: ");

		int library_card_number;
		bool wrong_data = true;
		while (wrong_data)
		{
			while (!(cin >> library_card_number))
			{
				cout << pl("Wprowadzono niepoprawnie numer. Ponów próbê: ");
				cin.clear();
				cin.ignore(999999, '\n');
			}
			cin.ignore(999999, '\n');
			if (is_library_card_number(library_card_number))
				wrong_data = false;
			else
			{
				cout << pl("Przyjmowany zakres to 100000 - 2140000000.") << endl;
				cout << pl("Wybierz numer karty bibliotecznej ponownie: ");
			}
		}

		//##### MySQL connection #########################################################
		MYSQL mysql;
		bool error = true;
		if (conn::mysql_connect(&mysql, conn::Connection_data("connect.txt")))
		{
			if (if_exist_patron_in_mysql_database(&mysql, library_card_number, patron))
			{
				if (patron.is_library_fees())
				{
					cout << endl << patron << endl;
					cout << pl("Nie mo¿na wypo¿yczyæ ksi¹¿ki temu klientowi, poniewa¿ posiada on karê.") << endl;
				}
				else
					error = false;
			}
			mysql_close(&mysql);
		}
		//#END# MySQL connection #########################################################
		return !error;
	}

	bool Transaction::add_ISBN_interface()
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
		bool error = true;
		if (conn::mysql_connect(&mysql, conn::Connection_data("connect.txt")))
		{
			if (if_exist_book_in_mysql_database(&mysql, ISBN_buffer, book))
			{
				if (book.schecked_out())
				{
					cout << endl << book << endl;
					cout << pl("Ta ksi¹¿ka jest aktualnie wypo¿yczona.") << endl;
				}
				else
					error = false;
			}
			mysql_close(&mysql);
		}
		//#END# MySQL connection #########################################################
		return !error;
	}

	bool Transaction::approve_add_transaction_interface()
	{
		date = sec_to_date(time(NULL));
		return_date = date;
		return_date.more_days(30);

		system("cls");
		cout << pl("Czy chcesz dodaæ wypo¿yczenie z nastêpuj¹cymi danymi?") << endl << endl;
		cout << *this << endl;
		cout << pl("Wybór (y/n): ");
		char c;
		for (c = ' '; c != 'y' && c != 'n';)
			c = _getch();

		if (c == 'y')
		{
			if (add_transaction_to_MySQL_datebase(*this))
			{
				cout << pl("Nowe wypo¿yczenie zosta³o dodane!") << endl << endl;
				return true;
			}
		}
		else if (c != 'n')
			cout << pl("Wyst¹pi³ b³¹d przy wyborze czy dodaæ wypo¿yczenie.") << endl;

		cout << pl("Wypo¿yczenie nie zosta³o dodane.") << endl;
		return false;
	}

	ostream& operator << (ostream& os, const Transaction& t)
	{
		os << pl("Wypo¿yczona ksi¹¿ka:") << endl << t.book << endl;
		os << pl("Wypo¿yczona przez:") << endl << t.patron << endl;
		os << pl("Data wypo¿yczenia: ") << t.date << endl;
		os << pl("Zwrot do:          ") << t.return_date << endl;
		return os;
	}

	bool add_transaction_to_MySQL_datebase(Transaction transaction)
	{
		//##### MySQL connection #########################################################
		MYSQL mysql;
		bool error = true;
		if (conn::mysql_connect(&mysql, conn::Connection_data("connect.txt")))
		{
			Patron patron = transaction.patron;
			if (if_exist_patron_in_mysql_database(&mysql, patron.slibrary_card_number(), patron))
			{
				if (patron.is_library_fees())
				{
					cout << endl << patron << endl;
					cout << pl("Nie mo¿na wypo¿yczyæ ksi¹¿ki temu klientowi, poniewa¿ posiada on karê.") << endl;
				}
				else
					error = false;
			}

			if (!error)
			{
				error = true;
				Book book = transaction.book;
				if (if_exist_book_in_mysql_database(&mysql, book.sISBN(), book))
				{
					if (book.schecked_out())
					{
						cout << endl << book << endl;
						cout << pl("Ta ksi¹¿ka jest aktualnie wypo¿yczona.") << endl;
					}
					else
						error = false;
				}
			}

			if (!error)
			{
				error = true;

				MYSQL_RES* result;
				string query = "SELECT * FROM transactions WHERE ISBN = ";
				query += transaction.book.sISBN();
				if (!mysql_query(&mysql, query.c_str()))
				{
					result = mysql_store_result(&mysql);

					if (mysql_num_rows(result) > 0)
					{
						if (!change_book_checked_out_in_mysql_database(&mysql, transaction.book.sISBN(), true))
						{
							cout << endl << pl("Ksi¹¿ka z ISBN = ") << transaction.book.sISBN() << pl(" nie zosta³a oznaczona jako wypo¿yczona.") << endl;
							cout << pl("Rêcznie zmieñ stan ksi¹¿ki na wypo¿yczony lub usuñ i ponownie dodaj wypo¿yczenie.") << endl;
						}
						else
							transaction.book.add_checked_out(true);

						cout << endl << transaction.book << endl;
						cout << pl("Ta ksi¹¿ka jest aktualnie wypo¿yczona.") << endl;
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
			}

			if (!error)
			{
				error = true;

				string query = "INSERT INTO `transactions` (`id`, `ISBN`, `library_card_number`, `date`, `return_date`) VALUES(NULL, '";
				query += transaction.book.sISBN() + "', '";
				query += to_string(transaction.patron.slibrary_card_number()) + "', '";
				query += date_to_mysql(transaction.date) + "', '";
				query += date_to_mysql(transaction.return_date) + "')";

				if (!mysql_query(&mysql, pl_to_mysql(query).c_str()))
				{
					if (!change_book_checked_out_in_mysql_database(&mysql, transaction.book.sISBN(), true))
					{
						cout << pl("Ksi¹¿ka z ISBN = ") << transaction.book.sISBN() << pl(" nie zosta³a oznaczona jako wypo¿yczona.") << endl;
						cout << pl("Rêcznie zmieñ stan ksi¹¿ki na wypo¿yczony lub usuñ i ponownie dodaj wypo¿yczenie.") << endl << endl;
					}

					error = false;
				}
				else
				{
					cout << "Error code: " << mysql_errno(&mysql) << endl;
					cout << "Query failed: " << pl_from_mysql(mysql_error(&mysql)) << endl;
				}
			}
			mysql_close(&mysql);
		}
		//#END# MySQL connection #########################################################
		return !error;
	}
//#END# Transaction ##########################################################

//##### Books ################################################################
	bool Library::add_book() 
	{
		Book book;
		cout << pl("---Dodawanie ksi¹¿ki---") << endl;
		if (!(book.add_ISBN_interface() &&
			book.add_title_interface() &&
			book.add_author_interface() &&
			book.add_genre_interface() &&
			book.add_copyright_date_interface() &&
			book.approve_add_book_interface()))
			return false;

		return true;
	}
//#END# Books ################################################################

//##### Patron ###############################################################
	bool Library::add_patron()
	{
		Patron patron;
		cout << pl("---Dodawanie klienta---") << endl;
		patron.add_registration_date(sec_to_date(time(NULL)));
		if (!(patron.add_user_name_interface() &&
			patron.add_address_interface() &&
			patron.approve_add_patron_interface()))
			return false;

		return true;
	}
//#END# Patron ###############################################################

//##### Transaction ##########################################################
	bool Library::add_transaction()
	{
		Transaction transaction;
		cout << pl("---Dodawanie wypo¿yczenia ---") << endl;
		if (!(transaction.add_library_card_number_interface() &&
			transaction.add_ISBN_interface() &&
			transaction.approve_add_transaction_interface()))
			return false;

		return true;
	}
//#END# Transaction ##########################################################

	void Library::show_library_fees()
	{
		system("cls");
		cout << pl("---U¿ytkownicy z karami---") << endl << endl;
		for (int i = 0; i < (int)Patrons.size(); ++i)
			if (Patrons[i].is_library_fees())
				cout << i + 1 << "." << endl << Patrons[i] << endl;
	}

	void Library::calculation_all_library_fees()
	{
		/*
		Date date_bufor = sec_to_date(time(NULL));
		if (actually_date != date_bufor)
		{
			cout << pl("Czy napewno dzisiaj jest ") << date_bufor << "? (y/n)";
			char c;
			for (c = ' '; c != 'y' && c != 'n';)
				c = _getch();

			system("cls");

			if (c == 'y')
				;
			else if (c == 'n')
			{
				cout << pl("Program korzysta z czasu systemowego przy naliczaniu kar.") << endl;
				cout << pl("Proszê ustawiæ odpowiedni¹ datê na komputerze.") << endl;
				return;
			}
			else
			{
				cout << pl("Wyst¹pi³ b³¹d przy potwierdzaniu daty.") << endl << endl;
				return;
			}
		}

		actually_date = date_bufor;

		for (int i = 0; i < (int)Transactions.size(); ++i)
			if (Transactions[i].return_date < actually_date)
			{
				for (int j = 0; j < (int)Patrons.size(); ++j)
				{
					if (Patrons[j] == Transactions[i].patron)
						Patrons[j].calculation_library_fees(Transactions[i].return_date, actually_date);
				}
			}
		//**/
	}

	void Library::wyjscie()
	{
		cout << pl("Czy na pewno chcesz wyjœæ z programu? (y/n)");
		char c;
		for (c = ' '; c != 'y' && c != 'n';)
			c = _getch();

		cout << endl << endl;
		if (c == 'y')
			exit(0);
		else if (c == 'n')
			;
		else
			cout << pl("Wyst¹pi³ b³¹d przy wyborze czy zamkn¹æ program.") << endl << endl;
	}

//##### Books #####################################################
	int Library::find_book_ISBN()
	{
		bool wrong_data;
		string ISBN;
		int pozycja_znalezionej_ksiazki = -1;

		cout << pl("Wyszukiwanie ksi¹¿ki") << endl;
		cout << pl("ISBN: ");
		wrong_data = true;
		while (wrong_data)
		{
			getline(cin, ISBN);
			if (is_ISBN(ISBN))
				wrong_data = false;
			else
			{
				cout << pl("Wprowadzono nieprawid³owe dane. ISBN sk³ada siê z 13 cyfr.") << endl;
				cout << pl("Podaj poprawny ISBN: ");
			}
		}

		system("cls");
		bool czy_znaleziono = false;
		cout << pl("Znalezione ksi¹¿ki:") << endl;
		for (int i = 0; i < (int)Books.size(); ++i)
			if (Books[i].sISBN() == ISBN)
			{
				cout << Books[i];
				pozycja_znalezionej_ksiazki = i;
				czy_znaleziono = true;
			}

		if (!czy_znaleziono)
		{
			system("cls");
			cout << pl("Nie znaleziono ksi¹¿ki, gdzie ISBN = ") << ISBN << endl;
			return -1;
		}
		return pozycja_znalezionej_ksiazki;
	}

	bool Library::find_book_title()
	{
		string title;

		cout << pl("Wyszukiwanie ksi¹¿ki") << endl;
		cout << pl("Tytu³: ");
		getline(cin, title);

		bool czy_znaleziono = false;
		cout << pl("Znalezione ksi¹¿ki:") << endl;
		for (int i = 0; i < (int)Books.size(); ++i)
			if (Books[i].stitle() == title)
			{
				cout << Books[i];
				czy_znaleziono = true;
			}

		if (!czy_znaleziono)
		{
			system("cls");
			cout << pl("Nie znaleziono ksi¹¿ki z podanym tytule.") << endl;
			return false;
		}
		return true;
	}

	bool Library::find_book_author()
	{
		string author;

		cout << pl("Wyszukiwanie ksi¹¿ki") << endl;
		cout << pl("Autor: ");
		getline(cin, author);

		bool czy_znaleziono = false;
		cout << pl("Znalezione ksi¹¿ki:") << endl;
		for (int i = 0; i < (int)Books.size(); ++i)
			if (Books[i].sauthor() == author)
			{
				cout << Books[i];
				czy_znaleziono = true;
			}

		if (!czy_znaleziono)
		{
			system("cls");
			cout << pl("Nie znaleziono ksi¹¿ki po podanym autorze.") << endl;
			return false;
		}
		return true;
	}

	void Library::wypisanie_wszystkich_ksiazek()
	{
		cout << pl("---Wszystkie ksi¹¿ki---") << endl << endl;
		string bufor = "";
		for (int i = 0; i < (int)Books.size(); ++i)
		{
			bufor += to_string(i + 1) + ".\n";
			bufor += "ISBN: " + Books[i].sISBN() + '\n';
			bufor += pl("Tytu³: ") + Books[i].stitle() + '\n';
			bufor += "Autor: " + Books[i].sauthor() + '\n';
			bufor += "Data wydania: " + to_string(Books[i].scopyright_date()) + "\n";
			bufor += "Gatunek: " + Books[i].sgenre() + "\n\n";
		}
		cout << bufor;
	}

	void Library::usuwanie_ksiazki()
	{
		cout << pl("Usuwanie ksia¿ki") << endl;
		int pozycja_znalezionej_ksiazki = find_book_ISBN();
		if (pozycja_znalezionej_ksiazki >= 0)
		{
			cout << pl("Czy na pewno usun¹æ tê ksi¹¿kê? (y/n)");
			char c;
			for (c = ' '; c != 'y' && c != 'n';)
				c = _getch();

			if (c == 'y')
			{
				Books.erase(Books.begin() + pozycja_znalezionej_ksiazki);
				cout << pl("Ksi¹¿ka zosta³a usuniêta.") << endl;

			}
			else if (c == 'n')
			{
				cout << pl("Ksi¹¿ka nie zosta³a usuniêta.") << endl;
			}
			else
			{
				cout << pl("Wyst¹pi³ b³¹d przy wyborze czy usun¹æ ksi¹¿kê. Ksi¹¿ka nie zosta³a usuniêta.") << endl << endl;
			}
		}

	}
//#END# Books #####################################################

	void Library::testy()
	{
		;
	} 
}

/*
//ALTER TABLE nazwa AUTO_INCREMENT = 500
//The COUNT() function returns the number of rows that matches a specified criteria.
string query = "SELECT MAX(library_card_number) FROM patrons";
*/

/*
CREATE TABLE `books` (
 `ISBN` bigint(20) NOT NULL,
 `title` tinytext COLLATE utf8_polish_ci NOT NULL,
 `author` tinytext COLLATE utf8_polish_ci NOT NULL,
 `genre` tinytext COLLATE utf8_polish_ci NOT NULL,
 `copyright_date` int(11) NOT NULL,
 `checked_out` tinyint(1) NOT NULL,
 PRIMARY KEY (`ISBN`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci
*/
