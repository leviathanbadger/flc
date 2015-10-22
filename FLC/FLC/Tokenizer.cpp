#include <sstream>
#include "stdafx.h"
#include "Tokenizer.h"
#include "CharacterLiteralToken.h"
#include "FloatLiteralToken.h"
#include "IntegerLiteralToken.h"
#include "IdentifierToken.h"
#include "StringLiteralToken.h"
#include "SymbolToken.h"
#include "ErrorToken.h"
#include "EndOfFileToken.h"

namespace flc
{
    namespace tokens
    {
        Tokenizer::Tokenizer()
        {   
        }
		Tokenizer::~Tokenizer()
		{
		}

		vector<Token*> Tokenizer::tokenize(istream *sourceFile, string path)
		{
			vector<Token*> *result = new vector<Token*>();
			int pos = 0;
			while (!sourceFile->eof()) {
				Token *next = parseNextToken(sourceFile, &pos, path);
				result->push_back(next);
				pos += next->getLength();
				if (sourceFile->eof())
					result->push_back(new EndOfFileToken(path, pos));
			}
			return *result;
		}
		
		bool Tokenizer::isWhiteSpace(char c) {
			return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
		}
		bool Tokenizer::isOctal(char c) {
			return (c >= '0' && c <= '7');
		}
		bool Tokenizer::isDecimal(char c) {
			return (c >= '0' && c <= '9');
		}
		bool Tokenizer::isHexadecimal(char c) {
			return (c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f');
		}
		bool Tokenizer::isAlphanumeric(char c) {
			return (c >= '0' && c <= '9' || c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == '_');
		}

		char Tokenizer::escapeSequenceToChar(istream *source, int *length) {
			char result;
			source->ignore();
			char nextChar = source->get();
			if (source->eof()) {
				return EOF;
			}

			stringstream subNum;
			switch (nextChar) {
			case 'a':
				result = '\x07';
				(*length)++;
				break;
			case 'b':
				result = '\x08';
				(*length)++;
				break;
			case 'f':
				result = '\x0C';
				(*length)++;
				break;
			case 'n':
				result = '\x0A';
				(*length)++;
				break;
			case 'r':
				result = '\x0D';
				(*length)++;
				break;
			case 't':
				result = '\x09';
				(*length)++;
				break;
			case 'v':
				result = '\x0b';
				(*length)++;
				break;
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
				subNum << nextChar;
				for (int i = 0; i < 2; i++) {
					if (!isOctal(source->peek())) {
						break;
					}
					subNum << source->get();
					(*length)++;
				}
				result = (char)stoi(subNum.str());
				break;
			case 'x':
				for (int i = 0; i < 2; i++) {
					if (!isHexadecimal(source->peek())) {
						break;
					}
					subNum << source->get();
					(*length)++;
				}
				result = (char)stoi(subNum.str(), nullptr, 16);
				break;
			case 'u':
			case 'U':
				// non-ascii encodings not implemented yet
				result = EOF;
				break;
			default:
				result = nextChar;
				(*length)++;
				break;
			}
			return result;
		}

		Token* Tokenizer::parseNextToken(istream *source, int *index, string path) {
			while (isWhiteSpace(source->peek())) {
				index++;source->get();
			}
			if (source->eof()) {
				return new EndOfFileToken(path, *index);
			}

			char nextChar = source->peek();
			if (nextChar == '\'') {
				return parseCharacterToken(source, index, path);
			}
			else if (nextChar == '"') {
				return parseStringToken(source, index, path);
			}
			else if (isDecimal(nextChar)) {
				return parseNumericToken(source, index, path);
			}
			else if (isAlphanumeric(nextChar) && !isDecimal(nextChar)) {
				return parseIdentifierToken(source, index, path);
			}
			else {
				return parseSymbolToken(source, index, path);
			}
		}

		Token* Tokenizer::parseCharacterToken(istream *source, int *index, string path) {
			source->ignore();
			if (source->eof())
				return new ErrorToken(path, *index, 1, "EOF while parsing char literal");

			char nextChar = source->get();
			int length = 2; char c;
			if (nextChar == '\\') {
				c = escapeSequenceToChar(source, &length);
			}
			else {
				c = nextChar;
			}

			if (source->eof()) {
				return new ErrorToken(path, *index, length, "EOF while parsing char literal");
			}
			else if (source->peek() != '\'') {
				return new ErrorToken(path, *index, length, "Invalid char literal");
			}
			else {
				source->ignore();
				return new CharacterLiteralToken(path, *index, length+1, c);
			}
		}
		Token* Tokenizer::parseNumericToken(istream *source, int *index, string path) {
			stringstream numberString;
			int length = 0;
			bool foundDecimal = false;
			char nextChar;
			while ( !source->eof() && (nextChar = source->get()) && (isDecimal(nextChar) || (nextChar == '.' && !foundDecimal)) ) {
				if (nextChar == '.')
					foundDecimal = true;
				numberString << nextChar;
				length++;
			}
			if (foundDecimal) {
				return new FloatLiteralToken(path, *index, length, stof(numberString.str()));
			}
			else {
				return new IntegerLiteralToken(path, *index, length, stoi(numberString.str()));
			}
		}
		Token* Tokenizer::parseStringToken(istream *source, int *index, string path) {
			source->ignore();
			stringstream result;
			int length = 1;
			char nextChar, c;
			while (!source->eof()) { 
				nextChar = source->get();
				if (nextChar == '"') {
					length++;
					break;
				}
				else if (nextChar == '\\') {
					c = escapeSequenceToChar(source, &length);
				}
				else {
					c = nextChar;
					length++;
				}
				result << c;
			}
			if (source->eof()) {
				return new ErrorToken(path, *index, length, "EOF while parsing string literal");
			}
			else {
				return new StringLiteralToken(path, *index, length, result.str());
			}
		}
		Token* Tokenizer::parseIdentifierToken(istream *source, int *index, string path) {
			stringstream identString;
			int length = 0;
			char nextChar;
			while (!source->eof() && (nextChar = source->get()) && isAlphanumeric(nextChar)) {
				identString << nextChar;
				length++;
			}
			return new IdentifierToken(path, *index, length, identString.str());
		}
		Token* Tokenizer::parseSymbolToken(istream *source, int *index, string path) {
            // Very basic symbols only until syntax is better defined
			char nextChar = source->get();
			if (nextChar == '+' ||
				nextChar == '-' ||
				nextChar == '*' ||
				nextChar == '/' ||
				nextChar == '%' ||
				nextChar == '(' ||
				nextChar == ')') {
				
				return new SymbolToken(path, *index, 1, string(1,nextChar));
			}
			else {
				return new ErrorToken(path, *index, 1, "Unknown symbol");
			}
		}
    }
}
