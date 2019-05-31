#include <stdio.h>
 
int main(int argc, char** argv)
{
    FILE *entrada, *saida;
    int contador[256];
    int c;
 
    entrada = fopen(argv[1],"r");
    if(!entrada)
    {
        fprintf(stderr,"Arquivo %s não pode ser aberto para leitura\n", argv[1]);
        return 1;
    }
    saida = fopen("contadorcep.csv","w");
    if(!saida)
    {
        fclose(entrada);
        fprintf(stderr,"Arquivo %s não pode ser aberto para escrita\n", argv[2]);
        return 1;
    }
 
    for (int i = 0; i < 256; i++){
        contador[i] = 0;
    }

    c = fgetc(entrada);
    while(c != EOF)
    {
        contador[c]++;
        c = fgetc(entrada);
    }
 
    for (int i = 0; i < 256; i++){
        fprintf(saida, "%d\t%d\n", i, contador[i]);
    }
 
    fclose(entrada);
    fclose(saida);
    return 0;
}