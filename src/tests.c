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

	// decimals
	assert(posfix_calculate("2.2 2.2 +") == 4.4);
	assert(posfix_calculate("2 5 /") == 0.4);


	// Edge cases
	assert(posfix_calculate("5") == 5.0);
	assert(posfix_calculate("2 10 10 + -") == -18.0);

}

void test_tokenizer(){
	Token* tokens[10];
	int size = 0;

	tokenize(tokens,&size,"x = 2 2 * ");
	for(int i = 0; i < size; i++){
		printf("%s ",type_string[tokens[i]->type]);
	}
	puts("");
	for(int i = 0; i < size; i++){
		printf("%s ",tokens[i]->value);
	}
	puts("");
}

void test_turing(){
	Calculator calc;
	calc.f_index = 0;

	eval(&calc, "f(x)= x 10 /");
	eval(&calc, "10 f(10) *");//call_function(calc.functions[0],1));

	/* for(double i = 0; i < 10; i ++){ */
	/* 	double ans = call_function(*calc.functions[0],i); */
	/* 	printf("f(%lf) = %lf\n",i,ans); */
	/* } */


	free_function(calc.functions[0]);
	//free_variable(calc.variables[0]);

}

int main(){
	test_posfix_calculate();
	//test_tokenizer();
	test_turing();
//	printf("%lf\n",posfix_calculate("x = 10 10 +"));
	printf("All tests passed!\n");
}
