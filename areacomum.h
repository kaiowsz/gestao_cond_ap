#ifndef AREACOMUM_H
#define AREACOMUM_H

typedef struct {
    int id_area;
    char nome[50];
    int capacidade_max;
    float taxa_limpeza;
} AreaComum;

void moduloAreas(AreaComum **vetor, int *qtd, int *tam);

int buscarIndiceArea(AreaComum *vetor, int qtd, int idBusca);
AreaComum* carregarAreas(int *qtd, int *tam);
void salvarAreas(AreaComum *vetor, int qtd);
AreaComum* cadastrarArea(AreaComum *vetor, int *qtd, int *tam);
void listarAreas(AreaComum *vetor, int qtd);
void alterarArea(AreaComum *vetor, int qtd);
void removerArea(AreaComum *vetor, int *qtd);

#endif