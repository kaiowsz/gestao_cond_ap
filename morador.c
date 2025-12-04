#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#include "morador.h"
#include "reserva.h"
#define ARQUIVO_MORADORES "moradores.bin"

int validarFormatoCPF(char *cpf) {
    int len = strlen(cpf);

    if(len != 11 && len != 14) {
        return 0;
    }

    for(int i = 0; i < len; i++) {
        if(!isdigit(cpf[i])) {
            return 0;
        }
    }

    return 1;
}

int validarNome(char *nome) {
    if(strlen(nome) < 3) return 0;

    for(int i = 0; i < strlen(nome); i++) {
        if(isdigit(nome[i])) return 0;
    }

    return 1;
}

Morador* carregarMoradores(int *qtd, int *tam) {
    setlocale(LC_ALL, "Portuguese");
    FILE *f = fopen(ARQUIVO_MORADORES, "rb");

    if(f == NULL) {
        printf("Arquivo '%s' não encontrado. Iniciando base vazia.\n", ARQUIVO_MORADORES);

        *qtd = 0;
        *tam = 5;

        return (Morador *) calloc(*tam, sizeof(Morador));
    }

    fseek(f, 0, SEEK_END);
    long tamanhoArquivo = ftell(f);
    rewind(f);

    int totalRegistros = tamanhoArquivo / sizeof(Morador);

    *qtd = totalRegistros;
    *tam = totalRegistros + 5;

    Morador *v = (Morador*) calloc(*tam, sizeof(Morador));

    if(v == NULL) {
        printf("Erro fatal: Falha na alocação de memória ao carregar moradores.\n");
        exit(1);
    }

    if(totalRegistros > 0) {
        fread(v, sizeof(Morador), totalRegistros, f);
    }

    fclose(f);
    printf("Sucesso. %d moradores carregados.\n", *qtd);
    return v;
}

void salvarMoradores(Morador *vetor, int qtd) {
    FILE *f = fopen(ARQUIVO_MORADORES, "wb");

    if(f == NULL) {
        printf("Erro: nao foi possivel abrir '%s' para salvar.\n", ARQUIVO_MORADORES);
    }

    fwrite(vetor, sizeof(Morador), qtd, f);

    fclose(f);
    printf("Dados salvos com sucesso.\n");
}

Morador* cadastrarMorador(Morador *vetor, int *qtd, int *tam) {

    if(*qtd >= *tam) {
        printf("\nAumentando espaço de memoria (de %d para %d)\n", *tam, (*tam + 5));
    }

    *tam += 5;

    Morador *temp = (Morador*) realloc(vetor, *tam * sizeof(Morador));

    if(temp == NULL) {
        printf("Erro: memoria insuficiente para novos cadastros.\n");
        return vetor;
    }

    vetor = temp;
    
    printf("Criando novo morador - insira os seguintes dados: \n");
    char cpfTemp[50];

    int cpfValido = 0;

    do {
        printf("CPF (apenas numeros): ");
        scanf(" %[^\n]", cpfTemp);

        if(!validarFormatoCPF(cpfTemp)) {
            printf("Erro: formato invalido. Use apenas numeros (11 digitos).\n");
        }
        else if(buscarIndiceMorador(vetor, *qtd, cpfTemp) != -1) {
            printf("Erro: esse CPF ja existe no sistema.\n");
        } else {
            cpfValido = 1;
        }
    } while(!cpfValido);

    strcpy(vetor[*qtd].cpf, cpfTemp);

    int nomeValido = 0;

    do {
        printf("Nome completo: ");
        scanf(" %[^\n]", vetor[*qtd].nome);

        if(!validarNome(vetor[*qtd].nome)) {
            printf("Erro: nome invalido. Nome deve ter no minimo 3 letras e nao pode conter numeros.\n");
        } else {
            nomeValido = 1;
        }
    } while(!nomeValido);

    printf("Bloco: ");
    scanf(" %[^\n]", vetor[*qtd].bloco);


    int apTemp;

    do {
        printf("Numero do apartamento: ");
        
        // scanf retorna se conseguiu ler algo. se n conseguiu, retorna 0 
        if(scanf("%d", &apTemp) != 1) {
            while(getchar() != '\n');
            apTemp = -1;
        }

        if(apTemp <= 0) {
            printf("Erro: numero invalido. Digite um valor positivo.\n");
        }
    } while(apTemp <= 0);

    vetor[*qtd].apartamento = apTemp;

    printf("Telefone: ");
    scanf(" %[^\n]", vetor[*qtd].telefone);

    (*qtd)++;

    printf("Morador cadastrado com sucesso.\n");

    return vetor;
}

void listarMoradores(Morador *vetor, int qtd) {
    if(qtd == 0) {
        printf("Nenhum morador cadastrado.\n");
        return;
    }

    printf("Lista de moradores (%d) registros\n", qtd);

    for(int i = 0; i < qtd; i++) {
        printf("[%d] %s - Apt %d Bloco %s - Tel: %s\n", 
        i, vetor[i].nome, vetor[i].apartamento, vetor[i].bloco, vetor[i].telefone);
    }
    printf("------------\n");
}

int buscarIndiceMorador(Morador *vetor, int qtd, char *cpfBusca) {
    for(int i = 0; i < qtd; i++) {
        if(strcmp(vetor[i].cpf, cpfBusca) == 0) {
            return i;
        }
    }

    return -1;
}

void alterarMorador(Morador *vetor, int qtd) {

    char cpfParaAlterar[15];

    printf("Alterar Dados\n");
    printf("Digite o CPF do morador: ");
    scanf(" %[^\n]", cpfParaAlterar);

    int index = buscarIndiceMorador(vetor, qtd, cpfParaAlterar);

    if(index == -1) {
        printf("Erro: CPF nao encontrado.\n");
        return;
    }

    printf("Novo nome: ");
    scanf(" %[^\n]", vetor[index].nome);

    printf("Novo telefone: ");
    scanf(" %[^\n]", vetor[index].telefone);

    printf("Dados atualizados com sucesso.\n");

}

void removerMorador(Morador *vetor, int *qtd, struct Reserva *vRes, int qRes) {

    char cpfParaRemover[15];

    printf("Remover Morador\n");

    printf("Digite o CPF: ");
    scanf(" %[^\n]", cpfParaRemover);

    if(moradorTemReserva(vRes, qRes, cpfParaRemover) == 1) {
        printf("Erro: nao e possivel remover esse morador.\n");
        printf("Morador tem reservas pendentes.\n");
        return;
    }

    int index = buscarIndiceMorador(vetor, *qtd, cpfParaRemover);

    if(index == -1) {
        printf("Erro: CPF nao encontrado.\n");
        return;
    }

    printf("\n---------------\n");
    printf("Tem certeza que deseja excluir: \n");
    printf("Nome: %s, CPF: %s\n", vetor[index].nome, vetor[index].cpf);
    printf("---------------\n");
    printf("1 - Sim, Excluir / 0 - Nao, cancelar\n>> ");

    int confirma;
    scanf("%d", &confirma);

    if(confirma != 1) {
        printf("Operacao cancelada.\n");
        return;
    }

    printf("Removendo...\n");

    for(int i = index; i < *qtd - 1; i++) {
        vetor[i] = vetor[i + 1];
    }

    (*qtd)--;

    printf("Morador removido com sucesso.\n");
}

void moduloMoradores(Morador **vetor, int *qtd, int *tam, struct Reserva *vRes, int qRes) {
    int opcao;

    do {
        system("cls");
        printf("GESTAO DE MORADORES\n");
        printf("1. Cadastrar novo morador\n");
        printf("2. Listar todos os moradores\n");
        printf("3. Alterar dados do morador\n");
        printf("4. Remover morador\n");
        printf("0. Voltar ao menu principal\n");
        printf(">> ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                *vetor = cadastrarMorador(*vetor, qtd, tam);
                system("pause");
                break;
            case 2:
                listarMoradores(*vetor, *qtd);
                system("pause");
                break;
            case 3:
                alterarMorador(*vetor, *qtd);
                system("pause");
                break;
            case 4:
                removerMorador(*vetor, qtd, vRes, qRes);
                system("pause");
                break;
            case 0:
                printf("Voltando...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while(opcao != 0);
}