#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "calculator.h"
#include "infix_converter.h"

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

void test_infix(){
	Token** inf_tokens = malloc(sizeof(Token)*10);
	Token** pos_tokens = malloc(sizeof(Token)*10);
	int size = 0;
	tokenize(inf_tokens,&size,"f(x) = x + 10");

	print_tokens(inf_tokens,size);

	infix_to_posfix(pos_tokens,inf_tokens,size);
	Calculator *calc = new_calculator();

	printf("%lf\n",eval_tokens(calc,pos_tokens,size));

	printf("%lf\n",eval(calc,"f(8) 2 +"));
}


int main(){
	//test_tokenizer();
	test_turing();
	test_infix();
	printf("All tests passed!\n");
}
