#include <iostream>
#include "test/test.hpp"

int main() {
	std::cout << "Test" << std::endl;

	int a = 1;
	CHECK(1 == 1); // success
	// CHECK(a != 2); // error
	// EQUAL(a, 2, "a == 2");

	displaySumaryError();
	return 0;
}