#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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
    FILE *indice = fopen(entradas, "r");
    fseek(indice, 0, SEEK_END);
    long posicao = ftell(indice);
    rewind(indice);
    long quantidade = posicao/sizeof(Entrada);
        if (quantidade == 0){
            fclose(indice);
            remove(entradas);
            i--;
        } else if (quantidade == 1){
            fclose(indice);
            char ordenados[20];
            sprintf(ordenados, "ordenados_%d.bin", i);
            rename(entradas, ordenados);
        } else {
            e = (Entrada*) malloc(posicao);
            fread(e, sizeof(Entrada), quantidade, indice);
            qsort(e, quantidade, sizeof(Entrada), compara);
            char ordenados[20];
            sprintf(ordenados, "ordenados_%d.bin", i);
            FILE *ordem = fopen(ordenados, "wb");
            fwrite(e, sizeof(Entrada), quantidade, ordem);
            free(e);
            fclose(ordem);
            fclose(indice);
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
    
        fread(&ea,sizeof(Entrada),1,a);
        fread(&eb,sizeof(Entrada),1,b);
    
        while (!feof(a) && !feof(b)){
            if (compara(&ea, &eb) < 0) {
                fwrite(&ea, sizeof(Entrada), 1, saida);
                fread(&ea, sizeof(Entrada), 1, a);
            } else {
                fwrite(&eb, sizeof(Entrada), 1, saida);
                fread(&eb, sizeof(Entrada), 1, b);
            }
        }
        while (!feof(a)){
            fwrite(&a, sizeof(Entrada), 1, saida);
            fread(&ea, sizeof(Entrada), 1, a);
        }
        while (!feof(b)){
            fwrite(&eb, sizeof(Entrada), 1,saida);
            fread(&eb, sizeof(Entrada), 1, b);
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
    FILE *indice = fopen(entradas, "wb");
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
                fwrite(&e, sizeof(Entrada), 1, indice);
                if (n%REGISTROS == 0){
                    fclose(indice);
                    ordena (i);
                    remove(entradas);                 
                    i++;
                    sprintf(entradas, "entradas_%d.bin", i);
                    indice = fopen(entradas, "wb");
                }
            }
            coluna++;
            campo = strtok(NULL,"\t");
        }
        posicao = ftell(bolsa);
        n++;
    }
    fclose(indice);
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

void ordenaIndice(){
	Entrada *e;
	FILE *indice, *indiceOrdenado;
	indice = fopen("indice.bin", "r");
    fseek(indice, 0, SEEK_END);
    long posicao = ftell(indice);
    rewind(indice);
    long quantidade = posicao/sizeof(Entrada);
    e = (Entrada*) malloc(posicao);
    fread(e, sizeof(Entrada), quantidade, indice);
    qsort(e, quantidade, sizeof(Entrada), compara);
    FILE *indiceOrdenado = fopen("indiceOrdenado", "wb");
    fwrite(e, sizeof(Entrada), quantidade, indiceOrdenado);
    free(e);
    fclose(indiceOrdenado);
    fclose(indice);
}

void buscaBinaria(){
	, *amostra
}

void criaAmostra(){
	FILE *indice = fopen("indice.bin", "r");
	FILE *amostra = fopen("amostra.bin", "wb");
	for (int i = 0; i < 1000; i++){
        fseek(indice, (rand()%(n-1))*sizeof(Entrada), SEEK_SET);
        fread(&e, sizeof(Entrada), 1, indice);
        fwrite(&e, sizeof(Entrada), 1, amostra);
    }
	fclose(amostra);
	fclose(indice);
}

void criaIndice(){
    char linha[2048];
    Entrada e;
    long n = 1;
    int coluna = 0;
    char* campo;
    FILE *bolsa = fopen("bolsa.csv", "r");
    FILE *indice = fopen("indice", "wb");
    fgets(linha, 2048, bolsa);
	fgets(linha, 2048, bolsa);
    long posicao = ftell(bolsa);
    while (!feof(bolsa))){
        fgets(linha, 2048, bolsa;
        coluna = 0;
        campo = strtok(linha,"\t");
        while (campo) {
            if (coluna == 7) {
                strcpy(e.nis, campo);
                e.ponteiro = posicao;
                fwrite(&e, sizeof(Entrada), 1, indice);
            }
            coluna++;
            campo = strtok(NULL,"\t");
        }
        posicao = ftell(bolsa);
        n++;
    }
	fclose(indice);
	fclose(bolsa);
}

int main(int argc, char** argv){
	criaIndice();
    return 0;
}