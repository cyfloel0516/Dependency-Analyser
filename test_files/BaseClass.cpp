#include "Test1.h"
#include <iostream>
#include <iostream>
int Test::BaseClass::method2() {
	try {
		int i = 0;
		std::cout << i;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
		return 0;
	}
}


int Test::BaseClass::method3() {
	return 0;
}



