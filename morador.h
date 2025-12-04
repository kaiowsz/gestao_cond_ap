#ifndef MORADOR_H
#define MORADOR_H

#include <stdio.h>

struct Reserva;

typedef struct {
    char nome[100];
    char cpf[15];
    char bloco[5];
    char telefone[15];
    int apartamento;
} Morador;

Morador* carregarMoradores(int *qtd, int *tam);
void salvarMoradores(Morador *vetor, int qtd);
Morador* cadastrarMorador(Morador *vetor, int *qtd, int *tam);
void listarMoradores(Morador *vetor, int qtd);
int buscarIndiceMorador(Morador *vetor, int qtd, char *cpfBusca);
void removerMorador(Morador *vetor, int *qtd, struct Reserva *vRes, int qRes);
void alterarMorador(Morador *vetor, int qtd);
int validarFormatoCPF(char *cpf);
void moduloMoradores(Morador **vetor, int *qtd, int *tam, struct Reserva *vRes, int qRes);

#endif