#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct Entrada {
    char nis[14];
    long ponteiro;
} Entrada;

typedef struct EntradaHash {
    long ponteiroHash;
    Entrada entr;
} EntradaHash;

void criaHash(){
    long tamanhoTabela = 13000081;
    Entrada e;
    EntradaHash eh;
    FILE *indice, *tabelaHash;
    long preenchido[tamanhoTabela] = {0};
    long indiceHash, ponteiroAux;
    eh.ponteiroHash = NULL;
    eh.nis = NULL;
    eh.ponteiro = NULL;
    tabelaHash = fopen("tabelaHash.bin", "rb+");
    fwrite(aux, sizeof(EntradaHash), tamanhoTabela, tabelaHash);
    indice = fopen("indice.bin", "r");
    while (fread(e, sizeof(Entrada), 1, indice)){
        if (feof(indice))
        break;
        indiceHash = e.nis/tamanhoTabela;
        long posicao = indiceHash * sizeof(EntradaHash);
        eh.entr = e;
        if (preenchido[indiceHash] == 0){
            eh.ponteiroHash = posicao;
            fseek(tabelaHash, 0, posicao);
            fwrite(eh, sizeof(EntradaHash), 1, tabelaHash);
        } else {
            eh.ponteiroHash = preenchido[indiceHash];
            fseek(tabelaHash, 0, SEEK_END);
            ponteiroAux = ftell(tabelaHash);
            fwrite(eh, sizeof(EntradaHash), 1, tabelaHash);
            preenchido[indiceHash] = ponteiroAux;
            posicao = ponteiroAux * sizeof(EntradaHash);
            fseek(tabelaHash, 0, posicao);
            fread(eh, sizeof(EntradaHash), 1, tabelaHash);
            eh.ponteiroHash = ponteiroAux;
            fseek(tabelaHash, -sizeof(EntradaHash), SEEK_CUR);
            fwrite(eh, sizeof(EntradaHash), 1, tabelaHash);
        }
    }
    fclose(indice);
    fclose(tabelaHash);    
}

int compara(const void *e1, const void *e2){
    return strncmp(((Entrada*)e1)->nis, ((EntradaHash*)e2)->nis, 14);
}

void buscaHash(){
    Entrada e;
    EntradaHash eh;
    long indiceHash, posicao, tamanhoTabela = 13000081;;
    clock_t tInicio, tFim, tTotal;
    criaHash();
    FILE *amostra = fopen("amostra.bin", "r");
    FILE *tabelaHash = fopen("tabelaHash.bin", "r");
    tInicio = clock();
    for (int i = 0; i < 1000; i++){
        fread(&e, sizeof(Entrada), 1, amostra);
        indiceHash = e.nis/tamanhoTabela;
        fseek(tabelaHash, 0, indiceHash);
        fread(&eh, sizeof(EntradaHash), 1, tabelaHash);
        if (strncmp(e.nis, eh.nis, 14) == 0){
            break;
        } else {
            
        }
        }
    }
    tFim = clock();
    tTotal = tFim - tInicio;
    printf("Tempo da Busca Binária: %fs\n", (double)tTotal/CLOCKS_PER_SEC);
}

int compara(const void *e1, const void *e2){
    return strncmp(((Entrada*)e1)->nis, ((Entrada*)e2)->nis, 14);
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
    indiceOrdenado = fopen("indiceOrdenado", "wb");
    fwrite(e, sizeof(Entrada), quantidade, indiceOrdenado);
    free(e);
    fclose(indiceOrdenado);
    fclose(indice);
}

void buscaBinaria(){
	Entrada e, a;
    long inicio = 0, meio, fim, final;
    clock_t tInicio, tFim, tTotal;
    ordenaIndice();
    FILE *indiceOrdenado = fopen("indiceOrdenado.bin", "r");
    fseek(indiceOrdenado, 0, SEEK_END);
    final = (ftell(indiceOrdenado)/sizeof(Entrada))-1;
    FILE *amostra = fopen("amostra.bin", "r");
    tInicio = clock();
    for (int i = 0; i < 1000; i++){
        fread(&a, sizeof(Entrada), 1, amostra);
        fim = final;
        while (inicio <= fim) {
            meio = (inicio + fim)/2;
            fseek(indiceOrdenado, meio * sizeof(Entrada), SEEK_SET);
            fread(&e, sizeof(Entrada), 1, indiceOrdenado);
            if (strncmp(a.nis, e.nis, 14) == 0)
                break;
        }
    }
    tFim = clock();
    tTotal = tFim - tInicio;
    printf("Tempo da Busca Binária: %fs\n", (double)tTotal/CLOCKS_PER_SEC);
}

void criaAmostra(){
	Entrada e;
    FILE *indice = fopen("indice.bin", "rb");
    fseek(indice, 0, SEEK_END);
    long quantidade = ftell(indice)/sizeof(Entrada);
    rewind(indice);
    FILE *amostra = fopen("amostra.bin", "wb");
	for (int i = 0; i < 1000; i++){
        fseek(indice, (rand()%quantidade)*sizeof(Entrada), SEEK_SET);
        fread(&e, sizeof(Entrada), 1, indice);
        fwrite(&e, sizeof(Entrada), 1, amostra);
    }
	fclose(amostra);
	fclose(indice);
}

void criaIndice(){
    char linha[2048];
    Entrada e;
    int coluna = 0;
    char* campo;
    FILE *bolsa = fopen("bolsa.csv", "r");
    FILE *indice = fopen("indice.bin", "wb");
    fgets(linha, 2048, bolsa);
    fgets(linha, 2048, bolsa);
    long posicao = ftell(bolsa);
    while (!feof(bolsa)){
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
        fgets(linha, 2048, bolsa);
        posicao = ftell(bolsa);
    }
	fclose(indice);
	fclose(bolsa);
}

int main(int argc, char** argv){
	criaIndice();
    criaAmostra();
    buscaBinaria();
    criaHash();
    return 0;
}