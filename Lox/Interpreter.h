#pragma once
#include <string>
#include <vector>
#include <list>
#include "Token.h"
#include "Expr.h"
#include "Stmt.h"
#include <vector>
#include "RuntimeError.h"

using namespace std;

class Interpreter : public ExprVisitor, public StmtVisitor {
	LoxValue result;
	
public:
	Environment environment;
	LoxValue getResult() {
		return result;
	}
	
	void visitLiteralExpr(const Literal* expr) override;

	void visitGroupingExpr(const Grouping* expr) override;

	void visitBlockStmt(const Block* expr) override;

	void visitExpressionStmt(const Expression* Expression); // I cant override?

	void visitPrintStmt(const Print* stmt) override;

	void visitVarStmt(const Var* stmt) override;

	void visitWhileStmt(const While* stmt);

	void visitIfStmt(const If* stmt) override;

	LoxValue evaluate(const Expr* expr);
	void execute(const Stmt* stmt);

	void executeBlock(vector<Stmt*> statements, Environment environment);

	bool isTruthy(LoxValue loxvalue);

	void checkNumberOperand(const Token oper, LoxValue operand);
	
	void checkNumberOperands(const Token oper, LoxValue left, LoxValue right);

	void visitUnaryExpr(const Unary* expr) override;

	void visitLogicalExpr(const Logical* expr);

	void visitVariableExpr(const Variable* expr);

	bool isEqual(LoxValue a, LoxValue b);

	void visitAssignExpr(const Assign* expr);

	virtual void visitBinaryExpr(const Binary* expr) override;

	string stringify(LoxValue loxvalue);

	//void interpret(Expr* expression); //expr

	void interpret(vector<Stmt*> statements); //Stmt
};

