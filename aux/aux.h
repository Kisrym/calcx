#include <string.h>
#include <ctype.h>

typedef struct Variavel {
    char name[32];
    double value;
} Variavel;

typedef struct Termos {
    double coef_x;
    double constante;
} Termos;

int fact(int a);
int find_variable(Variavel vars[], int size, char name[], double *result);
int find_function(const char *s);
int find_operator(const char *s);
int is_number(const char *s);
int is_variable(const char *s);
char *rtrim(char *str);
char *ltrim(char *str);