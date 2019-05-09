#include <stdio.h>
#include <string.h>

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

int main(int argc, char**argv)
{
    FILE *f;
    Endereco e;
    long inicio, meio, fim;
    int c, a;

    if(argc != 2)
    {
        fprintf(stderr, "USO: %s [CEP]", argv[0]);
        return 1;
    }

      f = fopen("cep_ordenado.dat","r");
    fseek (f, 0, SEEK_END);
    fim = (ftell(f)/sizeof(Endereco))-1;
    rewind (f);
    c = 0;
    a = 0;
    inicio = 0;
    while (fim >= inicio)
    {
        meio = (inicio+fim)/2;
        c++;
        fseek (f, meio * sizeof(Endereco), SEEK_SET);
        fread (&e, sizeof(Endereco), 1, f);
        if(strncmp(argv[1],e.cep,8)==0)
        {
            a++;
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
            break;
        }
        else if (strncmp(argv[1],e.cep,8) < 0){
            fim = meio - 1;
        }
        else {
            inicio = meio + 1;
        }
       
    }
    if (a == 0){
        printf("CEP não encontrado");
    }
    printf("Total Lido: %d\n", c);
    fclose(f);
}
