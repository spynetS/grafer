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


double infix_eval(char* str){
	Token** inf_tokens = malloc(sizeof(Token)*20);
	Token** pos_tokens = malloc(sizeof(Token)*20);
	int size = 0;
	tokenize(inf_tokens,&size,str);
	Calculator *calc = new_calculator();

	printf("%s\n",str);

	infix_to_posfix(pos_tokens,inf_tokens, size);

	return eval_tokens(calc,pos_tokens,size);
}

void test_infix(){

// Basic operations
	assert(infix_eval("3 + 4") == 7);
	assert(infix_eval("10 - 5") == 5);
	assert(infix_eval("6 * 7") == 42);
	assert(infix_eval("20 / 4") == 5);

// Order of operations (precedence)
	assert(infix_eval("2 + 3 * 4") == 14);          // 3*4 first, then add 2
	assert(infix_eval("(2 + 3) * 4") == 20);        // parentheses change precedence

// Division with non-integer result
	assert(infix_eval("7 / 2") == 3);               // Assuming integer division

// Negative numbers
	assert(infix_eval("-5 + 3") == -2);
	assert(infix_eval("3 + -5") == -2);
	assert(infix_eval("-3 * 4") == -12);

// Nested parentheses
	assert(infix_eval("((2 + 3) * (4 + 1))") == 25);
	assert(infix_eval("((1 + 2) + (3 + 4))") == 10);

// More complex expressions
	assert(infix_eval("5 + 3 * (10 - 4) / 2") == 14);  // 5 + (3 * 6 / 2) = 5 + 9

// Whitespaces
	assert(infix_eval("   8  *  2 ") == 16);

// Zero
	assert(infix_eval("0 + 5") == 5);
	assert(infix_eval("0 * 100") == 0);

}


int main(){
	//test_tokenizer();
	test_turing();
	test_infix();
	printf("All tests passed!\n");
}
