#include "CppUnitTest.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include "Tokenizer.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Scanner;

namespace Tokenizer_Test
{		
	TEST_CLASS(Tokenizer_Test)
	{
	public:
		TEST_METHOD(Test_Tokenizer_String)
		{
			// TODO: Your test code here
			std::string fileSpec = "../Tokenizer_Test/test_files/string.txt";

			std::ifstream in(fileSpec);

			Assert::AreEqual(in.good(), true, L"Cannot open the file.");
			
			Toker toker;
			toker.attach(&in);
			int count = 0;
			while (true)
			{
				
				Token tok = toker.getTok();

				Logger::WriteMessage(&(tok.tokenType
					+ ": " + tok.tokenValue)[0]);
				if (tok.tokenType == "Terminated") {
					break;
				}

				if (count == 0) {
					Assert::AreEqual(tok.tokenType, std::string("Alphanum"));
					Assert::AreEqual(tok.tokenValue, std::string("std"));
				}
				else if (count == 5) {
					std::string result = "Normal, quotation: \"\\\'\", slash: \\\\, tab: \\t, newline: \\n, display escape: \\\\n\\\\t,  \\\nanother case.";
					Logger::WriteMessage(&result[0]);
					Assert::AreEqual(tok.tokenValue, result);
				}

				count++;
			}

			std::cout << "\n\n";

			Assert::AreEqual(count, 8);
		}

		TEST_METHOD(Test_Tokenizer_Comment)
		{
			// TODO: Your test code here
			std::string fileSpec = "../Tokenizer_Test/test_files/comment.txt";

			std::ifstream in(fileSpec);

			Assert::AreEqual(in.good(), true, L"Cannot open the file.");

			Toker toker;
			toker.attach(&in);
			int count = 0;
			while (true)
			{
				Token tok = toker.getTok();
				Logger::WriteMessage(&(to_string(count) + tok.tokenType
					+ ": " + tok.tokenValue + std::string(": ") + to_string(tok.lineNumber) + "_" + to_string(tok.tokenIndex))[0]);
				if (tok.tokenType == "Terminated") {
					break;
				}

				if (count == 0) {
					Assert::AreEqual( std::string("/*\nThis is a comment //\nAnother Line\nThird line follows by end mark*/"), tok.tokenValue);
				}
				if (count == 1) {
					Assert::AreEqual(std::string("//Special one line comment"), tok.tokenValue);
				}
				if (count == 3) {
					Assert::AreEqual(std::string("//One line comment with special character /**/ //"), tok.tokenValue);
				}
				count++;
			}

			std::cout << "\n\n";

		}

		TEST_METHOD(Test_Tokenizer_Mixed)
		{
			// TODO: Your test code here
			std::string fileSpec = "../Tokenizer_Test/test_files/mixed.txt";

			std::ifstream in(fileSpec);

			Assert::AreEqual(in.good(), true, L"Cannot open the file.");

			Toker toker;
			toker.attach(&in);
			int count = 0;
			while (true)
			{
				if (count == 144) {
					char a = 1;
				}
				Token tok = toker.getTok();
				Logger::WriteMessage(&(to_string(count) + tok.tokenType
					+ ": " + tok.tokenValue + std::string(": ") + to_string(tok.lineNumber) + "_" + to_string(tok.tokenIndex))[0]);
				if (tok.tokenType == "Terminated") {
					break;
				}

				if (count == 91) {
					Assert::AreEqual(std::string("std"), std::string(tok.tokenValue));
				}

				if (count == 97) {
					Assert::AreEqual(std::string("string"), std::string(tok.tokenValue));
				}

				if (count == 109) {
					Assert::AreEqual(std::string("storeComments"), std::string(tok.tokenValue));
				}
				count++;
			}

			std::cout << "\n\n";

		}
	};
}