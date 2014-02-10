#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include "Test.h"

extern Test *vectorTest;

Test *tests[] = {
	vectorTest
};

unsigned int numTests = sizeof(tests) / sizeof(Test*);

int main() {
	int errors = 0;
	
	for (unsigned int i = 0; i < numTests; i++) {
		errors += (*tests[i])();
	}

	std::cout << Test::norm << "All tests complete : " << (errors == 0 ? Test::good : Test::bad) << errors << " errors" << Test::norm << "." << std::endl;

	return 0;
}
