//
// Created by joanm on 19/05/2021.
//

#ifndef PROJ3F1_AUX_FUNC_H
#define PROJ3F1_AUX_FUNC_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <locale.h>
#include "structs.h"

#define MAXNOME 100

void ler_nome(char *ptexto);
void criar_fich(char *ptexto);
void ler_pares(struct arvore_binaria * pa);
void contexto(long pos);
void menu(struct no *pa);
#endif //PROJ3F1_AUX_FUNC_H
