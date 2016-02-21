/* 
* Tokenizer.cpp: Implementation of tokenizer
* version: 1.0
* Language: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
* Source: Jim Fawcett, Syracuse University, CST 4-187
*/

#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include "Utils.h"

using namespace Scanner;
using namespace utils;
//--begin--Implementation of 

Token::Token() {
	tokenValue = "";
	tokenType = "";
}

void Token::clear() {
	tokenValue = "";
	tokenType = "";
}
//--end--implementation of Token

//--begin--declaration of TOKEN_TYPES
const std::string TOKEN_TYPES::TERMINATED = "Terminated";
const std::string TOKEN_TYPES::STRING = "String";
const std::string TOKEN_TYPES::CPPCOMMENT = "CppComment";
const std::string TOKEN_TYPES::CCOMMENT = "CComment";
const std::string TOKEN_TYPES::NEWLINE = "Newline";
const std::string TOKEN_TYPES::SPACE = "Space";
const std::string TOKEN_TYPES::PUNCTUATOR = "Punctuator";
const std::string TOKEN_TYPES::ALPHANUM = "Alphanum";
//--end--declaration of TOKEN_TYPES
static bool first = true;

/////////////////////////////////////////////////////////////////////
// Private abstract class ConsumeState
class Scanner::ConsumeState
{
public:
	ConsumeState();
	virtual ~ConsumeState();
	void attach(std::istream* pIn) { _pIn = pIn; }
	virtual void eatChars() = 0;
	void consumeChars() {
		_pState->eatChars();
		_pState->nextState();
	}
	bool canRead() { return _pIn->good(); }
	Token getTok() { return token; }
	bool hasTok() { return !token.tokenType.empty(); }
	ConsumeState* nextState();
	void clear() {
		token.clear();
		_pState = _pEatWhitespace;
	}
	static std::vector<std::string> singleTokens;
	static std::vector<std::string> doubleTokens;
	bool isSpecialSingleChar(const char &c);
	bool isSpecialSingleChar(const std::string &c);
	bool isSpecialCharPair(const std::string &p);
protected:
	static Token token;
	static std::istream* _pIn;
	static int currChar;
	static ConsumeState* _pState;
	static ConsumeState* _pEatCppComment;
	static ConsumeState* _pEatCComment;
	static ConsumeState* _pEatWhitespace;
	static ConsumeState* _pEatPunctuator;
	static ConsumeState* _pEatAlphanum;
	static ConsumeState* _pEatNewline;
	static ConsumeState* _pEatString;
	static ConsumeState* _pTerminate;
	
	static int lineNumber;
	static int tokenIndex;

	static bool inDoubleQuotes;
	static bool inSingleQuotes;
};

//----< initialize static data members >-----------------------------

Token ConsumeState::token;
std::istream* ConsumeState::_pIn;
int ConsumeState::currChar;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
ConsumeState* ConsumeState::_pEatString = nullptr;
ConsumeState* ConsumeState::_pTerminate = nullptr;
bool ConsumeState::inDoubleQuotes = false;
bool ConsumeState::inSingleQuotes = false;
int ConsumeState::lineNumber = 0;
int ConsumeState::tokenIndex = 0;

std::vector<std::string> ConsumeState::singleTokens = { "\"", "'", "<" , ">", "[", "]","{","}","(",")", ":", "=", "+", "-", "*", "/", "!" };
std::vector<std::string> ConsumeState::doubleTokens = { "<<", ">>", "::", "==", "+=", "-=", "*=", "/=", "\n" };

//----< transition to next state >-----------------------------------
ConsumeState* ConsumeState::nextState()
{
	int chNext = _pIn->peek();
	if (currChar == EOF) {
		_pState = _pTerminate;
		return _pTerminate;
	}
	if ((inDoubleQuotes == true && currChar != '"') || (inSingleQuotes == true && currChar != '\'')) {
		_pState = _pEatString;
		return _pEatString;
	}
	if (std::isspace(currChar) && currChar != '\n'){
		_pState = _pEatWhitespace;
		return _pEatWhitespace;
	}
	if (currChar == '/' && chNext == '/'){
		_pState = _pEatCppComment;
		return _pEatCppComment;
	}
	if (currChar == '*' && chNext == '/'){
		_pState = _pEatCComment;
		return _pEatCComment;
	}
	if (currChar == '/' && chNext == '*'){
		_pState = _pEatCComment;
		return _pEatCComment;
	}
	if (currChar == '\n'){
		_pState = _pEatNewline;
		return _pEatNewline;
	}
	if (std::isalnum(currChar) || currChar == '_'){
		_pState = _pEatAlphanum;
		return _pEatAlphanum;
	}
	if (ispunct(currChar) && currChar != '_'){
		_pState = _pEatPunctuator;
		return _pEatPunctuator;
	}
	throw(std::logic_error("invalid type"));
}
/////////////////////////////////////////////////////////////////////
// Derived state that consumes terminated and then stop state in terminated
class EatWhitespace : public ConsumeState{
public:
	virtual void eatChars(){
		token.clear();
		do {
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (std::isspace(currChar) && currChar != '\n');
	}
};

class Stateterminate : public ConsumeState{
public:
	virtual void eatChars(){
		token.clear();
		if (currChar == EOF) {
			token.tokenType = TOKEN_TYPES::TERMINATED;
		}
	}
};

/////////////////////////////////////////////////////////////////////
// Derived state that consumes string
/*
Backup function for escape character
*/
class EatString : public ConsumeState{
public:
	virtual void eatChars()
	{
		token.clear();
		token.tokenType = TOKEN_TYPES::STRING;
		token.tokenIndex = tokenIndex++;
		token.lineNumber = lineNumber;

		char next = _pIn->peek();
		do {
			/*
			char twoChars[3];
			_pIn->seekg(-1, ios_base::cur);
			_pIn->read(twoChars, sizeof(twoChars) - 1 );
			std::string whole(twoChars);
			_pIn->seekg(-1, ios_base::cur);
			*/
			std::string s = "  ";
			_pIn->seekg(-1, ios_base::cur);
			_pIn->read(&s[0], 2);
			_pIn->seekg(-1, ios_base::cur);
			std::string whole = utils::escape(s);

			if (currChar == '\\' && next == '\'' && inSingleQuotes) { // ' in ' '
				token.tokenValue += '\'';
				_pIn->get();
			}
			else if (currChar == '\\' && next == '"' && inDoubleQuotes) { // " in " "
				token.tokenValue += '"';
				_pIn->get();
			}
			else if (isSpecialSingleChar(whole)) {
				token.tokenValue += whole;
				_pIn->get();
			}
			else if (currChar == '\\') {
				token.tokenValue += currChar;
				token.tokenValue += _pIn->get();
			}
			else {
				token.tokenValue += currChar;
			}

			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
			next = _pIn->peek();
		} while (currChar != '"' && currChar != '\'');
	}
};

// Derived state that consumes CPP comment
class EatCppComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		token.tokenType = "CppComment";
		currChar = _pIn->get();
		currChar = _pIn->get();

		token.lineNumber = lineNumber;
		token.tokenIndex = tokenIndex++;

		token.tokenValue += "//";
		//std::cout << "\n  eating C++ comment";
		do {
			token.tokenValue += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();

		} while (currChar != '\n');
	}
};


// Consume C comment
class EatCComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		token.tokenType = "CComment";
		currChar = _pIn->get();
		currChar = _pIn->get();
		token.tokenValue += "/*";
		token.tokenIndex = tokenIndex++;
		//std::cout << "\n  eating C comment";
		do {
			token.tokenValue += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '*' || _pIn->peek() != '/');
		_pIn->get();
		token.tokenValue += "*/";
		currChar = _pIn->get();

		size_t newlineCount = std::count(token.tokenValue.begin(), token.tokenValue.end(), '\n');
		if (newlineCount > 0) {
			lineNumber += newlineCount;
			tokenIndex = 0;
		}
	}
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes punctuators and returns as token
class EatPunctuator : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		token.tokenType = "Punctuator";
		std::string next = "";
		std::string whole = "";

		token.tokenIndex = tokenIndex++;
		token.lineNumber = lineNumber;

		//std::cout << "\n  eating punctuator";
		do {
			next = _pIn->peek();
			whole = std::string(1, currChar) + next;

			if (std::find(doubleTokens.begin(), doubleTokens.end(), whole) != doubleTokens.end()) {
				if (!token.tokenValue.empty())
					break;
				token.tokenValue = whole;
				currChar = _pIn->get();
				currChar = _pIn->get();
				break;
			}
			else if (std::find(singleTokens.begin(), singleTokens.end(), std::string(1, currChar)) != singleTokens.end()) {
				if (!token.tokenValue.empty())
					break;
				token.tokenValue = currChar;
				currChar = _pIn->get();
				break;
			}
			else {
				token.tokenValue += currChar;
				currChar = _pIn->get();
				break;
			}

		} while (ispunct(currChar));

		//Set variable when begin to deal with quated string
		if (token.tokenValue == "\"" && inDoubleQuotes == false)
		{
			inDoubleQuotes = true;
		}
		else if (token.tokenValue == "'" && inSingleQuotes == false)
		{
			inSingleQuotes = true;
		}
		else if (token.tokenValue == "\"" && inDoubleQuotes == true)
		{
			inDoubleQuotes = false;
		}
		else if (token.tokenValue == "'" && inSingleQuotes == true)
		{
			inSingleQuotes = false;
		}

		_pState = nextState();
	}
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes alphanumerics and returns as token
class EatAlphanum : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		token.tokenType = "Alphanum";
		//std::cout << "\n  eating alphanum";
		token.tokenIndex = tokenIndex++;
		token.lineNumber = lineNumber;

		do {
			if (token.tokenValue == "1234") {
				char a = 1;
			}
			if (currChar == '\\') {
				token.tokenValue += currChar + _pIn->get();
			}
			else {
				token.tokenValue += currChar;
			}
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isalnum(currChar) || currChar == '\\' || currChar == '_');
		_pState = nextState();
	}
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes newline and returns as single
// character token.
class EatNewline : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		token.tokenType = TOKEN_TYPES::NEWLINE;
		token.tokenValue = "\n";
		token.tokenIndex = tokenIndex++;
		token.lineNumber = lineNumber;

		//std::cout << "\n  eating alphanum";
		currChar = _pIn->get();
		if (!_pIn->good())  // end of stream
			return;


		lineNumber++;
		tokenIndex = 0;
	}
};
//----< base class member function to create derived states >--------

ConsumeState::ConsumeState()
{
	if (first)
	{
		first = false;
		_pEatAlphanum = new EatAlphanum();
		_pEatCComment = new EatCComment();
		_pEatCppComment = new EatCppComment();
		_pEatPunctuator = new EatPunctuator();
		_pEatWhitespace = new EatWhitespace();
		_pEatNewline = new EatNewline();
		_pEatString = new EatString();
		_pTerminate = new Stateterminate();
		_pState = _pEatWhitespace;
	}
}

ConsumeState::~ConsumeState()
{
	if (!first)
	{
		first = true;
		delete _pEatAlphanum;
		delete _pEatCComment;
		delete _pEatCppComment;
		delete _pEatPunctuator;
		delete _pEatWhitespace;
		delete _pEatNewline;
		delete _pEatString;
	}
}

bool ConsumeState::isSpecialSingleChar(const char&c) {
	auto s = to_string(c);
	return isSpecialCharPair(s);
}
bool ConsumeState::isSpecialSingleChar(const string &c) {
	return singleTokens.end() != std::find(singleTokens.begin(), singleTokens.end(), c);
}
bool ConsumeState::isSpecialCharPair(const string &s) {
	return doubleTokens.end() != std::find(doubleTokens.begin(), doubleTokens.end(), s);

}
//----< initialize the toker to start with EatWhitespace >-----------
Toker::Toker() : pConsumer(new EatWhitespace()) {
	applyConfig();
}

Toker::~Toker() { delete pConsumer; }

// Read configuration from file and intialize the configure items
void Toker::applyConfig() {
	std::string fileSpec = "token_config.txt";
	std::ifstream in(fileSpec);
	char *line = new char[1000];
	
	for (std::string line; std::getline(in, line); ) {
		if (line.find("single_character_token") != -1) {
			int beginIndex = line.find_first_of('"');
			std::string configValue = line.substr(beginIndex + 1, line.size() - beginIndex - 2  );
			auto singleTokens = split(configValue, ',');
			std::transform(singleTokens.begin(), singleTokens.end(), singleTokens.begin(), trimL);
			setSpecialSingleChars(singleTokens, true);
		}
		else if (line.find("double_character_token") != -1) {
			int beginIndex = line.find_first_of('"');
			std::string configValue = trimL(line.substr(beginIndex + 1, line.size() - beginIndex - 2 ));
			auto doubleTokens = split(configValue, ',');
			std::transform(doubleTokens.begin(),doubleTokens.end(), doubleTokens.begin(), trimL);
			setSpecialCharPairs(doubleTokens, true);
		}
	}
}
	
// Attach a stream to Tokenizer and use getTok to parse and return token
bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		pConsumer = new EatWhitespace();
		return true;
	}
	return false;
}

// function to get token, this function will return a token every time it is invoked.
//----< this method will be called by SemiExpression >---------------
Token Toker::getTok()
{
	while (true)
	{
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;

		//insurance
		if (!pConsumer->canRead())
			return Token();
	}
	return pConsumer->getTok();
}

// Tell whether the consumer can continue to read stream and return token
bool Toker::canRead() { return pConsumer->canRead(); }

// Set special single character
// Parameters:
//    chars: vector of special single character such like ':', '!'
//    replace: whether to replace the old special single characters with the new configuration
void Toker::setSpecialSingleChars(const std::vector<std::string> chars, bool replace) {
	size_t i = 0;
	auto copyChars = std::vector<std::string>(chars);
	for (i = 0; i < copyChars.size(); i++) {
		copyChars[i] = utils::escape(copyChars[i]);
		if (copyChars[i].size() != 1) {
			copyChars.erase(copyChars.begin() + i);
			continue;
		}
	}
	
	if (replace) {
		pConsumer->singleTokens.clear();
		pConsumer->singleTokens = std::vector<std::string>(copyChars);
	}
	else {
		for (auto it = copyChars.begin(); it != copyChars.end(); it++) {
			std::string s = *it;
			if (pConsumer->singleTokens.end() == std::find(pConsumer->singleTokens.begin(), pConsumer->singleTokens.end(), s) ) {
				pConsumer->singleTokens.push_back(s);
			}
		}
	}
}

// Set special character pairs
// Parameters:
//    chars: vector of special characters such like '::', '{{'
//    replace: whether to replace the old special character pairs with the new configuration
void Toker::setSpecialCharPairs(const std::vector<std::string> chars, bool replace) {
	size_t i = 0;
	auto copyChars = std::vector<std::string>(chars);
	for (i = 0; i < copyChars.size(); i++) {
		if (copyChars[i].size() != 2) {
			copyChars.erase(copyChars.begin() + i);
		}
	}
	if (replace) {
		pConsumer->doubleTokens.clear();
		pConsumer->doubleTokens = std::vector<std::string>(copyChars);
	}
	else {
		for (auto it = copyChars.begin(); it != copyChars.end(); it++) {
			std::string s = *it;
			if (pConsumer->doubleTokens.end() == std::find(pConsumer->doubleTokens.begin(), pConsumer->doubleTokens.end(), s)) {
				pConsumer->doubleTokens.push_back(s);
			}
		}
	}
}

void Toker::clear() {
	pConsumer = new EatWhitespace();
}

std::vector<std::string> Toker::getSpecialSingleChars() {
	return std::vector<std::string>(pConsumer->singleTokens);
}

std::vector<std::string> Toker::getSpecialCharPairs() {
	return std::vector<std::string>(pConsumer->doubleTokens);
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	std::string fileSpec = "../Tokenizer/Tokenizer.cpp";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.attach(&in);
	while (in.good())
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	}
	std::cout << "\n\n";

	char str[256];
	std::cin.get(str, 256);
	return 0;
}

#endif
