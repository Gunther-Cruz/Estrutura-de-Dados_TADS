#include <stdio.h>
#include <locale.h> /*alara acentuação*/
#include <stdlib.h>
#include <string.h>


typedef struct data
{
    int dia, mes, ano;
} Data;

const Data hoje = {20, 10, 2024};


typedef struct vacina
{
    char nome[20];
    char fabricante[20];
    Data validade;
} Vacina;

typedef struct cao
{
    char nome[15];
    char nomeDono[30];
    char nomePai[15];
    char nomeMae[15];
    int idade;
    Data dNascimento;
    Vacina carterinha[10];
    int qtVacinas;
} Cao;

void mostraData(Data dt)
{ // imprime um data passada por valor
    printf(" %02d/%02d/%04d \n", dt.dia, dt.mes, dt.ano);
}

void mostraVacina(Vacina vc)
{ // imprime um vacina passada por valor
    printf("FABRICANTE: %s. ", vc.fabricante);
    printf("Vacina %s, data de validade: \n", vc.nome);
    mostraData(vc.validade);
}

void mostraDadosCao(Cao dog)
{ // imprime dados de um cão passado por valor
    printf("Nome: %s. Idade: %d meses/anos\n", dog.nome, dog.idade);
    printf("Data de Nascimento: ");
    mostraData(dog.dNascimento);
    printf("Dono: %s\n", dog.nomeDono);
    printf("Pais: %s e %s \n", dog.nomeMae, dog.nomePai );
    printf("Vacinas aplicadas (%d):\n", dog.qtVacinas);
    for (int i = 0; i < dog.qtVacinas; i++)
    {
        mostraVacina(dog.carterinha[i]);
    }
    printf("\n");
}

Data cadastraData(int d, int m, int a)
{ // cadastra e retorna uma nova data, passagem por valor
    Data data;
    data.dia = d;
    data.mes = m;
    data.ano = a;

    return data;
}

void cadastraVacina(Vacina *vc)
{ // recebe um poteiro de Vacina e preenche os campos do ponteiro "Passagem por referencia"
    printf("Insira o nome da Fabricante: ");
    scanf("%s", vc->fabricante);

    printf("Insira o nome da vacina a ser cadastrada: ");
    scanf("%s", vc->nome);

    printf("Insira a data de validade da vacina: ");
    int dia, mes, ano;
    scanf("%d %d %d", &dia, &mes, &ano);
    vc->validade = cadastraData(dia, mes, ano);
}

void cadastraCao(Cao *dog)
{ // recebe um poteiro de cao e preenche os campos do ponteiro "Passagem por referencia"

    printf("Nome do cachorro: ");
    scanf("%s", dog->nome);

    printf("Nome do pai do cachorro: ");
    scanf("%s", dog->nomePai);

    printf("Nome da mae do cachorro: ");
    scanf("%s", dog->nomeMae);

    printf("Nome do dono do cachorro: ");
    scanf("%s", dog->nomeDono);

    printf("Data de nascimento (dia mes ano): ");
    int dia, mes, ano;
    scanf("%d %d %d", &dia, &mes, &ano);
    dog->dNascimento = cadastraData(dia, mes, ano);
    dog->qtVacinas = 0;
    dog->idade = calcularIdade(*dog);
}

void vacinacao(Cao *dog, Vacina vc)
{
    if (dog->qtVacinas < 10)
    {
        dog->carterinha[dog->qtVacinas] = vc;
        dog->qtVacinas++;
    }
    else
    {
        printf("Cão já tomou o máximo de vacinas!\n");
    }
}

int calcularIdade(Cao cachorro)
{
    int idade = hoje.ano - cachorro.dNascimento.ano;
    if (hoje.mes < cachorro.dNascimento.mes)
    {
        idade--;
    }
    else
    {
        if (hoje.mes == cachorro.dNascimento.mes)
            if (hoje.dia < cachorro.dNascimento.dia)
                idade--;
    }

    return idade;
}

int calcularIdadeFilhote(Cao cachorro)
{
    int meses = (hoje.ano - cachorro.dNascimento.ano) * 12 + (hoje.mes - cachorro.dNascimento.mes);

    if (hoje.mes == cachorro.dNascimento.mes && hoje.dia < cachorro.dNascimento.dia)
    {
        meses--;
    }

    if (hoje.mes < cachorro.dNascimento.mes || (hoje.mes == cachorro.dNascimento.mes && hoje.dia < cachorro.dNascimento.dia))
    {
        meses--;
    }

    if (meses < 1)
    {
        meses = -1; 
    }

    return meses;
}

Cao *cruzamento(Cao pai, Cao mae)
{
    Cao *filhote = (Cao *)malloc(sizeof(Cao));

    printf("Nome do filhote: ");
    scanf("%s", filhote->nome);

    printf("Data de nascimento do filhote (dia mes ano): ");
    int dia, mes, ano;
    scanf("%d %d %d", &dia, &mes, &ano);
    filhote->dNascimento = cadastraData(dia, mes, ano);
    filhote->qtVacinas = 0; 
    filhote->idade = calcularIdadeFilhote(*filhote);

    strcpy(filhote->nomePai, pai.nome);
    strcpy(filhote->nomeMae, mae.nome);
    strcpy(filhote->nomeDono, pai.nomeDono); 
    return filhote;
}

int main()
{
    setlocale(LC_ALL, "");

    Cao caes[4];       
    Vacina vacinas[5]; 

    for (int i = 0; i < 4; i++)
    {
        printf("\nCadastro do cão %d:\n", i + 1);
        cadastraCao(&caes[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        printf("\nCadastro da vacina %d:\n", i + 1);
        cadastraVacina(&vacinas[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        vacinacao(&caes[i], vacinas[i]);
    }


    // Um cão recebe todas as vacinas
    for (int i = 0; i < 4; i++)
    {
        vacinacao(&caes[0], vacinas[i]);
    }

    // Cruzamento 
    Cao *filhote = cruzamento(caes[0], caes[1]);

    vacinacao(filhote, vacinas[2]);

    printf("\n Dados dos cães cadastrados n");
    for (int i = 0; i < 4; i++)
    {
        mostraDadosCao(caes[i]);
    }

    printf("\n Dados do filhote n");
    mostraDadosCao(*filhote);

    free(filhote);

    return 0;
}
