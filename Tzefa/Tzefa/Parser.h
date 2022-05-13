#pragma once

#include "AST.h"
//#include "Lexer.h"


typedef struct ParserOutput
{
	vector<AbstractSyntaxTree::Node*> parsed_data;
} ParserOutput;



static class Parser
{
private:
	vector<Token*> _tokens;
	int _currentPos; //current index in token vector


	//AbstractSyntaxTree::Node* parseNext();

	Token* getCurrToken();

	/*bool match(TokenType type, int offset);
	bool nextMatch(TokenType type);*/


	//helper func:
	AbstractSyntaxTree::VariableDeclaration* parseVariableDeclaration();
	AbstractSyntaxTree::CommandStatement* parseCommandStatement(Token* currentTok);
	AbstractSyntaxTree::IdentifierExpression* parseIdentifierExpression();
	//AbstractSyntaxTree::IfStatement* parseIfStatement();

public:
	Parser(vector<Token*> t) : _tokens(t) { _currentPos = 0; };
	ParserOutput parse();
	//const vector<Token*>& tokens
};
