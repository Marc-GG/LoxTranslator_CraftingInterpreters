#include <string>
#include <vector>
#include <list>
#include "Token.h"
#include "Expr.h"
#include <vector>
#include "RuntimeError.h"
#include "Lox.h"
#include "Interpreter.h"

using namespace std;



void Interpreter::visitLiteralExpr(const Literal* expr) {
	result = expr->value;
}

void Interpreter::visitGroupingExpr(const Grouping* expr)  {
	result =  evaluate(expr->expression);
}

LoxValue Interpreter::evaluate(const Expr* expr) {
	expr->accept(this);
	return result;
}

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
	result = LoxValue();
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
		

	// Unreachable
	
}


string Interpreter::stringify(LoxValue loxvalue) {
	if (std::holds_alternative<monostate>(loxvalue)) return "nil";

	if (std::holds_alternative<double>(loxvalue)) {
		string text = toString(loxvalue); // loxvalue.toString(); ??????????
		if (text.ends_with(".0")) {
			text = text.substr(0, text.length() - 2);
		}
	}

	return toString(loxvalue);
}
		

void Interpreter::interpret(Expr* expression) {
	try {
		LoxValue value = evaluate(expression);
		result = stringify(value);

	}
	catch (RuntimeError error) {
			myRuntimeError(error);
		}

	}


