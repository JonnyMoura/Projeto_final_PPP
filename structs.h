//
// Created by joanm on 05/06/2021.
//

#ifndef PROJ3F1_STRUCTS_H
#define PROJ3F1_STRUCTS_H
#define MAXNOME 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
////LISTA

struct no_fila {
    long posicao;
    struct no_fila * pseg;
};
struct lista{
    struct no_fila *raiz;
};

/////ARVORE
struct palavra {
    char pal[MAXNOME + 1];
    struct lista *lista_num;

};
struct no {
    struct palavra word;
    struct no *left;
    struct no *right;

};

struct arvore_binaria {
    struct no * raiz;
};


void inicializar_arvore(struct arvore_binaria * pa);
bool colocar(struct arvore_binaria * pa, const char *pal, char *pos);
void procura_contexto(struct no *pa,const char *pp);
void procura_letras(struct no *pa,const char *gp);

#endif //PROJ3F1_STRUCTS_H
