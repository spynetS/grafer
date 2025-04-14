#include "infix_converter.h"
#include "tokenizer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100  // or whatever size you want

typedef struct {
    Token *data[MAX_SIZE];
    int top;
} TokenPtrStack;
void init_stack(TokenPtrStack* stack) {
    stack->top = -1;
}
int is_empty(TokenPtrStack* stack) {
    return stack->top == -1;
}

int is_full(TokenPtrStack* stack) {
    return stack->top == MAX_SIZE - 1;
}
int push(TokenPtrStack* stack, Token *tok) {
    if (is_full(stack)) return 0;  // failure
    stack->data[++(stack->top)] = tok;
    return 1;  // success
}

Token *pop(TokenPtrStack* stack) {
    if (is_empty(stack)) return NULL;
    return stack->data[(stack->top)--];
}




int get_priority(const char* a){
    if(strcmp(a,"+") == 0 || strcmp(a,"-") == 0) return 2;
    if(strcmp(a,"*") == 0 || strcmp(a,"/") == 0) return 4;
    if(strcmp(a,"(") == 0 ) return 0;

    return -1;
}

void infix_to_posfix(Token** dest,int size, Token** src, int *size_pos){

    Token** cpy = malloc(sizeof(Token*)*size);
    for(int i = 0; i < size; i++){
        cpy[i] = malloc(sizeof(Token));
        cpy[i]->type = src[i]->type;
        cpy[i]->value = malloc(sizeof(char)*strlen(src[i]->value)+1);
        strcpy(cpy[i]->value,src[i]->value);
    }


    TokenPtrStack stack;
    init_stack(&stack);

    // instad of using src we should create a
    // copy so we dont change the src tokens

    for(int i = 0; i < size; i ++){
        Token *token = cpy[i];
        //printf("TOKEN %s ",token->value);
        switch(token->type){
            case C_P:
            case O_P:
                break;
            case OPERATOR:
                int new_priority = get_priority(token->value);
                printf("Push operator %s\n",token->value);
                if(!is_empty(&stack))
                {
                    Token *top = pop(&stack);
                    int top_priority = get_priority(top->value);
                    printf("%d < %d\n",top_priority, new_priority);
                    if(top_priority < new_priority){
                        puts("add to stack");
                        push(&stack,top); // add back to stack
                        push(&stack,token); // add the new
                    }
                    else{
                        puts("pop stack");

                        do {
                            printf("adds %s to posfix\n",top->value);
                            dest[(*size_pos)++] = top;
                            top = pop(&stack);
                            top_priority = get_priority(top->value);
                        }
                        while(!is_empty(&stack) && (top_priority >= new_priority));

                        push(&stack,token);
                    }
                }
                //empty stack push to it
                else{
                    push(&stack,token);
                }
                break;
            default:
                printf("Push operand %s\n",token->value);
                // should be copied instead
                dest[(*size_pos)++] = token;
                break;
        }
    }

    while(!is_empty(&stack)){
        Token *poped = pop(&stack);
        printf("Pop stack %s\n",poped->value);
        if(poped == NULL) break;
        dest[(*size_pos)++] = poped;
    }
    free(cpy); // free the cpy token array
}
