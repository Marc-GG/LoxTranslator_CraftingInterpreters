#include "Parser.h"
#include "Lox.h"
#include "Stmt.h"
#include <list>
#include <string>
#include <vector>
#include <map>
#include "Environment.h"
using namespace std;
unordered_map<string, LoxValue> values;

LoxValue Environment::get(Token name) {
	if (values.contains(name.lexeme)) {
		return values.at(name.lexeme); //.get?
	}
	if (enclosing != nullptr) return enclosing->get(name);

	throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(Token name, LoxValue value) {
	if (values.contains(name.lexeme)) {
		values.insert({ name.lexeme, value });
		return;
	}
	if (enclosing != nullptr) {
		enclosing->assign(name, value);
		return;
	}

	throw new RuntimeError(name, "Undefined Variable '" + name.lexeme + "'.");
}


void Environment::define(string name, LoxValue value) {
	values.insert({ name, value });
}

Environment::Environment() : enclosing{ nullptr } {}

Environment::Environment(Environment* enclosing) {
	this->enclosing = enclosing;
}
	