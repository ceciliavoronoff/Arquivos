#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//#define TAMANHOTABELA 13000081
#define TAMANHOTABELA 13

typedef struct _EntradaHash EntradaHash;

struct _EntradaHash {
    long long nisNumerico;
    long ponteiroOriginal;
    long ponteiroHash;
};

void abreHash(){
    int i;
    EntradaHash eh;
    FILE *tabelaHash = fopen("tabelaHash.bin", "wb");
    eh.nisNumerico = 0;
    eh.ponteiroHash = 0;
    eh.ponteiroOriginal = 0;
    for (i = 0; i < TAMANHOTABELA; i++){
    	fwrite(&eh, sizeof(EntradaHash), 1, tabelaHash);
	}
	fclose(tabelaHash);
}
	
void criaHash(){
	int i, coluna;
	char *campo, linha[2048];
	EntradaHash eh;
	FILE *bolsa = fopen("bolsa.csv", "r"), *tabelaHash = fopen("tabelaHash.bin", "rb+");
   	long long preenchido[TAMANHOTABELA];
    for (i = 0; i < TAMANHOTABELA; i++){
    	preenchido[i] = 0;
	}
	long long indiceHash, ponteiroAux, posicao;
   	fgets(linha, 2048, bolsa);
    eh.ponteiroOriginal = ftell(bolsa);
	char nis[14];
	for (i = 0; i < 13; i++){
   	fgets(linha, 2048, bolsa);
    //while (!feof(bolsa)){
        coluna = 0;
        campo = strtok(linha,"\t");
        while (campo) {
            if (coluna == 7) {
                strcpy(nis, campo);
                ponteiroAux = atoll(nis);
            }
            coluna++;
            campo = strtok(NULL,"\t");
        }
        eh.nisNumerico = ponteiroAux;
        indiceHash = ponteiroAux%TAMANHOTABELA;
        posicao = indiceHash * sizeof(EntradaHash);
        if (preenchido[indiceHash] == 0){
        	preenchido[indiceHash] = posicao;
            fseek(tabelaHash, posicao, SEEK_SET);
            fwrite(&eh, sizeof(EntradaHash), 1, tabelaHash);
            fclose(tabelaHash);
        } else {
            fseek(tabelaHash, 0, SEEK_END);
            preenchido[indiceHash] = ftell(tabelaHash);
            fseek(tabelaHash, 0, SEEK_END);
            fwrite(&eh, sizeof(EntradaHash), 1, tabelaHash);
            fread(&eh, sizeof(EntradaHash), 1, tabelaHash);
            eh.ponteiroHash = preenchido[indiceHash];
			fseek(tabelaHash, posicao, SEEK_SET);
            fwrite(&eh, sizeof(EntradaHash), 1, tabelaHash);
        }
        fgets(linha, 2048, bolsa);
	    eh.ponteiroOriginal = ftell(bolsa);    
    }
    fclose(bolsa);
    fclose(tabelaHash);    
}

int main (int argc, char** argv){
	abreHash();
	criaHash();
	return 0;
} 
