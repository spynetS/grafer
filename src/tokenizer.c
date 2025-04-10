#include <ctype.h>
#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stdlib.h>
#include <stdbool.h>

bool is_number(const char *str) {
	char *endptr;
	strtod(str, &endptr); // use strtol for integers
	return *str != '\0' && *endptr == '\0'; // must consume whole string
}


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

void add_token(Token** tokens, int* size,Type type, char* buffer){
		//trim(value);
	Token *token = malloc(sizeof(Token));
	token->type = type;
	token->value = strdup(buffer);
	trim(token->value);

	tokens[(*size)] = token;


	(*size) ++;

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
		//printf("reads %c ",value);
		if (buf_index < 63) { // keep one space for null terminator
			buffer[buf_index++] = value;
			buffer[buf_index] = '\0';
		}
		if (value == ')'){
			buffer[buf_index-1] = '\0';

			if(is_number(buffer)){
				add_token(tokens,size,NUMBER, buffer);
			}
			else{
				add_token(tokens,size,VARIABLE, buffer);
			}

			//add_token(tokens,size,PARAMETER,buffer);

			add_token(tokens,size,C_P,")");
			buffer[0]='\0';
			buf_index = 0;
		}
		else if(value == '(') {
			buffer[buf_index-1] = '\0';
			add_token(tokens,size,FUNC,buffer);
			add_token(tokens,size,O_P,"(");
			buffer[0]='\0';
			buf_index = 0;
		}
		else if(value == '='){
			// if prev was not a )
			// then it should be a variable

			add_token(tokens,size,EQUALS,"=");
			buffer[0]='\0';
			buf_index = 0;
			if(i+1 < strlen(expression)){
				i++; // this is to don't add the space after an operator
			}
		}
		else if(is_operator(buffer)){
			//printf("Operator: '%s'\n",buffer);
			add_token(tokens,size,OPERATOR,buffer);
			(buf_index) = 0;
			buffer[0] = '\0';  // Clears the content of the string (makes it empty)	tokens[(*size)] = token;


			if(i+1 < strlen(expression)){
				i++; // this is to don't add the space after an operator
			}
		}
		else if(value == ' '){
			trim(buffer);
			if (strcmp(buffer,"") == 0 || strcmp(buffer," ") == 0){
				//puts("empty value, continuing");
				continue;
			}
			if(is_number(buffer)){
				add_token(tokens,size,NUMBER, buffer);
			}
			else{
				add_token(tokens,size,VARIABLE, buffer);
			}

			(buf_index) = 0;
			buffer[0] = '\0';  // Clears the content of the string (makes it empty)	tokens[(*size)] = token;

		}

	}

	if(buf_index > 0){
		if(is_operator(buffer)){

		}
		else{
			if(is_number(buffer)){
				add_token(tokens,size,NUMBER, buffer);
			}
			else{
				add_token(tokens,size,VARIABLE, buffer);
			}

		}
	}
	free(buffer);
}

void free_token(Token* token){
	if(token != NULL){
		free(token->value);
		free(token);
	}
}
