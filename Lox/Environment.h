#pragma once
#include <string>
#include "Token.h"
#include "Expr.h"
#include "Stmt.h"
#include <vector>
#include <map>
using std::vector, std::string, std::map, std::unordered_map;

class Environment {
	Environment* enclosing;
public:
	Environment();

	Environment(Environment* enclosing);

	LoxValue get(Token name);

	void assign(Token name, LoxValue value);

	void define(string name, LoxValue value);
};