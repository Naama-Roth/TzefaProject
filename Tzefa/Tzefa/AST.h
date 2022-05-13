#pragma once

#include <iostream>
#include <vector>
#include "Lexer.h"

using std::vector;

namespace AbstractSyntaxTree
{
	class Expression;

	class Node
	{
	private:
		Node* _left;
		Node* _right;

	public:
		enum class node_type { null, type_expression, statement, function_definition, variable_declaration, type_definition };

		Node() = default;
		virtual ~Node() = default;

		Node* getLeft() { return _left; }
		Node* getRight() { return _right; }

		void setLeft(Node* n) { _left = n; }
		void setRight(Node* n) { _right = n; }


		virtual node_type getNodeType() const { return node_type::null; }
		virtual void print() {}
	};


	//this class represents type expressions (int, duble, wstring, array, pointer...)
	class TypeExpression : public Node
	{
	public:
		enum class type_expression_category { simple_type, array_type }; //later we will add arrays, pointers...

		TypeExpression() : Node() {}
		TypeExpression(type_expression_category type) : Node(), typeEC(type) {}
		virtual ~TypeExpression() = default;

		virtual node_type getNodeType() const { return node_type::type_expression; }
		virtual type_expression_category getTypeExpressionCategory() const { return typeEC; }
		virtual void print() const {};
	private:
		type_expression_category typeEC;
	};


	//this class represents statements - commands to be executed (int x, x += 5...)
	class Statement : public Node
	{
	private:
		bool _has_return = false;

	public:
		enum class statement_type { conditional_statement, if_statement, while_statement, for_statement, variable_declaration, return_statement, command_statement, expression }; //if else, while, for, x = <var>, return <var>, print(...), (x + 5, - 4, / 6...) ...

		Statement() : Node() {}
		virtual ~Statement() = default;

		virtual node_type getNodeType() const { return node_type::statement; }
		virtual statement_type getStatementType() const = 0;

		void setHasReturn(bool has_return) { this->_has_return = has_return; }
		bool getHasReturn() { return this->_has_return; }
		virtual void print() const { std::cout << _has_return; };
	};

	// this class represents command like print, return...
	class CommandStatement : public Statement
	{
	private:
		Token* _command;
		Expression* _expression;

	public:
		CommandStatement(Token* command, Expression* expression) : _command(command), _expression(expression) {};
		virtual ~CommandStatement() { delete(_expression); delete(_command); }

		virtual statement_type getStatementType() const { return statement_type::command_statement; }
		Token* getCommand() const { return _command; }
		Expression* getExpression() const { return _expression; }
	};


	// this class represents expressions which turns into a value (x + 5, - 4, / 6...)
	class Expression : public Statement
	{
	public:
		enum class expression_type { unary_operation, expression_list, binary_operation, identifier_expression, literal_expression, cast_expression, vargs_expression, type_instantiation };
		//unary operation are operations you "cant see" -> i++ and not i += 1, --5, !true...

		Expression() : Statement() {}
		virtual ~Expression() = default;

		virtual statement_type getStatementType() const { return statement_type::expression; }
		virtual expression_type getExpressionType() const = 0;
		virtual void print() const {};
	};

	// this class represents a name of a variable (indentifier)
	class IdentifierExpression : public Expression
	{
	private:
		std::wstring _identifier;

	public:
		IdentifierExpression() : Expression() {}
		IdentifierExpression(std::wstring name) : Expression(), _identifier(name) {}
		virtual ~IdentifierExpression() = default;

		std::wstring getIdentifier() const { return _identifier; }
		void setIdentifier(std::wstring iden) { _identifier = iden; }

		virtual expression_type getExpressionType() const { return expression_type::identifier_expression; }
		virtual void print() const { std::wcout << _identifier; };
	};
	//this class represents a binary operator (!=, =, -, +)
	class BinaryOperatorExpression : public Expression
	{
	private:
		std::wstring _operator;
		Token* _token;

	public:
		BinaryOperatorExpression() : Expression() {}
		BinaryOperatorExpression(std::wstring opr) : Expression(), _operator(opr) {}
		virtual ~BinaryOperatorExpression() = default;

		Token* getOperatorToken() { return _token; }
		void setOperatorToken(Token t);

		std::wstring getOperator() const { return _operator; }
		void setOperator(std::wstring iden) { _operator = iden; }

		virtual expression_type getExpressionType() const { return expression_type::binary_operation; }
		virtual void print() const { std::wcout << _operator; };
	};



	// this class represents unary operator expression: x++, --5, !true
	class UnaryOperatorExpression : public Expression
	{
	private:
		std::wstring _operator;
		Expression* _expression = nullptr;

	public:
		UnaryOperatorExpression() = default;
		virtual ~UnaryOperatorExpression() { delete(_expression); }

		virtual expression_type getExpressionType() const { return expression_type::unary_operation; }
		void setExpression(Expression* expression) { _expression = expression; }
		void setOperator(std::wstring op) { this->_operator = op; }

		std::wstring getOperator() const { return this->_operator; }
		Expression* getExpression() const { return this->_expression; }

		virtual void print() const { std::wcout << _operator << ", "; _expression->print(); };
	};



	class LiteralExpression : public Expression
	{
	public:
		LiteralExpression() : Expression() {};
		virtual ~LiteralExpression() = default;

		enum literal_type { int_type, string_type, char_type, bool_type, };

		virtual expression_type getExpressionType() const { return expression_type::literal_expression; }
		void setExpression(Expression* expression);

		//virtual funcsions
		virtual literal_type getLiteralType() const = 0;
		virtual wstring getValue() const = 0;
		virtual void print() const { };
	};

	//create int
	class LiteralInt : public LiteralExpression
	{
	private:
		int _value;
		const literal_type _ltype = int_type;

	public:
		LiteralInt(int val) : LiteralExpression(), _value(val) {}
		virtual ~LiteralInt() = default;

		//virtual funcsions
		virtual literal_type getLiteralType() const { return _ltype; }
		virtual wstring getValue() const { return std::to_wstring(_value); }
		virtual void print() const { std::cout << _value; };
	};

	class LiteralString : public LiteralExpression
	{
	private:
		wstring _value;
		const literal_type _ltype = string_type;

	public:
		LiteralString(wstring val) : LiteralExpression(), _value(val) {}
		virtual ~LiteralString() = default;

		//virtual funcsions
		virtual literal_type getLiteralType() const { return _ltype; }
		virtual wstring getValue() const { return _value; }
		virtual void print() const { std::wcout << _value; };
	};

	class LiteralChar : public LiteralExpression
	{
	private:
		char _value;
		const literal_type _ltype = string_type;

	public:
		LiteralChar(char val) : LiteralExpression(), _value(val) {}
		virtual ~LiteralChar() = default;

		//virtual funcsions
		virtual literal_type getLiteralType() const { return _ltype; }
		virtual wstring getValue() const { return wstring(1, _value); }
		virtual void print() const { std::cout << _value; };
	};

	class LiteralBool : public LiteralExpression
	{
	private:
		bool _value;
		const literal_type _ltype = bool_type;

	public:
		LiteralBool(bool val) : LiteralExpression(), _value(val) {}
		virtual ~LiteralBool() = default;

		//virtual funcsions
		virtual literal_type getLiteralType() const { return _ltype; }
		virtual wstring getValue() const { return _value ? L"true" : L"false"; }
		virtual void print() const { std::cout << _value; };
	};



	// This class represents a list of expressions - (5, 7, 4), (x, 7, "abc", 'a'), (!true, x = 3...)
	class ExpressionList : public Expression {
	private:
		std::vector<Expression*> _expressions;

	public:
		ExpressionList() = default;
		virtual ~ExpressionList();

		virtual expression_type getExpressionType() const { return expression_type::expression_list; }

		void addExpression(Expression* expression) { this->_expressions.push_back(expression); }
		const vector<Expression*>& getExpressions() const { return this->_expressions; }

		void setExpression(int position, Expression* expr)
		{
			if (position >= this->_expressions.size())
			{
				return;
			}
			this->_expressions[position] = expr;
		}

		virtual void print() const { for (Expression* e : _expressions) { e->print(); } }
	};


	/*class IfStatement : public Statement
	{
	private:
		Node* _condition;
		Statement* _statement; //inside the if
		map<Expression*, Statement*> _elifs;
		Statement* _elseAction;

	public:
		IfStatement(Node* condition, Statement* statement, map<Expression*, Statement*> elifs,
			Statement* elseAction) : _condition(condition), _statement(statement), _elifs(elifs), _elseAction(elseAction) {};

		virtual statement_type getStatementType() const { return statement_type::if_statement; }

		Node* getExpression() const { return _condition; }
		Statement* getStatement() const { return _statement; }
		map<Expression*, Statement*> getElifs() const { return _elifs; }
		Statement* getElse() const { return _elseAction; }

		void setExpression(Node* exp) { _condition = exp; }
		void setStatement(Statement* stmnt) { _statement = stmnt; }
	};
	*/

	// this class represents a variable (int a, wstring str, char g...)
	class VariableDeclaration
		: public Statement
	{
	private:
		wstring _varName;
		TypeExpression* _varType;

	public:
		VariableDeclaration() : Statement() { _varName = L""; _varType = nullptr; }
		~VariableDeclaration() { delete(_varType); }


		TypeExpression* getType() const { return _varType; }
		wstring getIdentifierName() const { return _varName; }
		void setType(TypeExpression* type) { _varType = type; }
		void setIdentifierName(wstring name) { _varName = name; }

		virtual node_type getNodeType() const { return node_type::variable_declaration; }
		virtual statement_type getStatementType() const { return Statement::statement_type::variable_declaration; }
		virtual void print() const { std::wcout << _varName << L", "; _varType->print(); };
	};

}
