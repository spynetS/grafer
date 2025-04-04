#ifndef POSFIX_H_
#define POSFIX_H_

#define MAX_SIZE 100

// Stack structure
#define Stack(type) \
    struct {         \
        type data[MAX_SIZE]; \
        int top;           \
    }

// Macros to initialize the stack, push, pop, and peek operations
#define INIT_STACK(stack)    (stack).top = -1
#define IS_EMPTY(stack)      ((stack).top == -1)
#define IS_FULL(stack)       ((stack).top == (MAX_SIZE - 1))
#define PUSH(stack, value)   do { if (!IS_FULL(stack)) (stack).data[++(stack).top] = (value); else printf("Stack is full!\n"); } while (0)
#define POP(stack)           ((IS_EMPTY(stack)) ? -1 : (stack).data[(stack).top--])
#define PEEK(stack)          ((IS_EMPTY(stack)) ? -1 : (stack).data[(stack).top]) free_stack(string_stack* stack);

typedef enum Type {
    NUMBER,
    OPERATOR
} Type;

typedef struct token {
    Type type;
    char* value;
} Token;


double calculate(char* experssion);

#endif // POSFIX_H_
