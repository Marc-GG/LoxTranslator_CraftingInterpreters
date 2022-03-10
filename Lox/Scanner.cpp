//Scanner
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <map>
#include <unordered_map>
#include <variant>


static void error(int line, std::string message);
static void report(int line, std::string where, std::string message);
class Lox {

public:
	static bool hadError;
};
bool Lox::hadError = false;
using namespace std;
using std::cout;
using std::endl;


enum class TokenType {

	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

	// One or two character tokens.
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	// Literals.
	IDENTIFIER, STRING, NUMBER,

	// Keywords.
	AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
	PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

	EOFF
};

string enumString(TokenType token) { //transforms enum characters into string
	std::map<TokenType, string> enum_map = {
		{TokenType::LEFT_PAREN, "LEFT_PAREN"},
		{TokenType::RIGHT_PAREN,"RIGHT_PAREN"},
		{TokenType::LEFT_BRACE,"LEFT_BRACE"},
		{TokenType::RIGHT_BRACE,"RIGHT_BRACE"},
		{TokenType::COMMA,"COMMA"},
		{TokenType::DOT,"DOT"},
		{TokenType::MINUS,"MINUS"},
		{TokenType::PLUS,"PLUS"},
		{TokenType::SEMICOLON,"SEMICOLON"},
		{TokenType::SLASH,"SLASH"},
		{TokenType::STAR,"STAR"},
		{TokenType::BANG,"BANG"},
		{TokenType::BANG_EQUAL,"BANG_EQUAL"},
		{TokenType::EQUAL,"EQUAL"},
		{TokenType::EQUAL_EQUAL,"EQUAL"},
		{TokenType::GREATER,"GREATER"},
		{TokenType::GREATER_EQUAL,"GREATER_EQUAL"},
		{TokenType::LESS,"LESS"},
		{TokenType::LESS_EQUAL,"LESS_EQUAL"},
		{TokenType::IDENTIFIER,"IDENTIFIER"},
		{TokenType::STRING,"STRING"},
		{TokenType::NUMBER,"NUMBER"},
		{TokenType::AND,"AND"},
		{TokenType::CLASS,"CLASS"},
		{TokenType::ELSE,"ELSE"},
		{TokenType::FALSE,"FALSE"},
		{TokenType::FUN,"FUN"},
		{TokenType::FOR,"FOR"},
		{TokenType::IF,"IF"},
		{TokenType::NIL,"NIL"},
		{TokenType::OR,"OR"},
		{TokenType::PRINT,"PRINT"},
		{TokenType::RETURN,"RETURN"},
		{TokenType::SUPER,"SUPER"},
		{TokenType::THIS,"THIS"},
		{TokenType::TRUE,"TRUE"},
		{TokenType::VAR,"VAR"},
		{TokenType::WHILE,"WHILE"},
		{TokenType::EOFF,"EOFF"},
	};

	return enum_map[token];
}

using LoxValue = std::variant<std::monostate, double, bool, string>;

class Token {

	
	TokenType type;
	string lexeme;
	LoxValue literal;
	int line;

public:
	Token(TokenType type, string lexeme, LoxValue literal, int line) {

		this->type = type;
		this->lexeme = lexeme;
		this->literal = literal;
		this->line = line;
	}

	string toString() {
		string beginningstring = enumString(type) + " " + lexeme + " ";

		if (holds_alternative<string>(literal)) {

			return  beginningstring + std::get<string>(literal);

		}
		else if (holds_alternative<double>(literal)) {
			return beginningstring + std::to_string(std::get<double>(literal));

		}
		else if (holds_alternative<bool>(literal)) {



			if (std::get<bool>(literal) == true) {

				return beginningstring + "true";

			}

			else

				return beginningstring + "false";



		}
		//Must be NILL
		else
			return beginningstring;
		
			
	}

};

class Scanner { 
	public:
		string source;
		std::vector<Token> tokens;

		Scanner(string source) {
			this->source = source;
		}

		size_t start = 0;
		size_t current = 0;
		size_t line = 1;


		bool isAtEnd() {

			return current >= source.length();
		}

		vector<Token> scanTokens() { 
			while (!isAtEnd()) {

				start = current;
				scanToken();
			}
			

			tokens.push_back(Token(TokenType::EOFF, "", LoxValue{}, line));


			return tokens;


		};
		

		char advance() {

			int x = source[current ++];

			return x;


		}

			
	
		//uhhhhh check the string its supposed to be object type, but i set object to string, so string should be fine.
		void addToken(TokenType type, LoxValue literal = LoxValue{}) {
	
			string text = source.substr(start, current - start);
			tokens.push_back(Token(type, text,  literal, line));
		
		
		}

		bool match(char expected) {

			
			if (isAtEnd()) return false;
			if (source[current] != expected) return false;

			current++;
			return true;
			
			

		}

		char peek() {

			if (isAtEnd()) return '\0';
			return source[current];
				
		}


		void String() {
			
			while (peek() != '"' && !isAtEnd()) {
				if (peek() == '\n') line++;
				advance();
			}
			if (isAtEnd()) {
				error(line, "Unterminated string."); //Lox.error ???
				return;
			}


			// The closing ".
			advance();

			// Trim the surrounding quotes.

			string value = source.substr(start + 1, current - 1);
			addToken(TokenType::STRING, value);
		}
		bool isDigit(char c) {
			return c >= '0' && c <= '9';
		
		}
		char peekNext() {
			if (current + 1 >= source.length()) return '\0';
			return source[current + 1];
		}
		void number() {
			while (isDigit(peek())) advance();
			// Look for a fractional part.
			if (peek() == '.' && isDigit(peekNext())) {
			// Consume the "."
				advance();
				while (isDigit(peek())) advance();
			}
			addToken (TokenType::NUMBER, std::stod(source.substr(start, current))); 
		
		}


		bool isAlpha(char c) {
			return (c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				c == '-';
		}

		bool isAlphaNumeric(char c) {
			return isAlpha(c) || isDigit(c);
		}


		void identifier() {
			while (isAlphaNumeric(peek())) advance();
			
			string text = source.substr(start, current);
			auto type = keywords.find(text);
			if (type == keywords.end())
				addToken(TokenType::IDENTIFIER);

			else

				addToken(type -> second);
		}
		
			void scanToken() {

			
			char c = advance();


			switch (c) {

			case '(': addToken(TokenType::LEFT_PAREN); break;
			case ')': addToken(TokenType::RIGHT_PAREN); break;
			case '{': addToken(TokenType::LEFT_BRACE); break;
			case '}': addToken(TokenType::RIGHT_BRACE); break;
			case ',': addToken(TokenType::COMMA); break;
			case '.': addToken(TokenType::DOT); break;
			case '-': addToken(TokenType::MINUS); break;
			case '+': addToken(TokenType::PLUS); break;
			case ';': addToken(TokenType::SEMICOLON); break;
			case '*': addToken(TokenType::STAR); break;

			case '!':
				addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
				break;
			case '=':
				addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
				break;
			case '<':
				addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
				break;
			case '>':
				addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
				break;

			case '/':

				if (match('/')) {
					// A coment goes until the end of the line.

					while (peek() != '\n' && !isAtEnd()) advance();
				}
				else {
					addToken(TokenType::SLASH);
				}
				break;

			case ' ':

			case '\r':

			case '\t':

				//Ignore whitespace.
				break;

			case '\n':
				line++;
				break;

			case '"': String(); break;


			case 'o':
				if (match('r')) {
				
					addToken(TokenType::OR);
				}
				break;

			default:
				if (isDigit(c)) {
					number();
				
				}

				else if (isAlpha(c)) {
					identifier();
				
				}


				else {
					error(line, "Unexpected character .");
				}

				break;
			}

			
		}



		std::unordered_map<string, TokenType> keywords  {
			{"and", TokenType::AND}, 
			{"class", TokenType::CLASS},
			{"else", TokenType::ELSE},
			{"false", TokenType::FALSE},
			{"for", TokenType::FOR},
			{"fun", TokenType::FUN},
			{"if", TokenType::IF},
			{"nil", TokenType::NIL},
			{"or", TokenType::OR},
			{"print", TokenType::PRINT},
			{"return", TokenType::RETURN},
			{"super", TokenType::SUPER},
			{"this", TokenType::THIS},
			{"true", TokenType::TRUE},
			{"var", TokenType::VAR},
			{"while", TokenType::WHILE}

		};

		
		
};





void run(string source) { // must contain Scanner and Token classes

	Scanner	scanner =  Scanner(source);
	vector<Token> tokens = scanner.scanTokens();

	for (Token token : tokens) {
		
		cout << token.toString() <<endl; //operator overload created in Token class - *** Clarify this works ***
	}

}

////////////////////////////////////////////Error Handeling

static void report(int line, string where, string message) { //what is had error?

	stringstream ss; //converting line int into string

	string lineconv;

	ss << line;
	ss >> lineconv;

	cout << ("[line " + lineconv + "] Error" + where + ": " + message);

	Lox::hadError = true;
}


static void error(int line, string message) {

	report(line, "", message);

}


void runFile(string filename) {
	ifstream input_file(filename);
	if (!input_file.is_open()) {
		cerr << "Could not open the file - '" << filename << "'" << "\n";
		exit(EXIT_FAILURE);
	}
	run(string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>())); // https://www.delftstack.com/howto/cpp/read-file-into-string-cpp/#:~:text=Use%20rdbuf%20to%20Read%20File%20Into%20String%20in,using%20the%20%3C%3C%20operator%20to%20the%20needed%20object.

	if (Lox::hadError) 
		return exit(65);
}

void runPrompt() { // must contain Scanner class before moving on.

	while (true) {

		std::cout << "> ";

		string line;
		getline(cin, line);

		if (line.empty()) {
			break;
		}
		run(line);
		Lox::hadError = false;

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
	
	

