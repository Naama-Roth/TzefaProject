#include <iostream>
#include <fstream>
#include <Vector>
#include <string>
#include <exception>
#include <locale>
#include <codecvt>

using std::locale;
using std::vector;
using std::string;
using std::wfstream;
using std::wstring;



class FileManager
{
public:
	static vector<wstring> read(wstring fileName);
	static void write(wstring fileName, vector<wstring> code);
};