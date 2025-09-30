#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cliente
{
    char nome[50];
    int id;
    struct cliente *prox;
} Cliente;

typedef struct carro
{
    char placa[8];
    char modelo[50];
    float valorLocacao;
    Cliente *locatario;
    struct carro *prox;
    struct carro *ant;
} Carro;

typedef struct listaCarros
{
    Carro *inicio;
    Carro *fim;
    int n_elementos;
} ListaCarros;

void inicializaLista(ListaCarros *l)
{
    l->inicio = NULL;
    l->fim = NULL;
    l->n_elementos = 0;
}
//ADICIONA CARRO NO FIM DA LISTA
void adicionaCarro(ListaCarros *lista, Carro *novoCarro)
{
    novoCarro->prox = NULL;
    novoCarro->ant = lista->fim;
    if (lista->fim != NULL)
    {
        lista->fim->prox = novoCarro;
    }
    else
    {
        lista->inicio = novoCarro;
    }
    lista->fim = novoCarro;
    lista->n_elementos++;
}

//REMOVE POR PLACA
Carro *removerCarro(ListaCarros *l, char placa[])
{
    Carro *atual = l->inicio;
    while (atual != NULL) {
        if (strcmp(atual->placa, placa) == 0) {
            if (atual->ant != NULL)
                atual->ant->prox = atual->prox;
            else
                l->inicio = atual->prox;

            if (atual->prox != NULL)
                atual->prox->ant = atual->ant;
            else
                l->fim = atual->ant;

            l->n_elementos--;
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

//MOSTRA VEICULOS(1) E SELECIONA PELA PLACA(2), CLIENTE SELECIONADO POR ID(3)
void alugaCarro(ListaCarros *livres, ListaCarros *locados, Cliente locatario[], int num_locatarios) {
    if (livres->n_elementos == 0) {
        printf("Nao ha carros disponiveis para locacao.\n");
        return;
    }

    // (1)
    printf("Carros disponiveis para locacao:\n");
    Carro *atual = livres->inicio;
    while (atual != NULL) {
        printf("Placa: %s, Modelo: %s, Valor: R$ %.2f\n", atual->placa, atual->modelo, atual->valorLocacao);
        atual = atual->prox;
    }

    // (2)
    char placa[10];
    printf("Digite a placa do Carro desejado: ");
    scanf("%s", placa);

    Carro *veiculoSelecionado = removerCarro(livres, placa);
    if (veiculoSelecionado == NULL) {
        printf("Carro nao encontrado.\n");
        return;
    }

    // (3)
    printf("Selecione um cliente:\n");
    for (int i = 0; i < num_locatarios; i++) {
        printf("%d - Nome: %s, ID: %d\n", i + 1, locatario[i].nome, locatario[i].id);
    }
    int id_cliente;
    printf("Digite o ID do cliente: ");
    scanf("%d", &id_cliente);

    if (id_cliente < 1 || id_cliente > num_locatarios) {
        printf("Cliente invalido.\n");
        adicionaCarro(livres, veiculoSelecionado);
        return;
    }

    veiculoSelecionado->locatario = &locatario[id_cliente - 1];
    adicionaCarro(locados, veiculoSelecionado);
    printf("Carro %s alugado para %s!\n", veiculoSelecionado->placa, locatario[id_cliente - 1].nome);
}

// MOSTRA VEICULOS A SEREM DEVOLCIDOS(1) E SELECIONA PELA PLACA(2), CLIENTE SELECIONADO POR ID(3)
void devolveCarro(ListaCarros *livres, ListaCarros *locados) {
    if (locados->n_elementos == 0) {
        printf("Nao ha carros locados.\n");
        return;
    }

    // (1)
    printf("Carros locados:\n");
    Carro *atual = locados->inicio;
    while (atual != NULL) {
        printf("Placa: %s, Modelo: %s, Cliente: %s, Valor: R$ %.2f\n", 
               atual->placa, atual->modelo, atual->locatario->nome, atual->valorLocacao);
        atual = atual->prox;
    }

    // (2)
    char placa[10];
    printf("Digite a placa do carro que deseja devolver: ");
    scanf("%s", placa);

    Carro *veiculoSelecionado = removerCarro(locados, placa);
    if (veiculoSelecionado == NULL) {
        printf("Carro nao encontrado.\n");
        return;
    }

    printf("Valor a pagar pela locacao do carro %s: R$ %.2f\n", veiculoSelecionado->placa, veiculoSelecionado->valorLocacao);
    veiculoSelecionado->locatario = NULL;
    adicionaCarro(livres, veiculoSelecionado);
    printf("Carro devolvido!\n");
}

int main() {
    ListaCarros livres, locados;
    inicializaLista(&livres);
    inicializaLista(&locados);

    // INICIALIZA CLIENTES
    Cliente locatarios[2] = {{"Gunther", 1, NULL}, {"Luciano ", 2, NULL}};

    // INICIALIZA CARROS
    Carro carro1 = {"ABC0000", "Ford Ka", 1500.0, NULL, NULL, NULL};
    Carro carro2 = {"DEF1111", "Honda CRV", 2500.0, NULL, NULL, NULL};
    Carro carro3 = {"GHI2222", "Mitsubishi Evo", 2700.0, NULL, NULL, NULL};
    Carro carro4 = {"JKL3333", "Chevrolet Cruze", 2200.0, NULL, NULL, NULL};
    Carro carro5 = {"MNO4444", "Renault Clio", 1200.0, NULL, NULL, NULL};
    adicionaCarro(&livres, &carro1);
    adicionaCarro(&livres, &carro2);
    adicionaCarro(&livres, &carro3);
    adicionaCarro(&livres, &carro4);
    adicionaCarro(&livres, &carro5);

    int opcao;
    do {
        printf("\nLocadora de Carros:\n");
        printf("1 -> Alugar Carro\n");
        printf("2 -> Devolver Carro\n");
        printf("0 -> Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                alugaCarro(&livres, &locados, locatarios, 2);
                break;
            case 2:
                devolveCarro(&livres, &locados);
                break;
            case 0:
                printf("Desligando o sistema.\n");
                break;
            default:
                printf("Opcao invalida\n");
        }
    } while (opcao != 0);

    return 0;
}
