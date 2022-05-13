#pragma once
#include "Parser.h"
//#include "AST.h"
#include "ErrorManager.h"
#include <map>

using std::pair;
using std::map;

#define VAR_NAME_SIZE 3
//EOL - end of line
#define EOL ';'
//SOB - start of block,  EOB - end of block
#define SOB '{'
#define EOB '}'


//errors:
#define ERR_DEFAULT "phrase doesn't exist."
#define ERR_NO_NAME "user didn't declared a var name."
#define ERR_NO_VAR "user didn't decrared a variable."
#define ERR_NO_VAR_VALUE "user didn't declared a value."

enum var_type { vt_int = 0, vt_char, vt_string };

static map<var_type, wstring> typstr{
	{vt_string, L"%ls" },
	{vt_char, L"%lc"},
	{vt_int, L"%ld"}
};

class Transpiler
{
private:
	map<wstring, pair<wstring, pair<var_type, wstring>>> _existing_vars; //<var in hebrew, <new temp var name>, <type, value>>>
	ParserOutput _p;

	int _currNodeIndex;
	int _currLine;


	wstring cmdcodeGen(AbstractSyntaxTree::CommandStatement* node);
	wstring litcodeGen(AbstractSyntaxTree::LiteralExpression* node);
	wstring varcodeGen(AbstractSyntaxTree::VariableDeclaration* node);
	wstring bincodeGen(AbstractSyntaxTree::BinaryOperatorExpression* node);
	wstring idecodeGen(AbstractSyntaxTree::IdentifierExpression* node);

	//string codeGen(AbstractSyntaxTree::IfStatement* node);

	wstring codeGen(AbstractSyntaxTree::Node* n);


	//helper funcs
	wstring getRandomVarName();
	wstring getKey(wstring value);
	var_type getVarType(wstring str);
	bool isNumber(wstring str);
	bool isChar(wstring str);
	bool isBool(wstring str);

	wstring setString(wstring str);
	wstring setChar(wstring str);

	vector<wstring> startProj();

public:
	Transpiler(ParserOutput p) : _p(p) { _currNodeIndex = 0; _currLine = 0; }
	~Transpiler() = default;

	vector<wstring> codeGen();
};
