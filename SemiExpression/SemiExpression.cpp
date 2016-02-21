/*
* Tokenizer.cpp: Implementation of semi expression
* version: 1.0
* Language: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
* Source: Jim Fawcett, Syracuse University, CST 4-187
*/

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExpression.h"
#include "../Tokenizer/Tokenizer.h"
#include "SemiExpression.h"
#include <algorithm>
#include "Utils.h"

using namespace Scanner;
using namespace utils;

// Constructor
SemiExpression::SemiExpression(Toker* pToker) : _pToker(pToker) {
	applyConfig();
}

// Determite if to can stop collecting tokens
bool SemiExpression::ifToStopCollect(const Token& token) {
	bool stop = false;
	auto tokenType = token.tokenType;
	auto tokenValue = token.tokenValue;
	if (tokenType == TOKEN_TYPES::PUNCTUATOR && tokenValue == "{")
		stop = true;
	else if (tokenType == TOKEN_TYPES::PUNCTUATOR && tokenValue == ";")
		stop = true;
	else if (tokenType == TOKEN_TYPES::PUNCTUATOR && tokenValue == "}")
		stop = true;
	else if (tokenType == TOKEN_TYPES::PUNCTUATOR && tokenValue == "#" && token.tokenIndex == 0) {
		stop = true;
	}
	else if (tokenType == TOKEN_TYPES::PUNCTUATOR && tokenValue == ":"
		&& _tokens.back().tokenType == TOKEN_TYPES::ALPHANUM
		&& (_tokens.back().tokenValue == "private" || _tokens.back().tokenValue == "protected" || _tokens.back().tokenValue == "public")) {
		stop = true;
	}
	return stop;
}

// Go through all ignore sequence and find a match one
// Return:
//    bool: if there is a ignore sequence match current tokens
bool SemiExpression::setCurrentSequence(const Token& token) {
	size_t tokenIndex = 0;
	bool findOne = false;
	for (tokenIndex; tokenIndex < _tokens.size(); tokenIndex++) {
		//Check sequence
		if (sequenceIndex != -1 && ignoreIndex == ignoreSequences[sequenceIndex].size())
			break;

		if (sequenceIndex != -1) {
			if (ignoreSequences[sequenceIndex][ignoreIndex] == _tokens[tokenIndex].tokenValue) {
				ignoreIndex++;
				findOne = true;
			}
		}
		else {
			for (auto seq : ignoreSequences) {
				sequenceIndex++;
				if (seq[0] == _tokens[tokenIndex].tokenValue) {
					ignoreIndex = 1;
					findOne = true;
				}
			}
			if (!findOne) {
				sequenceIndex = -1;
			}
		}
	}

	return findOne;
}

// To merge current buffer queue and then return and set as one token collection
void SemiExpression::mergeBufferThenOutput(bool lastCharInSequence) {
	if (!lastCharInSequence) {
		auto temp = std::vector<Token>();
		while (!tokenBuffer.empty()) {
			auto buffer = tokenBuffer.front();
			tokenBuffer.pop();
			for (auto tok : buffer) {
				temp.push_back(tok);
			}
		}
		for (auto tok : _tokens) {
			temp.push_back(tok);
		}
		_tokens = temp;
	}
	else {
		popQueue = true;
		auto temp = tokenBuffer.front();
		tokenBuffer.pop();
		tokenBuffer.push(_tokens);
		_tokens = temp;
	}
}

// Popup buffer queue as many token collections
void SemiExpression::popupBuffer() {
	auto temp = std::vector<Token>();
	if (!tokenBuffer.empty()) {
		auto buffer = tokenBuffer.front();
		tokenBuffer.pop();
		tokenBuffer.push(_tokens);
		_tokens = buffer;
		popQueue = true;
	}
}

// Process current token collection based on some status
// Parameters:
//    Token token: current token 
//    bool stop: if now can stop collecting
//    bool findOne: if there is a matched sequence
// Return:
//    bool: if to stop collecting
bool SemiExpression::processCollection(const Token& token, bool stop, bool findOne) {
	std::string tokenType = token.tokenType;
	std::string tokenValue = token.tokenValue;
	bool inSequence = false;	// Determine if current tokens are in a ignore sequence
	bool endSequence = false;	// Determine if it reach the end of current ignore sequence
	bool lastCharInSequence = false;	// Determine if the last character is in ignore sequence
	if (sequenceIndex != -1) {
		//It is in a sequence
		inSequence = true;
		endSequence = (ignoreIndex == ignoreSequences[sequenceIndex].size());
	}
	else {
		endSequence = true;
	}
	lastCharInSequence = (sequenceIndex != -1 && tokenValue == ignoreSequences[sequenceIndex][ignoreIndex - 1] && findOne);

	if (stop && endSequence && !lastCharInSequence) {
		mergeBufferThenOutput(lastCharInSequence);
		return true;
	}
	else if (stop && !endSequence && lastCharInSequence) {
		tokenBuffer.push(_tokens);
		_tokens.clear();
	}
	else if (stop && !endSequence && !lastCharInSequence) {
		popupBuffer();
		return true;
	}

	return false;
}

// This is to get token from Tokenizer and return the tokens.
// parameters:
//   clear: whethere to clear current collection
//          if ture, it will clear all tokens in current token collection
// return:
//    true: there are more tokens in codes
//    false: it is reach the end of codes
bool SemiExpression::get(bool clear)
{
	// Clear token collection
	if (clear) {
		_tokens.clear();
	}
	int count = 0;
	sequenceIndex = -1;
	ignoreIndex = -1;
	while (true)
	{
		//std::cout << count++ << endl;
		if (popQueue) {
			if (!tokenBuffer.empty()) {
				_tokens = tokenBuffer.front();
				tokenBuffer.pop();
				return true;
			}
			else {
				popQueue = false;
			}
		}
		Token token = _pToker->getTok();
		std::string tokenType = token.tokenType;
		std::string tokenValue = token.tokenValue;
		if (tokenType.empty()) {
			continue;
		}
		auto stop = ifToStopCollect(token);
		push_back(token);

		if (stop) {

			bool findOne = setCurrentSequence(token);
			bool finish = processCollection(token, stop, findOne);
			if (finish)
				return true;
		}
		if (tokenType == TOKEN_TYPES::TERMINATED)
		{
			return false;
		}
	}
	return false;
}

// Return token in index n
Token SemiExpression::operator[](size_t n)
{
	if (n < 0 || n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}

// Find a token based on token value
// Return:
//    size_t: index of the token
size_t Scanner::SemiExpression::find(const std::string & tok)
{
	int count = 0;
	for (auto token : _tokens) {
		if (token.tokenValue == tok) {
			return count;
		}
		count++;
	}
	return -1;
}

// Return the size of current token collection
size_t SemiExpression::length()
{
	return _tokens.size();
}

// Print out semi-expression result to command line
// Parameters:
//    bool showNewLines: if show new line
std::string SemiExpression::show(bool showNewLines )
{
	for (auto token : _tokens)
		if ((token.tokenValue != "\n" && token.tokenType != TOKEN_TYPES::NEWLINE) || showNewLines)
			std::cout << token.tokenValue << " ";
	std::cout << "\n";
	return std::string("");
}

// Push a token to the end of current token collection
void Scanner::SemiExpression::push_back(const Token tok)
{
	_tokens.push_back(tok);
}

// Un-implemented
bool Scanner::SemiExpression::merge(const Token firstTok, const Token secondTok)
{
	return false;
}

// Remove a token by token value
bool Scanner::SemiExpression::remove(const std::string& value){
	int index = 0;
	for (auto token : _tokens) {
		if (token.tokenValue == value) {
			_tokens.erase(_tokens.begin() + index);
			return true;
		}
		index++;
	}
	return false;
}

// Remove a token by token value and token type
bool Scanner::SemiExpression::remove(const Token tok)
{
	int count = 0;
	for (auto token : _tokens) {
		if (token.tokenType == tok.tokenType && token.tokenValue == tok.tokenValue) {
			_tokens.erase(_tokens.begin() + count);
			return true;
		}
		count++;
	}
	return false;
}

// Remove element in specified position
// Parameters:
//    size_t t: index of the element
bool Scanner::SemiExpression::remove(size_t i)
{
	if (i < _tokens.size()) {
		_tokens.erase(_tokens.begin() + i);
		return true;
	}
	return false;
}

// Change all token value to lower case
void Scanner::SemiExpression::toLower(){
	size_t i = 0;
	for (i = 0; i < _tokens.size(); i++) {
		auto token = _tokens.begin() + i;
		std::transform(token->tokenValue.begin(), token->tokenValue.end(), token->tokenValue.begin(), ::tolower);

	}
}

// Remove all newline and space in front of a string
void Scanner::SemiExpression::trimFront(){
	for (auto token : _tokens) {
		token.tokenValue = trimL(token.tokenValue);
	}
}

// Clear token collection
void Scanner::SemiExpression::clear()
{
	_tokens = std::vector<Token>();
}

// Compare if two ignore sequence are equal.
bool Scanner::SemiExpression::compareSequence(std::vector<string> s1, std::vector<string> s2) {
	if (s1.size() != s2.size()) {
		return false;
	}
	else {
		size_t i;
		for (i=0; i < s1.size(); i++) {
			if (s1[i] != s2[2]) {
				return false;
			}
		}
	}
	return true;
}

// Set ignore sequence by passing ignore sequence
// Parameters:
//    sequence: vector of string of ignore sequence
//    replace: if to replace old configuration
void Scanner::SemiExpression::setTerminator(std::vector<std::string> sequences, bool replace) {
	if (replace) {
		ignoreSequences = std::vector<std::vector<std::string>>();
	}
	for (auto s : sequences ) {
		if (!trimL(s).empty()) {
			std::string configValue = trimL(s);
			auto ignores = convertStringToVector(configValue);
			bool duplicated = false;
			for (auto s : ignoreSequences) {
				if (compareSequence(s, ignores)) {
					duplicated = true;
					break;
				}
			}
			if(!duplicated)
				ignoreSequences.push_back(ignores);
		}
	}
}

// Apply configuration from config file
void Scanner::SemiExpression::applyConfig() {
	std::string fileSpec = "./ignore_config.txt";
	std::ifstream in(fileSpec);

	vector<std::string> sequences;
	for (std::string line; std::getline(in, line); ) {
		if (!trimL(line).empty()) {
			std::string configValue = trimL(line);
			sequences.push_back(configValue);
		}
	}
	setTerminator(sequences);
}

// Convert a string to vector, split by some specific string.
std::vector<std::string> Scanner::SemiExpression::convertStringToVector(std::string s) {
	s = trimL(s);
	auto vectorS = split(s, ' ');
	return vectorS;
}

std::vector<vector<std::string>> Scanner::SemiExpression::getIgnoreSequence() {
	return std::vector<std::vector<std::string>>(ignoreSequences);
}


//----< test stub >--------------------------------------------------

#ifdef TEST_SEMIEXPRESSION

#include <fstream>
int main()
{
	Toker toker;
	std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker.attach(&in);

	SemiExpression semi(&toker);
	while (semi.get())
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (semi.length() > 0)
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
		std::cout << "\n\n";
	}
	return 0;
}
#endif