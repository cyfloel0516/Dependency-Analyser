#ifndef TEST1_H
#define TEST1_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

namespace Test {
	struct element {
		std::string name, type;
		std::vector<element> children;
	};

	enum Numbers { one = 1, two, three, four };

	class BaseClass {
	public:
		One() {};
		~One() {};
		void inlineMethod(int a) {
			int i;
		}
		int method2();
		static int method3();
	};

	template<typedef T>
	class TemplateBaseClass{
		static int templateClassFunc() { return 1; };
	};
}

#endif