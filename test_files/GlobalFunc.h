#ifndef TEST1_H
#define TEST1_H

#include <iostream>
#include <vector>
#include <map>

std::function<int(int, int)> globalLambda = [=](int a, int b) -> int {return a + b;};

namespace GFunc{
	inline void gFunctionInNS() {
		std::map<int, int> map = { { 1,1 },{ 2,2 } };
		while (true) {
			{
				std::cout << "\n Global Function";
			}
			break;
		}
	}
}


static void globalFuncOutsideNS() {
	std::map<int, int> map = { { 1,1 },{ 2,2 } };
	while (true) {
		{
			std::cout << "\n Global Function";
		}
		break;
	}
}

template<typedef T, typedef P>
static void templateFunc(int a) {
	return true;
}
