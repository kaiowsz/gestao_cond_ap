#ifndef RESERVA_H
#define RESERVA_H

#include "morador.h"
#include "areacomum.h"

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct Reserva {
    int id_reserva;
    char cpf_morador[15];
    int id_area;
    Data data_reserva;
} Reserva;

void gerarRelatorioReservas(Reserva *vRes, int qRes, Morador *vMor, int qMor, AreaComum *vArea, int qArea);

void moduloReservas(Reserva **vRes, int *qRes, int *tRes,
    Morador *vMor, int qMor,
    AreaComum *vArea, int qArea
);

int moradorTemReserva(Reserva *vRes, int qRes, char *cpf);

int verificarDisponibilidade(Reserva *vRes, int qtd, int idArea, int dia, int mes, int ano);

Reserva* carregarReservas(int *qtd, int *tam);
void salvarReservas(Reserva *vetor, int qtd);

Reserva* cadastrarReserva(Reserva *vRes, int *qRes, int *tRes,
    Morador *vMor, int qMor,
    AreaComum *vArea, int qArea
);

void listarReservas(Reserva *vRes, int qRes);

void cancelarReserva(Reserva *vRes, int *qRes);

void listarReservasDetalhadas(Reserva *vRes, int qRes, Morador *vMor, int qMor, AreaComum *vArea, int qArea);

#endif