#include "Lexer.h"



bool Lexer::isDigit(wstring ch)
{
    for (int i = 0; i < ch.size(); i++)
    {
        if (!(ch[i] >= '0' && ch[i] <= '9'))
            return false;
    }
    return true;
}

bool Lexer::isAlpha(wstring ch)
{
    for (int i = 0; i < ch.size(); i++)
    {
        if ((ch[i] == '\'' || ch[i] == '"') && (i == 0 || i == 1))
        {
            ;
        }
        else if (!(ch[i] >= ALEF_DEC_VALUE && ch[i] <= TAF_DEC_VALUE))
            return false;
    }
    return true;
}

bool Lexer::isAlphaStr(wstring ch)
{
    if ((ch[0] == '\'' && ch[ch.size() - 1] == '\'') || (ch[0] == '"' && ch[ch.size() - 1] == '"'))
    {
        return isAlpha(ch.substr(1, ch.size() - 2)) || isDigit(ch.substr(1, ch.size() - 2));
    }
    return false;
}

void Lexer::scanNumber(wstring s)
{
    int place = _index;
    wstring numberString = L"";
    if (!isDigit(s))
    {

    }

    addToken(NumberLiteral, numberString, new double(atof(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(numberString).c_str())));
}

Lexer::Lexer(vector<wstring> code)
{
    _code = code;

    reserved[L"גדול"] = Bigger;
}

vector<Token*> Lexer::scan()
{
    try
    {
        return _scan();
    }
    catch (std::exception& e)
    {
        throw("prob");
    }
}

inline vector<Token*> Lexer::_scan()
{
    wstring line = L"";
    vector<wstring> sl;
    int sind = 0;

    for (size_t i = 0; i < _code.size(); i++, _line++)
    {
        line = _code[i];
        sl = split(line);

        for (size_t j = 0; j < sl.size(); j++)
        {
            if (reserved.find(sl[j]) == reserved.end())
            {
                if (isDigit(sl[j]))
                {
                    addToken(NumberLiteral, sl[j], new double(atof(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(sl[j]).c_str())));
                }
                else if (isAlphaStr(sl[j]))
                {
                    addToken(StringLiteral, sl[j]);
                }
                else if (isAlpha(sl[j]))
                {
                    addToken(Identifier, sl[j]);
                }
            }
            else if (reserved[sl[j]] == TokenType::NewLine)
            {
                addToken(NewLine, sl[j]);
                _index = 1;
                sind = 0;
            }
            else
            {
                addToken(reserved[sl[j]], sl[j]);
            }
            _index += sl[j].size();
        }
    }

    addToken(EndOfFile, L"");
    return _tokens;
}

void Lexer::addToken(TokenType type, wstring lexeme, void* value)
{
    _tokens.emplace_back(new Token(type, lexeme, value, _line, _index));
}

vector<wstring> Lexer::split(wstring line)
{
    vector<wstring> spline;
    wstring part = L"";
    int index = 0, endpindex;
    while (index < line.size())
    {
        if (line[index] == ' ')
        {
            index++;
        }
        else
        {
            part = retWhenSpace(index, line);
            if (!isDigit(part))
            {
                spline.push_back(part);
            }
            else
            {
                spline.push_back(part);
            }
            index += part.size();
        }
        
    }
    return spline;
}

wstring Lexer::retWhenSpace(int sIndex, wstring line)
{
    wstring s = L"";
    int i = sIndex;

    if (line[sIndex] == L';')
    {
        return L";";
    }
    while (i < line.size() && line[i] != ' ')
    {
        if (line[i] == L';')
        {
            return s;
        }
        s += line[i];
        i++;
    }
    return s;
}
