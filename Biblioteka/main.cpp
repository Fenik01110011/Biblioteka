#include "Menu.h"
#include <iostream>
#include "Global.h"

int main()
{
	Menu biblioteka;
	while (true)
		biblioteka.spis_menu(biblioteka.nr_m);

	std::system("pause");
	return 0;
}