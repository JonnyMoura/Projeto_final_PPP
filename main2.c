//
// Created by joanm on 05/06/2021.
//

#include "aux_func.h"

int main() {
    struct arvore_binaria arvore;
    inicializar_arvore(&arvore);
    ler_pares(&arvore);
    menu(arvore.raiz);







    return 0;
}
