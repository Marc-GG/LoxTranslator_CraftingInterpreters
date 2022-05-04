#pragma once
#include <string>
#include "Token.h"
#include "Expr.h"
#include <vector>
#include <stdexcept>

using std::vector, std::runtime_error, std::string;
class Parser {
public:
	Parser(std::vector<Token> tokens);
	Expr* parse();
	class ParseError : public std::runtime_error {
	public:
		ParseError() : std::runtime_error("") {}
	};
private:
	std::vector<Token> tokens;
	int current = 0;
	Token peek();
	Token previous();
	ParseError error(Token token, string message); 
	bool isAtEnd(); 

	bool check(TokenType type);
	Token advance();
	bool match(vector<TokenType> types);
	/*
	Panic mode
	*/

	void synchronize();
	Token consume(TokenType type, string message);
	Expr* primary();
	Expr* unary();
	Expr* factor();
	Expr* term();
	Expr* comparison();
	Expr* equality();
	Expr* expression();
};