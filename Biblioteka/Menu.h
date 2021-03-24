#pragma once
#include "Library.h"

class Menu
{
	Library_ns::Library biblioteka; //utworzenie obiektu biblioteki w celu dostepu do funkci programu

public:
	long long nr_m; //zmienna ustawiana aby wybrac odpowiednie menu

	Menu(); //ustawia zmienna "nr_m" na wartosc 0 czyli na menu glowne

	void spis_menu(long long nr); //metoda odpowiadajaca za wywolanie odpowiedniej funcki menu programu

	//funkcja prosi o wybranie numeru, zakres wyboru taki jaki zostal przeslany do funkcji
	void wybor_opcji(long long nr, int ilosc_opcji, bool wybor_cofnij, bool wybor_menu_glowne);
	
	//##### metody wyswietlajace poszczegolne menu ##############################
	void menu_glowne(long long nr);

	void m_ksiazki(long long nr);
	void m_wyszukiwanie_ksiazki(long long nr);

	void m_klienci(long long nr);
	void m_wyszukiwanie_klienta(long long nr);

	void m_wypozyczenia(long long nr);
	void m_wyszukiwanie_wypozyczenia(long long nr);

	void m_kary(long long nr);

	void m_ustawienia(long long nr);
	//#END# metody wyswietlajace poszczegolne menu ##############################
};