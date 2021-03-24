#include <iostream>
#include <conio.h>
#include "Menu.h"
#include "Polish_characters.h"
#include "Global.h"

using namespace std;
using namespace PL;

//start zmiennej obiektu menu z wartoœci¹ 0 czyli menu g³ownym
Menu::Menu()
{
	nr_m = 0;
}

//opisana struktura menu
/*
Plan menu

MENU G£ÓWNE
1. Ksi¹¿ki
		KSI¥¯KI
		1. Wyszukaj
				Wyszukaj ksi¹¿kê po:
				1. ISBN
				2. Tytule
				3. Autorze
				4. Gatunku
				5. Dacie wydania
		2. Wyœwietl wszystkie
		3. Dodaj
		4. Usuñ
2. Klienci
		KLIENCI
		1. Wyszukaj
				Wyszukaj klienta po:
				1. Numerze karty bibliotecznej 
				2. Imieniu i nazwisku
		2. Wyœwietl wszystkich
		3. Dodaj
		4. Usuñ
3. Wypo¿yczenia
		WYPO¯YCZENIA
		1. Dodaj
		2. Usuñ
		3. Przed³u¿
		4. Wyszukaj
				Wyszukaj wypo¿yczenia po:
				1. Kliencie
				2. Ksi¹¿ce
				3. Dacie wypo¿yczenia
		5. Wyœwietl wszystkie
4. Kary
		KARY
		1. Sp³aæ
		2. SprawdŸ kare klienta
		3. Wyœwietl wszystkie
5. Ustawienia
		1. Baza danych
		2. Nazwa u¿ytkownika
		3. Has³o do programu
6. Wyjœcie

*/

//metoda odpowiadajaca za wywolanie odpowiedniej funcki menu programu
//zmienna "nr" odpowiada za wybor odpowiedniej funkcji, kazde kolejne podmenu to jedna cyfra wiecej
void Menu::spis_menu(long long nr)
{
	system("cls");
	switch (nr)
	{
	//case 0: biblioteka.testy(); break;
	case 0: menu_glowne(nr); break;
	/////////////////////////////////////////////////////////////////////////////
	case 1: m_ksiazki(nr); break;
		case 11: m_wyszukiwanie_ksiazki(nr); break;
			case 111: biblioteka.find_book_ISBN(); break;
			case 112: biblioteka.find_book_title(); break;
			case 113: biblioteka.find_book_author(); break;
			case 114: ; break;
			case 115: ; break;
		case 12: biblioteka.wypisanie_wszystkich_ksiazek(); break;
		case 13: biblioteka.add_book(); break;
		case 14: biblioteka.usuwanie_ksiazki(); break;
	/////////////////////////////////////////////////////////////////////////////
	case 2: m_klienci(nr); break;
			case 21: m_wyszukiwanie_klienta(nr); break;
					case 211:; break;
					case 212:; break;
			case 22:; break;
			case 23: biblioteka.add_patron(); break;
			case 24:; break;
	/////////////////////////////////////////////////////////////////////////////
	case 3: m_wypozyczenia(nr); break;
			case 31: biblioteka.add_transaction(); break;
			case 32:; break;
			case 33:; break;
			case 34: m_wyszukiwanie_wypozyczenia(nr); break;
					case 341:; break;
					case 342:; break;
					case 343:; break;
			case 35:; break;
	/////////////////////////////////////////////////////////////////////////////
	case 4: m_kary(nr); break;
			case 41:; break;
			case 42:; break;
			case 43:; break;
	/////////////////////////////////////////////////////////////////////////////
	case 5: m_ustawienia(nr); break;
			case 51:; break;
			case 52:; break;
			case 53:; break;
	/////////////////////////////////////////////////////////////////////////////
	case 6: biblioteka.wyjscie(); break;
	/////////////////////////////////////////////////////////////////////////////
	default:
	{
		cout << endl << pl("B³¹d funkcji \"wybor_menu()\" dla argumentu \"") << nr << "\"." << endl << endl;
		nr_m = nr / 100; //cofa menu o 2 poziomy
		Global::pause();
	}
	}

	if (nr_m == nr)
	{
		nr_m /= 10; //kiedy wybrany numer jest juz funkcja docelowa, to po jej wykonaniu wroci do poprzedniego menu
		cout << endl;
		Global::pause();
	}
}

/*funkcja prosi o wybranie numeru, zakres wyboru taki jaki zostal przeslany do funkcji
nr - numer menu w ktorym zostala wywolana ta funkcja
ilosc_opcji - do jakiego numeru ma byc mozliwy wybor(od 0 do 9)
wybor_cofnij - czy ma byc wyswietlana opcja "Esc - Cofnij" w menu
wybor_menu_glowne - czy ma byc wyswietlana opcja "m - Menu g³ówne" w menu*/
void Menu::wybor_opcji(long long nr, int ilosc_opcji, bool wybor_cofnij, bool wybor_menu_glowne)
{
	if (wybor_cofnij) cout << "Esc - Cofnij" << endl;
	if (wybor_menu_glowne) cout << pl("m - Menu g³ówne") << endl;
	cout << "Wybierz numer:";

	nr *= 10;
	bool zly_wybor; //jesli pod koniec petli ta zmienna bedzie miala nadal wartosc false to petla zostanie przerwana
	char wybor_char;
	do
	{
		zly_wybor = false;
		wybor_char = _getch();

		//ogranicza wybor do okreslonej liczby
		if ((wybor_char >= '1') && (wybor_char < ('1' + ilosc_opcji)) || (((wybor_char == 27) || (wybor_char == 'm')) && (nr != 0)))
		{
			switch (wybor_char)
			{
			case '1': nr_m = nr + 1; break;
			case '2': nr_m = nr + 2; break;
			case '3': nr_m = nr + 3; break;
			case '4': nr_m = nr + 4; break;
			case '5': nr_m = nr + 5; break;
			case '6': nr_m = nr + 6; break;
			case '7': nr_m = nr + 7; break;
			case '8': nr_m = nr + 8; break;
			case '9': nr_m = nr + 9; break;
			case 27: nr_m = nr/100; break;
			case 'm': nr_m = 0; break;
			default:
			{
				cout << endl << pl("B³¹d funkcji \"wybor_opcji()\" dla argumentu \"") << wybor_char << "\"." << endl << endl;
				nr_m = nr / 100; //cofa do wczesniejszego menu
				Global::pause();
			}
			}
		}
		else
		{
			zly_wybor = true;
			cout << endl << pl("Nie ma takiej opcji. Ponów wybór:");
		}
	} while (zly_wybor);
}

//##### metody wyswietlajace poszczegolne menu ########################
void Menu::menu_glowne(long long nr)
{
	cout << pl("MENU G£ÓWNE") << endl;
	cout << pl("1. Ksi¹¿ki") << endl;
	cout << pl("2. Klienci") << endl;
	cout << pl("3. Wypo¿yczenia") << endl;
	cout << pl("4. Kary") << endl;
	cout << pl("5. Ustawienia") << endl;
	cout << pl("6. Wyjœcie") << endl;

	wybor_opcji(nr, 6, false, false);
}

void Menu::m_ksiazki(long long nr)
{
	cout << pl("KSI¥¯KI") << endl;
	cout << pl("1. Wyszukaj") << endl;
	cout << pl("2. Wyœwietl wszystkie") << endl;
	cout << pl("3. Dodaj") << endl;
	cout << pl("4. Usuñ") << endl;

	wybor_opcji(nr, 4, true, false);
}
void Menu::m_wyszukiwanie_ksiazki(long long nr)
{
	cout << pl("Wyszukaj ksi¹¿kê po:") << endl;
	cout << pl("1. ISBN") << endl;
	cout << pl("2. Tytule") << endl;
	cout << pl("3. Autorze") << endl;
	cout << pl("4. Gatunku") << endl;
	cout << pl("5. Dacie wydania") << endl;

	wybor_opcji(nr, 5, true, true);
}

void Menu::m_klienci(long long nr)
{
	cout << pl("KLIENCI") << endl;
	cout << pl("1. Wyszukaj") << endl;
	cout << pl("2. Wyœwietl wszystkich") << endl;
	cout << pl("3. Dodaj") << endl;
	cout << pl("4. Usuñ") << endl;

	wybor_opcji(nr, 4, true, false);
}
void Menu::m_wyszukiwanie_klienta(long long nr)
{
	cout << pl("Wyszukaj klienta po:") << endl;
	cout << pl("1. Numerze karty bibliotecznej") << endl;
	cout << pl("2. Imieniu i nazwisku") << endl;

	wybor_opcji(nr, 2, true, true);
}

void Menu::m_wypozyczenia(long long nr)
{
	cout << pl("WYPO¯YCZENIA") << endl;
	cout << pl("1. Dodaj") << endl;
	cout << pl("2. Usuñ") << endl;
	cout << pl("3. Przed³u¿") << endl;
	cout << pl("4. Wyszukaj") << endl;
	cout << pl("5. Usuñ") << endl;

	wybor_opcji(nr, 5, true, false);
}
void Menu::m_wyszukiwanie_wypozyczenia(long long nr)
{
	cout << pl("Wyszukaj wypo¿yczenia po:") << endl;
	cout << pl("1. Kliencie") << endl;
	cout << pl("2. Ksi¹¿ce") << endl;
	cout << pl("3. Dacie wypo¿yczenia") << endl;

	wybor_opcji(nr, 3, true, true);
}

void Menu::m_kary(long long nr)
{
	cout << pl("KARY") << endl;
	cout << pl("1. Sp³aæ") << endl;
	cout << pl("2. SprawdŸ kare klienta") << endl;
	cout << pl("3. Wyœwietl wszystkie") << endl;

	wybor_opcji(nr, 3, true, false);
}

void Menu::m_ustawienia(long long nr)
{
	cout << pl("USTAWIENIA") << endl;
	cout << pl("1. Baza danych") << endl;
	cout << pl("2. Nazwa u¿ytkownika") << endl;
	cout << pl("3. Has³o do programu") << endl;

	wybor_opcji(nr, 3, true, false);
}
//#END# metody wyswietlajace poszczegolne menu ########################