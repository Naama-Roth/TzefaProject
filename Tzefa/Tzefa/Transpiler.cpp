#include "transpiler.h"
#include <typeinfo>

// "main" code gen func
vector<wstring> Transpiler::codeGen()
{
	_currNodeIndex = 0;

	int i = 0;
	vector<wstring> code;

	code = startProj();

	//we will do it each line seperatly
	for (_currNodeIndex = 0; _currNodeIndex < _p.parsed_data.size(); _currNodeIndex++)
	{
		_currLine = _currNodeIndex + 1;
		try
		{
			code.push_back(codeGen(_p.parsed_data[_currNodeIndex]));
		}
		catch (ErrorManager e)
		{
			throw(e.what());
		}
	}

	code.push_back(L"return 0; }"); //end proj
	code.push_back(L"");

	/*for (int i = 0; i < code.size(); i++)
	{
		std::wcout << "-->  " << code[i] << '\n';
	}*/

	return code;
}


wstring Transpiler::varcodeGen(AbstractSyntaxTree::VariableDeclaration* node)
{
	bool existFlag = false;
	wstring line = L"";
	var_type vtype;
	//if node is null - no variable
	if (node == nullptr)
	{
		throw(new ErrorManager::TranslationError(ERR_NO_VAR, _currLine));
	}

	//if there is no value
	if (node->getLeft() == NULL)
	{
		throw(new ErrorManager::TranslationError(ERR_NO_VAR_VALUE, _currLine));
	}

	wstring value = codeGen(_p.parsed_data[_currNodeIndex]->getRight()); //getting value
	vtype = getVarType(value);


	//checking if exists
	if (_existing_vars.count(node->getIdentifierName()) != 0)
	{
		existFlag = true;
	}


	//checking if the user put int in char, char in int....
	if (existFlag && vtype != (_existing_vars[node->getIdentifierName()]).second.first)
	{
		string wro_type = (vtype == var_type::vt_int ? "int" : (vtype == var_type::vt_char ? "char" : "char array (string)")); //wrong var type
		string rig_type = ((_existing_vars[node->getIdentifierName()]).second.first == var_type::vt_int ? "int" : ((_existing_vars[node->getIdentifierName()]).second.first == var_type::vt_char ? "char" : "char array (string)")); //right var type
		string ero = ("user inserted a " + wro_type + " instead of " + rig_type + ".");
		throw(new ErrorManager::SyntaxError(ero, _currLine));
	}

	if (existFlag)
	{
		_existing_vars[((AbstractSyntaxTree::IdentifierExpression*)node->getLeft())->getIdentifier()].second.second = value;
	}
	else //inserting into vars
	{
		_existing_vars[((AbstractSyntaxTree::IdentifierExpression*)node->getLeft())->getIdentifier()] = { getRandomVarName(), {vtype, value}};
	}


	switch (vtype)
	{
	case var_type::vt_int:
		line = L"int " + _existing_vars[((AbstractSyntaxTree::IdentifierExpression*)node->getLeft())->getIdentifier()].first + L" = " + value + L";";
		break;
	case var_type::vt_char:
		//line = L"wchar_t " + _existing_vars[((AbstractSyntaxTree::IdentifierExpression*)node->getLeft())->getIdentifier()].first + L" = '" + value + L"';";
		line = L"wchar_t " + _existing_vars[((AbstractSyntaxTree::IdentifierExpression*)node->getLeft())->getIdentifier()].first + L" = L" + value + L";";
		break;
	case var_type::vt_string:
		line = L"wchar_t " + _existing_vars[((AbstractSyntaxTree::IdentifierExpression*)node->getLeft())->getIdentifier()].first + L"[] = L" + setString(value) + L";";
		break;
	default:
		break;
	}

	return line;
}

wstring Transpiler::litcodeGen(AbstractSyntaxTree::LiteralExpression* node)
{
	if (node->getLiteralType() == AbstractSyntaxTree::LiteralExpression::int_type)
	{
		return node->getValue();
	}
	return setString(node->getValue());
}

wstring Transpiler::cmdcodeGen(AbstractSyntaxTree::CommandStatement* node)
{
	wstring line = L"";
	wstring value = L"";// codeGen(_p.parsed_data[_currNodeIndex]->getRight()); //getting value
	var_type type;
	//_currNodeIndex++;

	if (node == nullptr)
	{
		throw(new ErrorManager::SyntaxError(ERR_DEFAULT, _currLine));
	}

	if (dynamic_cast<AbstractSyntaxTree::IdentifierExpression*>(node->getRight()))
	{
		if (_existing_vars.count(((AbstractSyntaxTree::IdentifierExpression*)_p.parsed_data[_currNodeIndex]->getRight())->getIdentifier()) == 0)
		{
			throw(new ErrorManager::SyntaxError(ERR_NO_VAR, _currLine));
		}
		value = _existing_vars[((AbstractSyntaxTree::IdentifierExpression*)_p.parsed_data[_currNodeIndex]->getRight())->getIdentifier()].first;
		type = _existing_vars[getKey(value)].second.first;
	}
	else
	{
		value = codeGen(_p.parsed_data[_currNodeIndex]->getRight());
		if (value.size() <= 1)
		{
			//type = var_type::vt_char;
			type = var_type::vt_string;
		}
		else if (value[0] == L'"' && value[value.size() - 1] == L'"')
		{
			type = var_type::vt_string;
		}
		else
		{
			type = var_type::vt_int;
		}
	}

	switch (node->getCommand()->type)
	{
	case TokenType::Print:
		//line = L"wprintf(\"" + typstr[type] + L" \\n\", " + value + L");";
		switch (type)
		{
		case vt_int:
			line = L"wprintf(L\"%ld \\n\", " + value + L");";
			break;
		case vt_char:
			line = L"wprintf(L\"%lc \\n\", " + value + L");";
			break;
		case vt_string:
			line = L"wprintf(L\"%ls \\n\", &" + value + L");";
			break;
		default:
			break;
		}
		
		break;
	case TokenType::Return:
		line = L"return " + value + L";";
		break;
	default:
		throw(new ErrorManager::SyntaxError(ERR_DEFAULT, _currLine));
		break;
	}
	 
	return line;
}

wstring Transpiler::bincodeGen(AbstractSyntaxTree::BinaryOperatorExpression* node)
{
	wstring line = L"";
	line = codeGen(((AbstractSyntaxTree::LiteralExpression*)node->getLeft())) + node->getOperator() + codeGen(((AbstractSyntaxTree::LiteralExpression*)node->getRight()));
	return line;
}

wstring Transpiler::idecodeGen(AbstractSyntaxTree::IdentifierExpression* node)
{
	if (_existing_vars.count(node->getIdentifier()) == 0) //if doesnt exist
	{
		throw(new ErrorManager::SyntaxError(ERR_NO_VAR, _currLine));
	}
	return (_existing_vars[node->getIdentifier()].second.second);
}

wstring Transpiler::codeGen(AbstractSyntaxTree::Node* n)
{
	if (n == nullptr)
	{
		return L"";
	}
	if (n->getNodeType() == AbstractSyntaxTree::Node::node_type::statement)
	{
		if (((AbstractSyntaxTree::Statement*)n)->getStatementType() == AbstractSyntaxTree::Statement::statement_type::expression)
		{
			if (((AbstractSyntaxTree::Expression*)n)->getExpressionType() == AbstractSyntaxTree::Expression::expression_type::literal_expression)
			{
				return litcodeGen((AbstractSyntaxTree::LiteralExpression*)n);
			}
			else if (((AbstractSyntaxTree::BinaryOperatorExpression*)n)->getExpressionType() == AbstractSyntaxTree::BinaryOperatorExpression::expression_type::binary_operation)
			{
				return bincodeGen(((AbstractSyntaxTree::BinaryOperatorExpression*)n));
			}
			else if(((AbstractSyntaxTree::Expression*)n)->getExpressionType() == AbstractSyntaxTree::Expression::expression_type::identifier_expression)
			{
				return idecodeGen(((AbstractSyntaxTree::IdentifierExpression*)n));
			}
		}
		else if (((AbstractSyntaxTree::Statement*)n)->getStatementType() == AbstractSyntaxTree::Statement::statement_type::command_statement)
		{
			return cmdcodeGen((AbstractSyntaxTree::CommandStatement*)n);
		}
	}
	else if (n->getNodeType() == AbstractSyntaxTree::Node::node_type::variable_declaration)
	{
		return varcodeGen((AbstractSyntaxTree::VariableDeclaration*)n);
	}
	return L"";
}



//helper funcs
wstring Transpiler::getRandomVarName()
{
	wstring name = L"";
	for (int i = 0; i < VAR_NAME_SIZE; i++)
	{
		name += ('a' + rand() % 26);
	}
	return name;
}

wstring Transpiler::getKey(wstring value)
{
	for (auto it = _existing_vars.begin(); it != _existing_vars.end(); ++it)
	{
		if (it->second.first == value)
			return it->first;
	}
}

var_type Transpiler::getVarType(wstring str)
{
	if (isNumber(str))
	{
		return var_type::vt_int;
	}
	else if (isChar(str))
	{
		return var_type::vt_char;
	}
	return var_type::vt_string;
}

bool Transpiler::isNumber(wstring str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (!(str[i] - '0' >= 0 && str[i] - '9' <= 9))
			return false;
	}
	return true;
}

bool Transpiler::isChar(wstring str)
{
	return str.size() == 3 && str[0] == '\'' && str[2] == '\'';
}

vector<wstring> Transpiler::startProj()
{
	vector<wstring> proj;
	proj.push_back(L"#include <stdio.h>");
	//proj.push_back(L"#include <stdlib.h>");
	proj.push_back(L"#include <wchar.h>");
	//proj.push_back(L"typedef unsigned short wchar_t ;");
	proj.push_back(L"int main(void) {");

	return proj;
}


wstring Transpiler::setString(wstring str)
{
	wstring nStr = L"";
	/*if (str.size() == 3)
	{
		wchar_t c[3];
		c[0] = '\'';
		c[2] = '\'';
		return wstring(c);
	}*/
	if (str[0] == '\'' && str[2] == '\'' && str.size() == 3)
	{
		return str;
	}
	if (str[0] == L'"' && str[str.size() - 1] == '"')
	{
		for (int i = 1; i < str.size() - 1; i++)
		{
			nStr += str[i];
		}
		return L'"' + nStr + L'"';
	}
	if (str[0] != L'"')
	{
		nStr = L'"' + str;
	}
	if (str[str.size() - 1] != '"')
	{
		nStr = nStr + L'"';
	}
	return nStr;
}

wstring Transpiler::setChar(wstring str)
{
	wchar_t nStr[2];
	if (str.size() == 3)
	{
		nStr[0] = str[1];
		nStr[1] = 0;
		return wstring(nStr);
	}
	return str;
}
