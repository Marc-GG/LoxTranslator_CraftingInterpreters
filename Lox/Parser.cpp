#include "Parser.h"
#include "Lox.h"
using namespace std;
	Token Parser::peek() {
		return tokens[current];
	}

	Token Parser::previous() {
		return tokens[current - 1];
	}
	Parser::ParseError Parser::error(Token token, string message) {
		::error(token, message);
		return  ParseError();
	}
	bool Parser::isAtEnd() {
		return peek().type == TokenType::EOFF;
	}

	Parser::Parser(vector<Token> tokens) {
		this->tokens = tokens;
	}

	Expr* Parser::parse() {
		try {
			return expression();
		}
		catch (ParseError error) {
			return nullptr;
		}
	}

	bool Parser::check(TokenType type) {
		if (isAtEnd()) return false;
		return peek().type == type;
	}

	Token Parser::advance() {
		if (!isAtEnd()) current++;
		return previous();
	}

	bool Parser::match(vector<TokenType> types) {
		for (TokenType type : types) {
			if (check(type)) {
				advance();
				return true;
			}
		}
		return false;
	}
	/*
	Panic mode
	*/
	
	void Parser::synchronize() {
		advance();

		while (!isAtEnd()) {
			if (previous().type == TokenType::SEMICOLON) return;

			switch (peek().type) {
			case TokenType::CLASS:
			case TokenType::FUN:
			case TokenType::VAR:
			case TokenType::FOR:
			case TokenType::IF:
			case TokenType::WHILE:
			case TokenType::PRINT:
			case TokenType::RETURN:
				return;

			}
		}
	}

	Token Parser::consume(TokenType type, string message) {
		if (check(type)) return advance();
		throw error(peek(), message);
	}
	
	
	Expr* Parser::primary() {
		if (match({ TokenType::FALSE })) return new Literal(LoxValue{ false });
		if (match({ TokenType::TRUE })) return new Literal(LoxValue{true});
		if (match({ TokenType::NIL })) return new Literal(LoxValue{nullptr});

		if (match({TokenType::NUMBER, TokenType::STRING})) {
			return new Literal(LoxValue{ previous().literal });
		}

		if (match({TokenType::LEFT_PAREN})) {
			Expr* expr = expression();
			consume(TokenType::RIGHT_PAREN, "Expect ')' after expression."); 
			return new Grouping(expr);
		}
		throw error(peek(), "Expect expression.");
	}

	Expr* Parser::unary() {
		if (match({TokenType::BANG, TokenType::MINUS})) {
			Token oper = previous();
			Expr* right = unary();
			return new Unary(oper, right);
		}
		return primary();
	}
	
	Expr* Parser::factor() {
		Expr* expr = unary();

		while (match({TokenType::SLASH, TokenType::STAR})) {
			Token oper = previous();
			Expr* right = unary();
			expr = new Binary(expr, oper, right);
		}

		return expr;
	}
	Expr* Parser::term(){
		Expr* expr = factor();

		while (match({ TokenType::MINUS, TokenType::PLUS })) {
			Token oper = previous();
			Expr* right = factor();
			expr = new Binary(expr, oper, right);
			
		}
		return expr;
	}

	Expr* Parser::comparison() {
		Expr* expr = term();

		while (match({ TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL })) {
			Token oper = previous();
			Expr* right = term();
			expr = new Binary(expr, oper, right);
		}
		return expr;
	}


	Expr* Parser::equality() {
		Expr* expr = comparison();

		while (match({ TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL })) {
			Token oper = previous(); //operator
			Expr* right = comparison();
			expr = new Binary(expr, oper, right);
		}
		return expr;
	}

	Expr* Parser::expression() {
		return equality();
	}

