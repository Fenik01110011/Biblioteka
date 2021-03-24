#pragma once
#include <iostream>

namespace PL
{
	std::string pl(std::string str);
	std::string re_pl(std::string str);
	
	std::string pl_from_file(std::string str);
	//tekst z Visual Studio zapisywany jest normalnie do plikow
	//przez funkcje pl_to_file() trzeba przepuscic dane pobrane od uzytkownika 
	//lub tekst ktory wczesniej zostal przepuszczony przez ktoras z funkcji pl()
	std::string pl_to_file(std::string str);
	std::string pl_from_file_utf8(std::string str);
	std::string pl_to_file_utf8(std::string str);
	
	std::string pl_from_mysql(std::string str);
	//przyjmuje tekst ktory wczesniej zostal przepuszczony przez jedna z funkcji pl() lub pobrany od uzytkownika
	std::string pl_to_mysql(std::string str);

	std::wstring wpl(std::wstring wstr);
	std::string wstring_to_string(std::wstring wstr);
	std::wstring string_to_wstring(std::string str);
}