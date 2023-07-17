//
// Created by joanm on 19/05/2021.
//

#include "aux_func.h"
static char acentuadas[][8] =
        {"á", "Á", "à", "À", "ã", "Ã", "â", "Â", "ä", "Ä", "ç", "Ç",
         "é", "É", "è", "È", "ê", "Ê", "í", "Í", "ì", "Ì", "î", "Î",
         "ó", "Ó", "ò", "Ò", "õ", "Õ", "ô", "Ô", "ö", "Ö", "ú", "Ú",
         "ù", "Ù", "û", "Û", "\0"};
static char base[] =
        {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'c', 'c',
         'e', 'e', 'e', 'e', 'e', 'e', 'i', 'i', 'i', 'i', 'i', 'i',
         'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'u', 'u',
         'u', 'u', 'u', 'u', '\0'};

static int comp_mchar(const char *um, const char *outro) {
    int i = 0;
    while (um[i] != 0) {
        if (um[i] != outro[i])
            return 0;
        i++;
    }
    return i;
}

static void strtobase_u8(char *dest, const char *orig) {
    int i = 0, j = 0, c;
    // percorrer a string origem
    c = (unsigned char) orig[0];
    while (c != 0) {
        if ((c & 0x80) == 0) {
            // é um carater ASCII, basta converter as letras para minusculas
            if (c <= 'Z' && c >= 'A')
                c = c + 'a' - 'A';
            i++;
            dest[j] = (char) c;
            j++;
        } else {
            // é um carater UTF-8 com mais de um byte
            int k = 0, tc = 0;
            while (acentuadas[k][0] != 0) {
                // se for uma das letras acentuadas, retira-se o acento
                if ((tc = comp_mchar(&acentuadas[k][0], &orig[i])) != 0) {
                    // encontramos uma minúscula acentuada
                    dest[j] = base[k];
                    j++;
                    i = i + tc;
                    break;
                }
                k++;
            }
            if (tc == 0) {
                // não é uma letra acentuada, logo é de copiar para
                //   o destino, sem alterações, todos os carateres UTF-8
                do {
                    dest[j] = orig[i];
                    i++;
                    j++;
                } while ((orig[i] & 0xC0) == 0x80);
            }
        }
        c = (unsigned char) orig[i];
    }
    dest[j] = 0;
}


static size_t strlen_u8(const char *s) {// função da biblioteca utf-8
    size_t tam = 0, i = 0;
    while (s[i] != 0) {
        if ((s[i] & 0xC0) != 0x80)
            tam++;
        i++;
    }
    return tam;
}
void ler_nome(char *ptexto){//função para retirar o nome do ficheiro de texto
    FILE *fich=NULL;
    fich=fopen("param1.txt","r");
    if(fich==NULL){
        fprintf(stderr,"Ficheiro não existe\n");
    }
    else {
        if(fscanf(fich,"%s",ptexto)!=1){
            fprintf(stderr,"Erro na leitura\n");
        }


    }
    fclose(fich);


}
static bool limpa_texto(int carater){
    if((isalpha(carater)!=0) ||  (char)carater=='-'||(carater & 0x80) != 0){//testa se o carater é uma letra, um hífen(independentemente de ser multibyte)
        return true;
    }
    else{
        return false;
    }
}


void criar_fich(char *ptexto) {
    FILE *fich = NULL;
    FILE *fich_saida = NULL;
    fich = fopen(ptexto, "r");
    if (fich == NULL) {
        fprintf(stderr, "Ficheiro não existe\n");
    }
    fich_saida = fopen("pal_texto.txt", "w");
    if (fich_saida == NULL) {
        fprintf(stderr, "Erro na criação de ficheiro\n");
    }
    int c;
    size_t pos = 0;
    char palavra[MAXNOME + 1] = "";
    while ((c = fgetc(fich) ) != EOF){
        pos+=sizeof((char)c);//posição de cada carater no ficheiro de texto
        if(limpa_texto(c) == true){
            strcat(palavra, (const char *) &c);
        }
        else{
            if(strlen_u8(palavra)>1) { //a palavra tem mais de um carater
                if(fprintf(fich_saida, "%s=%lu\n", palavra, (pos - (strlen(palavra)+1)))<0){//posição da primeira letra
                    fprintf(stderr, "Erro na colocação\n");
                }
            }
            palavra[0]=0;
        }
    }
    fclose(fich);
    fclose(fich_saida);
}

//////////parte 2



void ler_pares(struct arvore_binaria * pa){
    char nome[MAXNOME+1]="";
    printf("Introduza nome do ficheiro\n");
    scanf("%s",nome);
    FILE *fich=NULL;
    fich = fopen(nome, "r");//Utilizador introduz nome do ficheiro com os pares posição
    while (fich == NULL) {
        fprintf(stderr, "Ficheiro não existe.Introduza corretamente\n");
        scanf("%s",nome);
        fich = fopen(nome, "r");
    }
    char pal[MAXNOME+1]="";
    char pos[MAXNOME+1]="";
    char pal_strtobase[MAXNOME+1]="";
    int c;

    while((c = fgetc(fich) ) != EOF){//separa para duas strings os carateres relativos a posiçao e a palavra
        if((isalpha(c)!=0) || ((char)c=='-')||((c & 0x80) != 0)){
            strcat(pal, (const char *) &c);
        }
        if(isdigit(c)!=0){
            strcat(pos, (const char *) &c);
        }
        if((char) c=='\n'){
            strtobase_u8(pal_strtobase,pal);
            if(colocar(pa,pal_strtobase,pos)==false){
                printf("Erro na colocação\n");
            }
            pal[0]=0;
            pos[0]=0;
            pal_strtobase[0]=0;
        }
    }
    fclose(fich);
}

void contexto(long pos){
    char nome_fich[MAXNOME+1]="";
    FILE *fich_n=NULL;
    fich_n=fopen("param1.txt","r");
    if(fich_n==NULL){
        fprintf(stderr,"Ficheiro não existe\n");
    }
    else {
        if(fscanf(fich_n,"%s",nome_fich)!=1){
            fprintf(stderr,"Erro na leitura\n");
        }
    }
    fclose(fich_n);
    FILE *fich=NULL;
    fich=fopen(nome_fich,"r");
    if(fich==NULL){
        fprintf(stderr,"Ficheiro não existe\n");
    }
    int pos_t=0;
    long i=pos;
    int c;
    char pal[MAXNOME+1]="";
    char pal_t[MAXNOME+1]="";
    while(pos_t<=4){
        if(fseek(fich,i,SEEK_SET)!=0){//recua o iterador até ter atingido 5 palavras para tras(exceto caso tenha menos de 2 carateres)
            printf("Erro na leitura\n");
        }
        c=fgetc(fich);
        if((char)c==' ' || (char)c=='\n'){//string temporaria pal_t permite fazer uma verificação do tamanho da palavra
            if(strlen_u8(pal_t)>1){
                pos_t+=1;
            }
            pal_t[0]=0;
        }
        if((isalpha(c)!=0) || ((char)c=='-')||((c & 0x80) != 0)){
            strcat(pal_t,(const char *) &c);
        }
        i--;
        if(i==0){
            break;
        }
    }
    pos_t=0;
    if(fseek(fich,i+1,SEEK_SET)!=0){
        printf("Erro na leitura\n");
    }
    while((c = fgetc(fich) ) != EOF){//le e imprime seguidamente as 15 palavras de contexto(tras e frente)
        if(pos_t<=15){
            if((isalpha(c)!=0) || ((char)c=='-')||((c & 0x80) != 0)){
                strcat(pal,(const char *) &c);
            }
            if((char) c==' ' || (char)c=='\n'){
                if(strlen_u8(pal)>1) {
                    printf("%s ", pal);
                    pos_t += 1;
                }
                pal[0]=0;
            }
        }}
    printf("\n");
    fclose(fich);
}
void menu(struct no *pa){//menu que permite aos utilizadores decidir a funcionalidade a utilizar,protegido para o caso de o utilizador introduzir valor invalidos
    char opcao_str[MAXNOME+1]="";
    char *ptr;
    long int opcao=0;
    while(opcao!=3){
        printf("Selecione uma das seguintes opções.\n1-Mostrar contexto de uma palavra\n2-Procurar palavras começadas por uma letra ou gama de letras\n3-Fechar programa.\n");
        scanf("%s",opcao_str);
        opcao=strtol(opcao_str,&ptr,10);
        opcao_str[0]=0;
        while(opcao<1 || opcao>3){
            printf("Introduza uma das opções!\n");
            scanf("%str",opcao_str);
            opcao=strtol(opcao_str,&ptr,10);
            opcao_str[0]=0;
        }
        if(opcao==1){//opçao relativa à procura do contexto
            int i=0;
            char palavra[MAXNOME+1]="";
            char palavra_su8[MAXNOME+1]="";
            printf("Introduza a palavra a ser procurada\n");
            scanf("%s",palavra);
            strtobase_u8(palavra_su8,palavra);
            while(i<strlen(palavra_su8) || strlen(palavra_su8)<=1){
                if(isalpha((int)palavra_su8[i])==0 || strlen(palavra_su8)<=1){
                    printf("Introduza uma palavra válida\n");
                    palavra[0]=0;
                    palavra_su8[0]=0;
                    scanf("%s",palavra);
                    strtobase_u8(palavra_su8,palavra);
                }
                i++;

            }
            procura_contexto(pa,palavra_su8);

        }
        if(opcao==2){//opçao relativa à procura de palavras que começam por uma letra ou gama de letras
            char gama[2]="";
            char gama_su8[2]="";
            printf("Introduza letra ou gama de letras\n");
            scanf("%s",gama);
            strtobase_u8(gama_su8,gama);
            while(isalpha((int)gama_su8[0])==0 || (int)gama_su8[1]==0 && gama_su8[1]!='\0' || strlen(gama_su8)>2){
                printf("Carateres não válidos. Introuza novamente\n");
                scanf("%s",gama);
                strtobase_u8(gama_su8,gama);
            }
            procura_letras(pa,gama_su8);
        }


    }}

