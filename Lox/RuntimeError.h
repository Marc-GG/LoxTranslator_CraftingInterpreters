#pragma once
#include <string>
#include "Token.h"
#include <stdexcept>
using std::runtime_error, std::string;

class RuntimeError : public runtime_error {
	
	
public:
	const Token token;
	RuntimeError(const Token token, string message) : runtime_error(message),
		token{token}
	{
		
	}
};
