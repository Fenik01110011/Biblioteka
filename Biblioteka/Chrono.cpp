#include <iostream>
#include <string>
#include "Chrono.h"

using namespace std;

namespace Chrono
{
	//##### Klasa Date #########################################################
	
	//ustawia date, jesli data jest niepoprawna wyrzuci blad
	Date::Date(int dd, Month mm, int yy)
		:d{ dd }, m{ mm }, y{ yy }
	{
		if (!is_date(dd, mm, yy)) throw invalid{}; //jesli data jest niepoprawna wyrzuci blad
	}

	//tworzy domyslna date
	const Date& default_date()
	{
		static Date dd(1, Month::jan, 1970);
		return dd;
	}

	//ustawia domyslna date podana w funkcji default_date()
	Date::Date()
		:d{ default_date().day() },
		m{ default_date().month() },
		y{ default_date().year() }
	{}

	//zwraca date jako napisa w postaci dd.mm.rrrr
	string Date::date_to_str()
	{
		string calosc = "";
		if (d < 10)
			calosc += '0';
		calosc += to_string(d) + '.';
		if ((int)m < 10)
			calosc += '0';
		calosc += to_string((int)m) + '.';
		calosc += to_string(y);
		return calosc;
	}

	//ustawia dzien, wyrzuci blad jesli dzien jest niepoprawny
	void Date::add_day(int n)
	{
		if (!is_date(n, m, y))
			throw invalid{};
		else
			d = n;
	}

	//ustawia miesiac, wyrzuci blad jesli miesiac jest niepoprawny
	void Date::add_month(int n)
	{
		if (!is_date(d, Month(n), y))
			throw invalid{};
		else
			m = Month(n);
	}

	//ustawia rok
	void Date::add_year(int n)
	{
		if (!is_date(d, m, n))
			throw invalid{};
		else
			y = n;
	}
	
	//dodaje podana liczbe dni do daty
	void Date::more_days(int n)
	{
		*this = days_to_date(date_to_days(*this) + n);
	}
	//#END# Klasa Date #########################################################

	//sprawdza czy podana data jest poprawna
	bool is_date(int d, Month m, int y)
	{
		if (d <= 0 || d > 31) return false;
		if (m < Month::jan || m > Month::dec) return false;
		int days_in_month = 31;
		switch ((int)m)
		{
		case 4: case 6: case 9: case 11:
			days_in_month = 30; break;
		case 2: 
			if(leapyear(y)) days_in_month = 29;
			else days_in_month = 28;
		}
		if (d > days_in_month) return false;

		return true;
	}

	//sprawdza czy podany rok jest przestepny
	bool leapyear(int y)
	{
		if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)
			return true;
		else
			return false;
	}

	//##### przeciazenia operatorow klasy Date #######################
	bool operator==(const Date& a, const Date& b)
	{
		return a.year() == b.year()
			&& a.month() == b.month()
			&& a.day() == b.day();
	}
	bool operator!=(const Date& a, const Date& b)
	{
		return !(a == b);
	}
	bool operator > (const Date& a, const Date& b)
	{
		if(a.year() > b.year())
			return true;
		else if (a.year() < b.year())
			return false;
		else if(a.month() > b.month())
			return true;
		else if (a.month() < b.month())
			return false;
		else if (a.day() > b.day())
			return true;
		else if (a.day() < b.day())
			return false;
		else
			return false;
	}
	bool operator >= (const Date& a, const Date& b)
	{
		return a > b || a == b;
	}
	bool operator < (const Date& a, const Date& b)
	{
		return !(a >= b);
	}
	bool operator <= (const Date& a, const Date& b)
	{
		return a < b || a == b;
	}
	//#END# przeciazenia operatorow klasy Date #######################

	//wypisuje date w formacie dd.mm.rrrr
	ostream& operator << (ostream& os, const Date& d)
	{
		return os << Date(d).date_to_str();
	}

	//przyjmuje date w formacie dd.mm.rrrr, jesli data jest niepoprawna wyrzuca blad Date::invalid
	istream& operator>>(istream& is, Date& dd)
	{
		int d, m, y;
		char ch1, ch2;
		is >> d >> ch1 >> m >> ch2 >> y;

		if (!is) return is;
		else if (ch1 != '.' || ch2 != '.') 
		{
			is.clear(ios_base::failbit); //ustawia blad strumienia
			return is;
		}

		dd = Date(d, Month(m), y);

		return is;
	}

	//##### Zamiana czasu ###############################################
	int date_to_days(Date date)
	{
		int days = 0;
		if (date.year() >= 1970)
		{
			days += date.day();

			for (int i = (int)date.month() - 1; i >= 1; --i)
			{
				switch (i)
				{
				case 4: case 6: case 9: case 11: days += 30; break;
				case 2: if (leapyear(date.year())) days += 29; else days += 28; break;
				default: days += 31;
				}
			}

			for (int i = date.year(); i > 1970; --i)
				if (leapyear(i)) days += 366;
				else days += 365;
		}
		else
		{
			days += date.day() - 1;

			for (int i = 12; i >= (int)date.month(); --i)
			{
				switch (i)
				{
				case 4: case 6: case 9: case 11: days -= 30; break;
				case 2: if (leapyear(date.year())) days -= 29; else days -= 28; break;
				default: days -= 31;
				}
			}

			for (int i = date.year() + 1; i < 1970; ++i)
				if (leapyear(i)) days -= 366;
				else days -= 365;
		}
		return days;
	}
	long long date_to_sec(Date date)
	{
		return days_to_sec(date_to_days(date));
	}
	Date days_to_date(int days)
	{
		Date date(1,Month(1),1970);
		if (days >= 0)
		{
			for (int i = 1970;; ++i)
				if (!leapyear(i) && days > 365) days -= 365;
				else if (days > 366) days -= 366;
				else {date.add_year(i); break;}

			int i = 1;
			for (bool loop = true; loop; ++i)
			{
				switch (i)
				{
				case 4: case 6: case 9: case 11:
					if (days > 30) days -= 30;
					else loop = false; break;
				case 2:
					if (!leapyear(date.year()) && days > 28) days -= 28;
					else if (days > 29) days -= 29;
					else loop = false; break;
				default: 
					if (days > 31) days -= 31;
					else loop = false;
				}
			}
			date.add_month(i-1);

			date.add_day(days);
		}
		else
		{
			for (int i = 1969;; --i)
				if (!leapyear(i) && days < -365) days += 365;
				else if (days < -366) days += 366;
				else { date.add_year(i); break; }

			for (int i = 12; i >= 0; --i)
			{
				switch (i)
				{
				case 4: case 6: case 9: case 11: days += 30; break;
				case 2: days += 28; if (leapyear(date.year())) ++days; break;
				default: days += 31;
				}
				if (days >= 0) { date.add_month(i); break; }
			}

			date.add_day(days + 1);
		}
		return date;
	}
	long long days_to_sec(int days)
	{
		if (days >= 0) return ((long long)days - 1) * 86400;
		else return ((long long)days + 1) * 86400 - 1;
	}
	Date sec_to_date(long long seconds)
	{
		return days_to_date(sec_to_days(seconds));
	}
	int sec_to_days(long long seconds)
	{
		if (seconds >= 0) return (int)(seconds / 86400) + 1;
		else return (int)(seconds / 86400) - 1;
	}
	//#END# Zamiana czasu ###############################################

	string date_to_mysql(Date date)
	{
		string mysql_date = "";
		mysql_date += to_string(date.year()) + "-";
		if ((int)date.month() < 10)
			mysql_date += '0';
		mysql_date += to_string((int)date.month()) + "-";
		if (date.day() < 10)
			mysql_date += '0';
		mysql_date += to_string(date.day());

		return mysql_date;
	}
	Date date_from_mysql(string mysql_date)
	{
		string date[3];
		int string_length = (int)mysql_date.length();
		int j = 0;
		for (int i = 0; i < 3; ++i)
		{ 
			for (date[i] = ""; j < string_length; ++j)
			{
				if (mysql_date[j] == '-' || mysql_date[j] == '.') break;
				date[i] += mysql_date[j];
			}
			++j;
		}

		try
		{
			return Date(atoi(date[2].c_str()), Month(atoi(date[1].c_str())), atoi(date[1].c_str()));
		}
		catch (...)
		{
			throw Date::invalid{};
		}
	}

}