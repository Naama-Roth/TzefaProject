#include "Parser.h"


//lexer will return vector with every single word.
/*
vector:
int
i
=
x
;
/n
i
=
i
*
3
;
/n
.........
*/

ParserOutput Parser::parse()
{
	//Parser p(tokens);
	AbstractSyntaxTree::Node* tree = new AbstractSyntaxTree::Node(); //temp
	AbstractSyntaxTree::Node* node = nullptr;
	AbstractSyntaxTree::Node* temp = tree;
	AbstractSyntaxTree::Node* st = tree;
	ParserOutput result;

	Token* currentToken = getCurrToken();
	int currentLine = currentToken->line;

	AbstractSyntaxTree::Expression* expression;
	AbstractSyntaxTree::CommandStatement* cmd;
	AbstractSyntaxTree::VariableDeclaration* var;
	AbstractSyntaxTree::BinaryOperatorExpression* binOp;
	AbstractSyntaxTree::IdentifierExpression* iden;

	bool com = false;
	bool flag = false;
	bool first = true;

	//we will parse one line at a time
	//while ((node = p.parseNext()) != nullptr && currentToken != nullptr)
	while (currentToken->type != EndOfFile)
	{
		while (currentToken->type != EndOfFile && currentLine == currentToken->line)
		{
			switch (currentToken->type)
			{
			case Identifier:
				iden = parseIdentifierExpression();
				node = iden;
				//com = true;
				break;

			case NumberLiteral:
				node = new AbstractSyntaxTree::LiteralInt(std::atoi(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(currentToken->lexeme).c_str()));
				break;
			case StringLiteral:
			case OpenParen:
			case CloseParen:
				node = new AbstractSyntaxTree::LiteralString(currentToken->lexeme);
				break;
				//need to add in lexer char

			case True:
				node = new AbstractSyntaxTree::LiteralBool(true);
				break;
			case False:
				node = new AbstractSyntaxTree::LiteralBool(false);
				break;

				//all the operators
			case Equals:
			case Plus:
			case Minus:
			case Multiply:
			case Divide:
			case Modulo:
			case Power:
			case Bigger:
			case BiggerEq:
			case Smaller:
			case SmallerEq:
			case And:
			case Or:
			case Not:
			case NotEquals:
				com = true;
				binOp = new AbstractSyntaxTree::BinaryOperatorExpression();
				binOp->setOperatorToken(*currentToken);
				binOp->setOperator(currentToken->lexeme);
				node = binOp;
				break;
			case Assign:
				var = parseVariableDeclaration();
				node = var;
				com = true;
				break;
				//all commands
			case Print:
			case Return:
				//the second var in command statement builder is for return value
				node = parseCommandStatement(currentToken);
				com = true;
				break;
			default:
				break;
			}

			temp = tree;

			while (true)
			{
				if (com)
				{
					if (!first)
					{
						if (flag)
						{
							st = temp;
							flag = false;
						}
						while (temp->getRight() != nullptr)
						{
							temp = temp->getRight();
						}
						temp->setRight(node);
					}
					else
					{
						node->setLeft(tree->getLeft());
						tree = node;
						first = false;
					}
					break;
				}
				else if (temp->getLeft() == nullptr)
				{
					temp->setLeft(node);
					if (flag)
					{
						flag = false;
						st = temp;
					}
					break;
				}
				else
				{
					if (temp->getRight() == nullptr)
					{
						temp->setRight(node);
						if (flag)
						{
							flag = false;
							st = temp;
						}
						break;
					}
					else
					{
						if (flag)
						{
							flag = false;
							st = temp;
						}
						temp = temp->getLeft();
					}
				}
			}
			com = false;
			_currentPos++;
			currentToken = getCurrToken();
		}

		first = true;
		currentLine = currentToken->line;
		result.parsed_data.push_back(tree);
		tree = new AbstractSyntaxTree::Node();
	}

		
	return result;
}


Token* Parser::getCurrToken()
{
	return (_currentPos < _tokens.size()) ? _tokens[_currentPos] : nullptr;
}


//helper func
AbstractSyntaxTree::VariableDeclaration* Parser::parseVariableDeclaration()
{
	Token* currentToken = getCurrToken();


	//since the parser dont know the language, we dont need to know what the type (int, string, char...)

	//create type expression
	AbstractSyntaxTree::TypeExpression* type = new AbstractSyntaxTree::TypeExpression(AbstractSyntaxTree::TypeExpression::type_expression_category::simple_type); //curently we only have simple types (int, string...)


	//the next part will be the name of the expression
	wstring name = currentToken->lexeme;

	//now we'll make the variable itself
	AbstractSyntaxTree::VariableDeclaration* var = new AbstractSyntaxTree::VariableDeclaration();
	var->setIdentifierName(name);
	var->setType(type);

	return var;
}

AbstractSyntaxTree::CommandStatement* Parser::parseCommandStatement(Token* currentTok)
{
	//we are going to parse next node
	AbstractSyntaxTree::CommandStatement* cmd = new AbstractSyntaxTree::CommandStatement(currentTok, nullptr);

	return cmd;
}

AbstractSyntaxTree::IdentifierExpression* Parser::parseIdentifierExpression()
{
	Token* currTok = getCurrToken();
	if (currTok == nullptr)
	{
		return nullptr;
	}
	return new AbstractSyntaxTree::IdentifierExpression(currTok->lexeme);
}
