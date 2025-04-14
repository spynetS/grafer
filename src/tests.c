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


double infix_eval(Calculator *calc, char* str){
	Token** inf_tokens = malloc(sizeof(Token*)*20);
	Token** pos_tokens = malloc(sizeof(Token*)*20);
	int size_inf = 0;
	int size = 0;
	tokenize(inf_tokens,&size_inf,str);

	infix_to_posfix(pos_tokens,size_inf, inf_tokens, &size);
	//print_tokens(pos_tokens, size);

	double ans = eval_tokens(calc,pos_tokens,size);
	for(int i = 0; i < size_inf;i ++){
		free_token(inf_tokens[i]);
	}
	for(int i = 0; i < size;i ++){
		free_token(pos_tokens[i]);
	}

	free(inf_tokens);
	free(pos_tokens);

	return ans;
}

void test_infix(){

// Basic operations
	Calculator *calc = new_calculator();
	free_calculator(calc);

	infix_eval(calc,"x = 3 + 4");

	assert(infix_eval(calc,"x") == 7);
	assert(infix_eval(calc,"x + 3") == 10);

	assert(infix_eval(calc,"10 - 5") == 5);
	assert(infix_eval(calc,"6 * 7") == 42);
	assert(infix_eval(calc,"20 / 4") == 5);

// Order of operations (precedence)
	assert(infix_eval(calc,"2 + 3 * 4") == 14);
	// 3*4 first, then add 2
	assert(infix_eval(calc,"(2 + 3) * 4") == 20);        // parentheses change precedence

// Division with non-integer result
	assert(infix_eval(calc,"7 / 2") == 3.5);               // Assuming integer division

// Negative numbers
	assert(infix_eval(calc,"-5 + 3") == -2);
	assert(infix_eval(calc,"3 + -5") == -2);
	assert(infix_eval(calc,"-3 * 4") == -12);

// Nested parentheses
	assert(infix_eval(calc,"((2 + 3) * (4 + 1))") == 25);
	assert(infix_eval(calc,"((1 + 2) + (3 + 4))") == 10);

// More complex expressions
	assert(infix_eval(calc,"5 + 3 * (10 - 4) / 2") == 14);  // 5 + (3 * 6 / 2) = 5 + 9

// Whitespaces
	assert(infix_eval(calc,"   8  *  2 ") == 16);

// Zero
	assert(infix_eval(calc,"0 + 5") == 5);
	assert(infix_eval(calc,"0 * 100") == 0);

}


int main(){
	//test_tokenizer();
	//test_turing();
	test_infix();
	printf("All tests passed!\n");
}
