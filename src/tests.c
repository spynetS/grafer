#include <assert.h>
#include <stdio.h>
#include "tokenizer.h"
#include "calculator.h"

void test_turing(){
	Calculator *calc = new_calculator();

	// settings some values
	eval(calc, "x = 10");
	eval(calc, "f(x) = x 10 +");
	eval(calc, "g(x) = f(x) 2 *");

	// testing the values
	assert(eval(calc,"10 10 2 * +") == 30);
	assert(eval(calc,"x") == 10);
	assert(eval(calc,"f(10)") == 20);
	assert(eval(calc, "g(x)")== 40);

	free_calculator(calc);
}

int main(){
	//test_tokenizer();
	test_turing();
	printf("All tests passed!\n");
}
