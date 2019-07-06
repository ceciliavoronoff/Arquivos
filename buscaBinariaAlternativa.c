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

void ordenaIndice(){
	Entrada *e;
	FILE *indice, *indiceOrdenado;
	indice = fopen("indice.bin", "rb");
    fseek(indice, 0, SEEK_END);
    long posicao = ftell(indice);
    rewind(indice);
    long quantidade = posicao/sizeof(Entrada);
    e = (Entrada*) malloc(posicao);
    fread(e, sizeof(Entrada), quantidade, indice);
    qsort(e, quantidade, sizeof(Entrada), compara);
    indiceOrdenado = fopen("indiceOrdenado.bin", "wb");
    fwrite(e, sizeof(Entrada), quantidade, indiceOrdenado);
    free(e);
    fclose(indiceOrdenado);
    fclose(indice);
}

void criaIndice(){
    char linha[2048];
    Entrada e;
    int coluna = 0, i;
    char* campo;
    FILE *bolsa = fopen("bolsa.csv", "r");
    FILE *indice = fopen("indice.bin", "wb");
    fgets(linha, 2048, bolsa);
    long posicao = ftell(bolsa);
	fgets(linha, 2048, bolsa);
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
        posicao = ftell(bolsa);
        fgets(linha, 2048, bolsa);
        
    }
	fclose(indice);
	fclose(bolsa);
}

long buscaIndice(char pesquisa[14]){

    long posicao, primeiro, ultimo, meio;
    FILE *f = fopen("indiceOrdenado.bin","rb"); //abre arquivo
    fseek(f,0,SEEK_END); //move o cabeçote para o final do arquivo
    posicao = ftell(f); //pega a posição do cabeçote
    rewind(f); //volta o cabeçote para o começo do arquivo
    
    primeiro = 0;  //primeira linha
    ultimo = (posicao/sizeof(Entrada))-1; //penúltima linha

    int achou = 0, lidos = 0;

    Entrada e;
    
    while(primeiro <= ultimo)
    {
        lidos++;
        meio = (primeiro+ultimo)/2;
        fseek(f,meio*sizeof(Entrada),SEEK_SET); //move o cabeçote para a linha do meio
        fread(&e,sizeof(Entrada),1,f);
        if(strncmp(pesquisa,e.nis,14) == 0)
        {
            return e.ponteiro;
            break;
        }
        else if(strncmp(pesquisa,e.nis,14) > 0) //ir para a direita
        {
            primeiro = meio+1;
        }
        else //ir para a esquerda
        {
            ultimo = meio-1;
        }
    }   
    fclose(f);
    return achou;
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
    indice = fopen("indice.bin", "rb");
    if (indice == NULL){
        criaIndice();
        indice = fopen("indice.bin", "rb");
    }
    long posicao = buscaIndice(pesquisa);
    fclose(indice);
    if (posicao == 0){
        printf("NIS nao cadastrado");
    } else {
        buscaBolsa(posicao);
    }
}

int main(int argc, char** argv){
    busca();
    return 0;
}
