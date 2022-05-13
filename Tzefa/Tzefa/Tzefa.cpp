#include <iostream>
#include "FileManager.h"
#include "Lexer.h"
#include "Parser.h"
#include "Transpiler.h"
#include "RunC.h"
#include <fcntl.h> // for _setmode
#include <io.h> // for _setmode
#include <windows.h> // for SetCurrentConsoleFontEx

#define C_FILE L"out.c"

using std::wcout;
using std::endl;
using std::wcin;

void setConsole();
wstring reverseString(wstring line);


int main()
{
    setConsole();

    wstring path;

    // print
    wcout << reverseString(L"ברוכים הבאים לשפת התכנות צפע") << endl;
    wcout << reverseString(L"אנא הכנס את הנתיב לקובץ הקוד שלך:") << endl;
    
    //wcin >> path;
    path = L"a.txt";
   
    vector<wstring> codeLines = FileManager::read(path); //getting the code in hebrew from the code file

    wcout << reverseString(L"...מעבד את הקוד...           ") << endl;
    wcout << L"                            ." << endl;
    wcout << L"                            ." << endl;
    wcout << L"                            ." << endl;

    Lexer lex(codeLines);
    vector<Token*> tokenList = lex.scan(); //gets the translated list of tokens 

    wcout << reverseString(L"הקובץ עובד!                  ") << endl << endl;
    
    wcout << reverseString(L"...מפרסר את הקוד...          ") << endl;
    wcout << L"                            ." << endl;
    wcout << L"                            ." << endl;
    wcout << L"                            ." << endl;

    Parser p(tokenList);
    ParserOutput pOut = p.parse();

    wcout << reverseString(L"הקובץ פורסר!                 ") << endl << endl;
    
    wcout << reverseString(L"...מתרגם את הקוד...          ") << endl;
    wcout << L"                            ." << endl;
    wcout << L"                            ." << endl;
    wcout << L"                            ." << endl;

    Transpiler t(pOut);
    vector<wstring> code = t.codeGen();

    wcout << reverseString(L"הקובץ תורגם!                 ") << endl << endl;
    
    FileManager::write(C_FILE, code);

    wcout << C_FILE << reverseString(L"הקוד המתורגם נמצא בקובץ ") << endl;

    wcout << endl << reverseString(L"הרצה:                        ") << endl;

    RunC::run(C_FILE);

    wcout << endl << endl;
}


void setConsole()
{
    // set the console to support unicode/wchar. 
    _setmode(_fileno(stdout), _O_U16TEXT);

    // tell the console to use a supported font.
    CONSOLE_FONT_INFOEX info = { 0 };
    info.cbSize = sizeof(info);
    info.dwFontSize.Y = 20;
    wcscpy_s(info.FaceName, L"Courier New");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);

}

wstring reverseString(wstring line)
{
    wstring retLine = L"";
    for (int i = line.length() - 1; i >= 0; i--)
    {
        retLine += line[i];
    }
    return retLine;
}