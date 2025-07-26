#include "aux.h"
#include <stdio.h>

int fact(int a) {
    if (a < 0) return -1;
    if (a == 0) return 1;
    if (a <= 2) return a;

    int resultado = 1;
    for (int i = 2; i < a; i++) {
        resultado *= a;
    }

    return resultado;
}

int find_variable(Variavel vars[], int size, char name[], double *result) {
    for (int i = 0; i < size; i++) {
        if (!strcmp(vars[i].name, name)) {
            *result = vars[i].value;
            return i;
        }
    }

    return -1;
}

int find_operator(const char *s) {
    while (*s != '\0') {
        if (!isalpha(*s) && *s != ' ') return 1;
        s++;
    }
    return 0;
}

int find_function(const char *s) {
    return (!(strcmp(s, "cos")) || !(strcmp(s, "sin")) || !(strcmp(s, "tan")) || !(strcmp(s, "sqrt")));
}

int is_number(const char *s) {
    while (*s != '\0') {
        if (!isdigit(*s) && *s != '.' && *s != ' ') return 0;
        s++;
    }
    return 1;
}

int is_variable(const char *s) {
    while (*s != '\0') {
        if (isdigit(*s) || !isalpha(*s)) return 0;
        s++;
    }
    return 1;
}

int contains_variable(Variavel vars[], int qtd_vars, const char *s) {
    for (int i = 0; i < qtd_vars; i++) {
        if (strstr(s, vars[i].name)) return 1;
    }

    return 0;
}

char *rtrim(char *s){
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *ltrim(char *s) {
    while(isspace(*s)) s++;
    return s;
}