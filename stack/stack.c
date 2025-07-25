#include "stack.h"

Stack *new_stack() {
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    if (!stack) return NULL;

    stack->size = 0;
    stack->top = NULL;

    return stack;
}

void push(Stack *stack, char *data, int str_size) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->str = (char *) malloc(sizeof(char) * (str_size + 1));
    strcpy(node->str, data);
    node->prox = NULL;

    if (!stack->top) {
        stack->top = node;
    }
    else {
        node->prox = stack->top;
        stack->top = node;
    }

    stack->size++;
}

char* pop(Stack *stack) {
    if (!stack->top) {
        return '\0';
    }

    Node *temp = stack->top;
    char* resultado = temp->str;

    stack->top = stack->top->prox;
    free(temp);
    stack->size--;
    return resultado;
}

char *front(const Stack *stack) {
    return stack->top->str;
}

int is_empty(const Stack *stack) {
    return stack->size == 0;
}

void free_stack(Stack *stack) {
    while (stack->top) {
        Node *temp = stack->top;
        stack->top = stack->top->prox;
        free(temp->str);
        free(temp);
    }

    free(stack);
}