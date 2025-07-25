#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Node {
    char *str;
    struct Node *prox;
} Node;

typedef struct Stack {
    Node *top;
    int size;
} Stack;


Stack *new_stack();
void push(Stack *stack, char *data, int str_size);
char *pop(Stack *stack);
char *front(const Stack *stack);
int is_empty(const Stack *stack);
void free_stack(Stack *stack);

#endif