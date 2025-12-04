#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reserva.h"

#define ARQUIVO_RESERVAS "reservas.bin"

int gerarIdReserva(Reserva *vetor, int qtd) {
    int maior = 0;

    for(int i = 0; i < qtd; i++) {
        if(vetor[i].id_reserva > maior) {
            maior = vetor[i].id_reserva;
        }
    }

    return maior + 1;
}

void imprimirLinhaReserva(FILE *saida, Reserva reserva, Morador *vMor, AreaComum *area) {
    fprintf(saida, "| #%-4d | %02d/%02d/%04d | ", reserva.id_reserva,
    reserva.data_reserva.dia, reserva.data_reserva.mes, reserva.data_reserva.ano);

    if(area != NULL) {
        fprintf(saida, "%-20s | ", area->nome);
    } else {
        fprintf(saida, "Area Desc. (ID %d)           |", reserva.id_area);
    }

    if(vMor != NULL) {
        fprintf(saida, "%-20s (CPF: %s)\n", vMor->nome, reserva.cpf_morador);
    }
}

int anoBissexto(int ano) {
    // divisível por 4 e nao termina em 00 OU divisível por 400.
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

int moradorTemReserva(Reserva *vRes, int qRes, char *cpf) {
    for(int i = 0; i < qRes; i++) {
        if(strcmp(vRes[i].cpf_morador, cpf) == 0) {
            // Há ao menos uma reserva nesse cpf.
            return 1;
        }
    }

    // Sem reservas.
    return 0;
}

int verificarDataValida(int dia, int mes, int ano) {
    if(ano < 2024) return 0;
    if(mes < 1 || mes > 12) return 0;
    if (dia < 1) return 0;

    int diasNoMes;

    if(mes == 2) {
        diasNoMes = anoBissexto(ano) ? 29 : 28;
    } else if(mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        diasNoMes = 30;
    } else {
        diasNoMes = 31;
    }

    if(dia > diasNoMes) return 0;

    return 1;
}

int verificarDisponibilidade(Reserva *vRes, int qtd, int idArea, int dia, int mes, int ano) {
    for(int i = 0; i < qtd; i++) {
        if(vRes[i].id_area == idArea) {
            if(
                vRes[i].data_reserva.dia == dia &&
                vRes[i].data_reserva.mes == mes &&
                vRes[i].data_reserva.ano == ano
            ) {
                // Ocupado.
                return 0;
            }
        }
    }

    return 1;
}

int buscarIndiceReserva(Reserva *vRes, int qtd, int idBusca) {
    for(int i = 0; i < qtd; i++) {
        if(vRes[i].id_reserva == idBusca) {
            return i;
        }
    }

    return -1;
}

void cancelarReserva(Reserva *vRes, int *qRes) {
    int idBusca;
    printf("Cancelamento de reserva\n");
    printf("Digite o ID da reserva para cancelar: ");
    scanf("%d", &idBusca);

    int index = buscarIndiceReserva(vRes, *qRes, idBusca);

    if(index == -1) {
        printf("Erro: reserva nao encontrada.\n");
        system("pause");
        return;
    }

    printf("\n-----------------------------------\n");
    printf("Reserva encontrada: \n");
    printf("Data: %02d/%02d/%04d\n", vRes[index].data_reserva.dia, vRes[index].data_reserva.mes, vRes[index].data_reserva.ano);
    printf("CPF do responsavel: %s\n", vRes[index].cpf_morador);
    printf("\n-----------------------------------\n");
    printf("Tem certeza que deseja cancelar? (1 - Sim / 0 - Nao): ");

    int confirma;

    scanf("%d", &confirma);

    if(confirma != 1) {
        printf("Operacao cancelada.\n");
        return;
    }

    for(int j = index; j < *qRes - 1; j++) {
        vRes[j] = vRes[j + 1];
    }

    (*qRes)--;

    printf("Sucesso. Reserva cancelada e removida do sistema.\n");
    system("pause");
}

void gerarRelatorioReservas(Reserva *vRes, int qRes, Morador *vMor, int qMor, AreaComum *vArea, int qArea) {
    int opcao;
    printf("\n=== RELATORIO DE RESERVAS ===\n");
    printf("Filtrar por: \n");
    printf("1. CPF do Morador\n");
    printf("2. ID da Area Comum\n");
    printf("3. Mes de Referencia\n");
    printf("4. Listar Tudo (Sem filtro)\n");
    printf("0. Voltar ao menu principal\n");
    printf(">> ");
    scanf("%d", &opcao);

    if(opcao == 0) {
        return;
    }

    char cpfBusca[15];
    int idAreaBusca = 0;
    int mesBusca = 0;

    if(opcao == 1) {
        printf("Digite o CPF: ");
        scanf(" %[^\n]", cpfBusca);
    } else if(opcao == 2) {
        printf("Digite o ID da Area: ");
        scanf("%d", &idAreaBusca);
    } else if(opcao == 3) {
        printf("Digite o mes (1-12) ");
        scanf("%d", &mesBusca);
    }

    int salvarArquivo = 0;
    printf("Deseja salvar em arquivo texto? (1 - Sim, 0 - Nao) ");
    scanf("%d", &salvarArquivo);

    FILE *saida;

    if(salvarArquivo) {
        saida = fopen("relatorio_reservas.txt", "w");

        if(!saida) {
            printf("Erro ao criar arquivo. Exibindo na tela. \n");
            saida = stdout;
        } else {
            printf("Gerando arquivo 'relatorio_reservas.txt'...\n");
        }
    } else {
        saida = stdout;
    }

    fprintf(saida, "-----------------------------------------------------------------------\n");
    fprintf(saida, "| ID    | DATA       | AREA                 | RESPONSAVEL           \n");
    fprintf(saida, "-----------------------------------------------------------------------\n");

    int encontrados = 0;

    for(int i = 0; i < qRes; i++) {
        int mostrar = 0;

        switch(opcao) {
            case 1:
                if(strcmp(vRes[i].cpf_morador, cpfBusca) == 0) mostrar = 1;
                break;
            case 2:
                if(vRes[i].id_area == idAreaBusca) mostrar = 1;
                break;
            case 3:
                if(vRes[i].data_reserva.mes == mesBusca) mostrar = 1;
                break;
            case 4:
                // Mostrar tudo
                mostrar = 1;
                break;
        }

        if(mostrar) {
            Morador *morador = NULL;
            AreaComum *area = NULL;

            int indexM = buscarIndiceMorador(vMor, qMor, vRes[i].cpf_morador);
            if(indexM != -1) morador = &vMor[indexM];

            int indexA = buscarIndiceArea(vArea, qArea, vRes[i].id_area);
            if(indexM != -1) area = &vArea[indexA]; 
            
            imprimirLinhaReserva(saida, vRes[i], morador, area);
            encontrados++;
        }
    }

    fprintf(saida, "-----------------------------------------------------------------------\n");
    fprintf(saida, "Total de registros encontrados: %d\n", encontrados);

    if(salvarArquivo) {
        fclose(saida);
        printf("Relatorio salvo com sucesso.\n");
    }
}

void listarReservasDetalhadas(Reserva *vRes, int qRes, Morador *vMor, int qMor, AreaComum *vArea, int qArea) {
    printf("Relatorio Detalhado de Reservas\n");

    printf("%-5s | %-10s | %-20s | %-20s\n", "ID", "DATA", "AREA", "RESPONSAVEL");
    printf("--------------------------------------------------------------\n");

    for(int i = 0; i < qRes; i++) {
        char nomeArea[50] = "---";
        int indexArea = buscarIndiceArea(vArea, qArea, vRes[i].id_area);
        if(indexArea != -1) {
            strcpy(nomeArea, vArea[indexArea].nome);
        }

        char nomeMorador[100] = "---";
        int indexMorador = buscarIndiceMorador(vMor, qMor, vRes[i].cpf_morador);
        if(indexMorador != -1) {
            strcpy(nomeMorador, vMor[indexMorador].nome);
        }

        printf("%-5d | %02d/%02d/%04d | %-20s | %-20s\n", 
            vRes[i].id_reserva, 
            vRes[i].data_reserva.dia, vRes[i].data_reserva.mes, vRes[i].data_reserva.ano,
            nomeArea,
            nomeMorador
        );
    }

    printf("--------------------------------------------------------------\n");
}

Reserva* carregarReservas(int *qtd, int *tam) {
    FILE *f = fopen(ARQUIVO_RESERVAS, "rb");

    if(!f) {
        *qtd = 0;
        *tam = 5;

        return (Reserva *) calloc(*tam, sizeof(Reserva));
    }

    fseek(f, 0, SEEK_END);
    long tamanhoArquivo = ftell(f);
    rewind(f);

    int totalReservas = tamanhoArquivo / sizeof(Reserva);

    *qtd = totalReservas;
    *tam = totalReservas + 5;

    Reserva *v = (Reserva *) calloc(*tam, sizeof(Reserva));

    if(totalReservas > 0) {
        fread(v, sizeof(Reserva), totalReservas, f);
    }

    fclose(f);

    printf("Reservas carregadas com sucesso.\n");

    return v;
}

void salvarReservas(Reserva *vetor, int qtd) {
    FILE *f = fopen(ARQUIVO_RESERVAS, "wb");

    if(f) {
        fwrite(vetor, sizeof(Reserva), qtd, f);
        fclose(f);
    }
}

Reserva* cadastrarReserva(
    Reserva *vRes, int *qRes, int *tRes,
    Morador *vMor, int qMor,
    AreaComum *vArea, int qArea
) {

    if(*qRes >= *tRes) {
        *tRes += 5;
        vRes = (Reserva*) realloc(vRes, sizeof(Reserva) * *tRes);
    }

    printf("Nova reserva\n");

    char cpfTemp[15];

    printf("Digite o CPF do morador: ");
    scanf(" %[^\n]", cpfTemp);

    int indexMorador = buscarIndiceMorador(vMor, qMor, cpfTemp);

    if(indexMorador == -1) {
        printf("Erro: morador nao encontrado.\n");
        return vRes;
    }
    
    printf(">> Morador identificado: %s\n", vMor[indexMorador].nome);

    int idAreaTemp;

    printf("Digite o ID da area desejada: ");
    scanf("%d", &idAreaTemp);

    int indexArea = buscarIndiceArea(vArea, qArea, idAreaTemp);

    if(indexArea == -1) {
        printf("Erro: area nao encontrada.\n");
        return vRes;
    }

    printf(">> Area identificada: %s\n", vArea[indexArea].nome);

    // Pós Verificação

    int novoId = gerarIdReserva(vRes, *qRes);
    vRes[*qRes].id_reserva = novoId;
    strcpy(vRes[*qRes].cpf_morador, cpfTemp);
    vRes[*qRes].id_area = idAreaTemp;

    printf("Data da reserva (dia mes ano): ");

    int dia, mes, ano;
    scanf("%d %d %d", &dia, &mes, &ano);

    if(!verificarDataValida(dia, mes, ano)) {
        printf("\nErro: Data invalida. Verifique o dia/mes/ano.\n");
        return vRes;
    }

    if(verificarDisponibilidade(vRes, *qRes, idAreaTemp, dia, mes, ano) == 0) {
        printf("Erro: Esta area ja esta reservada para essa data.\n");
        printf("Por favor, escolha outro dia ou outra area.\n");
        return vRes;
    }

    vRes[*qRes].data_reserva.dia = dia;
    vRes[*qRes].data_reserva.mes = mes;
    vRes[*qRes].data_reserva.ano = ano;

    (*qRes)++;
    printf("Reserva #%d confirmada com sucesso!\n", novoId);

    return vRes;
}

void listarReservas(Reserva *vRes, int qRes) {
    printf("Lista de reservas\n");

    for(int i = 0; i < qRes; i++) {
        printf("Reserva #%d | CPF: %s | Area ID: %d | Data: %02d/%02d/%d\n", vRes[i].id_reserva, vRes[i].cpf_morador, vRes[i].id_area, vRes[i].data_reserva.dia, vRes[i].data_reserva.mes, vRes[i].data_reserva.ano);
    }
}

void moduloReservas(
    Reserva **vRes, int *qRes, int *tRes,
    Morador *vMor, int qMor,
    AreaComum *vArea, int qArea
) {

    int opcao;

    do {
        system("cls");
        printf("GESTAO DE RESERVAS\n");
        printf("1. Nova reserva\n");
        printf("2. Listar reservas\n");
        printf("3. Cancelar Reserva\n");
        printf("0. Voltar ao menu principal\n");
        printf(">> ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                *vRes = cadastrarReserva(*vRes, qRes, tRes, vMor, qMor, vArea, qArea);
                system("pause");
                break;
            case 2:
                listarReservas(*vRes, *qRes);
                system("pause");
                break;
            case 3:
                cancelarReserva(*vRes, qRes);
                break;
            case 0:
                printf("Voltando...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while(opcao != 0);
}