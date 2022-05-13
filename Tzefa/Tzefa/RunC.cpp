#include "RunC.h"

wstring RunC::deleteLastChar(wstring toCut)
{
	toCut.pop_back();
	toCut.pop_back();
	toCut += L".exe";
	return toCut;
}

void RunC::run(wstring fileName)
{
	wstring exeFile = deleteLastChar(fileName);

	wstring command = L"gcc -o " + exeFile + L" " + fileName;

	wstring a = exeFile.c_str();
	system(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(command).c_str());
	system(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(exeFile).c_str());
	
}