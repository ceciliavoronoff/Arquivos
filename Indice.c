#include <stdio.h>
#include <string.h>

typedef struct Entrada {
    char nis[15];
    char numero[10];
    char ponteiro[10];
} Entrada;

int compara(const void *e1, const void *e2){
    return strncmp(((Entrada*)e1)->nis, ((Entrada*)e2)->nis, 15);
}

void ordena(FILE * f, int i, int quantidade){
    Entrada *e;
    long posicao;
    fseek(f, 0, SEEK_END);
    posicao = ftell(f);
    e = (Entrada*) malloc(posicao);
    rewind(f);
    fread(e, sizeof(Entrada), quantidade, f);
    qsort(e, quantidade, sizeof(Entrada), compara);
    char[20] em_ordem;
    sprintf(em_ordem, "em_ordem_%d", i);
    fopen(em_ordem, "w");
    fwrite(e, sizeof(Entrada), quantidade, em_ordem);
    fclose(em_ordem);
}

void intercala(int j, int i){
        FILE *a, *b, *saida;
        Entrada ea, eb;
        char primeiro[20];
        char segundo[20];
        char retorno[20];

        sprintf(primeiro, "em_ordem%d.dat", j);
        sprintf(segundo, "em_ordem%d.dat", (j+1));
        sprintf(retorno, "em_ordem%d.dat", i);
 
        a = fopen(primeiro,"r");
        b = fopen(segundo,"r");
        saida = fopen(retorno,"w");
 
        fread(&ea,sizeof(Entrada),1,a);
        fread(&eb,sizeof(Entrada),1,b);
 
        while(!feof(a) && !feof(b)) {
            if(compara(&ea,&eb)<0) {
                fwrite(&ea,sizeof(Entrada),1,saida);
                fread(&ea,sizeof(Entrada),1,a);
            } else {
                fwrite(&eb,sizeof(Entrada),1,saida);
                fread(&eb,sizeof(Entrada),1,b);
            }
        } 
        
        while(!feof(a)) {
            fwrite(&ea,sizeof(Entrada),1,saida);
            fread(&ea,sizeof(Entrada),1,a);        
        }

        while(!feof(b)) {
            fwrite(&eb,sizeof(Entrada),1,saida);
            fread(&eb,sizeof(Entrada),1,b);        
        }
 
    fclose(a);
    fclose(b);
    fclose(saida);
}

void coleta(){
    char linha[2048];
    char nis[15];
    char numero[10];
    char ponteiro[10];
    long n = 1;
    int coluna = 0;
    long posicao;
    char* campo;
    int i = 0;
    int j = 0;
    FILE *f = fopen("bolsa.csv", "r");
    fgets(linha, 2048, f);
    posicao = ftell(f);

    fgets(linha, 2048, f);
    char entradas[20];
    //while(!feof(f)){
        for (int k = 0; k < 3; k++){
        sprintf(entradas, "entradas_%d", i);
        FILE *saida = fopen(entradas, "a");
        coluna = 0;
        int aux = 0;
        campo = strtok(linha,"\t");
        while(campo)
        {
            if(coluna == 7)
            {
                strcpy(nis, campo);
                fprintf(saida, "%.15s\t", campo);
                aux = ftell(saida);
                printf("%d\n", aux);
                numero = (char[])n;
                //strcpy(numero, auxnumero);
                fprintf(saida, "%.10s\t", numero);
                aux = ftell(saida);
                printf("%d\n", aux);
                char auxponteiro = (char)posicao;
                strcpy(ponteiro, auxponteiro);
                fprintf(saida, "%.10s\t", ponteiro);
                aux = ftell(saida);
                printf("%d\n", aux);
            }
            coluna++;
            campo = strtok(NULL,"\t");
        }
        posicao = ftell(f);
        fgets(linha, 2048, f);
        n++;
        if (n%50 == 0){
            ordena(saida, i, 50);
            fclose(saida);
            remove(saida);
            i++;
            sprintf(entradas, "entradas_%d", i);
            saida = fopen(entradas, "a");
        }
    }
    if (n%50 == 0){
        fclose(saida);
        remove(saida);
    } else if (n%50 == 1){
        char[20] em_ordem;
        sprintf(em_ordem, "em_ordem_%d", i);
        rename(saida, em_ordem);
        i++;
    } else {
        ordena(saida, i, n%50);
        fclose(saida);
        remove(saida);
        i++;
    }

    while ((j+1) < i){
        intecala(j, i);
        j += 2;
        i++; 
    }

    if (j < i){
        i++;
        intecala(j, i);
        sprintf(em_ordem, "em_ordem%d.dat", i);
        rename(em_ordem, "indice.dat");
    } else {
        sprintf(em_ordem, "em_ordem%d.dat", i);
        rename(em_ordem, "indice.dat");
    }
    fclose(f);
    
}

int main(int argc, char** argv){
    coleta();
    return 0;
}