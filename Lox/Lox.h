#pragma once
#include <string>
#include "Token.h"
#include "RuntimeError.h"

using std::string;

void error(Token token, string message);

void report(int line, string where, string message);

void error(int line, string message);

void myRuntimeError(RuntimeError error);