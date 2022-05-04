//Main Lox
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Scanner.cpp"
#include "Token.h"
#include "Expr.h"
#include "Parser.h"
#include "Lox.h"
#include "Interpreter.h"
#include "RuntimeError.h"

Interpreter interpreter =  Interpreter();

bool hadError = false;
static bool hadRuntimeError = false;

using namespace std;
using std::cout;
using std::endl;


void run(string source) { // must contain Scanner and Token classes

	Scanner	scanner = Scanner(source);
	vector<Token> tokens = scanner.scanTokens(); 
	
	Parser parser = Parser(tokens);
	Expr* expression = parser.parse();

	if (hadError) return;
	interpreter.interpret(expression);
	cout << toString(interpreter.getResult()) << endl;
}
void error(Token token, string message) {
	if (token.type == TokenType::EOFF) {
		report(token.line, " at end", message);
	}
	else {
		report(token.line, " at '" + token.lexeme + "'", message);
	}
}

void error(int line, string message) {
	report(line, "", message);
}

void myRuntimeError(RuntimeError error) {
	cout << "\n[line " << error.token.line << "]"<<endl;
	hadRuntimeError = true;
}

void report(int line, string where, string message) { //what is had error?

	stringstream ss; //converting line int into string

	string lineconv;

	ss << line;
	ss >> lineconv;

	cout << ("[line " + lineconv + "] Error" + where + ": " + message);

	hadError = true;
}

void runFile(string filename) {
	ifstream input_file(filename);
	if (!input_file.is_open()) {
		cerr << "Could not open the file - '" << filename << "'" << "\n";
		exit(EXIT_FAILURE);
	}
	run(string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>())); // https://www.delftstack.com/howto/cpp/read-file-into-string-cpp/#:~:text=Use%20rdbuf%20to%20Read%20File%20Into%20String%20in,using%20the%20%3C%3C%20operator%20to%20the%20needed%20object.

	if (hadError)
		return exit(65);
	if (hadRuntimeError)
		return exit(70);
}

void runPrompt() {

	while (true) {

		std::cout << "> ";

		string line;
		getline(cin, line);

		if (line.empty()) {
			break;
		}
		run(line);
		hadError = false;

	}
}


int main(int argc, char** argv) {
	string x = "Usage: jlox [script]";
	if (argc > 2) {
		cout << x;
		cout << endl;
		return EXIT_SUCCESS;
	}
	else if (argc == 2) {
		runFile(argv[1]);
		return 0;
	}
	else {
		runPrompt();
		return 0;
	}
}



