#pragma once
#include <iostream>
#include <vector>
#include <mysql.h>
#include "Chrono.h"
#include "Book.h"
#include "Patron.h"
#include "Connection_data.h"

namespace Library_ns
{
	class Transaction
	{
	public:
		Patron_ns::Patron patron;
		Book_ns::Book book;
		Chrono::Date date, return_date;

		Transaction(Patron_ns::Patron kpatron, Book_ns::Book kbook, Chrono::Date kdate, Chrono::Date kreturn_date, bool for_create_static);
		Transaction(Patron_ns::Patron kpatron, Book_ns::Book kbook, Chrono::Date kdate, Chrono::Date kreturn_date);
		Transaction();

		bool add_library_card_number_interface();
		bool add_ISBN_interface();
		bool approve_add_transaction_interface();
	};

	std::ostream& operator << (std::ostream& os, const Transaction& t);

	bool add_transaction_to_MySQL_datebase(Transaction transaction);

	class Library
	{
	private:
		std::vector <Book_ns::Book> Books;
		std::vector <Patron_ns::Patron> Patrons;
		std::vector <Transaction> Transactions;
		Chrono::Date actually_date;

	public:
		class invalid {};

		bool add_book();
		bool add_patron();
		bool add_transaction();

		void show_library_fees();
		void calculation_all_library_fees();

		void ksiazki();
		void klienci();
		void wypozyczenia();
		void przedluzenie_wypozyczenia();
		void naliczone_kary();
		void splata_kary();
		void wyjscie();

		int find_book_ISBN(); //prosi o podanie ISBN, wypisuje znalezione ksiazki, zwraca pozycje znalezionej ksiazki
		bool find_book_title();
		bool find_book_author();

		void wypisanie_wszystkich_ksiazek();

		void usuwanie_ksiazki();

		void testy();
	};
}
