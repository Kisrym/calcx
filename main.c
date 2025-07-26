#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "stack.h"
#include "aux.h"

#define MAX_INPUT 100
#define MAX_VARIABLES 100

char **split(char str[], int size, char delimiter, int *size_tokens) {
    int count = 1;
    for (int i = 0; i < size; i++) {
        if (str[i] == delimiter) {
            count++; // quantidade de delimitadores
        }
    }

    *size_tokens = count;
    char **arr_tokens = (char **) malloc(sizeof(char *) * count); // array de strings dinamicamente alocadas

    int start = 0, j = 0;
    for (int i = 0; i <= size; i++) {
        if (str[i] == delimiter || str[i] == '\0') {
            int length = i - start;

            arr_tokens[j] = (char *) malloc((length + 1) * sizeof(char)); // alocar espaço pra cada string

            strncpy(arr_tokens[j], &str[start], length); // copia a string do start até o tamanho total até agora
            arr_tokens[j][length] = '\0';

            j++;
            start = i + 1; // atualiza o inicio pro proximo caractere
        }
    }

    return arr_tokens;
}

void free_tokens(char **arr, int size) {
    for (int i = 0; i < size; i++) {
        free(arr[i]);
    }
    free(arr);
}

int procedencia(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '(') return 0; // caso especial
    return 3; // funcoes/exponencial/sqrt
}

Termos analyze_side(char **arr, int size) {
    Termos resultado = {0, 0};
    Stack *valores = new_stack();

    int unique_x = 1;
    for (int i = 0; i < size; i++) {
        if (is_number(arr[i]) || is_variable(arr[i])) {
            push(valores, arr[i], strlen(arr[i]));
        }

        else {
            char *tmp1 = pop(valores);
            char *tmp2 = pop(valores);
            // 2 - 3
            // 2 3 -
            if (arr[i][0] == '+') {
                unique_x = 0;
                if (is_variable(tmp1)) {
                    resultado.constante += atof(tmp2);
                    resultado.coef_x += 1; // so tem o x isolado

                    char tmp[32];
                    sprintf(tmp, "%lf", 0.);
                    push(valores, tmp, strlen(tmp));
                }
                else if (is_variable(tmp2)) {
                    resultado.constante += atof(tmp1);
                    resultado.coef_x += 1; // so tem o x isolado

                    char tmp[32];
                    sprintf(tmp, "%lf", 0.);
                    push(valores, tmp, strlen(tmp));
                }

                else {
                    double a = atof(tmp1);
                    double b = atof(tmp2);
                    char tmp[32];

                    sprintf(tmp, "%lf", a + b);
                    push(valores, tmp, strlen(tmp));
                }
            }

            else if (arr[i][0] == '-') {
                unique_x = 0;
                if (is_variable(tmp2)) {
                    resultado.constante -= atof(tmp1);
                    resultado.coef_x += 1;

                    char tmp[32];
                    sprintf(tmp, "%lf", 0.);
                    push(valores, tmp, strlen(tmp));
                }
                else if (is_variable(tmp1)) { // 3 - x
                    resultado.constante += atof(tmp2);
                    resultado.coef_x -= 1;

                    char tmp[32];
                    sprintf(tmp, "%lf", 0.);
                    push(valores, tmp, strlen(tmp));
                }
                else {
                    double b = atof(tmp1);
                    double a = atof(tmp2);
                    char tmp[32];

                    sprintf(tmp, "%lf", a - b);
                    push(valores, tmp, strlen(tmp));
                }
            }

            else if (arr[i][0] == '*') {
                unique_x = 0;
                if (is_variable(tmp1)) {
                    resultado.coef_x += atof(tmp2);

                    char tmp[32];
                    sprintf(tmp, "%lf", 0.);
                    push(valores, tmp, strlen(tmp));
                }
                else if (is_variable(tmp2)) { // 3 - x
                    resultado.coef_x += atof(tmp1);

                    char tmp[32];
                    sprintf(tmp, "%lf", 0.);
                    push(valores, tmp, strlen(tmp));
                }
                else {
                    double a = atof(tmp1);
                    double b = atof(tmp2);
                    char tmp[32];

                    sprintf(tmp, "%lf", a * b);
                    push(valores, tmp, strlen(tmp));
                }
            }

            free(tmp1);
            free(tmp2);
        }

    }

    if (!is_empty(valores)) {
        char *tmp = pop(valores);
        resultado.constante += atof(tmp);
        free(tmp);
    }

    if (unique_x) resultado.coef_x++;

    free_stack(valores);
    return resultado;
}

char **in_pos_conversion(char **arr, int size, int* size_arr) {
    int capacity = size;
    char **saida = (char **) malloc(sizeof(char *) * capacity); // array dinamico

    Stack *operadores = new_stack();
    int j = 0;
    for (int i = 0; i < size; i++) {
        if (j == capacity) {
            capacity *= 2;
            saida = realloc(saida, sizeof(char *) * capacity);
        }

        if (is_number(arr[i])) { // caso seja um numero, adiciona direto na saida
            saida[j] = (char *) malloc(sizeof(char) * (strlen(arr[i]) + 1));
            strcpy(saida[j++], arr[i]);
        }

        else {
            if (arr[i][0] == ')') { // se estiver fechando os parenteses, adicionara tudo à saida ate a abertura deles
                while (front(operadores)[0] != '(') {
                    char *tmp = pop(operadores);
                    saida[j] = (char *) malloc(sizeof(char) * (strlen(tmp) + 1));
                    strcpy(saida[j++], tmp);
                    free(tmp);
                }
                free(pop(operadores)); // tirar o ( do stack
                continue;
            }

            // enquanto a procedencia do operador atual for maior ou igual ao novo, adicionará tudo à saída
            while (!is_empty(operadores) && arr[i][0] != '(' && procedencia(front(operadores)[0]) >= procedencia(arr[i][0])) {
                char *tmp = pop(operadores);
                saida[j] = (char *) malloc(sizeof(char) * (strlen(tmp) + 1));
                strcpy(saida[j++], tmp);
                free(tmp);
            }

            push(operadores, arr[i], strlen(arr[i]));
        }
    }

    // adicionará o que sobrou
    while (!is_empty(operadores)) {
        char *tmp = pop(operadores);
        saida[j] = (char *) malloc(sizeof(char) * (strlen(tmp) + 1));
        strcpy(saida[j++], tmp);
        free(tmp);
    }

    free_stack(operadores);
    free_tokens(arr, size);
    *size_arr = j; // tamanho do novo array
    return saida;
}

void apply_operator(Stack *stack, char* op) {
    if (op[0] == '+' || op[0] == '-' || op[0] == '*' || op[0] == '/' || op[0] == '^') {
        char *tmp2 = pop(stack);
        char *tmp1 = pop(stack);

        if (op[0] == '+') {
            double a = atof(tmp1);
            double b = atof(tmp2);
            char tmp[32];

            sprintf(tmp, "%lf", a + b);
            push(stack, tmp, strlen(tmp));
        }

        else if (op[0] == '-') {
            double a = atof(tmp1);
            double b = atof(tmp2);
            char tmp[32];

            sprintf(tmp, "%lf", a - b);
            push(stack, tmp, strlen(tmp));
        }

        else if (op[0] == '*') {
            double a = atof(tmp1);
            double b = atof(tmp2);
            char tmp[32];

            sprintf(tmp, "%lf", a * b);
            push(stack, tmp, strlen(tmp));
        }

        else if (op[0] == '/') {
            double a = atof(tmp1);
            double b = atof(tmp2);
            char tmp[32];

            sprintf(tmp, "%lf", a / b);
            push(stack, tmp, strlen(tmp));
        }

        else if (op[0] == '^') {
            double a = atof(tmp1);
            double b = atof(tmp2);
            char tmp[32];

            sprintf(tmp, "%lf", pow(a, b));
            push(stack, tmp, strlen(tmp));
        }

        free(tmp1);
        free(tmp2);
    }

    else {
        char *tmp1 = pop(stack);

        if (!strcmp("sin", op)) {
            double a = atof(tmp1);
            char tmp[32];

            sprintf(tmp, "%lf", sin(a));
            push(stack, tmp, strlen(tmp));
        }

        else if (!strcmp("cos", op)) {
            double a = atof(tmp1);
            char tmp[32];

            sprintf(tmp, "%lf", cos(a));
            push(stack, tmp, strlen(tmp));
        }

        else if (!strcmp("tan", op)) {
            double a = atof(tmp1);
            char tmp[32];

            sprintf(tmp, "%lf", tan(a));
            push(stack, tmp, strlen(tmp));
        }

        else if (!strcmp("sqrt", op)) {
            double a = atof(tmp1);
            char tmp[32];

            sprintf(tmp, "%lf", sqrt(a));
            push(stack, tmp, strlen(tmp));
        }

        else if (op[0] == '!') {
            int a = atoi(tmp1);
            char tmp[32];

            sprintf(tmp, "%d", fact(a));
            push(stack, tmp, strlen(tmp));
        }

        free(tmp1);
    }
}


double calculate(char **arr, int size, Variavel vars[], int qtd_vars) {
    Stack *valores = new_stack();

    double result;
    for (int i = 0; i < size; i++) {
        if (is_number(arr[i])) {
            push(valores, arr[i], strlen(arr[i]));
        }

        else if (find_operator(arr[i]) || find_function(arr[i])) {
            apply_operator(valores, arr[i]);
        }

        else if (find_variable(vars, qtd_vars, arr[i], &result) != -1) { // substitui o valor da variável, caso haja
            char tmp[32];
            sprintf(tmp, "%lf", result);
            push(valores, tmp, strlen(tmp));
        }

        else {
            printf("variable not setted\n");
            free_stack(valores);
            return 0.0;
        }
    }

    double resultado = atof(front(valores));
    free_stack(valores);
    return resultado;
}

double evaluate(char input[], Variavel vars[], int qtd_vars) {
    int input_size;
    int pos_size;
    char **sentenca = split(input, strlen(input), ' ', &input_size);
    char **sentenca_pos_fixa = in_pos_conversion(sentenca, input_size, &pos_size);
    double resultado = calculate(sentenca_pos_fixa, pos_size, vars, qtd_vars);
    free_tokens(sentenca_pos_fixa, pos_size); // a sentenca foi devidamente apagada no in_pos_conversion() */

    return resultado;
}

int main() {
    Variavel vars[MAX_VARIABLES];
    int qtd_vars = 0;

    char input[MAX_INPUT];

    while (1) {
        printf(">> ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) break; // CTRL+D mata o programa

        input[strcspn(input, "\n")] = '\0'; // tira o \n do final da string

        if (strcmp(input, "exit") == 0) break;

        if (strcspn(input, "=") != strlen(input)) { // achou um "="
            int input_size;
            char **sentenca = split(input, strlen(input), '=', &input_size);

            if (find_operator(sentenca[0]) || (!is_number(sentenca[1]) && !contains_variable(vars, qtd_vars, sentenca[1]))) {
                rtrim(sentenca[0]);

                int expressao_size;
                int pos_size;
                char **expressao0 = split(sentenca[0], strlen(sentenca[0]), ' ', &expressao_size);
                char **sentenca_pos_fixa0 = in_pos_conversion(expressao0, expressao_size, &pos_size);
                Termos L_side = analyze_side(sentenca_pos_fixa0, pos_size);
                free_tokens(sentenca_pos_fixa0, pos_size);

                char **expressao1 = split(sentenca[1], strlen(sentenca[1]), ' ', &expressao_size);
                char **sentenca_pos_fixa1 = in_pos_conversion(expressao1, expressao_size, &pos_size);
                Termos R_side = analyze_side(sentenca_pos_fixa1, pos_size);
                free_tokens(sentenca_pos_fixa1, pos_size);

                double coef_x = L_side.coef_x - R_side.coef_x;
                double constant = R_side.constante - L_side.constante;

                if (coef_x == 0) {
                    printf("infinitas soluções\n");
                }
                else {
                    double x = constant / coef_x;
                    printf("x = %lf\n", x);
                }

            }

            else {
                // tira o espaco apos a variavel, caso haja
                //sentenca[0][strcspn(sentenca[0], " ")] = '\0'; 
                rtrim(sentenca[0]);

                double result;
                int index = find_variable(vars, qtd_vars, sentenca[0], &result);
                
                if (index != -1) { // achou uma variavel ja existente
                    printf("achou variavel existente\n");
                    vars[index].value = evaluate(sentenca[1], vars, qtd_vars); // atualiza seu valor
                    printf("[%s] : [%lf]\n", vars[index].name, vars[index].value);
                }
                // variavel nova
                else {
                    printf("achou variavel nova\n");
                    strcpy(vars[qtd_vars].name, sentenca[0]);
                    vars[qtd_vars].value = evaluate(sentenca[1], vars, qtd_vars);
                    printf("[%s] : [%lf]\n", vars[qtd_vars].name, vars[qtd_vars].value);
                    qtd_vars++;
                }
            }

            free_tokens(sentenca, input_size); // limpando a sentenca
            
        }
        else {
            rtrim(input);
            printf("%lf\n", evaluate(input, vars, qtd_vars));
        }
    }

    return 0;
}