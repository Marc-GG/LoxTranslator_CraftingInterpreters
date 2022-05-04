//Scanner
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <sstream>
#include "Token.h"
#include "Lox.h"

using namespace std;
class Scanner { 
	public:
		std::unordered_map<string, TokenType> keywords{
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
			   {"while", TokenType::WHILE} };
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



		
};


