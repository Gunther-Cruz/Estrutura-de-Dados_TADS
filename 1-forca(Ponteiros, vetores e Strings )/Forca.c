#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function Prototypes - servem para o compilador ler as funçoes que serao executadas no programa com antecedencia(meu vscode acusou erro por muito tempo quando nao fazia isso, mas nao deixava de rodar)
void mostraForca(int erros);
void sorteiaPalavra(char *palavra);
void imprimePalavra(char *letrasEncontradas, int tamanho);
void criaVetorCaracteres(char *vetorLetras, int tamanho);
int verificaAcerto(char *palavraSecreta, char *letrasEncontradas, char letra);
void fimDeJogo(int erros, int tamanho, char *letrasEncontradas);

int main()
{
    char jogarNovamente;
    do
    {
        //Inicializa os vetores necessarios para sortear e printar a palavra secreta na tela
        char palavraSorteada[20];
        sorteiaPalavra(palavraSorteada);

        int tamanho = strlen(palavraSorteada);
        char letrasEncontradas[20];
        int erros = 0;
        char letra;

        criaVetorCaracteres(letrasEncontradas, tamanho);

        // Loop do round
        while (erros < 6)
        {
            mostraForca(erros);
            imprimePalavra(letrasEncontradas, tamanho);

            printf("Digite uma letra: ");
            scanf(" %c", &letra);

            int acertos = verificaAcerto(palavraSorteada, letrasEncontradas, letra);

            if (acertos == 0)
            {
                erros++;
            }

            fimDeJogo(erros, tamanho, letrasEncontradas);

            if (strchr(letrasEncontradas, '_') == NULL)
            {
                printf("Voce ganhou!\n");
                printf("A palavra era: %s\n", palavraSorteada);

                break;
            }
        }

        if (erros >= 6)
        {
            mostraForca(erros);
            printf("GAME OVER! A palavra era: %s\n", palavraSorteada);
        }

        printf("Deseja jogar de novo? (S/N): ");
        scanf(" %c", &jogarNovamente);

    } while (jogarNovamente == 'S' || jogarNovamente == 's'); 


    return 0;
}

// Monta e mostra a forca, modifica conforme a quantidade de erros/encontrei essa maneira para fazer usando o operador condicional ternario '?', mais curto que if-else-switch
void mostraForca(int erros)
{
    printf(" ____\n");
    printf(" |   |\n");
    printf(" |   %c\n", (erros >= 1) ? 'O' : ' ');
    printf(" |  %c%c%c\n", (erros >= 3) ? '/' : ' ', (erros >= 2) ? '|' : ' ', (erros >= 4) ? '\\' : ' ');
    printf(" |  %c %c\n", (erros >= 5) ? '/' : ' ', (erros >= 6) ? '\\' : ' ');
    printf(" |\n");
    printf("_|_\n\n");
}

// Sorteia a palavra secreta
void sorteiaPalavra(char *palavra)
{
    srand(time(NULL));
    char palavras[20][20] = {
        "programacao", "computador", "algoritmo", "linguagem", "ponteiro",
        "vetor", "string", "compilador", "biblioteca", "processador",
        "variavel", "funcao", "parametro", "argumento", "sintaxe",
        "estruturas", "recursao", "heap", "pilha", "fila"};
    strcpy(palavra, palavras[rand() % 20]);
}

// Imprime letra encontrada digitada
void imprimePalavra(char *letrasEncontradas, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        printf("%c ", letrasEncontradas[i]);
    }
    printf("\n\n");
}

// Cria o vetor com os espaços que aparecem na tela
void criaVetorCaracteres(char *vetorLetras, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        vetorLetras[i] = '_';
    }
}

// Verifica se a entrada do jogador é um acerto, contabiliza erro no main se for errado
int verificaAcerto(char *palavraSecreta, char *letrasEncontradas, char letra)
{
    int acertos = 0;
    for (int i = 0; i < strlen(palavraSecreta); i++)
    {
        if (palavraSecreta[i] == letra)
        {
            letrasEncontradas[i] = letra;
            acertos++;
        }
    }
    return acertos;
}

// Verifica se o jogo acaba por 6 erros do jogador ou por achar a palavra secreta
void fimDeJogo(int erros, int tamanho, char *letrasEncontradas)
{
    if (erros >= 6)
    {
        printf("Você cometeu 6 erros. GAME OVER!\n");
    }
    else if (strchr(letrasEncontradas, '_') == NULL)
    {
        printf("");
    }
}
