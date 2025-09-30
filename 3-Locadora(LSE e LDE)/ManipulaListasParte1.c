#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estruturas para o Carro, Cliente e Lista Duplamente Encadeada de Carros

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


void inicializaLista(ListaCarros *lista)
{
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->n_elementos = 0;
}

// A) ADICIONA UM CARRO AO FIM DA LISTA
void adicionaCarroFim(ListaCarros *lista, Carro *novoCarro)
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

// B) ADICIONA UM CARRO AO INICIO DA LISTA
void adicionaCarroInicio(ListaCarros *lista, Carro *novoCarro)
{
    novoCarro->ant = NULL;
    novoCarro->prox = lista->inicio;
    if (lista->inicio != NULL)
    {
        lista->inicio->ant = novoCarro;
    }
    else
    {
        lista->fim = novoCarro;
    }
    lista->inicio = novoCarro;
    lista->n_elementos++;
}

// C) ADICIONA UM CARRO A POSIÇAO ESCOLHIDA NA LISTA
int adicionaCarroPosicao(ListaCarros *lista, Carro *novoCarro, int pos)
{
    if (pos < 0 || pos > lista->n_elementos)
        return 0; // Posição inválida
    if (pos == 0)
    {
        adicionaCarroInicio(lista, novoCarro);
    }
    else if (pos == lista->n_elementos)
    {
        adicionaCarroFim(lista, novoCarro);
    }
    else
    {
        Carro *atual = lista->inicio;
        for (int i = 0; i < pos - 1; i++)
        {
            atual = atual->prox;
        }
        novoCarro->prox = atual->prox;
        novoCarro->ant = atual;
        if (atual->prox != NULL)
            atual->prox->ant = novoCarro;
        atual->prox = novoCarro;
        lista->n_elementos++;
    }
    return 1;
}

// D) REMOVE O PRIMEIRO CARRO DA LISTA
Carro *removeInicio(ListaCarros *lista)
{
    if (lista->inicio == NULL)
        return NULL; // Lista vazia
    Carro *removido = lista->inicio;
    lista->inicio = removido->prox;
    if (lista->inicio != NULL)
        lista->inicio->ant = NULL;
    else
        lista->fim = NULL;
    lista->n_elementos--;
    return removido;
}

// E) REMOVE O ULTIMO CARRO DA LISTA
Carro *removeFim(ListaCarros *lista)
{
    if (lista->fim == NULL)
        return NULL; // Lista vazia
    Carro *removido = lista->fim;
    lista->fim = removido->ant;
    if (lista->fim != NULL)
        lista->fim->prox = NULL;
    else
        lista->inicio = NULL;
    lista->n_elementos--;
    return removido;
}

// F)  REMOVE CARRO DA POSIÇAO ESCOLHIDA NA LISTA

Carro *removePosicao(ListaCarros *lista, int pos)
{
    if (pos < 0 || pos >= lista->n_elementos)
        return NULL; // Posição inválida ou lista vazia
    Carro *removido;
    if (pos == 0)
    {
        removido = removeInicio(lista);
    }
    else if (pos == lista->n_elementos - 1)
    {
        removido = removeFim(lista);
    }
    else
    {
        Carro *atual = lista->inicio;
        for (int i = 0; i < pos; i++)
            atual = atual->prox;
        removido = atual;
        atual->ant->prox = atual->prox;
        if (atual->prox != NULL)
            atual->prox->ant = atual->ant;
        lista->n_elementos--;
    }
    return removido;
}

// H) RETORNA QUANTIDADE DE CARROS EM UMA LISTA
int quantidadeCarros(ListaCarros *lista)
{
    return lista->n_elementos;
}

// I) MOSTRA OS CARROS EM UMA LISTA
void mostraLista(ListaCarros *lista)
{
    Carro *atual = lista->inicio;
    while (atual != NULL)
    {
        printf("Placa: %s, Modelo: %s, Valor: %.2f\n", atual->placa, atual->modelo, atual->valorLocacao);
        atual = atual->prox;
    }
}

// J) APAGA UM CARRO
void apagaCarro(Carro *carro)
{
    free(carro);
}

// APAGA A LISTA INTEIRA
void apagaLista(ListaCarros *lista)
{
    Carro *atual = lista->inicio;
    while (atual != NULL)
    {
        Carro *prox = atual->prox;
        apagaCarro(atual);
        atual = prox;
    }
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->n_elementos = 0;
}

// MENU
void menu(ListaCarros *livres, ListaCarros *locados)
{
    int opcao;
    do
    {
        printf("\nMenu:\n");
        printf("1 - Inserir um Elemento no Início\n");
        printf("2 - Inserir um Elemento no Fim\n");
        printf("3 - Inserir um Elemento na Posição\n");
        printf("4 - Remove um elemento no Início\n");
        printf("5 - Remove um elemento no Fim\n");
        printf("6 - Remove um elemento na Posição\n");
        printf("7 - Mostra Lista\n");
        printf("8 - Apaga um Elemento da Lista\n");
        printf("9 - Apaga a Lista\n");
        printf("0 - Fim do Programa - Lista de Chamada\n");
        printf("Escolha uma opção: \n");
        scanf("%d", &opcao);

        Carro *novoCarro;
        int pos;
        char placa[8];

        switch (opcao)
        {
        case 1:
            novoCarro = (Carro *)malloc(sizeof(Carro));
            printf("Digite a placa, modelo e valor de locação do carro: ");
            scanf("%s %s %f", novoCarro->placa, novoCarro->modelo, &novoCarro->valorLocacao);
            novoCarro->locatario = NULL;
            adicionaCarroInicio(livres, novoCarro);
            break;
        case 2:
            novoCarro = (Carro *)malloc(sizeof(Carro));
            printf("Digite a placa, modelo e valor de locação do carro: ");
            scanf("%s %s %f", novoCarro->placa, novoCarro->modelo, &novoCarro->valorLocacao);
            novoCarro->locatario = NULL;
            adicionaCarroFim(livres, novoCarro);
            break;
        case 3:
            novoCarro = (Carro *)malloc(sizeof(Carro));
            printf("Digite a placa, modelo e valor de locação do carro: ");
            scanf("%s %s %f", novoCarro->placa, novoCarro->modelo, &novoCarro->valorLocacao);
            novoCarro->locatario = NULL;
            printf("Digite a posição para inserir o carro: ");
            scanf("%d", &pos);
            if (!adicionaCarroPosicao(livres, novoCarro, pos))
            {
                printf("Posição inválida!\n");
                free(novoCarro);
            }
            break;
        case 4:
            apagaCarro(removeInicio(livres));
            break;
        case 5:
            apagaCarro(removeFim(livres));
            break;
        case 6:
            printf("Digite a posição para remover o carro: ");
            scanf("%d", &pos);
            apagaCarro(removePosicao(livres, pos));
            break;
        case 7:
            mostraLista(livres);
            break;
        case 8:
            printf("Digite a placa do carro a ser apagado: ");
            scanf("%s", placa);
            Carro *atual = livres->inicio;
            while (atual != NULL)
            {
                if (strcmp(atual->placa, placa) == 0)
                {
                    if (atual->ant)
                        atual->ant->prox = atual->prox;
                    if (atual->prox)
                        atual->prox->ant = atual->ant;
                    if (atual == livres->inicio)
                        livres->inicio = atual->prox;
                    if (atual == livres->fim)
                        livres->fim = atual->ant;
                    apagaCarro(atual);
                    livres->n_elementos--;
                    break;
                }
                atual = atual->prox;
            }
            break;
        case 9:
            apagaLista(livres);
            break;
        case 0:
            printf("Encerrando o programa.\n");
            break;
        default:
            printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}


int main()
{
    ListaCarros livres;
    ListaCarros locados;
    inicializaLista(&livres);
    inicializaLista(&locados);
    menu(&livres, &locados);
    return 0;
}
