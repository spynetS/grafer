#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphing.h"
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

	eval(calc,"f(x) = x 10 -");
	assert(eval(calc,"f(10)")==0);

	free_calculator(calc);
}

void test_graph(){
	Calculator *calc = new_calculator();
	calc->graph->x_min = -10;
	calc->graph->x_max = 20;
	calc->graph->y_min = -10;
	calc->graph->y_max = 30;

	eval(calc,"g(x) = x 10 - 2 ^ 1 +");
	eval(calc,"f(x) = x 10 +");
	eval(calc,"h(x) = x 3 ^");
	system("clear");
	draw(calc, calc->functions[0]);
	draw(calc, calc->functions[1]);
	//draw(calc, calc->functions[2]);
}


int main(){
	//test_tokenizer();
	test_turing();
	test_graph();
	printf("All tests passed!\n");
}
