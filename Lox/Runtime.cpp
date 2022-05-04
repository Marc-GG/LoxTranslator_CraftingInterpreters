#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <list>
#include "Token.h"
#include "Expr.h"
#include <vector>

using std::string;

class RuntimeError : RuntimeException {
	Token token;
	RuntimeError(Token token, string message) {
		super(message);
		this->token = token;
	}
};