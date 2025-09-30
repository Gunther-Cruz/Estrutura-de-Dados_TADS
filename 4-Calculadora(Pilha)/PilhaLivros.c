#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAX 100

typedef struct
{
    float elementos[MAX];
    int topo;
} PilhaFloat;

typedef struct
{
    char elementos[MAX];
    int topo;
} PilhaChar;

// inicializa a pilha de números float
void inicializaPilhaFloat(PilhaFloat *p)
{
    p->topo = -1;
}

// verifica se a pilha de números float está vazia
int estaVaziaFloat(PilhaFloat *p)
{
    return p->topo == -1;
}

// adiciona um número na pilha de float
void empilhaFloat(PilhaFloat *p, float valor)
{
    if (p->topo < MAX - 1)
    {
        p->elementos[++(p->topo)] = valor;
    }
}

// desempilha a pilha de float
float desempilhaFloat(PilhaFloat *p)
{
    if (!estaVaziaFloat(p))
    {
        return p->elementos[(p->topo)--];
    }
    return 0;
}

// inicializa a pilha de caracteres
void inicializaPilhaChar(PilhaChar *p)
{
    p->topo = -1;
}

// verifica se a pilha de caracteres está vazia
int estaVaziaChar(PilhaChar *p)
{
    return p->topo == -1;
}

// adiciona um caractere na pilha de caracteres
void empilhaChar(PilhaChar *p, char valor)
{
    if (p->topo < MAX - 1)
    {
        p->elementos[++(p->topo)] = valor;
    }
}

// desempilha a pilha de caracteres
char desempilhaChar(PilhaChar *p)
{
    if (!estaVaziaChar(p))
    {
        return p->elementos[(p->topo)--];
    }
    return '\0';
}

char topoChar(PilhaChar *p)
{
    if (!estaVaziaChar(p))
    {
        return p->elementos[p->topo];
    }
    return '\0';
}

// precedência dos operadores matemáticos
int ordemCorreta(char operador)
{
    if (operador == '^' || operador == '~')
        return 3;
    if (operador == '*' || operador == '/')
        return 2;
    if (operador == '+' || operador == '-')
        return 1;
    return 0;
}

// Aplica uma operação entre dois operandos
float aplicaOperador(float a, float b, char operador)
{
    switch (operador)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    case '^':
        return pow(a, b);
    default:
        return 0;
    }
}

float resolverExpressao(char *expressao)
{
    PilhaFloat operandos;
    PilhaChar operadores;
    inicializaPilhaFloat(&operandos);
    inicializaPilhaChar(&operadores);

    for (int i = 0; i < strlen(expressao); i++)
    {
        char atual = expressao[i];

        if (isspace(atual)) // ignorar espaços
            continue;

        if (isdigit(atual))
        {
            float numero = 0;
            while (isdigit(expressao[i]))
            {
                numero = numero * 10 + (expressao[i] - '0');
                i++;
            }
            if (expressao[i] == '.')
            { 
                i++;
                float decimal = 0.1;
                while (isdigit(expressao[i]))
                {
                    numero += (expressao[i] - '0') * decimal;
                    decimal /= 10;
                    i++;
                }
            }
            i--;
            empilhaFloat(&operandos, numero); 
        }
        //operador de raiz quadrada
        else if (atual == '~')
        {
            i++;
            while (isspace(expressao[i]))
                i++;
            if (isdigit(expressao[i]))
            {
                float numero = 0;
                while (isdigit(expressao[i]))
                {
                    numero = numero * 10 + (expressao[i] - '0');
                    i++;
                }
                i--;
                empilhaFloat(&operandos, sqrt(numero)); 
            }
        }

        else if (atual == '(' || atual == '[' || atual == '{')
        {
            empilhaChar(&operadores, atual);
        }
        else if (atual == ')' || atual == ']' || atual == '}')
        {
            char abre = (atual == ')') ? '(' : (atual == ']') ? '[' : '{';
            while (!estaVaziaChar(&operadores) && topoChar(&operadores) != abre)
            {
                char operador = desempilhaChar(&operadores);
                float b = desempilhaFloat(&operandos);
                float a = desempilhaFloat(&operandos);
                empilhaFloat(&operandos, aplicaOperador(a, b, operador)); 
            }
            desempilhaChar(&operadores); 
        }
        else
        {
            while (!estaVaziaChar(&operadores) && ordemCorreta(topoChar(&operadores)) >= ordemCorreta(atual))
            {
                char operador = desempilhaChar(&operadores);
                float b = desempilhaFloat(&operandos);
                float a = desempilhaFloat(&operandos);
                empilhaFloat(&operandos, aplicaOperador(a, b, operador));
            }
            empilhaChar(&operadores, atual); 
        }
    }

    //operadores ainda nao resolvidas
    while (!estaVaziaChar(&operadores))
    {
        char operador = desempilhaChar(&operadores);
        float b = desempilhaFloat(&operandos);
        float a = desempilhaFloat(&operandos);
        empilhaFloat(&operandos, aplicaOperador(a, b, operador));
    }

    return desempilhaFloat(&operandos); // Retorna o resultado final
}

int main()
{
    char expressao[MAX];
    printf("Digite a expressao: ");
    scanf("%99[^\n]", expressao);

    printf("Resultado: %.2f\n", resolverExpressao(expressao));

    return 0;
}
