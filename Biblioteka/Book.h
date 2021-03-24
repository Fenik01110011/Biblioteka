#pragma once
#include <iostream>
#include <string>
#include <mysql.h>
#include "Chrono.h"

namespace Book_ns
{
	class Book
	{
	private:
		std::string ISBN; //13 cyfrowy identyfikator ksiazki
		std::string title;
		std::string author;
		std::string genre;
		int copyright_date;
		bool checked_out;

	public:
		class invalid {};
		Book(std::string kISBN, std::string ktitle, std::string kauthor, std::string kgenre, int kcopyright_date, bool kchecked_out);
		Book();

		std::string sISBN() const { return ISBN; }
		std::string stitle() const { return title; }
		std::string sauthor() const { return author; }
		std::string sgenre() const { return genre; }
		int scopyright_date() const { return copyright_date; }
		bool schecked_out() const { return checked_out; }

		void add_ISBN(std::string str);
		void add_title(std::string str);
		void add_author(std::string str);
		void add_genre(std::string g);
		void add_copyright_date(int year);
		void add_checked_out(bool b);

		bool add_ISBN_interface();
		bool add_title_interface();
		bool add_author_interface();
		bool add_genre_interface();
		bool add_copyright_date_interface();
		bool approve_add_book_interface();

		void mysql_book_sanitization(MYSQL* mysql);
	};

	bool is_ISBN(std::string s);
	bool operator==(const Book& a, const Book& b);
	bool operator!=(const Book& a, const Book& b);
	std::ostream& operator << (std::ostream& os, const Book& b);

	std::string delete_spaces_and_dashes(std::string text);
	Book MySQL_row_to_book(MYSQL_ROW row);
	bool add_book_to_MySQL_datebase(Book book);
	bool delete_book_from_MySQL_datebase(std::string ISBN);
	bool if_exist_book_in_mysql_database(MYSQL* mysql, std::string ISBN, Book& book);
	bool change_book_checked_out_in_mysql_database(MYSQL* mysql, std::string ISBN, bool checked_out);
}
