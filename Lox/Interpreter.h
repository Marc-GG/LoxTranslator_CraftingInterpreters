#pragma once
#include <string>
#include <vector>
#include <list>
#include "Token.h"
#include "Expr.h"
#include <vector>
#include "RuntimeError.h"

using namespace std;

class Interpreter : public ExprVisitor {
	LoxValue result;

public:
	LoxValue getResult() {
		return result;
	}

	void visitLiteralExpr(const Literal* expr) override;

	void visitGroupingExpr(const Grouping* expr) override;

	LoxValue evaluate(const Expr* expr);

	bool isTruthy(LoxValue loxvalue);

	void checkNumberOperand(const Token oper, LoxValue operand);
	
	void checkNumberOperands(const Token oper, LoxValue left, LoxValue right);

	void visitUnaryExpr(const Unary* expr) override;

	bool isEqual(LoxValue a, LoxValue b);

	virtual void visitBinaryExpr(const Binary* expr) override;

	string stringify(LoxValue loxvalue);

	void interpret(Expr* expression);
};

