#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cliente {
    char nome[20];
    int operacao; // 1 - Saque, 2 - Depósito
    float valor;
    int id;
    struct cliente *proximo;
} Cliente;

typedef struct fila {
    Cliente *inicio;
    Cliente *fim;
    int qtd;
} Fila;

Fila *criaFila() {
    Fila *fl = (Fila *)malloc(sizeof(Fila));
    fl->inicio = NULL;
    fl->fim = NULL;
    fl->qtd = 0;
    return fl;
}

Cliente *cadastraNovoCliente(char nome[], int op, float vl, int id) {
    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    strcpy(novo->nome, nome);
    novo->operacao = op;
    novo->valor = vl;
    novo->id = id;
    novo->proximo = NULL;
    return novo;
}

void enfileirar(Fila *fl, Cliente *cl) {
    if (fl->inicio == NULL) {
        fl->inicio = cl;
    } else {
        fl->fim->proximo = cl;
    }
    cl->proximo = NULL;
    fl->fim = cl;
    fl->qtd++;
}

Cliente *desenfileirar(Fila *fl) {
    Cliente *aux = fl->inicio;
    if (aux == NULL) {
        printf("\nErro - Fila Vazia\n");
        return NULL;
    } else {
        fl->inicio = aux->proximo;
        fl->qtd--;
        aux->proximo = NULL;
        if (fl->inicio == NULL) {
            fl->fim = NULL;
        }
    }
    return aux;
}

void mostraCliente(Cliente cl) {
    printf("\nId: %d \n\tNome: %s \n\tOperacao: %s \n\tValor: %.2f\n",
           cl.id, cl.nome, (cl.operacao == 1 ? "Saque" : "Depósito"), cl.valor);
}

void mostraFila(Fila *fl) {
    printf("\nInício da Fila\n");
    Cliente *aux = fl->inicio;
    while (aux != NULL) {
        mostraCliente(*aux);
        aux = aux->proximo;
    }
    printf("\nFim da Fila\n");
}

void apagaCliente(Cliente *cl) {
    free(cl);
}

void apagaFila(Fila *fl) {
    Cliente *aux = desenfileirar(fl);
    while (aux != NULL) {
        apagaCliente(aux);
        aux = desenfileirar(fl);
    }
}

void mostraQuadro(Fila *fG, Fila *fP, Fila *fGG, Fila *fGP) {
    printf("Atendimento ao Caixa:\n");
    if (fP->inicio != NULL) {
        printf("  - Prioritario N = %d\n", fP->inicio->id);
    } else {
        printf("  - Prioritario N = 0\n");
    }

    if (fG->inicio != NULL) {
        printf("  - Geral N = %d\n", fG->inicio->id);
    } else {
        printf("  - Geral N = 0\n");
    }

    printf("\nAtendimento ao Gerente:\n");
    if (fGP->inicio != NULL) {
        printf("  - Prioritario N = %d\n", fGP->inicio->id);
    } else {
        printf("  - Prioritario N = 0\n");
    }

    if (fGG->inicio != NULL) {
        printf("  - Geral N = %d\n", fGG->inicio->id);
    } else {
        printf("  - Geral N = 0\n");
    }
}

void processaOperacao(Fila *log, Cliente *cl, float *saldo) {
    printf("\nProcessando operacao para cliente:\n");
    mostraCliente(*cl);

    if (cl->operacao == 1) {
        if (*saldo >= cl->valor) {
            *saldo -= cl->valor;
            printf("Saque realizado com sucesso! Valor: %.2f\n", cl->valor);
        } else {
            printf("Saldo insuficiente para saque! Operacao cancelada.\n");
        }
    } else if (cl->operacao == 2) { 
        *saldo += cl->valor;
        printf("Deposito realizado com sucesso! Valor: %.2f\n", cl->valor);
    }

    printf("Saldo atual do caixa: %.2f\n", *saldo);

    enfileirar(log, cl);
}

void menuCaixa(Fila *fG, Fila *fP, Fila *fGG, Fila *fGP, Fila *logCaixa, Fila *logGerente, int sG, int sP, int sGG, int sGP, float *saldo) {
    int opAux;
    int op, tipoCliente;
    Cliente *cl;
    char nome[20];
    float vl;
    int contadorCliente = 0;

    do {
        printf("\n\nInforme uma Opcao:");
        printf("\n1 - Inserir Cliente na Fila");
        printf("\n2 - Remover Cliente da Fila");
        printf("\n3 - Mostrar Filas");
        printf("\n4 - Apagar Filas");
        printf("\n5 - Chamar Cliente");
        printf("\n0 - Sair do Programa");
        printf("\nInforme uma opcao: ");
        scanf("%d", &opAux);

        switch (opAux) {
            case 1:
                printf("\nInserir Cliente na Fila\n");
                printf("Informe o seu nome: ");
                scanf("%s", nome);
                printf("Informe a operacao (0 - Gerente, 1 - Saque, 2 - Deposito): ");
                scanf("%d", &op);
                if (op != 0) {
                    printf("Informe o valor: ");
                    scanf("%f", &vl);
                } else {
                    vl = 0;
                }
                printf("Tipo de cliente (1 - Prioritario, 2 - Geral): ");
                scanf("%d", &tipoCliente);
                if (op == 0) {
                    if (tipoCliente == 1) {
                        enfileirar(fGP, cadastraNovoCliente(nome, op, vl, sGP++));
                    } else {
                        enfileirar(fGG, cadastraNovoCliente(nome, op, vl, sGG++));
                    }
                } else {
                    if (tipoCliente == 1) {
                        enfileirar(fP, cadastraNovoCliente(nome, op, vl, sP++));
                    } else {
                        enfileirar(fG, cadastraNovoCliente(nome, op, vl, sG++));
                    }
                }
                printf("\nCliente inserido com sucesso!\n");
                break;

            case 2:
                printf("\nRemover Cliente da Fila\n");
                printf("Remover de qual fila? (1 - Prioritario, 2 - Geral): ");
                scanf("%d", &tipoCliente);
                if (tipoCliente == 1) {
                    cl = desenfileirar(fP);
                } else {
                    cl = desenfileirar(fG);
                }
                if (cl != NULL) {
                    mostraCliente(*cl);
                    apagaCliente(cl);
                }
                printf("\nRemocao realizada com sucesso!\n");
                break;

            case 3:
                printf("\nMostrar Filas\n");
                printf("Mostrar qual fila? (0 - Gerente) - (1 - Caixas prioritario e geral): ");
                scanf("%d", &tipoCliente);
                if (tipoCliente == 0) {
                    printf("Fila Prioritaria:\n");
                    mostraFila(fGP);
                    printf("\nFila Geral:\n");
                    mostraFila(fGG);
                } else {
                    printf("Fila Prioritaria:\n");
                    mostraFila(fP);
                    printf("\nFila Geral:\n");
                    mostraFila(fG);
                }
                break;

            case 4:
                printf("\nApagar Filas\n");
                printf("Apagando Fila Prioritaria...\n");
                apagaFila(fP);
                printf("Apagando Fila Geral...\n");
                apagaFila(fG);
                printf("Apagando Fila Prioritaria do Gerente...\n");
                apagaFila(fGP);
                printf("Apagando Fila Geral do Gerente...\n");
                apagaFila(fGG);
                break;

            case 5:
                printf("\nChamar Cliente:\n");
                if (contadorCliente < 2) {
                    if (fP->inicio != NULL) {
                        cl = desenfileirar(fP);
                        printf("Chamando Cliente Prioritario ao Caixa!\n");
                        processaOperacao(logCaixa, cl, saldo);
                    }
                    if (fGP->inicio != NULL) {
                        cl = desenfileirar(fGP);
                        printf("Chamando Cliente Prioritario ao Gerente!\n");
                        processaOperacao(logGerente, cl, saldo);
                    }
                    contadorCliente++;
                } else {
                    if (fG->inicio != NULL) {
                        cl = desenfileirar(fG);
                        printf("Chamando Cliente Geral ao Caixa!\n");
                        processaOperacao(logCaixa, cl, saldo);
                    }
                    if (fGG->inicio != NULL) {
                        cl = desenfileirar(fGG);
                        printf("Chamando Cliente Geral ao Gerente!\n");
                        processaOperacao(logGerente, cl, saldo);
                    }
                    contadorCliente = 0;
                }
                mostraQuadro(fG, fP, fGG, fGP);
                break;

            default:
                if (opAux != 0) {
                    printf("\nOpção invalida!\n");
                }
        }

    } while (opAux != 0);
}

int main() {
    Fila *filaGeralCaixa = criaFila();
    Fila *filaPrioritarioCaixa = criaFila();
    Fila *filaGeralGerente = criaFila();
    Fila *filaPrioritarioGerente = criaFila();
    Fila *logCaixa = criaFila();
    Fila *logGerente = criaFila();
    float saldoCaixa = 1000.0;

    int sG = 1, sP = 1, sGG = 1, sGP = 1;

    menuCaixa(filaGeralCaixa, filaPrioritarioCaixa, filaGeralGerente, filaPrioritarioGerente, logCaixa, logGerente, sG, sP, sGG, sGP, &saldoCaixa);

    apagaFila(filaGeralCaixa);
    apagaFila(filaPrioritarioCaixa);
    apagaFila(filaGeralGerente);
    apagaFila(filaPrioritarioGerente);
    apagaFila(logCaixa);
    apagaFila(logGerente);

    free(filaGeralCaixa);
    free(filaPrioritarioCaixa);
    free(filaGeralGerente);
    free(filaPrioritarioGerente);
    free(logCaixa);
    free(logGerente);

    return 0;
}
