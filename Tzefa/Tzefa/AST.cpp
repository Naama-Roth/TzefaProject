#include "AST.h"
using namespace AbstractSyntaxTree;

void BinaryOperatorExpression::setOperatorToken(Token t)
{
	_operator = t.lexeme;
	_token = new Token(t);
}
