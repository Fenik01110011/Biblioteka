#include "Global.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Polish_characters.h"

using namespace std;
using namespace PL;

char Global::pause()
{
	Sleep(500);
	cout << pl("Naci�nij dowolny klawisz, aby kontynuowa�...");
	return _getch();
}