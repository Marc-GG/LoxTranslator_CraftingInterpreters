#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <map>
#include <variant>
#include <filesystem>
#include "Expr.h"
#include "AstPrinter.h"

using namespace std;

class AstPrinter : public Visitor {
	string result;
	string parenthesize(string name, vector<const Expr*> exprs) {  //Expr... exprs Define the second parameter
		string builder = "(" + name;
		for (auto expr : exprs) {
			builder += " ";
			expr->accept(this);
			builder += result;
		}
		builder += ")";
		return builder;
	}
public:

	string print(Expr* expr) {
		expr->accept(this);
		return result;
	}

	AstPrinter() : result{""} {}

	virtual void visitBinaryExpr(const Binary* expr) override {
		result = parenthesize(expr->oper->lexeme, { expr->left , expr->right });
	}

	virtual void visitGroupingExpr(const Grouping* expr) override{
		result = parenthesize("group", {expr->expression });
	}

	virtual void visitLiteralExpr(const Literal* expr) override {
		if (expr->value == nullptr) {
			result = "nil";
		}
		else {
			result = toString(*expr->value); // DONT FORGET TO DEREFERENCE LOX VALUE TO AVOID BOOL
		}
	}

	virtual void visitUnaryExpr(const Unary* expr) override {
		result = parenthesize(expr->oper->lexeme, { expr->right });
	}

};
int main(vector<string> args) {
	Expr* expression = new Binary(
		new Unary(
			new Token(TokenType::MINUS, "-", monostate(), 1),
			new Literal(new LoxValue(123.0))),
			new Token(TokenType::STAR, "*", monostate(), 1),
		new Grouping(
			new Literal(new LoxValue(45.67)
			)
		)
	);

	cout << (new AstPrinter())->print(expression) << endl;
}
