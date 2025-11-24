#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#include "morador.h"
#include "areacomum.h"
#include "reserva.h"

int main() {

    setlocale(LC_ALL, "Portuguese");

    Morador *moradores = NULL;
    int qtdMoradores = 0, tamMoradores = 0;

    AreaComum *areas = NULL;
    int qtdAreas = 0, tamAreas = 0;

    Morador *reservas = NULL;
    int qtdReservas = 0, tamReservas = 0;

    printf("Carregando sistema...\n");

    moradores = carregarMoradores(&qtdMoradores, &tamMoradores);
    areas = carregarAreas(&qtdAreas, &tamAreas);

    int opcao = 0;
    
    do {
        system("cls");
        printf("GESTÃO CONDOMINIAL - BEM VINDO\n");
        printf("1. Módulo Moradores\n");
        printf("2. Módulo Áreas Comuns\n");
        printf("3. Módulo Reservas\n");
        printf("4. Relatórios\n");
        printf("0. Sair e Salvar\n");
        printf(">> ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Entrando no módulo de moradores...\n");
                moduloMoradores(&moradores, &qtdMoradores, &tamMoradores);
                break;
            case 2:
                printf("Entrando no módulo de áreas...\n");
                moduloAreas(&areas, &qtdAreas, &tamAreas);
                break;
            case 3:
                printf("Entrando no módulo de reservas...\n");
                break;
            case 4:
                printf("Entrando nos relatórios...\n");
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida.\n");
                system("pause");
        }
    } while(opcao != 0);

    printf("Salvando os dados...\n");
    salvarMoradores(moradores, qtdMoradores);
    salvarAreas(areas, qtdAreas);

    if(moradores) free(moradores);
    if(areas) free(areas);
    if(reservas) free(reservas);

    return 0;
}