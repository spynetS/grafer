#include "calculator.h"
#include "graphing.h"
#include "tokenizer.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



Calculator *new_calculator(){
    Calculator *calc = malloc(sizeof(Calculator));
    calc->f_index = 0;
    calc->v_index = 0;

	calc->graph = malloc(sizeof(Graph));
	calc->graph->x_max = 10;
	calc->graph->y_max = 10;
	calc->graph->x_min = 0;
	calc->graph->y_min = 0;
	calc->graph->width = 30;
	calc->graph->height = 50;
	
    for (int i = 0; i < 10; i++) {
        calc->functions[i] = NULL;
        calc->variables[i] = NULL;
    }
    return calc;
}
void free_calculator(Calculator* calc){
    for(int i = 0; i < calc->f_index; i++){
        free_function(calc->functions[i]);
    }
    for(int i = 0; i < calc->v_index; i++){
        free_variable(calc->variables[i]);
    }
	free(calc->graph);
    free(calc);
}

Variable* get_variable(Calculator *calc, char* name) {
    if (!calc || !name) return NULL;

    for (int i = 0; i < calc->v_index; i++) {
        Variable* var = calc->variables[i];
        if (var && var->name && strcmp(var->name, name) == 0) {
            return var;
        }
    }
    return NULL;
}


void free_function(Function* func){
    if(func != NULL){
        free(func->name);
        for(int i = 0; i < func->size; i ++){
            free_token(func->expression[i]);
        }
        free(func->expression);
        free(func);
    }

}
void free_variable(Variable* var){
    if(var != NULL){
        free(var->name);
        free(var);
    }
}

void print_tokens(Token** tokens, int size){
    printf("TOKENS: ");
    for(int i = 0; i < size; i++){
        printf("%s ",type_string[tokens[i]->type]);
	}
    printf("\nVALUE: ");
    for(int i = 0; i < size; i++){
        printf("%s ",tokens[i]->value);
	}
    puts("");
}
double eval(Calculator* calc, char *str){

    Token** tokens = malloc(sizeof(Token)*100);
	int size = 0;
	tokenize(tokens,&size,str);

    double ans = eval_tokens(calc,tokens,size);

    for(int i = 0; i < size; i ++){
        free_token(tokens[i]);
    }
    free(tokens);
    return ans;
}

double eval_tokens(Calculator* calc, Token** tokens, int size){
    double ans = 0;

	Stack(double) stack;
	INIT_STACK(stack);


    // TODO
    // replace all variables with the value if exists
    // repalce function calls with function calls if exists

    //print_tokens(tokens,size);

	for(int i = 0; i < size; i++){
        Token* token = tokens[i];
        if(i + 2 < size && tokens[i]->type == VARIABLE &&
           tokens[i+1]->type == EQUALS ){
            double value = eval_tokens(calc,&tokens[i+2], size-2);
            //printf("variable assignment %s = %lf\n",tokens[i]->value,value);
            add_variable(calc, tokens[i]->value, value);
            i += 2;
        }
        else if(i + 4 < size && tokens[i]->type == FUNC &&
           tokens[i+1]->type == O_P &&
           tokens[i+2]->type == VARIABLE &&
           tokens[i+3]->type == C_P &&
           tokens[i+4]->type == EQUALS){
            //printf("Function assignment\n");
            add_function(calc,  tokens[i]->value, &tokens[i+5],size-5);
            i+=5;
        }
        else if(i + 3 < size && tokens[i]->type == FUNC &&
           tokens[i+1]->type == O_P &&
           tokens[i+2]->type == VARIABLE &&
           tokens[i+3]->type == C_P){
            Variable* variable = get_variable(calc, tokens[i+2]->value);

            if(variable == NULL) {
                printf("EROOR");
                exit(1);
            }

            Function *func = get_function(calc,token->value);
            if(func != NULL){
                double value = call_function(calc,*func,variable->value);
                //printf("function %s(%lf) returns %lf\n",token->value,variable->value,value);
                PUSH(stack,value);
            }


            i+=3; // move forward the tokens we checked in the if
        }
        else if(i + 3 < size && tokens[i]->type == FUNC &&
           tokens[i+1]->type == O_P &&
           tokens[i+2]->type == NUMBER &&
           tokens[i+3]->type == C_P) {

            double parameter;
            sscanf(tokens[i+2]->value,"%lf",&parameter);

            Function *func = get_function(calc,token->value);
            if(func != NULL){
                double value = call_function(calc,*func,parameter);
                //printf("function %s(%lf) returns %lf\n",token->value,parameter,value);
                PUSH(stack,value);

            }

            i+=3; // move forward the tokens we checked in the if
        }
        else{
            switch(token->type){
                case VARIABLE:
                    Variable *variable = get_variable(calc,token->value);
                    if(variable == NULL){
                        printf("NO VARIABLE WITH %s\n",token->value);
                    }
                    else{
                        PUSH(stack,variable->value);
                    }
                    break;
                case NUMBER:
                    double number;
                    sscanf(token->value,"%lf",&number);
                    //printf("adds number %lf to stack\n",number);
                    PUSH(stack,number);
                    break;
                case OPERATOR:
                    //printf("pops from stack\n");
                    double b = POP(stack);
                    double a = POP(stack);

                    double ans = operate(token->value,a,b);

                    //printf("%lf %s %lf = %lf \n", a,token->value, b,ans);
                    PUSH(stack,ans);
                    break;
                default:
                    break;
            }
        }
    }
	ans = POP(stack);
    return ans;
}

Function* get_function(Calculator *calc, char* name) {
    if (!calc || !name) return NULL;

    for (int i = 0; i < calc->f_index; i++) {
        Function* f = calc->functions[i];
        if (f && f->name && strcmp(f->name, name) == 0) {
            return f;
        }
    }
    return NULL;
}

double call_function(Calculator *calc, Function function, double x) {
    // Deep copy of function tokens
    Token** copied_tokens = malloc(sizeof(Token*) * function.size);

    for (int i = 0; i < function.size; i++) {
        copied_tokens[i] = malloc(sizeof(Token));
        copied_tokens[i]->type = function.expression[i]->type;

        copied_tokens[i]->value = strdup(function.expression[i]->value);

        if (strcmp(copied_tokens[i]->value, "x") == 0) {
            // Replace "x" with value of x
            char repl[100];
            sprintf(repl, "%lf", x);
            free(copied_tokens[i]->value);
            copied_tokens[i]->value = strdup(repl);
            copied_tokens[i]->type = NUMBER;
        }
    }

    double ans = eval_tokens(calc, copied_tokens, function.size);

    // Free the copied tokens
    for (int i = 0; i < function.size; i++) {
        free(copied_tokens[i]->value);
        free(copied_tokens[i]);
    }
    free(copied_tokens);

    return ans;
}

void add_function(Calculator* calculator, char *name, Token **expression, int size) {
    // Check if function with that name exists
    for (int i = 0; i < calculator->f_index; i++) {
        if (strcmp(calculator->functions[i]->name, name) == 0) {
            // Free old expression
            for (int j = 0; j < calculator->functions[i]->size; j++) {
                free(calculator->functions[i]->expression[j]->value);
                free(calculator->functions[i]->expression[j]);
            }
            free(calculator->functions[i]->expression);

            // Allocate new expression
            calculator->functions[i]->expression = malloc(sizeof(Token*) * size);
            for (int j = 0; j < size; j++) {
                Token *token = malloc(sizeof(Token));
                token->type = expression[j]->type;
                token->value = malloc(strlen(expression[j]->value) + 1);
                strcpy(token->value, expression[j]->value);
                calculator->functions[i]->expression[j] = token;
            }
            calculator->functions[i]->size = size;
            return;
        }
    }

    // If function does not exist, add new
    Function *function = malloc(sizeof(Function));
    function->expression = malloc(sizeof(Token*) * size);

    for (int i = 0; i < size; i++) {
        Token *token = malloc(sizeof(Token));
        token->type = expression[i]->type;
        token->value = malloc(strlen(expression[i]->value) + 1);
        strcpy(token->value, expression[i]->value);
        function->expression[i] = token;
    }

    function->size = size;
    function->name = malloc(strlen(name) + 1);
    strcpy(function->name, name);

    calculator->functions[calculator->f_index++] = function;
}

void add_variable(Calculator* calculator, char* name, double value){

    for(int i = 0; i < calculator->v_index; i++){
        if(strcmp(calculator->variables[i]->name,name)==0){
            calculator->variables[i]->value = value;
            return;
        }
    }

    Variable* var = malloc(sizeof(Variable));
    var->name = malloc(sizeof(char)*strlen(name)+1);

    strcpy(var->name,name);
    var->value = value;
    calculator->variables[(calculator->v_index)++] = var;
}
