#pragma once
#include <string.h>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <variant>
#include <unordered_map>
enum class TokenType {

	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

	// One or two character tokens.
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	// Literals.
	IDENTIFIER, STRING, NUMBER,

	// Keywords.
	AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
	PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

	EOFF
};

std::string enumString(TokenType token);

using LoxValue = std::variant<std::monostate, double, bool, std::string>;

std::string toString(LoxValue x);
class Token {

public:
	TokenType type;
	std::string lexeme;
	LoxValue literal;
	int line;

public:
	Token(TokenType type, std::string lexeme, LoxValue literal, int line);

	std::string toString();
};