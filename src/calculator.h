#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include "tokenizer.h"


typedef struct graph{
	float x_min;
    float x_max;
    float y_min;
    float y_max;
    int width;      // in pixels
    int height;     // in pixels
} Graph;

typedef struct function{
	char* name;
	Token** expression;
	int size;
} Function;

typedef struct variable{
	char* name;
	double value;
} Variable;

// TODO fix dynamic arrays
typedef struct calculator{
	Function *functions[10];
	int f_index;
	Variable *variables[10];
	int v_index;
	Graph *graph;

} Calculator;

Calculator *new_calculator();
void free_calculator(Calculator* calc);
void free_function(Function* func);
void free_variable(Variable* var);


void add_function(Calculator* calculator, char* name, Token** expression, int size);
void add_variable(Calculator* calculator, char* name, double value);

Function *get_function(Calculator *calc, char* name);
double call_function(Calculator *calc,Function function, double x);
double eval(Calculator* calc, char *str);
double eval_tokens(Calculator* calc, Token** tokens, int size);


#endif // CALCULATOR_H_
