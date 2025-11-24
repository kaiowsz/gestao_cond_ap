#ifndef RESERVA_H
#define RESERVA_H

#include "morador.h"
#include "areacomum.h"

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int id_reserva;
    char cpf_morador[15];
    int id_area;
    Data data_reserva;
} Reserva;

void moduloReservas(Reserva **vRes, int *qRes, int *tRes,
    Morador *vMor, int qMor,
    AreaComum *vArea, int qArea
);

Reserva* carregarReservas(int *qtd, int *tam);
void salvarReservas(Reserva *vetor, int qtd);

Reserva cadastrarReserva(Reserva *vRes, int *qRes, int *tRes,
    Morador *vMor, int qMor,
    AreaComum *vArea, int qArea
);

void listarReservas(Reserva *vRes, int qRes);

#endif