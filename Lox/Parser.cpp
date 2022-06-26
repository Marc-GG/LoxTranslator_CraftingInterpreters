#include "Parser.h"
#include "Lox.h"
#include "Stmt.h"
#include <typeinfo>
#include <any>
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
	//Expr
	
	//Expr* Parser::parse() {
	//	try {
	//		return expression();
	//	}
	//	catch (ParseError error) {
	//		return nullptr;
	//	}
	//}
	
	
	Stmt* Parser::printStatement() {
		Expr* value = expression();
		consume(TokenType::SEMICOLON, "Expect ';' after value.");
		return new Print(value); //Ask dakota about it
	}

	Stmt* Parser::expressionStatement() {
		Expr* expr = expression();
		consume(TokenType::SEMICOLON, "Expect ';' after expression.");
		return new Expression(expr);
	}

	Stmt* Parser::ifStatement() {
		consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
		Expr* condition = expression();
		consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

		Stmt* thenBranch = statement();
		Stmt* elseBranch = nullptr;
		if (match({ TokenType::ELSE })) {
			elseBranch = statement();
		}
		return new If(condition, thenBranch, elseBranch);
	}

	Stmt* Parser::whileStatement() {
		consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
		Expr* condition = expression();
		consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
		Stmt* body = statement();

		return new While(condition, body);
	}

	Stmt* Parser::forStatement() {
		consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

		Stmt* initializer;
		if (match({TokenType::SEMICOLON})) {
			initializer = nullptr;
		}
		else if (match({ TokenType::VAR })) {
			initializer = varDeclaration();
		}
		else {
			initializer = expressionStatement();
		}

		Expr* condition = nullptr;
		if (!check(TokenType::SEMICOLON)) {
			condition = expression();
		}
		consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

		Expr* increment = nullptr;
		if (!check(TokenType::RIGHT_PAREN)) {
			increment = expression();
		}
		consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");
		Stmt* body = statement();

		if (increment != nullptr) {
			body = new Block(vector<Stmt*>{body, new Expression(increment)});
		}
		if (condition == nullptr) condition = new Literal(true);
		body = new While(condition, body);

		if (initializer != nullptr) {
			body = new Block(vector<Stmt*>{initializer, body});
		}
		return body;
	}

	Expr* Parser::And() {
		Expr* expr = equality();

		while (match({ TokenType::AND })) {
			Token oper = previous();
			Expr* right = equality();
			expr = new Logical(expr, oper, right);
		}
		return expr;
	}

	Expr* Parser::Or() {
		Expr* expr = And();

		while (match({ TokenType::OR })) {
			Token oper = previous();
			Expr* right = And();
			expr = new Logical(expr, oper, right);
		}
		return expr;
	}

	Expr* Parser::assignment() {
		//Expr* expr = equality();
		Expr* expr = Or();
		if (match({TokenType::EQUAL})) {
			Token equals = previous();
			Expr* value = assignment();

			if (typeid(expr) == typeid(Variable)) {
				Token name = dynamic_cast<Variable*>(expr)->name;
				return new Assign(name, value);
			}
			error(equals, "Invalid assignment target.");
		}
		return expr;
	}

	vector<Stmt*> Parser::block() {
		vector<Stmt*> statements;

		while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
			statements.push_back(declaration());
		}
		consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
		return statements;
	}

	Stmt* Parser::statement() {
		if (match({ TokenType::FOR })) return forStatement();
		if (match({ TokenType::IF })) return ifStatement();
		if (match({ TokenType::PRINT })) return printStatement();
		if (match({ TokenType::WHILE })) return whileStatement();
		if (match({ TokenType::LEFT_BRACE })) return new Block(block());
		return expressionStatement();
	}

	Stmt* Parser::varDeclaration() {
		Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

		Expr* initializer = nullptr;
		if (match({TokenType::EQUAL})) {
			initializer = expression();
		}
		consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
		return new Var(name, initializer);
	}
	
	Stmt* Parser::declaration() {
		try {
			if (match({ TokenType::VAR })) return varDeclaration();

			return statement();
		}
		catch (ParseError error) {
			synchronize();
			return nullptr;
		}
	}

	//Stmt
	//------------------------------------------------------------------------------------
	vector<Stmt*> Parser::parse() {
		vector<Stmt*> statements;
		while (!isAtEnd()) {
			//statements.push_back(statement());
			statements.push_back(declaration());
		}
		return statements;
	}
	
	//------------------------------------------------------------------------------------
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
		if (match({ TokenType::FALSE })) return  new Literal(LoxValue{false});
		if (match({ TokenType::TRUE })) return new Literal(LoxValue{true});
		if (match({ TokenType::NIL })) return new Literal(LoxValue{nullptr});

		if (match({TokenType::NUMBER, TokenType::STRING})) {
			return new Literal(LoxValue{ previous().literal });
		}

		if (match({ TokenType::IDENTIFIER })) {
			return new Variable(previous());
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
		//return equality();
		return assignment();
	}




