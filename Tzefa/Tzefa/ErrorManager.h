#pragma once
#include <exception>
#include <iostream>
#include <string>

using std::string;

class ErrorManager : public std::exception
{
public:
	class TranslationError
	{
	private:
		string _error;
		int _line;
		string _subMessage;

	public:
		TranslationError(const string error, int line, const string subMessage = "") : _error(error), _line(line), _subMessage(subMessage) {}
		~TranslationError() {}

		const char* what() { return (string("Syntax Error: ") += _error, std::to_string(_line), (_subMessage.size() == 0 ? _error : _subMessage)).c_str(); }
	};

	static class ParserError
	{
	private:
		string _error;
		int _line;
		string _subMessage;

	public:
		ParserError(const string error, int line, const string subMessage = "") : _error(error), _line(line), _subMessage(subMessage) {}
		~ParserError() {}

		const char* what() { return (string("Parser Error: ") += _error, std::to_string(_line), (_subMessage.size() == 0 ? _error : _subMessage)).c_str(); }
	};

	class LexerError
	{
	private:
		string _error;
		int _line;
		string _subMessage;

	public:
		LexerError(const string error, int line, const string subMessage = "") : _error(error), _line(line), _subMessage(subMessage) {}
		~LexerError() {}

		const char* what() { return (string("Lexer Error: ") += _error, std::to_string(_line), (_subMessage.size() == 0 ? _error : _subMessage)).c_str(); }
	};

	class SyntaxError
	{
	private:
		string _error;
		int _line;
		string _subMessage;

	public:
		SyntaxError(const string error, int line, const string subMessage = "") : _error(error), _line(line), _subMessage(subMessage) {}
		~SyntaxError() {}

		const char* what() { return (string("Syntax Error: ") += _error, std::to_string(_line), (_subMessage.size() == 0 ? _error : _subMessage)).c_str(); }
	};
};
