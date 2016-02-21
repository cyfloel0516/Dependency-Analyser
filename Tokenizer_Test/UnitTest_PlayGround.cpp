#include "CppUnitTest.h"
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
namespace Tokenizer_Test
{		

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}


	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	TEST_CLASS(Tokenizer_Test)
	{
	public:
		TEST_METHOD(TestMethod2);
	};
	inline void Tokenizer_Test::TestMethod2()
	{
		// TODO: Your test code here
		//std::vector<std::string> test = { "123", "234" };
		//Assert::AreEqual(std::find(test.begin(), test.end(), "2334") != test.end(), true);
		//Assert::AreEqual(test.find("test1")->second, );
		std::string fileSpec = "config.txt";
		std::ifstream in(fileSpec);
		if (!in.good())
		{
			Logger::WriteMessage("no file");
		}
		else {

			char *line = new char[1000];

			for (std::string line; std::getline(in, line); ) {
				Logger::WriteMessage(&line[0]);
				Logger::WriteMessage(&to_string(line.find("single_character_token"))[0]);
				if (line.find("single_character_token") != -1) {
					int beginIndex = line.find_first_of('"');
					std::string configValue = line.substr(beginIndex + 1, configValue.size() - beginIndex);
					auto singleTokens = split(configValue, ',');
					for (auto token : singleTokens) {
						Logger::WriteMessage(&token[0]);
					}
					
				}
			}
		}
	}
}