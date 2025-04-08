#include "turing.h"
#include "posfix.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

void print_tokens(Token** tokens, int size){
    for(int i = 0; i < size; i++){

        printf("%s ",type_string[tokens[i]->type]);
	}
    puts("");
    for(int i = 0; i < size; i++){
        printf("%s ",tokens[i]->value);
	}
    puts("");
}

double eval(Calculator* calc, char *str){
    double ans = 0;
    Token** tokens = malloc(sizeof(Token)*100);
	int size = 0;
    double x = 0;
	tokenize(tokens,&size,str);

	for(int i = 0; i < size; i++){
        if(tokens[i]->type == VARIABLE &&
           tokens[i+1]->type == EQUALS ){
            printf("variable assignment\n");
        }
        if(tokens[i]->type == FUNC &&
           tokens[i+1]->type == O_P &&
           tokens[i+2]->type == PARAMETER &&
           tokens[i+3]->type == C_P &&
           tokens[i+4]->type == EQUALS){
            printf("Function assignment\n");
            add_function(calc,  tokens[i]->value, &tokens[i+5],size-5);

        }
    }
    print_tokens(tokens,size);
    for(int i = 0; i < size; i ++){
        free_token(tokens[i]);
    }
    free(tokens);

    return ans;
}

double call_function(Function *function, double x){
    for(int i = 0; i < function->size; i++){
        // instead of x here add a parameter value to the function and pass in
        // that also in the function to let the user set what name of the parameter
        // they want
        if(strcmp(function->expression[i]->value,"x")==0){
            char repl[100];
            sprintf(repl,"%lf",x);

            strcpy(function->expression[i]->value,repl);
            function->expression[i]->type=NUMBER;
        }
	}

	return  posfix_calculate_tokens(function->expression,function->size);
}

void add_function(Calculator* calculator, char *name, Token **expression, int size){
    Function *function = malloc(sizeof(Function));

    function->expression = malloc(sizeof(Token*)*size);

    for(int i = 0 ; i < size; i ++){
        Token* token = malloc(sizeof(Token));

        token->type = expression[i]->type;
        token->value = malloc(sizeof(char)*strlen(expression[i]->value)+1);
        strcpy(token->value, expression[i]->value);
        function->expression[i] = token;
    }
    //print_tokens(function->expression, size);

    function->size = size;
    function->name = malloc(sizeof(char)*strlen(name)+1);
    strcpy(function->name,name);

	calculator->functions[calculator->f_index++] = function;
}
