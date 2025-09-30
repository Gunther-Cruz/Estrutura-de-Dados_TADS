#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Carro
{
    char placa[8];
    char modelo[20];
    float valor_locacao;
    struct Carro *proximo;
} Carro;

typedef struct
{
    Carro *primeiro;
    int tamanho;
} ListaCarros;

typedef struct
{
    ListaCarros buckets[11];
} TabelaHash;

void inicializarLista(ListaCarros *lista)
{
    lista->primeiro = NULL;
    lista->tamanho = 0;
}

void inicializarTabelaHash(TabelaHash *hash)
{
    for (int i = 0; i < 11; i++)
    {
        inicializarLista(&hash->buckets[i]);
    }
}

void gerarStringAleatoria(char *str, int tamanho)
{
    const char caracteres[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < tamanho - 1; i++)
    {
        str[i] = caracteres[rand() % (sizeof(caracteres) - 1)];
    }
    str[tamanho - 1] = '\0';
}

void adicionarCarro(ListaCarros *lista, const char *placa, const char *modelo, float valor)
{
    Carro *novoCarro = (Carro *)malloc(sizeof(Carro));
    strcpy(novoCarro->placa, placa);
    strcpy(novoCarro->modelo, modelo);
    novoCarro->valor_locacao = valor;
    novoCarro->proximo = lista->primeiro;
    lista->primeiro = novoCarro;
    lista->tamanho++;
}

void gerarCarrosAleatorios(ListaCarros *lista)
{
    char placa[8];
    char modelo[20];
    float valor;
    for (int i = 0; i < 1000; i++)
    {
        gerarStringAleatoria(placa, 8);
        gerarStringAleatoria(modelo, 10);
        valor = (rand() % 5000) / 100.0 + 50.0; 
        adicionarCarro(lista, placa, modelo, valor);
    }
}

void exibirLista(ListaCarros *lista)
{
    Carro *atual = lista->primeiro;
    while (atual != NULL)
    {
        printf("Placa: %s, Modelo: %s, Valor: %.2f\n", atual->placa, atual->modelo, atual->valor_locacao);
        atual = atual->proximo;
    }
}

void copiarLista(ListaCarros *origem, ListaCarros *destino)
{
    inicializarLista(destino);
    Carro *atual = origem->primeiro;
    while (atual != NULL)
    {
        adicionarCarro(destino, atual->placa, atual->modelo, atual->valor_locacao);
        atual = atual->proximo;
    }
}

//ordenação SelectionSort
void ordenarPorSelectionSort(ListaCarros *lista)
{
    for (Carro *i = lista->primeiro; i != NULL; i = i->proximo)
    {
        Carro *min = i;
        for (Carro *j = i->proximo; j != NULL; j = j->proximo)
        {
            if (j->valor_locacao < min->valor_locacao)
            {
                min = j;
            }
        }
        if (min != i)
        {
            char tempPlaca[8], tempModelo[20];
            float tempValor = i->valor_locacao;

            strcpy(tempPlaca, i->placa);
            strcpy(tempModelo, i->modelo);

            i->valor_locacao = min->valor_locacao;
            strcpy(i->placa, min->placa);
            strcpy(i->modelo, min->modelo);

            min->valor_locacao = tempValor;
            strcpy(min->placa, tempPlaca);
            strcpy(min->modelo, tempModelo);
        }
    }
}

//ordenação por InsertionSort
void ordenarPorInsercao(ListaCarros *lista)
{
    Carro *sorted = NULL;
    Carro *atual = lista->primeiro;

    while (atual != NULL)
    {
        Carro *next = atual->proximo;

        if (sorted == NULL || sorted->valor_locacao >= atual->valor_locacao)
        {
            atual->proximo = sorted;
            sorted = atual;
        }
        else
        {
            Carro *temp = sorted;
            while (temp->proximo != NULL && temp->proximo->valor_locacao < atual->valor_locacao)
            {
                temp = temp->proximo;
            }
            atual->proximo = temp->proximo;
            temp->proximo = atual;
        }
        atual = next;
    }
    lista->primeiro = sorted;
}

//ordenação por BubbleSort
void ordenarPorBubbleSort(ListaCarros *lista)
{
    int trocou;
    do
    {
        trocou = 0;
        Carro *atual = lista->primeiro;
        while (atual != NULL && atual->proximo != NULL)
        {
            if (atual->valor_locacao > atual->proximo->valor_locacao)
            {
                char tempPlaca[8], tempModelo[20];
                float tempValor = atual->valor_locacao;

                strcpy(tempPlaca, atual->placa);
                strcpy(tempModelo, atual->modelo);

                atual->valor_locacao = atual->proximo->valor_locacao;
                strcpy(atual->placa, atual->proximo->placa);
                strcpy(atual->modelo, atual->proximo->modelo);

                atual->proximo->valor_locacao = tempValor;
                strcpy(atual->proximo->placa, tempPlaca);
                strcpy(atual->proximo->modelo, tempModelo);

                trocou = 1;
            }
            atual = atual->proximo;
        }
    } while (trocou);
}

// ordenação por BubbleSort
void ordenarPorShellSort(ListaCarros *lista)
{
    int gap, tamanho = lista->tamanho, i, j;
    Carro **carros; 
    Carro *atual = lista->primeiro;

    carros = (Carro **)malloc(tamanho * sizeof(Carro *));
    if (carros == NULL)
    {
        printf("Erro ao alocar memória para o array de carros.\n");
        return;
    }

    for (i = 0; i < tamanho; i++)
    {
        carros[i] = atual;
        atual = atual->proximo;
    }

    for (gap = tamanho / 2; gap > 0; gap /= 2)
    {
        for (i = gap; i < tamanho; i++)
        {
            Carro *temp = carros[i];
            for (j = i; j >= gap && carros[j - gap]->valor_locacao > temp->valor_locacao; j -= gap)
            {
                carros[j] = carros[j - gap];
            }
            carros[j] = temp;
        }
    }

    lista->primeiro = carros[0];
    atual = lista->primeiro;
    for (i = 1; i < tamanho; i++)
    {
        atual->proximo = carros[i];
        atual = atual->proximo;
    }
    atual->proximo = NULL;

    free(carros);
}

int calcularHash(const char *modelo)
{
    int soma = 0;
    for (int i = 0; modelo[i] != '\0'; i++)
    {
        soma += modelo[i];
    }
    return soma % 11;
}

//inserir na tabela hash
void inserirNaTabelaHash(TabelaHash *hash, const char *placa, const char *modelo, float valor)
{
    int indice = calcularHash(modelo);
    adicionarCarro(&hash->buckets[indice], placa, modelo, valor);
}

//buscar na tabela hash
Carro *buscarNaTabelaHash(TabelaHash *hash, const char *modelo)
{
    int indice = calcularHash(modelo);
    Carro *atual = hash->buckets[indice].primeiro;
    while (atual != NULL)
    {
        if (strcmp(atual->modelo, modelo) == 0)
        {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

//buscar em uma lista
Carro *buscarNaLista(ListaCarros *lista, const char *modelo)
{
    Carro *atual = lista->primeiro;
    while (atual != NULL)
    {
        if (strcmp(atual->modelo, modelo) == 0)
        {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void medir_tempo(void (*sort_func)(ListaCarros *), ListaCarros *lista, const char *descricao)
{
    clock_t inicio, fim;
    double tempo_gasto;

    printf("\nIniciando %s...\n", descricao);
    inicio = clock();
    sort_func(lista);
    fim = clock();

    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("%s concluído. Tempo gasto: %.6f segundos\n", descricao, tempo_gasto);
}

void menu()
{
    ListaCarros listaOriginal, copia1, copia2, copia3;
    TabelaHash tabelaHash;
    int opcao;

    inicializarLista(&listaOriginal);
    inicializarTabelaHash(&tabelaHash);

    do
    {
        printf("\nMenu:\n");
        printf("1. Criar a Lista LSE e Mostrar os Elementos\n");
        printf("2. Criar novas Listas LSE ordenadas com os tempos de execução\n");
        printf("3. Implementar a tabela HASH, e encontrar elementos na tabela\n");
        printf("4. Realizar buscas nas listas e comparar os tempos\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            gerarCarrosAleatorios(&listaOriginal);
            printf("Lista gerada:\n");
            exibirLista(&listaOriginal);
            break;

        case 2:
            copiarLista(&listaOriginal, &copia1);
            copiarLista(&listaOriginal, &copia2);
            copiarLista(&listaOriginal, &copia3);

            medir_tempo(ordenarPorShellSort, &copia1, "Shell Sort");
            medir_tempo(ordenarPorBubbleSort, &copia2, "Bubble Sort");
            medir_tempo(ordenarPorInsercao, &copia3, "Insertion Sort");
            break;

        case 3:
        {
            char modelo[20];
            printf("Inserindo elementos na tabela hash...\n");
            Carro *atual = listaOriginal.primeiro;
            while (atual != NULL)
            {
                inserirNaTabelaHash(&tabelaHash, atual->placa, atual->modelo, atual->valor_locacao);
                atual = atual->proximo;
            }
            exibirLista(&listaOriginal);
            printf("Digite o modelo para buscar na tabela hash: ");
            scanf("%s", modelo);
            Carro *encontrado = buscarNaTabelaHash(&tabelaHash, modelo);
            if (encontrado != NULL)
            {
                printf("Carro encontrado - Placa: %s, Modelo: %s, Valor: %.2f\n", encontrado->placa, encontrado->modelo, encontrado->valor_locacao);
            }
            else
            {
                printf("Carro nao encontrado.\n");
            }
        }
        break;

        case 4:
        {
            char modelo[20];
            printf("Digite o modelo para buscar: ");
            scanf("%s", modelo);

            clock_t inicio, fim;
            double tempo_gasto;

            // Busca lista original
            inicio = clock();
            Carro *encontrado = buscarNaLista(&listaOriginal, modelo);
            fim = clock();
            tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            if (encontrado != NULL)
            {
                printf("Carro encontrado na lista original - Placa: %s, Modelo: %s, Valor: %.2f\n", encontrado->placa, encontrado->modelo, encontrado->valor_locacao);
            }
            else
            {
                printf("Carro nao encontrado na lista original.\n");
            }
            printf("Tempo de busca na lista original: %.6f segundos\n", tempo_gasto);

            // Busca listas ordenadas
            ListaCarros *listas[] = {&copia1, &copia2, &copia3};
            const char *nomes[] = {"Shell Sort", "Bubble Sort", "Insertion Sort"};

            for (int i = 0; i < 3; i++)
            {
                inicio = clock();
                encontrado = buscarNaLista(listas[i], modelo);
                fim = clock();
                tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

                if (encontrado != NULL)
                {
                    printf("Carro encontrado na lista %s - Placa: %s, Modelo: %s, Valor: %.2f\n", nomes[i], encontrado->placa, encontrado->modelo, encontrado->valor_locacao);
                }
                else
                {
                    printf("Carro nao encontrado na lista %s.\n", nomes[i]);
                }
                printf("Tempo de busca na lista %s: %.6f segundos\n", nomes[i], tempo_gasto);
            }

            // Busca tabela hash
            inicio = clock();
            encontrado = buscarNaTabelaHash(&tabelaHash, modelo);
            fim = clock();
            tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            if (encontrado != NULL)
            {
                printf("Carro encontrado na tabela hash - Placa: %s, Modelo: %s, Valor: %.2f\n", encontrado->placa, encontrado->modelo, encontrado->valor_locacao);
            }
            else
            {
                printf("Carro nao encontrado na tabela hash.\n");
            }
            printf("Tempo de busca na tabela hash: %.6f segundos\n", tempo_gasto);
        }
        break;

        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);
}

int main()
{
    srand(time(NULL));
    menu();
    return 0;
}
