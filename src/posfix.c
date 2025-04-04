#include <ctype.h>
#include "posfix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void trim(char* str) {
    // Check for NULL string
    if (str == NULL) return;

    // Trim leading spaces
    while (isspace((unsigned char)*str)) {
        str++;
    }

    // Trim trailing spaces
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }
}


int is_operator(const char* operator){
	if(strcmp(operator,"+")==0) return 1;
	if(strcmp(operator,"-")==0) return 1;
	if(strcmp(operator,"*")==0) return 1;
	if(strcmp(operator,"/")==0) return 1;
	return 0;
}

double operate(char* operator, double a, double b){

	assert(is_operator(operator));
	if(strcmp(operator,"+")==0) return a+b;
	else if(strcmp(operator,"-")==0) return a-b;
	else if(strcmp(operator,"*")==0) return a*b;
	else if(strcmp(operator,"/")==0) return a/b;

	return -1;
}

/*
 * Will go through a posfix expression an tokenize it.
 * It will add a Token allocated on the heap in the tokens
 * array.
 *
 * */
void tokenize(Token** tokens, int* size ,char* expression) {


	char* buffer = malloc(sizeof(char)*32);
	int buf_index = 0;

	for (int i = 0; i < strlen(expression); i++){

		char value = expression[i];
		//printf("reads %c",value);
		if (buf_index < 63) { // keep one space for null terminator
			buffer[buf_index++] = value;
			buffer[buf_index] = '\0';
		}
		//printf(", buffer is '%s'\n",buffer);


		if(is_operator(buffer)){
			printf("Operator: '%s'\n",buffer);


			//trim(value);
			Token *token = malloc(sizeof(Token));
			token->type = OPERATOR;
			token->value = strdup(buffer);

			tokens[(*size)] = token;

			buf_index = 0;
			buffer[0] = '\0';  // Clears the content of the string (makes it empty)	tokens[(*size)] = token;
			(*size) ++;

			if(i+1 < strlen(expression)){
				puts("here");
				i++; // this is to don't add the space after an operator
			}
		}
		else if(value == ' '){

			if (strcmp(buffer,"") == 0){
				puts("empty value, continuing");
				continue;
			}

			printf("Value: '%s'\n",buffer);

			Token *token = malloc(sizeof(Token));
			token->type = NUMBER;
			token->value = strdup(buffer);

			//trim(token->value);
			tokens[(*size)] = token;

			buf_index = 0;
			buffer[0] = '\0';  // Clears the content of the string (makes it empty)tokens[(*size)] = token;
			(*size) ++;
		}

	}
	if(buf_index > 0){
		if(is_operator(buffer)){

		}
		else{
			Token *token = malloc(sizeof(Token));
			token->type = NUMBER;
			token->value = strdup(buffer);

			//trim(token->value);
			tokens[(*size)] = token;
			(*size)++;
		}
	}
	free(buffer);
}

char type_string[2][10] = {"NUMBER","OPERATOR"};

void free_token(Token* token){
	if(token != NULL){
		free(token->value);
		free(token);
	}
}

double calculate(char* expression){
	printf("%s\n",expression);
	Stack(double) stack;
	INIT_STACK(stack);

	Token* tokens[128];
	int size = 0;
	tokenize(tokens,&size,expression);

	for(int i = 0; i < size;i ++){
		Token* token = tokens[i];
		printf("%s ",type_string[token->type]);
		printf("%s\n",token->value);
		switch(token->type){
			case NUMBER:

				double number = atoi(token->value);
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
	double ans = POP(stack);
	printf("%lf\n",ans);

	for(int i = 0; i < size;i ++){
		free_token(tokens[i]);
	}


	return ans;
}
