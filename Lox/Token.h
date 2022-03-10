#pragma once
#include <string.h>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <variant>
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

std::string enumString(TokenType token) { //transforms enum characters into string
	std::map<TokenType, std::string> enum_map = {
		{TokenType::LEFT_PAREN, "LEFT_PAREN"},
		{TokenType::RIGHT_PAREN,"RIGHT_PAREN"},
		{TokenType::LEFT_BRACE,"LEFT_BRACE"},
		{TokenType::RIGHT_BRACE,"RIGHT_BRACE"},
		{TokenType::COMMA,"COMMA"},
		{TokenType::DOT,"DOT"},
		{TokenType::MINUS,"MINUS"},
		{TokenType::PLUS,"PLUS"},
		{TokenType::SEMICOLON,"SEMICOLON"},
		{TokenType::SLASH,"SLASH"},
		{TokenType::STAR,"STAR"},
		{TokenType::BANG,"BANG"},
		{TokenType::BANG_EQUAL,"BANG_EQUAL"},
		{TokenType::EQUAL,"EQUAL"},
		{TokenType::EQUAL_EQUAL,"EQUAL"},
		{TokenType::GREATER,"GREATER"},
		{TokenType::GREATER_EQUAL,"GREATER_EQUAL"},
		{TokenType::LESS,"LESS"},
		{TokenType::LESS_EQUAL,"LESS_EQUAL"},
		{TokenType::IDENTIFIER,"IDENTIFIER"},
		{TokenType::STRING,"STRING"},
		{TokenType::NUMBER,"NUMBER"},
		{TokenType::AND,"AND"},
		{TokenType::CLASS,"CLASS"},
		{TokenType::ELSE,"ELSE"},
		{TokenType::FALSE,"FALSE"},
		{TokenType::FUN,"FUN"},
		{TokenType::FOR,"FOR"},
		{TokenType::IF,"IF"},
		{TokenType::NIL,"NIL"},
		{TokenType::OR,"OR"},
		{TokenType::PRINT,"PRINT"},
		{TokenType::RETURN,"RETURN"},
		{TokenType::SUPER,"SUPER"},
		{TokenType::THIS,"THIS"},
		{TokenType::TRUE,"TRUE"},
		{TokenType::VAR,"VAR"},
		{TokenType::WHILE,"WHILE"},
		{TokenType::EOFF,"EOFF"},
	};

	return enum_map[token];
}
using LoxValue = std::variant<std::monostate, double, bool, std::string>;

std::string toString(LoxValue x) {
	if (std::holds_alternative<std::string>(x)) {
		return get<std::string>(x);
	}
	else if (std::holds_alternative<monostate>(x)) {
		return "nil";
	}
	else if (std::holds_alternative<double>(x)) {
		return std::to_string(get<double>(x));
	}
	else if (std::holds_alternative<bool>(x)){
		return std::to_string(get<bool>(x));
	}
}
class Token {

public:
	TokenType type;
	std::string lexeme;
	LoxValue literal;
	int line;

public:
	Token(TokenType type, std::string lexeme, LoxValue literal, int line) {

		this->type = type;
		this->lexeme = lexeme;
		this->literal = literal;
		this->line = line;
	}

	std::string toString() {
		std::string beginningstring = enumString(type) + " " + lexeme + " ";

		if (std::holds_alternative<std::string>(literal)) {

			return  beginningstring + std::get<std::string>(literal);

		}
		else if (std::holds_alternative<double>(literal)) {
			return beginningstring + std::to_string(std::get<double>(literal));

		}
		else if (std::holds_alternative<bool>(literal)) {



			if (std::get<bool>(literal) == true) {

				return beginningstring + "true";

			}

			else

				return beginningstring + "false";



		}
		//Must be NILL
		else
			return beginningstring;


	}

};