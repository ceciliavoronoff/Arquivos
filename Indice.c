// Escolhi fazer com a limitação de memória para pensar em maneiras de trabalhar com arquivos muito grandes,
// que não poderiam ser processados de uma vez.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define REGISTROS 500000

typedef struct Entrada {
    char nis[14];
    long ponteiro;
} Entrada;

int compara(const void *e1, const void *e2){
    return strncmp(((Entrada*)e1)->nis, ((Entrada*)e2)->nis, 14);
}

void ordena(int i){
    Entrada *e;
    char entradas[20];
    sprintf(entradas, "entradas_%d.bin", i);
    FILE *coleta = fopen(entradas, "r");
    fseek(coleta, 0, SEEK_END);
    long posicao = ftell(coleta);
    rewind(coleta);
    long quantidade = posicao/sizeof(Entrada);
        if (quantidade == 0){
            fclose(coleta);
            remove(entradas);
        } else if (quantidade == 1){
            fclose(coleta);
            char ordenados[20];
            sprintf(ordenados, "ordenados_%d.bin", i);
            rename(entradas, ordenados);
        } else {
            e = (Entrada*) malloc(posicao);
            fread(e, sizeof(Entrada), quantidade, coleta);
            qsort(e, quantidade, sizeof(Entrada), compara);
            char ordenados[20];
            sprintf(ordenados, "ordenados_%d.bin", i);
            FILE *ordem = fopen(ordenados, "wb");
            fwrite(e, sizeof(Entrada), quantidade, ordem);
            free(e);
            fclose(ordem);
            fclose(coleta);
        }
    }
void intercala(int i){
    FILE *a, *b, *saida;
    Entrada ea, eb;
    int j = 0;
    char primeiro[20];
    char segundo[20];
    char retorno[20];
        
    while ((j+1) < i){
        sprintf(primeiro, "ordenados_%d.bin", j);
        j++;
        sprintf(segundo, "ordenados_%d.bin", j);
        sprintf(retorno, "ordenados_%d.bin", i);
 
        a = fopen(primeiro,"r");
        b = fopen(segundo,"r");
        saida = fopen(retorno,"w");
    
        fseek(a, 0, SEEK_END);
        long quantidade = ftell(a) / sizeof(Entrada);
        rewind(a);
        fseek(b, 0, SEEK_END);
        quantidade += ftell(b) / sizeof(Entrada);
        rewind(b);
        fread(&ea,sizeof(Entrada),1,a);
        fread(&eb,sizeof(Entrada),1,b);
    
        for (int k = 0; k <= quantidade; k++){
            if (feof(a) && feof(b)){
                break;
            } else if (feof(a)){
                fwrite(&eb, sizeof(Entrada), 1,saida);
                fread(&eb, sizeof(Entrada), 1, b);        
            } else if (feof(b)){
                fwrite(&a, sizeof(Entrada), 1, saida);
                fread(&ea, sizeof(Entrada), 1, a);
            } else {
                if (compara(&ea, &eb) < 0) {
                    fwrite(&ea, sizeof(Entrada), 1, saida);
                    fread(&ea, sizeof(Entrada), 1, a);
                } else {
                    fwrite(&eb, sizeof(Entrada), 1, saida);
                    fread(&eb, sizeof(Entrada), 1, b);
                }   
            }
        }
        fclose(a);
        remove(primeiro);
        fclose(b);
        remove(segundo);
        fclose(saida);
        j++;
        i++;
    }
sprintf(retorno, "ordenados_%d.bin", j);
rename(retorno, "indice.bin");
}

void criaIndice(){
    char linha[2048];
    Entrada e;
    long n = 1;
    int coluna = 0;
    char* campo;
    int i = 0;
    FILE *bolsa = fopen("bolsa.csv", "r");
    fgets(linha, 2048, bolsa);
    char entradas[20];
    sprintf(entradas, "entradas_%d.bin", i);
    FILE *coleta = fopen(entradas, "wb");
    long posicao = ftell(bolsa);
    while (fgets(linha, 2048, bolsa)){
        if (feof(bolsa))
            break;
        coluna = 0;
        campo = strtok(linha,"\t");
        while (campo) {
            if (coluna == 7) {
                strcpy(e.nis, campo);
                e.ponteiro = posicao;
                fwrite(&e, sizeof(Entrada), 1, coleta);
                if (n%REGISTROS == 0){
                    fclose(coleta);
                    ordena (i);
                    remove(entradas);                 
                    i++;
                    sprintf(entradas, "entradas_%d.bin", i);
                    coleta = fopen(entradas, "wb");
                }
            }
            coluna++;
            campo = strtok(NULL,"\t");
        }
        posicao = ftell(bolsa);
        n++;
    }
    fclose(coleta);
    ordena(i);
    remove(entradas);
    intercala(i+1);
}

long buscaIndice(FILE *indice, char pesquisa[14]){
    Entrada e;
    long inicio = 0, meio, fim, encontrado = 0;
    fseek(indice, 0, SEEK_END);
    fim = (ftell(indice)/sizeof(Entrada))-1;
    while (inicio <= fim) {
        meio = (inicio + fim)/2;
        fseek(indice, meio * sizeof(Entrada), SEEK_SET);
        fread(&e, sizeof(Entrada), 1, indice);
        if (strncmp(pesquisa, e.nis, 14) == 0){
            encontrado++;
            return e.ponteiro;
        } else if (strncmp(pesquisa, e.nis, 14) < 0)
            fim = meio - 1;
        else
            inicio = meio + 1;
    }
    return encontrado;
}

void buscaBolsa(long posicao){
    FILE *bolsa = fopen("bolsa.csv", "r");
    Entrada e;
    char linha[2048];
    char* campo;
    fseek(bolsa, posicao, SEEK_SET);
    fgets(linha, 2048, bolsa);
    campo = strtok(linha,"\t");
    printf("\n\n");
    printf ("UF: %s\n", campo);
    campo = strtok(NULL, "\t");
    printf ("Código SIAFI: %s\n", campo);
    campo = strtok(NULL, "\t");
    printf ("Município: %s\n", campo);
    campo = strtok(NULL, "\t");
    printf ("Código Função: %s\n", campo);
    campo = strtok(NULL, "\t");
    printf ("Código Subfunção: %s\n", campo);
    campo = strtok(NULL, "\t");
    printf ("Código Programa: %s\n", campo);
    campo = strtok(NULL, "\t");
    printf ("Código Ação: %s\n", campo);
    campo = strtok(NULL, "\t");
    printf ("NIS Favorecido: %s\n", campo);
    campo = strtok(NULL, "\t");
    printf ("Nome Favorecido: %s\n", campo);
    campo = strtok(NULL, "\t");
    printf ("Fonte-Finalidade: %s\n", campo);
    campo = strtok(NULL, "\t");
    printf ("Valor Parcela: %s\n", campo);
    campo = strtok(NULL, "\t");
    printf ("Mês Competência: %s\n", campo);
    campo = strtok(NULL, "\t");
    fclose(bolsa);
}

void busca(){
    FILE *indice;
    printf("Digite o NIS desejado: \n");
    char pesquisa[14]; 
    scanf("%s", pesquisa);
    indice = fopen("indice.bin", "r");
    if (indice == NULL){
        criaIndice();
        indice = fopen("indice.bin", "r");
    }
    long posicao = buscaIndice(indice, pesquisa);
    fclose(indice);
    if (posicao == 0){
        printf("NIS não cadastrado");
    } else {
        buscaBolsa(posicao);
    }
}

int main(int argc, char** argv){
    busca();
    return 0;
}
