#include <assert.h>
#include <stdio.h>
#include "posfix.h"


void test_calculate() {
	// Basic arithmetic assert(calculate("3 4 +") == 7.0);
	assert(calculate("10 5 -") == 5.0);
	assert(calculate("2 3 *") == 6.0);
	assert(calculate("8 2 /") == 4.0);

	// Multiple operations
	assert(calculate("2 3 + 5 *") == 25.0);
	assert(calculate("5 1 2 + 4 * + 3 -") == 14.0);
	assert(calculate("10 2 8 * + 3 -") == 23.0);

	// Repeated operations
	assert(calculate("1 2 + 3 + 4 + 5 +") == 15.0);

	// Edge cases
	assert(calculate("5") == 5.0);
	assert(calculate("2 10 10 + -") == -18.0);

	printf("All tests passed! 🎉\n");
}

int main(){
	test_calculate();
}
