#ifndef TURING_H_
#define TURING_H_


// holds the name to be called
#include "posfix.h"
typedef struct function{
	char* name;
	Token** expression;
	int size;
} Function;

typedef struct variable{
	char* name;
	double value;
} Variable;

typedef struct calculator{
	Function *functions[10];
	int f_index;
	Variable variables[10];
	int v_index;

} Calculator;

void add_function(Calculator* calculator, char* name, Token** expression, int size);
void add_variable(Calculator* calculator, char* name, double value);

double call_function(Function* function, double x);
double eval(Calculator* calc, char *str);

#endif // TURING_H_
