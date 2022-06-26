#include <string.h>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <variant>
#include <unordered_map>
#include "Token.h"

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
		{TokenType::QUESTION,"QUESTION"},
		{TokenType::COLON, "COLON"},
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
		return std::get<std::string>(x);
	}
	else if (std::holds_alternative<double>(x)) {
		return std::to_string(std::get<double>(x));
	}
	else if (std::holds_alternative<bool>(x)){
		return std::to_string(std::get<bool>(x));
	}
	else //if(std::holds_alternative<std::monostate>(x)) //Otherwise assume monostate or null therefore return "nil"
	{
		return "nil";
	}
}

Token::Token(TokenType type, std::string lexeme, LoxValue literal, int line) {

	this->type = type;
	this->lexeme = lexeme;
	this->literal = literal;
	this->line = line;
}

std::string Token::toString() {
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
	