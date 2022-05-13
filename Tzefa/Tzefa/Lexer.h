#pragma once
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <exception>
#include <string>
#include <locale>
#include <codecvt>

using std::string;
using std::map;
using std::unordered_map;
using std::vector;
using std::to_string;
using std::wstring;

using convert_type = std::codecvt_utf8<wchar_t>;

#define ALEF_DEC_VALUE 1488
#define TAF_DEC_VALUE 1514

static enum TokenType { OpenParen = 0, CloseParen, OpenBlock, CloseBlock,
	Bigger, Smaller, BiggerEq, SmallerEq, Assign,
	Plus, Minus, Divide, Multiply, Modulo, Power,
	Identifier, StringLiteral, NumberLiteral, False, True, None,
	And, Or, Not, Equals, NotEquals,
	If, Else, While, For, In,
	Print, Return, Break, Continue, Exit,
	Semicolon, NewLine, EndOfFile, Indent, Colon, Comma, Dot,
};


//for all the reserved tokens (words)
static map<wstring, TokenType> reserved{{L"גדול", Bigger},
	{L"גדוש", BiggerEq},
	{L"קטן", Smaller},
	{L"קטש", SmallerEq },

	{L"ועוד", Plus},
	{L"פחות", Minus},
	{L"חלקי", Divide},
	{L"כפול", Multiply},

	{L"הדפס", Print},
	{L"החזר", Return},
	{L"עצור", Break},
	{L"המשך", Continue},
	{L"צא", Exit},

	{L"וגם", And},
	{L"או", Or},
	{L"לא", Not},
	{L"שווה", Equals},
	{L"שונה", NotEquals},

	{L"שגוי", False},
	{L"נכון", True},
	{L"כלום", None},

	{L"אם", If},
	{L"אז", Else},
	{L"עדש", While},
	{L"לכל", For},
	{L"בתוך", In},

	{L";", NewLine},
	{L"=", Assign},
	{L"(", OpenParen},
	{L")", CloseParen}
};


class Token
{
public:
	wstring lexeme;
	TokenType type;
	void* value;
	unsigned int line;
	unsigned int index;

	Token(TokenType type, wstring lexeme, void* value, unsigned int line, unsigned int index) :
		lexeme(std::move(lexeme)), type(type), value(value), line(line), index(index) {}

	string errorSignature();
	static string errorSignature(int line, int index, wstring lexeme = NULL);
};


class Lexer
{
private:
	vector<wstring> _code;
	vector<Token*> _tokens;
	wstring::iterator _start;
	wstring::iterator _iterator;
	wstring::iterator _end;
	unsigned int _index = 1;
	unsigned int _line = 1;

	inline vector<Token*> _scan();
	void addToken(TokenType type, wstring lexeme, void* value = nullptr);

	static bool isDigit(wstring ch);
	static bool isAlpha(wstring ch);
	bool isAlphaStr(wstring ch);

	vector<wstring> split(wstring line);
	wstring retWhenSpace(int sIndex, wstring line);

	void scanNumber(wstring s);

public:
	Lexer(vector<wstring> code);
	vector<Token*> scan();
};

