#pragma once
#include <iostream>

namespace Chrono
{
	//12 miesiecy, kazdemu z miesiecy odpowiada trzyliterowy angielski skrot
	enum class Month
	{
		jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
	};

	class Date
	{
	private:
			int d; //day
			Month m; //month
			int y; //year

	public:
		class invalid {}; //klasa do rzucania bledow
		Date(int d, Month m, int y); //ustawia date, jesli data jest niepoprawna wyrzuci blad
		Date(); //ustawia domyslna date podana w funkcji default_date()
		
		int day() const { return d; }  //zwraca dzien
		Month month() const { return m; } //zwraca miesiac
		int year() const{ return y; } //zwraca rok

		std::string date_to_str(); //zwraca date jako napisa w postaci dd.mm.rrrr
		
		void add_day(int n); //ustawia dzien, wyrzuci blad jesli dzien jest niepoprawny
		void add_month(int n); //ustawia miesiac, wyrzuci blad jesli miesiac jest niepoprawny
		void add_year(int n); //ustawia rok

		void more_days(int n); //dodaje podana liczbe dni do daty
	};

	bool is_date(int d, Month m, int y); //sprawdza czy podana data jest poprawna
	bool leapyear(int y); //sprawdza czy podany rok jest przestepny

	//##### przeciazenia operatorow klasy Date #######################
	bool operator==(const Date& a, const Date& b);
	bool operator!=(const Date& a, const Date& b);
	bool operator > (const Date& a, const Date& b);
	bool operator >= (const Date& a, const Date& b);
	bool operator < (const Date& a, const Date& b);
	bool operator <= (const Date& a, const Date& b);
	//#END# przeciazenia operatorow klasy Date #######################

	std::ostream& operator << (std::ostream& os, const Date& d); //wypisuje date w formacie dd.mm.rrrr
	std::istream& operator>>(std::istream& is, Date& dd); //przyjmuje date w formacie dd.mm.rrrr, jesli data jest niepoprawna wyrzuca blad Date::invalid

	//##### Zamiana czasu ###############################################
	int date_to_days(Date date);
	long long date_to_sec(Date date);
	Date days_to_date(int days);
	long long days_to_sec(int days);
	Date sec_to_date(long long seconds);
	int sec_to_days(long long seconds);
	//#END# Zamiana czasu ###############################################

	std::string date_to_mysql(Date date);
	Date date_from_mysql(std::string date);
}



