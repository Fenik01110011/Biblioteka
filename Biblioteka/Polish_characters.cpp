#include <iostream>
#include <string>
#include <fstream>
#include "Polish_characters.h"

using namespace std;

namespace PL
{
	string pl(string str)
	{
		char IDE_char[18] = { -71, -26, -22, -77, -15, -13, -100, -65, -97, -91, -58, -54, -93, -47, -45, -116, -81, -113 };
		char console_char[18] = { -91, -122, -87, -120, -28, -94, -104, -66, -85, -92, -113, -88, -99, -29, -32, -105, -67, -115 };
		for (int i = 0; i < (int)str.length(); ++i)
			if(str[i] < 0)
				for (int j = 0; j < 18; ++j)
					if (str[i] == IDE_char[j])
						{ str[i] = console_char[j]; break; }
		return str;
	}
	string re_pl(string str)
	{
		char IDE_char[18] = { -71, -26, -22, -77, -15, -13, -100, -65, -97, -91, -58, -54, -93, -47, -45, -116, -81, -113 };
		char console_char[18] = { -91, -122, -87, -120, -28, -94, -104, -66, -85, -92, -113, -88, -99, -29, -32, -105, -67, -115 };
		for (int i = 0; i < (int)str.length(); ++i)
			if (str[i] < 0)
				for (int j = 0; j < 18; ++j)
					if (str[i] == console_char[j])
						{ str[i] = IDE_char[j]; break; }
		return str;
	}
	
	string pl_from_file(string str)
	{
		return pl(str);
	}
	string pl_to_file(string str)
	{
		return re_pl(str);
	}
	string pl_from_file_utf8(string str)
	{
		char file_char[36] = { -60, -123, -60, -121, -60, -103, -59, -126, -59, -124, -61, -77, -59, -101, -59, -68, -59, -70, -60, -124, -60, -122, -60, -104, -59, -127, -59, -125, -61, -109, -59, -102, -59, -69, -59, -71 };
		char console_char[18] = { -91, -122, -87, -120, -28, -94, -104, -66, -85, -92, -113, -88, -99, -29, -32, -105, -67, -115 };
		string new_str = "";

		for (int i = 0; i < (int)str.length(); ++i)
			if (str[i] < 0 && i < (int)str.length() - 1)
			{
				int j;
				for (j = 0; j < 18; ++j)
					if (str[i] == file_char[j * 2] && str[long long(i) + 1] == file_char[j * 2 + 1])
					{
						new_str += console_char[j];
						++i;
						break;
					}
				if (j >= 18) new_str += str[i];
			}
			else
				new_str += str[i];
		return new_str;
	}
	string pl_to_file_utf8(string str)
	{
		char file_char[36] = { -60, -123, -60, -121, -60, -103, -59, -126, -59, -124, -61, -77, -59, -101, -59, -68, -59, -70, -60, -124, -60, -122, -60, -104, -59, -127, -59, -125, -61, -109, -59, -102, -59, -69, -59, -71 };
		char console_char[18] = { -91, -122, -87, -120, -28, -94, -104, -66, -85, -92, -113, -88, -99, -29, -32, -105, -67, -115 };
		string new_str = "";

		for (int i = 0; i < (int)str.length(); ++i)
			if (str[i] < 0)
			{
				int j;
				for (j = 0; j < 18; ++j)
					if (str[i] == console_char[j])
					{
						new_str += file_char[j * 2];
						new_str += file_char[j * 2 + 1];
						break;
					}
				if (j >= 18) new_str += str[i];
			}
			else
				new_str += str[i];
		return new_str;
	}

	string pl_from_mysql(string str)
	{
		char console_char[18] = { -91, -122, -87, -120, -28, -94, -104, -66, -85, -92, -113, -88, -99, -29, -32, -105, -67, -115 };
		char MySQL_char[18] = { -79, -26, -22, -77, -15, -13, -74, -65, -68, -95, -58, -54, -93, -47, -45, -90, -81, -84 };
		for (int i = 0; i < (int)str.length(); ++i)
			if (str[i] < 0)
				for (int j = 0; j < 18; ++j)
					if (str[i] == MySQL_char[j]) { str[i] = console_char[j]; break; }
		return str;
	}
	string pl_to_mysql(string str)
	{
		char console_char[18] = { -91, -122, -87, -120, -28, -94, -104, -66, -85, -92, -113, -88, -99, -29, -32, -105, -67, -115 };
		char MySQL_char[18] = { -79, -26, -22, -77, -15, -13, -74, -65, -68, -95, -58, -54, -93, -47, -45, -90, -81, -84 };
		for (int i = 0; i < (int)str.length(); ++i)
			if(str[i] < 0)
				for (int j = 0; j < 18; ++j)
					if (str[i] == console_char[j]) { str[i] = MySQL_char[j]; break; }
		return str;
	}
	
	wstring wpl(wstring wstr)
	{
		wchar_t c[] = L"\245\206\251\210\344\242\230\276\253\244\217\250\235\343\340\227\275\215¹æê³ñóœ¿Ÿ¥ÆÊ£ÑÓŒ¯";
		for (int i = 0; i < (int)wstr.length(); ++i)
			for (int j = 0; j < 18; ++j)
				if (wstr[i] == c[j + 18]) { wstr[i] = c[j]; break; }
		return wstr;
	}
	string wstring_to_string(wstring wstr)
	{
		wstr = wpl(wstr);
		string str = "";
		for (std::wstring::size_type i = 0; i < wstr.size(); i++)
			str += (char)wstr[i];
		return str;
	}
	wstring string_to_wstring(string str)
	{
		wstring wstr = L"";
		for (std::string::size_type i = 0; i < str.size(); i++)
			wstr += (wchar_t)str[i];
		return wstr;
	}
}