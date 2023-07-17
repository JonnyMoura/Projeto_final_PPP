//
// Created by joanm on 05/06/2021.
//
#include "structs.h"
#include "aux_func.h"
void inicializar_arvore(struct arvore_binaria * pa) {// deve ser sempre chamada antes da árvore ser usada
    pa->raiz = NULL;
}


static struct no *addtree(struct no *, struct no *,char*pos);
static bool colocar_lista(struct lista *pf,char *pos);

bool colocar(struct arvore_binaria * pa, const char *pal, char*pos) {//coloca um novo nó na lista com a palvra e a lista de posiçoes
    struct no *p;
    p = (struct no *) malloc(sizeof(struct no)); // make a new node
    if (p == NULL)
        return false;
    strcpy(p->word.pal,pal);
    struct lista *l;
    l=(struct lista *) malloc(sizeof(struct lista));

    p->word.lista_num=l;
    p->word.lista_num->raiz=NULL;//raiz da lista associada a cada no da arvore(posiçoes de cada palavra)
    p->left = p->right = NULL;
    pa->raiz = addtree(pa->raiz, p,pos);
    return true;
}

struct no *addtree(struct no *pr, struct no *p,char *pos) {
    char pal1[MAXNOME+1],pal2[MAXNOME+1];
    if (pr == NULL) {
        pr = p;
        colocar_lista(pr->word.lista_num,pos);//caso esteja a adicionar um nó folha vai colocar a posição da primeira ocorrência da nova palavra
    } else {
        strcpy(pal1,p->word.pal);
        strcpy(pal2,pr->word.pal);
        if ((strcmp(pal1,pal2))==0) {//caso a palavra do nó atual e do nó a adicionar sejam iguais adiciona a posição da ocorrência ao nó atual
            colocar_lista(pr->word.lista_num,pos);
            return pr;}
        if ((strcmp(pal1,pal2)) > 0) {//caso nao sejam iguais ou nao atinja o nó folha continua a procurar na árvore
            pr->right = addtree(pr->right, p,pos);

        } else if (strcmp(pal1,pal2) < 0)
            pr->left = addtree(pr->left, p,pos);
    }
    return pr;
}



/////////lista
static bool colocar_lista(struct lista *pf,char *pos) {
    struct no_fila * aux, * prox, * anterior;

    //Obter espaço para um novo nó
    aux = (struct no_fila *) malloc(sizeof(struct no_fila));
    if (aux == NULL)
        //não há espaço
        return false;

    //construir novo nó da fila

    char *ptr;
    aux->posicao=strtol(pos,&ptr,10);
    aux->pseg = NULL;

    //Procurar a posição onde o número deve ficar
    if (pf->raiz == NULL) {
        // fila vazia, é a primeira posição
        pf->raiz = aux;
    } else {
        // fila contém mensagens
        if (pf->raiz->posicao<aux->posicao) {
            // inserir à entrada da lista
            aux->pseg = pf->raiz;
            pf->raiz = aux;
        } else {
            // procurar posição de inserção
            anterior = pf->raiz;
            prox = pf->raiz->pseg;
            while (prox != NULL && (prox->posicao)>=(aux->posicao)) {
                anterior = prox;
                prox = prox->pseg;
            }
            if (prox == NULL) {
                // inserir à saída da lista
                anterior->pseg = aux;
            } else {
                // inserir a meio da lista
                anterior->pseg = aux;
                aux->pseg = prox;
            }
        }
    }
    return true;
}


void procura_letras(struct no *pa,const char *gp) {
    if (pa== NULL) { //termina as chamadas recursivas, não há mais nada neste ramo
        return;
    } else {
        procura_letras(pa->left,gp);//vai imprimir o lado esquerdo da árvore(caso a primeira letra da palavra do nó esteja dentro da gama de letras ou seja igual à letra inseridas pelo utilizador
        if((pa->word.pal[0]>=gp[0] && pa->word.pal[0]<=gp[1]) || pa->word.pal[0]==gp[0]){
            struct no_fila *p=pa->word.lista_num->raiz;
            while(p!=NULL){
                printf("%s=%ld\n",pa->word.pal,p->posicao);
                p=p->pseg;
            }}
        procura_letras(pa->right,gp); //vai imprimir o lado direito da árvore(nas mesmas condições de anteriormente)
    }
}

void procura_contexto(struct no *pa,const char *pp){
    if (pa== NULL) { //termina as chamadas recursivas, não há mais nada neste ramo
        return;
    } else {//caso a palavra do nó seja igual à inserida pelo utilizador procura o contexto de cada ocorrência da mesma
        procura_contexto(pa->left,pp);
        if(strcmp(pa->word.pal,pp)==0){
            struct no_fila *p=pa->word.lista_num->raiz;
            while(p!=NULL){
                contexto(p->posicao);//função utilizada para procurar contexto
                p=p->pseg;
            }

        }
        procura_contexto(pa->right,pp);
    }
}