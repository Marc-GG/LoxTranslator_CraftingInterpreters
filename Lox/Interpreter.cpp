#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include "Token.h"
#include "Expr.h"
#include "Stmt.h"
#include <vector>
#include "RuntimeError.h"
#include "Lox.h"
#include "Interpreter.h"
#include "Environment.h"

using namespace std;
//Expr visit
void Interpreter::visitLiteralExpr(const Literal* expr) {
	result = expr->value;
}

void Interpreter::visitLogicalExpr(const Logical* expr) {
	LoxValue left = evaluate(expr->left);

	if (expr->oper.type == TokenType::OR) {
		if (isTruthy(left)) result = left;
	}
	else {
		if (!isTruthy(left)) result = left;
	}
	result = evaluate(expr->right);
}

void Interpreter::visitGroupingExpr(const Grouping* expr)  {
	result =  evaluate(expr->expression);
}

LoxValue Interpreter::evaluate(const Expr* expr) {
	expr->accept(this);
	return result;
}

void Interpreter::execute(const Stmt* stmt) {
	stmt->accept(this);
}

void Interpreter::executeBlock(vector<Stmt*> statements, Environment environment) {
	Environment previous = this->environment;

	try {
		this->environment = environment;

		for (Stmt* statement : statements) {
			execute(statement);
		}
	} catch (...) {
			this->environment = previous;
		throw;
	}

	this->environment = previous;
}

//Stmt visit
void Interpreter::visitBlockStmt(const Block* stmt) {
	executeBlock(stmt->statements, new Environment(environment));
	result = LoxValue();
}

void Interpreter::visitExpressionStmt(const Expression* stmt) {
	evaluate(stmt->expression);
	result = LoxValue();
}

void Interpreter::visitPrintStmt(const Print* stmt) {
	LoxValue value = evaluate(stmt->expression);
	std::cout << stringify(value) << endl;
	result = LoxValue();
}

void Interpreter::visitVarStmt(const Var* stmt) {
	LoxValue value = LoxValue();
	if (stmt->initializer != nullptr) {
		value = evaluate(stmt->initializer);
	}
	environment.define(stmt->name.lexeme, value);
	result = LoxValue();
}

void Interpreter::visitWhileStmt(const While* stmt) {
	while (isTruthy(evaluate(stmt->condition))) {
		execute(stmt->body);
	}
	result = LoxValue();
}

void Interpreter::visitIfStmt(const If* stmt) {
	if (isTruthy(evaluate(stmt->condition))) {
		execute(stmt->thenBranch);
	}
	else if (stmt->elseBranch != nullptr){
		execute(stmt->elseBranch);
	}
	result = LoxValue();
}

void Interpreter::visitAssignExpr(const Assign* expr) { 
	LoxValue value = evaluate(expr->value);
	environment.assign(expr->name, value);
	result = value;
};
// --------------------------------------------------------

bool Interpreter::isTruthy(LoxValue loxvalue) {
	if (std::holds_alternative<monostate>(loxvalue)) return false;
	if (std::holds_alternative<bool>(loxvalue)) return std::get<bool>(loxvalue);
	return true;
}
void Interpreter::checkNumberOperand(const Token oper, LoxValue operand) {
	if (std::holds_alternative<double>(operand)) return;
	throw RuntimeError(oper, "Operand must be a number.");
}
void Interpreter::checkNumberOperands(const Token oper, LoxValue left, LoxValue right) {
	if (std::holds_alternative<double>(right) && std::holds_alternative<double>(left)) return;
	throw RuntimeError(oper, "Operands must be numbers.");
}

void Interpreter::visitUnaryExpr(const Unary* expr)  {
	LoxValue right = evaluate(expr->right);
		
	switch (expr->oper.type) {
	case TokenType::BANG:
		result = !isTruthy(right);
		break;
	case TokenType::MINUS: 
		checkNumberOperand(expr->oper, right);
		result = -(std::get<double>(right));
		break;
	}
		
	//Unreachable.
	//result = LoxValue();
}

void Interpreter::visitVariableExpr(const Variable* expr) {
	result = environment.get(expr->name);
}
bool Interpreter::isEqual(LoxValue a, LoxValue b) {
	if (std::holds_alternative<monostate>(a) && std::holds_alternative<monostate>(b)) return true;
	if (std::holds_alternative<monostate>(a)) return false;

	if (a == b) {
		return true;
	}
	else {
		return false;
	}
}
void Interpreter::visitBinaryExpr(const Binary* expr)  {
	LoxValue left = evaluate(expr->left);
	LoxValue right = evaluate(expr->right);

	switch (expr->oper.type) {
	case TokenType::GREATER:
		checkNumberOperands(expr->oper, left, right);
		result = std::get<double>(left) > std::get<double>(right);
		break;
	case TokenType::GREATER_EQUAL:
		checkNumberOperands(expr->oper, left, right);
		result = std::get<double>(left) >= std::get<double>(right);
		break;
	case TokenType::LESS:
		checkNumberOperands(expr->oper, left, right);
		result = std::get<double>(left) < std::get<double>(right);
		break;
	case TokenType::LESS_EQUAL:
		checkNumberOperands(expr->oper, left, right);
		result = std::get<double>(left) <= std::get<double>(right);
		break;
	case TokenType::MINUS:
		checkNumberOperands(expr->oper, left, right);
		result = std::get<double>(left) - std::get<double>(right);
		break;
	case TokenType::PLUS:
		if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
			result = std::get<double>(left) + std::get<double>(right);
		}
		if (std::holds_alternative<string>(left) && holds_alternative<string>(right)) {
			result =  std::get<string>(left) +  std::get<string>(right);
		}
		break;
	case TokenType::SLASH:
		checkNumberOperands(expr->oper, left, right);
		result = std::get<double>(left) / std::get<double>(right);
		break;
	case TokenType::STAR:
		checkNumberOperands(expr->oper, left, right);
		result = std::get<double>(left) * std::get<double>(right);
		break;
	case TokenType::BANG_EQUAL: result = !isEqual(left, right);
		break;
	case TokenType::EQUAL_EQUAL: result = isEqual(left, right);
		break;
	}
		
	//result = LoxValue();
	// Unreachable
	
}


string Interpreter::stringify(LoxValue loxvalue) {
	if (std::holds_alternative<monostate>(loxvalue)) return "nil";
	if (std::holds_alternative<double>(loxvalue)) {
		
		string text = toString(loxvalue); // loxvalue.toString(); ??????????
		double number = std::get<double>(loxvalue);
		std::string x = std::to_string(number);

		if (text.ends_with(".0")) {
			text = text.substr(0, text.length() - 2);
		}

		return std::to_string((int)number);
	}
	if (std::holds_alternative<string>(loxvalue)) {
		string text = toString(loxvalue);
		if (text.ends_with("\"")) {
			text = text.substr(0, text.length() - 1);
		}
		if (text.ends_with(";")) {
			text = text.substr(0, text.length() - 2);
		}
		return text;
	}
	if (std::holds_alternative<bool>(loxvalue)) {
		return std::get<bool>(loxvalue) ? "true" : "false";
	}


	return toString(loxvalue);
}
		
/*
void Interpreter::interpret(Expr* expression) {
	try {
		LoxValue value = evaluate(expression);
		result = stringify(value);

	}
	catch (RuntimeError error) {
			myRuntimeError(error);
		}

	}
*/

//Stmt
void Interpreter::interpret(vector<Stmt*> statements) {
	try {
		for (Stmt* statement : statements) {
			execute(statement);
		}

	}
	catch (RuntimeError error) {
		myRuntimeError(error);
	}

}

