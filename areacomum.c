#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "areacomum.h"
#include "reserva.h"

#define ARQUIVO_AREACOMUM "areas.bin"

int gerarID(AreaComum *vetor, int qtd) {
    int maior = 0;

    for(int i = 0; i < qtd; i++) {
        if(vetor[i].id_area > maior) {
            maior = vetor[i].id_area;
        }
    }

    return maior + 1;
}

int buscarIndiceArea(AreaComum *vetor, int qtd, int idBusca) {
    for(int i = 0; i < qtd; i++) {
        if(vetor[i].id_area == idBusca) {
            return i;
        }
    }

    return -1;
}

AreaComum* carregarAreas(int *qtd, int *tam) {
    FILE *f = fopen(ARQUIVO_AREACOMUM, "rb");

    if(f == NULL) {
        *qtd = 0;
        *tam = 5;
        return (AreaComum*) calloc(*tam, sizeof(AreaComum));
    }

    fseek(f, 0, SEEK_END);
    long tamanhoArquivo = ftell(f);
    rewind(f);

    int totalRegistros = tamanhoArquivo / sizeof(AreaComum);

    *qtd = totalRegistros;
    *tam = totalRegistros + 5;

    AreaComum *v = (AreaComum *) calloc(*tam, sizeof(AreaComum));

    if(totalRegistros > 0) {
        fread(v, sizeof(AreaComum), totalRegistros, f);
    }

    fclose(f);
    printf("Areas carregadas.\n", *qtd);
    return v;
}

void salvarAreas(AreaComum *vetor, int qtd) {
    FILE *f = fopen("areas.bin", "wb");

    if(f != NULL) {
        fwrite(vetor, sizeof(AreaComum), qtd, f);
        fclose(f);
    }
}

AreaComum* cadastrarArea(AreaComum *vetor, int *qtd, int *tam) {
    if(*qtd >= *tam) {
        *tam += 5;
        vetor = (AreaComum*) realloc(vetor, sizeof(AreaComum) * *tam);
    }

    int novoId = gerarID(vetor, *qtd);
    vetor[*qtd].id_area = novoId;

    printf("--- Nova area ---\n");

    printf("Nome da area: ");
    scanf(" %[^\n]", vetor[*qtd].nome);

    printf("Capacidade maxima: ");
    scanf("%d", &vetor[*qtd].capacidade_max);

    printf("Taxa de Limpeza (R$): ");
    scanf("%f", &vetor[*qtd].taxa_limpeza);

    (*qtd)++;

    printf("Area cadastrada com sucesso.\n");
    return vetor;
}

void listarAreas(AreaComum *vetor, int qtd) {
    printf("--- Lista de areas comuns ---\n");
    for(int i = 0; i < qtd; i++) {
        printf("ID (%d) %s | Cap. Max. %d pessoas | Taxa: R$ %.2f\n", vetor[i].id_area, vetor[i].nome, vetor[i].capacidade_max, vetor[i].taxa_limpeza);
    }
}

void alterarArea(AreaComum *vetor, int qtd) {
    int idBusca;

    printf("Insira o ID da area para alterar: ");
    scanf("%d", &idBusca);

    int index = buscarIndiceArea(vetor, qtd, idBusca);

    if(index == -1) {
        printf("Area nao encontrada!\n");
        return;
    }

    printf("Area encontrada: %s (ID %d)\n", vetor[index].nome, vetor[index].id_area);

    printf("Deseja alterar o nome desta area? (1 - Sim / 0 - Nao): ");
    int op;
    scanf("%d", &op);

    if(op == 1) {
        char novoNome[50];
        scanf(" %[^\n]", novoNome);

        if(strlen(novoNome) < 2) {
            printf("Nome muito curto/invalido.\n");
        } else {
            strcpy(vetor[index].nome, novoNome);
            printf("Sucesso. Area renomeada para '%s'.\n", novoNome);
        }
    }

    printf("Nova taxa: ");
    scanf("%f", &vetor[index].taxa_limpeza);

    printf("Dados atualizados.\n");
}

void removerArea(AreaComum *vetor, int *qtd, struct Reserva *vRes, int qRes) {
    int idBusca;

    printf("Insira o ID da area a ser removida: ");
    scanf("%d", &idBusca);

    int index = buscarIndiceArea(vetor, *qtd, idBusca);

    if(index == -1) {
        printf("Area nao encontrada.\n");
        return;
    }

    if(areaTemReserva(vRes, qRes, idBusca)) {
        printf("Erro: Nao e possivel remover essa area.\n");
        printf("Existem reservas agendadas para este local.\n");
        printf("Cancele as reservas pendentes antes de excluir a area.\n");
        system("pause");
        return;
    }

    for(int j = index; j < *qtd - 1; j++) {
        vetor[j] = vetor[j + 1];
    }

    (*qtd)--;
    printf("Area removida com sucesso.\n");
}

void moduloAreas(AreaComum **vetor, int *qtd, int *tam, struct Reserva *vRes, int qRes) {
    int opcao;

    do {
        system("cls");
        printf("GESTAO DE AREAS\n");
        printf("1. Cadastrar nova area\n");
        printf("2. Listar todas as areas\n");
        printf("3. Alterar dados da area\n");
        printf("4. Remover area\n");
        printf("0. Voltar ao menu principal\n");
        printf(">> ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                *vetor = cadastrarArea(*vetor, qtd, tam);
                system("pause");
                break;
            case 2:
                listarAreas(*vetor, *qtd);
                system("pause");
                break;
            case 3:
                alterarArea(*vetor, *qtd);
                system("pause");
                break;
            case 4:
                removerArea(*vetor, qtd, vRes, qRes);
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