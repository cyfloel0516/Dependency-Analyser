/*
* Tokenizer_Test_Console.cpp: A complete test for Tokenizer and SemiExpression
* Version: 1.0
* Application: Visual Studio 2015
* Author: Junkai Yang
*/


#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "Utils.h"
#include <windows.h>

using namespace std;
using namespace Scanner;
using namespace utils;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Print original test file and then move the pointer of stream back to position 0
void printFile(std::istream* s) {
	char* buffer = new char[1024];
	std::string line;
	while (s->good()){
		getline(*s, line);
		cout << line << endl;
	}
	s->seekg(0);
}

//print tokens of a stream from a file
void printTokenizerTest(std::istream* in, Toker toker = Toker()) {
	if (!in->good())
	{
		return;
	}
	toker.attach(in);
	while (true)
	{
		auto tok = toker.getTok();
		if (tok.tokenType == TOKEN_TYPES::TERMINATED) {
			return;
		}

		if (tok.tokenType != TOKEN_TYPES::NEWLINE) {
			auto copyString = std::string(tok.tokenType);
			copyString.resize(11, ' ');
			std::cout << copyString << ": " << tok.tokenValue
				<< endl << endl;
		}
	}
	//toker.clear();
	return ;
}

void printTokenCollection(SemiExpression& semi) {
	size_t index = 0;
	for (index = 0; index < semi.length(); index++) {
		auto tok = semi[index];
		if (tok.tokenType != TOKEN_TYPES::NEWLINE) {
			cout << tok.tokenValue << endl;
		}
		else {
			cout << "(Newline)" << endl;
		}
	}
}

void runAlphanumericTest() {

	std::string fileSpec = "test_files/token_alphanumeric.txt";
	std::fstream in(fileSpec);

	SetConsoleTextAttribute(hConsole, 3);
	cout << "--------Test 1 for requirement 4.1: Alphanumeric    -------------------" << endl;
	cout << "-- Test File: token_alphanumeric.txt                                 --" << endl;
	cout << "-- Design to test the generation of aplhanumeric tokens              --" << endl;
	cout << "-----------------------------------------------------------------------" << endl;

	SetConsoleTextAttribute(hConsole, 14);
	cout << "----------------------------Original Text:----------------------------" << endl;
	printFile(&in);
	in.close();
	in = fstream(fileSpec);
	SetConsoleTextAttribute(hConsole, 10);
	cout << "----------------------------Output Tokens----------------------------" << endl;
	printTokenizerTest(&in);

	SetConsoleTextAttribute(hConsole, 15);
	cout << "----------------------------Notes for this test----------------------" << endl;
	cout << "--You can see with the default special single character setting,   --" << endl;
	cout << "\\n will consider as two normal characterr. In next test, you can  --" << endl;
	cout << "set \\n as single special character and see the output.            --" << endl;
	cout << "---------------------------------------------------------------------" << endl;

}

void runPunctuatorTest() {
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::string fileSpec = "test_files/token_punctuator.txt";
	std::fstream in(fileSpec);

	SetConsoleTextAttribute(hConsole, 3);
	cout << "--------Test 2 for requirement 4.2: Punctuator      --------------------" << endl;
	cout << "-- Test File: token_punctuator.txt                                    --" << endl;
	cout << "-- Design to test the generation of punctuator tokens                 --" << endl;
	cout << "--------------------------------------------------------------- --------" << endl;

	SetConsoleTextAttribute(hConsole, 14);
	cout << "----------------------------Original Text:----------------------------" << endl;
	printFile(&in);
	in.close();
	in = fstream(fileSpec);
	SetConsoleTextAttribute(hConsole, 10);
	cout << "----------------------------Output Tokens----------------------------" << endl;
	printTokenizerTest(&in);

	SetConsoleTextAttribute(hConsole, 15);
	cout << "----------------------------Notes for this test----------------------" << endl;
	cout << "--This case does show the ability of my code to detect the special --" << endl;
	cout << "--characters but in next two test case you will see the special    --" << endl;
	cout << "--character test in detail.                                        --" << endl;
	cout << "---------------------------------------------------------------------" << endl;

}

void runSingleCharacterTest() {
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string userInput;

	std::string fileSpec = "test_files/token_special_single_char.txt";
	std::fstream in(fileSpec);

	SetConsoleTextAttribute(hConsole, 3);
	cout << "--------Test 3 for requirement 4.3: Single Special Character------------" << endl;
	cout << "-- Test File: token_special_single_char.txt                           --" << endl;
	cout << "-- Design to test the processing of special one character and function--" << endl;
	cout << "--------------------------------------------------------------- --------" << endl;

	cout << "Please enter the special single characters, separated by space, for example: , + - !" << endl;
	cout << "You can try \\n or just type enter to skip setting: " << endl;
	SetConsoleTextAttribute(hConsole, 15);
	getline(std::cin, userInput);

	Toker tokenizer;
	if (!trimL(userInput).empty()) {
		SetConsoleTextAttribute(hConsole, 3);
		trimL(userInput);
		auto characters = split(userInput, ' ');
		cout << "Do you want to replace the built-it special single characters? Yes(Y) or No(N), default is N:  ";
		SetConsoleTextAttribute(hConsole, 15);
		getline(std::cin, userInput);
		SetConsoleTextAttribute(hConsole, 3);
		bool replace = (userInput == "Y" || userInput == "Yes" || userInput == "y");
		tokenizer.setSpecialSingleChars(characters, replace);
	}
	SetConsoleTextAttribute(hConsole, 3);
	cout << endl <<"Current special single character setting: " << endl;
	SetConsoleTextAttribute(hConsole, 15);
	auto currectChars = tokenizer.getSpecialSingleChars();
	for (auto c : currectChars) {
		cout << utils::un_escape(c) << "  ";
	}
	cout << endl;
	SetConsoleTextAttribute(hConsole, 14);
	cout << "----------------------------Original Text:----------------------------" << endl;
	printFile(&in);
	in.close();
	in = fstream(fileSpec);
	SetConsoleTextAttribute(hConsole, 10);
	cout << "----------------------------Output Tokens----------------------------" << endl;
	printTokenizerTest(&in, tokenizer);

	SetConsoleTextAttribute(hConsole, 15);
	cout << "----------------------------Notes for this test----------------------" << endl;
	cout << "--You can see that the setSpecialSingleChars will detect duplicated--" << endl;
	cout << "--items and remove them. You can also choose whether to replace    --" << endl;
	cout << "--old configuration by passing parameter to function.              --" << endl;
	cout << "--                                                                 --" <<endl;
	cout << "--My version use configure file: token_config.txt to initialize    --" << endl;
	cout << "--the default configurations, it frees you from recompling the code --" << endl;
	cout << "--every time you want to change the default setting.               --" << endl;
	cout << "---------------------------------------------------------------------" << endl;
}

void runDoubleCharactersTest() {
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string userInput;

	std::string fileSpec = "test_files/token_special_double_chars.txt";
	std::fstream in(fileSpec);

	SetConsoleTextAttribute(hConsole, 3);
	cout << "--------Test 4 for Requirement 4.3: Double Special Characters-----------" << endl;
	cout << "-- Test File: token_special_double_chars.txt                          --" << endl;
	cout << "-- Design to test the processing of special two character and function--" << endl;
	cout << "--------------------------------------------------------------- --------" << endl;

	cout << "Please enter the special single characters, separated by space, for example: , + - !" << endl;
	cout << "You can try +- or just type enter to skip setting: " << endl;
	SetConsoleTextAttribute(hConsole, 15);
	getline(std::cin, userInput);
	Toker tokenizer;
	SetConsoleTextAttribute(hConsole, 3);
	if (!trimL(userInput).empty()) {
		
		trimL(userInput);
		auto characters = split(userInput, ' ');
		cout << "Do you want to replace the built-it special two characters? Yes(Y) or No(N), default is N: ";
		SetConsoleTextAttribute(hConsole, 15);
		getline(std::cin, userInput);
		SetConsoleTextAttribute(hConsole, 3);
		bool replace = (userInput == "Y" || userInput == "Yes" || userInput == "y");
		tokenizer.setSpecialCharPairs(characters, replace);
	}
	cout << "Current special two characters: " << endl;
	auto currectChars = tokenizer.getSpecialCharPairs();
	for (auto c : currectChars) {
		cout << c << "  ";
	}
	cout << endl;
	SetConsoleTextAttribute(hConsole, 14);
	cout << "----------------------------Original Text:----------------------------" << endl;
	printFile(&in);
	in.close();
	in = fstream(fileSpec);
	SetConsoleTextAttribute(hConsole, 10);
	cout << "----------------------------Output Tokens----------------------------" << endl;
	printTokenizerTest(&in, tokenizer);

	SetConsoleTextAttribute(hConsole, 15);
	cout << "----------------------------Notes for this test----------------------" << endl;
	cout << "--Same design as single special character processing.              --" << endl;
	cout << "---------------------------------------------------------------------" << endl;
}

void runStringTest() {
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::string fileSpec = "test_files/token_string.txt";
	std::fstream in(fileSpec);

	SetConsoleTextAttribute(hConsole, 3);
	cout << "--------Test 6 for requirement 4.6: quoted string----------------------" << endl;
	cout << "-- Test File: token_string.txt                                       --" << endl;
	cout << "-- Design to test the function generate string token                 --" << endl;
	cout << "-- First line is a double quoted string with special characters      --" << endl;
	cout << "-- Second line is a single quoted string with soecial characters     --" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
	SetConsoleTextAttribute(hConsole, 14);
	cout << "----------------------------Original Text:----------------------------" << endl;
	printFile(&in);
	in.close();
	in = fstream(fileSpec);
	SetConsoleTextAttribute(hConsole, 10);
	cout << "----------------------------Output Tokens----------------------------" << endl;
	printTokenizerTest(&in);
	SetConsoleTextAttribute(hConsole, 15);
	cout << "----------------------------Notes for this test------------------------" << endl;
	cout << "--This program can check the quotes inside the string but ignore the --" << endl;
	cout << "--escape characters such like \\n and \\t.                             --" << endl;
	cout << "--                                                                   --" << endl;
	cout << "--However, there are two special cases:                              --" << endl;
	cout << "--1. You set escape character in change the special character setting--" << endl;
	cout << "--by using tokenizer.setSpecialSingleChars() then it will detect     --" << endl;
	cout << "--special single escape character in string such like \\n.            --" << endl;
	cout << "--                                                                   --" << endl;
	cout << "--2. If you use \\ to start a newline in your editor, the program will --" << endl;
	cout << "--also start a newline to keep the original structure of the string  --" << endl;
	cout << "-----------------------------------------------------------------------" << endl;

}

void runCommentTest() {
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::string fileSpec = "test_files/token_comment.txt";
	std::fstream in2(fileSpec);

	SetConsoleTextAttribute(hConsole, 3);
	cout << "--------Test 5 for requirements 4.5: test for comment------------------" << endl;
	cout << "-- Test File: token_comment.txt                                      --" << endl;
	cout << "-- Will return cpp comment and c comment                             --" << endl;
	cout << "-- Top three lines is a c style comment, but in the end of this      --" << endl;
	cout << "-- comment, it is followed by a cpp comment in the same line.        --" << endl;
	cout << "-- Fourth line is a cpp comment.                                     --" << endl;
	cout << "-----------------------------------------------------------------------" << endl << endl;

	SetConsoleTextAttribute(hConsole, 14);
	cout << "----------------------------Original Text:----------------------------" << endl;
	printFile(&in2);
	in2.clear();
	in2.close();
	in2 = fstream(fileSpec);
	cout << endl;
	SetConsoleTextAttribute(hConsole, 10);
	cout << "----------------------------Output Tokens----------------------------" << endl;
	printTokenizerTest(&in2);

	SetConsoleTextAttribute(hConsole, 15);
	cout << "----------------------------Notes for this test----------------------" << endl;
	cout << "--This program behaves well even in the case of nested comment such--" << endl;
	cout << "--like '/* // */' or '// /*123', you can examine it in the comments--" << endl;
	cout << "--of this test case.                                               --" << endl;
	cout << "--Also, this case's return includes comment symbol. But actually   --" << endl;
	cout << "--it is unnecessary becasue my token structure does track the state--" << endl;
	cout << "--token so we can know the comment token is c style or c++ style.  --" << endl;
	cout << "---------------------------------------------------------------------" << endl;
}

void runMixedTest() {
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::string fileSpec = "test_files/Tokenizer.cpp";
	std::fstream in2(fileSpec);

	SetConsoleTextAttribute(hConsole, 3);
	cout << "--------Test 7: Mixed Test with a complicated cpp file-----------------" << endl;
	cout << "-- Test File: Tokenizer.cpp                                          --" << endl;
	cout << "--This test use a real cpp file to test the function of tokenizer    --" << endl;
	cout << "--It is designed to check if the transition between state is good and--" << endl;
	cout << "some other special cases.                                            --" << endl;
	cout << "-----------------------------------------------------------------------" << endl << endl;

	SetConsoleTextAttribute(hConsole, 14);
	cout << "----------------------------Original Text:----------------------------" << endl;
	printFile(&in2);
	in2.clear();
	in2.close();
	in2 = fstream(fileSpec);
	cout << endl;
	SetConsoleTextAttribute(hConsole, 10);
	cout << "----------------------------Output Tokens----------------------------" << endl;
	printTokenizerTest(&in2);

	SetConsoleTextAttribute(hConsole, 15);
	cout << "----------------------------Notes for this test----------------------" << endl;
	cout << "--None                                                             --" << endl;
	cout << "---------------------------------------------------------------------" << endl;
}

void runSequenceTest() {
	std::string userInput;

	system("CLS");
	runAlphanumericTest();

	std::cout << "Press enter to the next test." << endl;
	getline(std::cin, userInput);
	system("CLS");
	runPunctuatorTest();

	std::cout << "Press enter to the next test." << endl;
	getline(std::cin, userInput);
	system("CLS");
	runSingleCharacterTest();

	std::cout << "Press enter to the next test." << endl;
	getline(std::cin, userInput);
	system("CLS");
	runDoubleCharactersTest();

	std::cout << "Press enter to the next test." << endl;
	getline(std::cin, userInput);
	system("CLS");
	runCommentTest();

	std::cout << "Press enter to the next test." << endl;
	getline(std::cin, userInput);
	system("CLS");
	runStringTest();

	std::cout << "Press enter to the next test." << endl;
	getline(std::cin, userInput);
	system("CLS");
	runMixedTest();

}

void runTokenizerTest() {

	std::string userInput;
	
	while (true) {
		system("CLS");
		SetConsoleTextAttribute(hConsole, 3);
		std::cout << "--------------------------------------------------------------" << endl;
		std::cout << "--Author: Junkai Yang, jyang125@syr.edu                     --" << endl;
		std::cout << "--Source: Jim Fawcett                                       --" << endl;
		std::cout << "--For CES687 Project 1: Tokenizer                           --" << endl;
		std::cout << "--I redesign the token structure and introduce some other   --" << endl;
		std::cout << "--state to process string and termination.                  --" << endl;
		std::cout << "--You can alos see that I track the token type in my token  --" << endl;
		std::cout << "--structure because as a foudation function, I believe some --" << endl;
		std::cout << "--\"over design\" is necessary.                             --" << endl;
		std::cout << "---------------              :-)                 -------------" << endl << endl;
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Please select test case:" << endl;
		std::cout << "1. Run all test cases one by one (preferred)" << endl;
		std::cout << "2. Alphanumeric Test" << endl;
		std::cout << "3. Punctuator Test" << endl;
		std::cout << "4. Special One Character Test" << endl;
		std::cout << "5. Special Two Characters Test" << endl;
		std::cout << "6. Comment Test" << endl;
		std::cout << "7. Quoted String Test" << endl;
		std::cout << "8. Mixed Test with complicated cpp file" << endl;
		std::cout << "9. Back to previous menu" << endl;

		getline(cin, userInput);
		userInput = trimL(userInput);
		if (userInput.size() == 1) {
			if (userInput == "1") {
				system("CLS");
				runSequenceTest();
			}
			else if(userInput == "2"){
				system("CLS");
				runAlphanumericTest();
			}
			else if (userInput == "3") {
				system("CLS");
				runPunctuatorTest();
			}
			else if (userInput == "4") {
				system("CLS");
				runSingleCharacterTest();
			}
			else if (userInput == "5") {
				system("CLS");
				runDoubleCharactersTest();
			}
			else if (userInput == "6") {
				system("CLS");
				runCommentTest();
			}
			else if (userInput == "7") {
				system("CLS");
				runStringTest();
			}
			else if (userInput == "8") {
				system("CLS");
				runMixedTest();
			}
			else if (userInput == "9") {
				return;
			}
			else {
				std::cout << "Please enter number to select test case." << endl;
				continue;
			}
		}
		else {
			std::cout << "Please enter number to select test case." << endl;
			continue;
		}
		std::cout << endl <<"Press enter to the next test." << endl;
		getline(std::cin, userInput);
	}
}

void printSemiTestResult(SemiExpression semi) {
	while (true)
	{
		bool next = semi.get();
		SetConsoleTextAttribute(hConsole, 8);
		std::cout << "--semiExpression--" << endl;
		SetConsoleTextAttribute(hConsole, 10);
		semi.show(false);
		std::cout << endl;
		if (!next)
			return;
	}
}

void runSemiMixedTest() {
	Toker toker;
	std::string fileSpec = "test_files/Tokenizer.cpp";
	std::fstream in(fileSpec);
	SetConsoleTextAttribute(hConsole, 3);
	cout << "-------Semi Expression Test 3: Mixed Test                    ---------" << endl;
	cout << "-- Test File: Tokenizer.cpp                                         --" << endl;
	cout << "----------------------------------------------------------------------" << endl;

	SetConsoleTextAttribute(hConsole, 14);
	cout << "----------------------------Original Text:----------------------------" << endl;
	printFile(&in);
	in.clear();
	in.close();
	in = fstream(fileSpec);
	cout << endl;

	toker.attach(&in);
	SemiExpression semi(&toker);
	SetConsoleTextAttribute(hConsole, 10);
	cout << "----------------------------Output Tokens----------------------------" << endl;
	printSemiTestResult(semi);

	SetConsoleTextAttribute(hConsole, 15);
	cout << "----------------------------Notes for this test------------------------" << endl;
	cout << "--It is a complicated file test for you to examine the function of   --" << endl;
	cout << "--token collecting.                                                  --" << endl;
	
cout << "----------------------------------------------------------------------" << endl;
}

void runSemiGeneralTest() {
	Toker toker;
	std::string fileSpec = "test_files/semi_general.txt";
	std::fstream in(fileSpec);

	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, 3);
	cout << "-------Semi Expression Test 1 for requirement 6, 7: General Test-------" << endl;
	cout << "-- Test File: semi_general.txt                                      --" << endl;
	cout << "--This test design to test the termination of token collecting in   --" << endl;
	cout << "--normal terminator such like ; { and some special cases such like  --" << endl;
	cout << "--public :                                                          --" << endl;
	cout << "----------------------------------------------------------------------" << endl;

	SetConsoleTextAttribute(hConsole, 14);
	cout << "----------------------------Original Text:----------------------------" << endl;
	printFile(&in);
	in.clear();
	in.close();
	in = fstream(fileSpec);
	cout << endl;

	toker.attach(&in);
	SemiExpression semi(&toker);
	SetConsoleTextAttribute(hConsole, 10);
	cout << "----------------------------Output Tokens----------------------------" << endl;
	printSemiTestResult(semi);

	SetConsoleTextAttribute(hConsole, 15);
	cout << "----------------------------Notes for this test----------------------" << endl;
	cout << "--This test includes all terminate cases and you can follow the    --" << endl;
	cout << "--comments to examine the result.                                  --" << endl;
	cout << "--Notice that the code can deal with the space before the first '#'--" << endl;
	cout << "--of a new line. Also the code will distinguish the token type to  --" << endl;
	cout << "--avoid from terminating from some 'bad' string such like '{#}'    --" << endl;
	cout << "---------------------------------------------------------------------" << endl;
}


void runSemiInterfaceTest() {
	std::string userInput;
	Toker toker;
	std::string fileSpec = "test_files/semi_general.txt";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << " Something wrong with test file. " << fileSpec << endl;
		return;
	}

	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SemiExpression semi(&toker);
	toker.attach(&in);
	SetConsoleTextAttribute(hConsole, 3);
	cout << "-------Semi Expression Test 4 for requirement 9: Semi Interface Test---" << endl;
	cout << "--Test File: semi_general.txt                                        --" << endl;
	cout << "--This test design to test the interface of semi expression.         --" << endl;
	cout << "-----------------------------------------------------------------------" << endl << endl;
	while (true)
	{
		bool next = semi.get(false);
		if (!next)
			break;
	}
	SetConsoleTextAttribute(hConsole, 14);
	cout << "current token collection:" << endl;
	//semi.show(true);
	printTokenCollection(semi);
	//-----------------------------------------------------------------------
	cout << "---------------------------Get token by index.---------------------------------" << endl;
	cout << "Please input the index of token you want to get: ";
	SetConsoleTextAttribute(hConsole, 15);
	getline(std::cin, userInput);
	SetConsoleTextAttribute(hConsole, 14);
	int index = 0;
	try {
		index = stoi(userInput);
	}
	catch (const std::exception& e) {
		cout << e.what();
		cout << "Invalid input, set index as 0 by default!" << endl;
	}

	try {
		auto tok = semi[index];
		cout << "Here is your token: " << endl;
		cout << "Token type: " << tok.tokenType << ". Token value: " << tok.tokenValue << endl;
	}
	catch (const std::exception& e) {
		cout << e.what();
		cout << "can find the token in such index, index may out of range!";
	}
	cout << endl;
	
	//-----------------------------------------------------------------------
	cout << "---------------------------Delete token by index.---------------------------------" << endl;
	cout << "To delete a token, please enter the index of the token: ";
	SetConsoleTextAttribute(hConsole, 15);
	getline(std::cin, userInput);
	SetConsoleTextAttribute(hConsole, 14);
	try {
		index = stoi(userInput);
	}
	catch (const std::exception& e) {
		cout << e.what();
		cout << "Invalid input, set index as 0 by default!" << endl;
	}

	semi.remove(index);
	cout << "Token has been deleted, current tokens: " << endl;
	printTokenCollection(semi);

	//-----------------------------------------------------------------------
	cout << "---------------------------Add a token.---------------------------------" << endl;;
	cout << "To add a token, please enter the token value, the token is treated as aplhanumeric by default:  ";
	SetConsoleTextAttribute(hConsole, 15);
	getline(std::cin, userInput);
	Token tok = Token();
	tok.tokenType = TOKEN_TYPES::ALPHANUM;
	tok.tokenValue = userInput;
	semi.push_back(tok);

	SetConsoleTextAttribute(hConsole, 14);
	semi.remove(index);
	cout << "Token has been added, current tokens: " << endl;
	printTokenCollection(semi);

	//-----------------------------------------------------------------------
	cout << "---------------------------Find a token.---------------------------------" << endl;
	cout << "Please input the token value you want to find: ";
	SetConsoleTextAttribute(hConsole, 15);
	getline(std::cin, userInput);
	index = semi.find(userInput);
	SetConsoleTextAttribute(hConsole, 14);
	cout << "Token index: " << index << endl;

	//-----------------------------------------------------------------------
	cout << "---------------------------To lower test---------------------------------" << endl;
	cout << "I am going transform all token value to lower case." << endl;
	getline(std::cin, userInput);
	cout << "Done! Current tokens: " << endl;
	semi.toLower();
	printTokenCollection(semi);

	//-----------------------------------------------------------------------
	cout << "---------------------------Trim front test---------------------------------" << endl;
	cout << "I am going trim all token value." << endl;
	getline(std::cin, userInput);
	cout << "Done! Current tokens: " << endl;
	semi.trimFront();
	printTokenCollection(semi);

	cout << "You finish the interface test, thank you!" << endl;
}



void runSemiCustomTest() {
	std::string userInput;
	Toker toker;
	std::string fileSpec = "test_files/semi_custom.txt";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << " Something wrong with test file. " << fileSpec << endl;
		return;
	}

	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SemiExpression semi(&toker);

	SetConsoleTextAttribute(hConsole, 3);
	cout << "-------Semi Expression Test 2 for requirement 8: Custom Terminator----" << endl;
	cout << "-- Test File: semi_custom.txt                                       --" << endl;
	cout << "--This test design to test the custom termination of token collecting." << endl;
	cout << "----------------------------------------------------------------------" << endl << endl;

	cout << "There is a built-in terminate sequence 'for ( ; ; )'" << endl;
	cout << "Now you can input your custom terminate sequence, one line for one terminator." << endl;
	cout << "Sequence should be like for ( ; ; ), use space to separate punctuator." << endl;
	cout << "You can try  { ; ;  to see what happen. Empty line to stop setting: " << endl;

	SetConsoleTextAttribute(hConsole, 15);
	std::vector<std::string> terminators;
	while (getline(std::cin, userInput)) {
		if (trimL(userInput).empty()) {
			break;
		}
		else {
			terminators.push_back(userInput);
		}
	}
	SetConsoleTextAttribute(hConsole, 3);
	cout << "Do you want to replace the built-it terminators? Yes(Y) or No(N), default is N: ";
	SetConsoleTextAttribute(hConsole, 15);
	getline(std::cin, userInput);
	bool replace = (userInput == "Y" || userInput == "Yes" || userInput == "y");
	semi.setTerminator(terminators, replace);
	SetConsoleTextAttribute(hConsole, 3);
	cout << "Current terminator sequences are: " << endl;
	SetConsoleTextAttribute(hConsole, 15);
	auto sequences = semi.getIgnoreSequence();
	for (auto s : sequences) {
		for (auto c : s) {
			cout << c << " ";
		}
		cout << endl;
	}
	SetConsoleTextAttribute(hConsole, 14);
	cout << "----------------------------Original Text:----------------------------" << endl;
	printFile(&in);
	in.clear();
	in.close();
	in = fstream(fileSpec);
	toker.attach(&in);
	SetConsoleTextAttribute(hConsole, 10);
	cout << "----------------------------Output Tokens----------------------------" << endl;
	printSemiTestResult(semi);
	SetConsoleTextAttribute(hConsole, 15);
	cout << "----------------------------Notes for this test----------------------" << endl;
	cout << "--This test includes all terminate cases and you can follow the    --" << endl;
	cout << "--comments to examine the result.                                  --" << endl;
	cout << "--                                                                 --" << endl;
	cout << "--Now it only support terminator sequence such like for ( ; ; )    --" << endl;
	cout << "--But in the future I hope the function can support other more     --" << endl;
	cout << "--complicated terminating condition                                --" << endl;
	cout << "---------------------------------------------------------------------" << endl;
}

void runSemiSequenceTest() {
	std::string userInput;

	system("CLS");
	runSemiGeneralTest();

	std::cout << "Press enter to the next test." << endl;
	getline(std::cin, userInput);
	system("CLS");
	runSemiCustomTest();

	std::cout << "Press enter to the next test." << endl;
	getline(std::cin, userInput);
	system("CLS");
	runSemiMixedTest();

	std::cout << "Press enter to the next test." << endl;
	getline(std::cin, userInput);
	system("CLS");
	runSemiInterfaceTest();
}

void runSemiTest() {
	std::string userInput;

	while (true) {
		system("CLS");
		
		std::cout << "--------------------------------------------------------------" << endl;
		std::cout << "--Author: Junkai Yang, jyang125@syr.edu                     --" << endl;
		std::cout << "--Source: Jim Fawcett                                       --" << endl;
		std::cout << "--For CES687 Project 1: Semi Expression                     --" << endl;
		std::cout << "--This test part can help to check the function of semi     --" << endl;
		std::cout << "--expression. Benefit from the implementation of tokenizer, --" << endl;
		std::cout << "--can easily tokenizer to get token and do collection.      --" << endl;
		std::cout << "--You can run specific test case or run them one by one.    --" << endl;
		std::cout << "---------------              :-)                 -------------" << endl << endl;
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Please select test case:" << endl;
		std::cout << "1. Run all test cases one by one (preferred)" << endl;
		std::cout << "2. General Test" << endl;
		std::cout << "3. Custom Terminator Test" << endl;
		std::cout << "4. Mixed Test with complicated cpp file" << endl;
		std::cout << "5. Semi Interface Test" << endl;
		std::cout << "6. Back to previous menu" << endl;

		getline(cin, userInput);
		userInput = trimL(userInput);
		if (userInput.size() == 1) {
			if (userInput == "1") {
				system("CLS");
				runSemiSequenceTest();
			}
			else if (userInput == "2") {
				system("CLS");
				runSemiGeneralTest();
			}
			else if (userInput == "3") {
				system("CLS");
				runSemiCustomTest();
			}
			else if (userInput == "4") {
				system("CLS");
				runSemiMixedTest();
			}
			else if (userInput == "5") {
				system("CLS");
				runSemiInterfaceTest();
			}
			else if (userInput == "6") {
				return;
			}
			else {
				std::cout << "Please enter number to select test case." << endl;
				continue;
			}
		}
		else {
			std::cout << "Please enter number to select test case." << endl;
			continue;
		}
		std::cout << endl << "Press enter to the next test." << endl;
		getline(std::cin, userInput);
	}

}


int main()
{
	std::string userInput;

	while (true) {
		system("CLS");

		SetConsoleTextAttribute(hConsole, 3);
		std::cout << "----------------------------------------------------------------------------------" << endl;
		std::cout << "--Author: Junkai Yang, jyang125@syr.edu                                         --" << endl;
		std::cout << "--For CES687 Project 1                                                          --" << endl;
		std::cout << "--This program includes two parts, one for tokenizer, another for semiexpression--" << endl;
		std::cout << "--Each part of test contains a batch of test cases, you can examine the function--" << endl;
		std::cout << "--of the program through these cases.                                           --" << endl;
		std::cout << "----------------------------------------------------------------------------------" << endl;
		std::cout << "--I believe automatic test is better than manual check so I also use CPP        --" << endl;
		std::cout << "--Unittest Framework to do unit test. You can use VS to run the unittests.      --" << endl;
		std::cout << "----------------------------------------------------------------------------------" << endl;
		std::cout << "--Guidance for TA or Instructor:                                                --" << endl;
		std::cout << "Req 1, use VS2015 and C++ console project:                       code checking" << endl;
		std::cout << "Req 2, use C++ standard library's streams for all I/O and...:    code checking" << endl;
		std::cout << "Req 3, packages structure:                                       code checking" << endl;
		std::cout << "Req 4, tokenizer functions:                                      run 1.Toeknizer Test->1.Sequence Test" << endl;
		std::cout << "Req 5, get() function in Toker class:                            as well as Req 4" << endl;
		std::cout << "Req 6 7 8, Semi Expression functions:                            run 2.SemiExpression->1.Sequence Test" << endl;
		std::cout << "Req 9, implementation of interface:                              run 2.SemiExpression->5.Interface Test" << endl;
		std::cout << "Req 10, include an automated unit test                           see this whole test program" << endl;
		std::cout << "---------------------------------------------------------------------------------" << endl;
		std::cout << "---------------              :-)                 -------------" << endl << endl;
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Please select test item:" << endl;
		std::cout << "1. Tokenizer test" << endl;
		std::cout << "2. SemiExpression test" << endl;
		std::cout << "Please type number and press enter to select test item: " << endl;

		getline(cin, userInput);
		userInput = trimL(userInput);
		if (userInput.size() != 1 || (userInput != "1" && userInput != "2")) {
			continue;
		}
		else {
			if (userInput == "1") {
				runTokenizerTest();
			}
			else if (userInput == "2") {
				runSemiTest();
			}
		}
	}
	

	return 1;
}

