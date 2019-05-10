#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

void selecao(){
    FILE *f;
    Endereco e;
    long fim;

    f = fopen("cep.dat","r");
    FILE *cria = fopen("selecao.dat", "w");
    fseek (f, 0, SEEK_END);
    fim = (ftell(f)/sizeof(Endereco))-1;
    rewind (f);
    srand((unsigned) time (NULL));
    
    for (int i = 0; i < 80; i++){
        fseek(f, (rand()%fim)*sizeof(Endereco), SEEK_SET);
        fread(&e, sizeof(Endereco), 1, f);
        fwrite(&e, sizeof(Endereco), 1, cria);
    }
    
    fclose(f);
    fclose(cria);
}

void divisao(){
    FILE *g = fopen("selecao.dat", "r");
    Endereco e;
    for (int i = 1; i <= 8; i++){
        char oito_n[20];
        sprintf(oito_n, "oito_%d.dat", i);
        FILE *menor = fopen(oito_n, "a");
        for (int j = 0; j < 10; j++){
            fread(&e, sizeof(Endereco), 1, g);
            fwrite(&e, sizeof(Endereco), 1, menor);
        }
        fclose(menor);
    } 
    fclose(g);
}

int compara(const void *e1, const void *e2){
    return strncmp((*(Endereco*)e1).cep,(*(Endereco*)e2).cep, 8);
}

void ordenacao(){
    FILE *f, *saida;
    Endereco *e;
    long posicao, quantidade;
    for(int i = 1; i <= 8; i++){
        char oito_n [20];
        sprintf(oito_n, "oito_%d.dat", i);
        f = fopen(oito_n, "r");
        char em_ordem[20];
        sprintf(em_ordem, "em_ordem%d.dat", i);
        saida = fopen(em_ordem, "w");
        fseek (f, 0, SEEK_END);
        posicao = ftell(f);
        rewind(f);
        quantidade = posicao/sizeof(Endereco);
        e = (Endereco*) malloc (posicao);
        fread (e, sizeof(Endereco), quantidade, f);
        fclose(f);
        qsort(e, quantidade, sizeof(Endereco), compara);
        fwrite(e, sizeof(Endereco), quantidade, saida);
        free(e);
        fclose(saida);
    }
}

void intercalacao(){

    FILE *a, *b, *saida;
    Endereco ea, eb;
    int i = 1;
    int j = 9;

    char primeiro[20];
    char segundo[20];
    char retorno[20];

    while(j <= 15){

    sprintf(primeiro, "em_ordem%d.dat", i);
    i++;
    sprintf(segundo, "em_ordem%d.dat", i);
    sprintf(retorno, "em_ordem%d.dat", j);
 
    a = fopen(primeiro,"r");
    b = fopen(segundo,"r");
    saida = fopen(retorno,"w");
 
    fread(&ea,sizeof(Endereco),1,a);
    fread(&eb,sizeof(Endereco),1,b);
 
    while(!feof(a) && !feof(b))
    {
        if(compara(&ea,&eb)<0)
        {
            fwrite(&ea,sizeof(Endereco),1,saida);
            fread(&ea,sizeof(Endereco),1,a);
        }
        else
        {
            fwrite(&eb,sizeof(Endereco),1,saida);
            fread(&eb,sizeof(Endereco),1,b);
        }
    }
 
    while(!feof(a))
    {
        fwrite(&ea,sizeof(Endereco),1,saida);
        fread(&ea,sizeof(Endereco),1,a);        
    }
    while(!feof(b))
    {
        fwrite(&eb,sizeof(Endereco),1,saida);
        fread(&eb,sizeof(Endereco),1,b);        
    }
 
    fclose(a);
    fclose(b);
    fclose(saida);

    i++;
    j++;

    }
rename("em_ordem15.dat", "intercalado.dat");
}

void remocao(){
    remove("selecao.dat");
    for (int i = 1; i <= 8; i++){
        char oito_n[20];
        sprintf(oito_n, "oito_%d.dat", i);
        remove(oito_n);
    }
    for (int j = 1; j < 15; j++){
        char em_ordem[20];
        sprintf(em_ordem, "em_ordem%d.dat", j);
        remove(em_ordem);

    }
}

int main()
{
    selecao();
    divisao();
    ordenacao();
    intercalacao();
    remocao();

    return 0;
}
