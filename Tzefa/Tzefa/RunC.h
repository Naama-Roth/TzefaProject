#pragma once
#include <iostream>
#include <stdio.h>
#include <locale>
#include <codecvt>

using std::string;
using std::wstring;

using convert_type = std::codecvt_utf8<wchar_t>;

class RunC
{
public:
	static void run(wstring fileName);

private:
	static wstring deleteLastChar(wstring toCut);
};