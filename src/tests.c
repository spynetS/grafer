#include <assert.h>
#include <stdio.h>
#include "posfix.h"
#include "turing.h"


void test_posfix_calculate() {
	// Basic arithmetic assert(posfix_calculate("3 4 +") == 7.0);
	assert(posfix_calculate("10 5 -") == 5.0);
	assert(posfix_calculate("2 3 *") == 6.0);
	assert(posfix_calculate("8 2 /") == 4.0);

	// Multiple operations
	assert(posfix_calculate("2 3 + 5 *") == 25.0);
	assert(posfix_calculate("5 1 2 + 4 * + 3 -") == 14.0);
	assert(posfix_calculate("10 2 8 * + 3 -") == 23.0);

	// Repeated operations
	assert(posfix_calculate("1 2 + 3 + 4 + 5 +") == 15.0);

	// Edge cases
	assert(posfix_calculate("5") == 5.0);
	assert(posfix_calculate("2 10 10 + -") == -18.0);



}

void test_turing(){
	Calculator calc;
	calc.f_index = 0;
	char exp[6] = {"x 20 *"};
	add_function(&calc,"f",exp );

	for (int i = 0; i < 1; i++) {
		printf("Name: %s\nExpression: %s\n",calc.functions[i].name,calc.functions[i].expression);
		for (int j=0; j < 100; j++) {
			printf("%lf\n",call_function(&calc.functions[i], j));
		}

	}

}

int main(){
	test_posfix_calculate();

	test_turing();
	printf("All tests passed! 🎉\n");
}
