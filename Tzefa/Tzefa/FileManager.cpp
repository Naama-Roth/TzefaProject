#include "FileManager.h"

#include <fstream>
#include <locale>
#include <codecvt>

vector<wstring> FileManager::read(wstring fileName)
{
    wfstream file;
    file.open(fileName, std::ios::in);

    file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

    vector<wstring> fileContent;

    if (file.is_open())
    {
        wstring line;
        while (file.good())
        {
            getline(file, line);
            fileContent.push_back(line);
        }
        file.close();
    }
    else
    {
        throw("Couldn't open the code file");
    }

    return fileContent;
}

void FileManager::write(wstring fileName, vector<wstring> code)
{
    const locale utf8_locale = locale(locale(), new std::codecvt_utf8<wchar_t>());

    std::wfstream file;
    file.open(fileName, std::ios::out);

    if (file.is_open())
    {
        file.imbue(utf8_locale);

        for (size_t i = 0; i < code.size(); i++)
        {
            file << code[i] << '\n';
            std::wcout << code[i] << '\n';
        }
        
    }
    else
    {
        throw("Couldn't open the code file");
    }
    file.close();
}