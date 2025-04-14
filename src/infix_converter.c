#include "infix_converter.h"
#include "tokenizer.h"
#include <stdbool.h>
#include <stdio.h>
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




bool get_priority(const char* a){
    if(strcmp(a,"+") == 0 || strcmp(a,"-")) return 2;
    if(strcmp(a,"*") == 0 || strcmp(a,"/") ) return 4;
    if(strcmp(a,"(") == 0 ) return 0;

    return -1;
}

void infix_to_posfix(Token** dest, Token** src, int size){
    TokenPtrStack stack;
    init_stack(&stack);

    int dest_index = 0;
    // instad of using src we should create a
    // copy so we dont change the src tokens

    for(int i = 0; i < size; i ++){
        Token *token = src[i];
        printf("TOKEN %s ",token->value);
        switch(token->type){
            case C_P:
            case O_P:
                break;
            case OPERATOR:
                int new_priority = get_priority(token->value);
                if(!is_empty(&stack))
                {
                    Token *top = pop(&stack);
                    int top_priority = get_priority(top->value);
                    if(top_priority < new_priority){

                        push(&stack,top); // add back to stack
                        push(&stack,token); // add the new
                    }
                    else{
                        puts("\nPOPPING");
                        while(!is_empty(&stack) && (top_priority >= new_priority))
                        {
                            dest[dest_index++] = top;
                            top = pop(&stack);
                            top_priority = get_priority(top->value);
                        }


                        push(&stack,token);
                    }
                }
                //empty stack push to it
                else{
                    push(&stack,token);
                }
                break;
            default:
                //puts("Push operand");
                // should be copied instead
                dest[dest_index++] = token;
                break;
        }
    }

    while(!is_empty(&stack)){
        dest[dest_index++] = pop(&stack);
    }
    puts("");

//    print_tokens(dest, dest_index);
}
