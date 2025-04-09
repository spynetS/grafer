#include "turing.h"
#include "posfix.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


double get_variable_value(Calculator *calc, char* name){
    for(int i =0; i < calc->v_index; i ++){
        if(strcmp(calc->variables[i]->name,name)==0){
            return calc->variables[i]->value;
        }
    }
    return 69;
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

    print_tokens(tokens,size);

	for(int i = 0; i < size; i++){
        Token* token = tokens[i];

        if(tokens[i]->type == VARIABLE &&
           tokens[i+1]->type == EQUALS ){
            double value = eval_tokens(calc,&tokens[i+2], size-2);
            printf("variable assignment %s = %lf\n",tokens[i]->value,value);
            add_variable(calc, tokens[i]->value, value);
            i+= 2;
        }
        else if(tokens[i]->type == FUNC &&
           tokens[i+1]->type == O_P &&
           tokens[i+2]->type == VARIABLE &&
           tokens[i+3]->type == C_P &&
           tokens[i+4]->type == EQUALS){
            printf("Function assignment\n");
            add_function(calc,  tokens[i]->value, &tokens[i+5],size-5);
            i+=5;
        }
        else if(tokens[i]->type == FUNC &&
           tokens[i+1]->type == O_P &&
           tokens[i+2]->type == VARIABLE &&
           tokens[i+3]->type == C_P){
            double parameter = get_variable_value(calc, tokens[i+2]->value);

            double value = call_function_name(calc,token->value,parameter);
            printf("function %s(%lf) returns %lf\n",token->value,parameter,value);
            PUSH(stack,value);

            i+=3; // move forward the tokens we checked in the if
        }
        else if(tokens[i]->type == FUNC &&
           tokens[i+1]->type == O_P &&
           tokens[i+2]->type == NUMBER &&
           tokens[i+3]->type == C_P){
            double parameter;
            sscanf(tokens[i+2]->value,"%lf",&parameter);
            double value = call_function_name(calc,token->value,parameter);
            printf("function %s(%lf) returns %lf\n",token->value,parameter,value);
            PUSH(stack,value);

            i+=3; // move forward the tokens we checked in the if
        }
        else{
            switch(token->type){
                case VARIABLE:
                    double value = get_variable_value(calc,token->value);
                    printf("Variable %s holds %lf\n",token->value,value);
                    PUSH(stack,value);
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

            }
        }
    }
	ans = POP(stack);
    printf("%lf\n",ans);

    return ans;
}

double call_function_name(Calculator *calc, char* name, double value){
    for(int i = 0; i < calc->f_index; i ++){
        if(strcmp(calc->functions[i]->name,name)==0){
            return call_function(calc,*calc->functions[i], value);
        }
    }
    return 69;
}

double call_function(Calculator *calc, Function function, double x){

    char* ptr;
    for(int i = 0; i < function.size; i++){
        // instead of x here add a parameter value to the function and pass in
        // that also in the function to let the user set what name of the parameter
        // they want
        if(strcmp(function.expression[i]->value,"x")==0){
            char* cpy = strdup(function.expression[i]->value);
            char repl[100];
            sprintf(repl,"%lf",x);
            ptr = function.expression[i]->value;
            strcpy(function.expression[i]->value,repl);
            function.expression[i]->type=NUMBER;
        }

	}
    double ans = eval_tokens(calc,function.expression,function.size);
    strcpy(ptr,"x");
    return ans;
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
    calculator->variables[calculator->v_index++] = var;
}
