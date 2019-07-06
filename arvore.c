#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TAM_CHAVE 14
#define TAM_PAG 300
 
 
typedef struct Entrada {
    char nis[14];
    long ponteiro;
} Entrada;

typedef struct _ArvoreB_Cabecalho {
    long raiz;
}  ArvoreB_Cabecalho;
 
typedef struct _ArvoreB {
    ArvoreB_Cabecalho* cabecalho;
    FILE* f;    
} ArvoreB;
 
typedef struct _ArvoreB_Elemento {
    struct Entrada Entrada;
	//char Entrada->nis[14];
    long posicaoOriginal;
    long posicaoRegistro;
    long paginaDireita;
} ArvoreB_Elemento;
 
typedef struct _ArvoreB_Pagina {
    int quantidadeElementos;
    long paginaEsquerda;
    ArvoreB_Elemento elementos[TAM_PAG];
} ArvoreB_Pagina;

void ArvoreB_desalocaPagina(ArvoreB_Pagina* p)
{
    if(p)
    {
        free(p);
    }
}
 
int ArvoreB_Compara(const void *e1, const void *e2){
    return strncmp(((Entrada*)e1)->nis, ((Entrada*)e2)->nis, 14);
}

/*
int ArvoreB_Compara(const void *e1, const void* e2)
{
    return strncmp(((ArvoreB_Elemento*)e1)->Entrada.nis,((ArvoreB_Elemento*)e2)->Entrada.nis,TAM_CHAVE); 
}*/
 
void ArvoreB_escreveCabecalho(ArvoreB* arvore)
{
    fseek(arvore->f, 0, SEEK_SET);
    fwrite(arvore->cabecalho, sizeof(ArvoreB_Cabecalho), 1, arvore->f);
}
 
ArvoreB_Pagina* ArvoreB_alocaPagina(){
    int i;
    ArvoreB_Pagina* resp = (ArvoreB_Pagina*) malloc(sizeof(ArvoreB_Pagina));
    resp->quantidadeElementos = 0;
    resp->paginaEsquerda = 0;
    for(i=0; i<TAM_PAG; i++)
    {
        memset(resp->elementos[i].Entrada.nis,'\0',TAM_CHAVE);
        resp->elementos[i].paginaDireita = 0;
        resp->elementos[i].posicaoRegistro = 0;
    }
    return resp;
}
 
void ArvoreB_Fecha(ArvoreB* arvore)
{
    if(arvore)
    {
        ArvoreB_escreveCabecalho(arvore);
        fclose(arvore->f);
        free(arvore->cabecalho);
        free(arvore);
    }
}
 
ArvoreB_Elemento* ArvoreB_Split(ArvoreB *arvore, long posicaoPagina, ArvoreB_Pagina* pagina, ArvoreB_Elemento* overflow)
{
    int i;
    ArvoreB_Elemento aux;
    ArvoreB_Elemento* resp;
    ArvoreB_Pagina* paginaSplit;
    // O elemento na área de overflow é menor que o ultimo elemento da página?
    if(ArvoreB_Compara(overflow,&pagina->elementos[pagina->quantidadeElementos-1])<0)
    {
        // Trocar o último com o elemento no overflow e reordenar a página.
        aux = *overflow;
        *overflow = pagina->elementos[pagina->quantidadeElementos-1];
        pagina->elementos[pagina->quantidadeElementos-1] = aux;
        qsort(pagina->elementos,pagina->quantidadeElementos,sizeof(ArvoreB_Elemento),ArvoreB_Compara);
    }
    paginaSplit = ArvoreB_alocaPagina();
    for(i=TAM_PAG/2+1;i<TAM_PAG;i++)
    {
        paginaSplit->elementos[paginaSplit->quantidadeElementos] = pagina->elementos[i];
        paginaSplit->quantidadeElementos++;
    }
    paginaSplit->elementos[paginaSplit->quantidadeElementos] = *overflow;
    paginaSplit->quantidadeElementos++;
    pagina->quantidadeElementos = TAM_PAG/2;
    fseek(arvore->f, posicaoPagina, SEEK_SET);
    fwrite(pagina, sizeof(ArvoreB_Pagina),1,arvore->f);
    fseek(arvore->f,0,SEEK_END);
    resp = (ArvoreB_Elemento*) malloc(sizeof(ArvoreB_Elemento));
    paginaSplit->paginaEsquerda = pagina->elementos[TAM_PAG/2].paginaDireita;
    memcpy(resp->Entrada.nis,pagina->elementos[TAM_PAG/2].Entrada.nis,TAM_CHAVE);
    resp->posicaoRegistro = pagina->elementos[TAM_PAG/2].posicaoRegistro;
    resp->paginaDireita = ftell(arvore->f);
    fwrite(paginaSplit, sizeof(ArvoreB_Pagina),1,arvore->f);
    ArvoreB_desalocaPagina(paginaSplit);
    return resp;
}
 
ArvoreB_Elemento* ArvoreB_Insere_Recursiva(ArvoreB* arvore, long posicaoPagina, struct Entrada Entrada, long posicaoRegistro)
{
    int i;
    long posicaoPaginaFilho;
    ArvoreB_Elemento* resp = (ArvoreB_Elemento*) 0;
    ArvoreB_Elemento *elementoSplit = (ArvoreB_Elemento*) 0;
    ArvoreB_Elemento overflow;
    // printf("Inserindo %.8s na página %ld\n", chave, posicaoPagina);
    ArvoreB_Pagina *pagina = ArvoreB_alocaPagina();
    fseek(arvore->f, posicaoPagina, SEEK_SET);
    fread(pagina, sizeof(ArvoreB_Pagina),1,arvore->f);
    if(pagina->paginaEsquerda == 0) // Folha
    {
        if(pagina->quantidadeElementos < TAM_PAG)
        {
            memcpy(pagina->elementos[pagina->quantidadeElementos].Entrada.nis,Entrada.nis,TAM_CHAVE);
            pagina->elementos[pagina->quantidadeElementos].posicaoRegistro = posicaoRegistro;
            pagina->quantidadeElementos++;
            qsort(pagina->elementos,pagina->quantidadeElementos,sizeof(ArvoreB_Elemento),ArvoreB_Compara);
            fseek(arvore->f, posicaoPagina, SEEK_SET);
            fwrite(pagina, sizeof(ArvoreB_Pagina),1,arvore->f);
        } else {
            memcpy(overflow.Entrada.nis, Entrada.nis, TAM_CHAVE);
            overflow.posicaoRegistro = posicaoRegistro;
            overflow.paginaDireita = 0;
            resp = ArvoreB_Split(arvore, posicaoPagina, pagina, &overflow);
        }
    }
    else // Não é folha
    {
        posicaoPaginaFilho = pagina->paginaEsquerda;
        for(i=0; i<pagina->quantidadeElementos; i++)
        {
            if(strncmp(Entrada.nis,pagina->elementos[i].Entrada.nis,TAM_CHAVE)<0)
            {
                break;
            }
            posicaoPaginaFilho = pagina->elementos[i].paginaDireita;
        }
        elementoSplit = ArvoreB_Insere_Recursiva(arvore,posicaoPaginaFilho,Entrada,posicaoRegistro);
        if(elementoSplit)
        {
            if(pagina->quantidadeElementos < TAM_PAG)
            {
                pagina->elementos[pagina->quantidadeElementos] = *elementoSplit;
                pagina->quantidadeElementos++;
                qsort(pagina->elementos,pagina->quantidadeElementos,sizeof(ArvoreB_Elemento),ArvoreB_Compara);
                fseek(arvore->f,posicaoPagina,SEEK_SET);
                fwrite(pagina,sizeof(ArvoreB_Pagina),1,arvore->f);
                free(elementoSplit);
            }
            else
            {
                resp = ArvoreB_Split(arvore, posicaoPagina, pagina, elementoSplit);
                free(elementoSplit);
            }
        }
    }
    ArvoreB_desalocaPagina(pagina);
    return resp;
}
 
void ArvoreB_Insere(ArvoreB* arvore, struct Entrada Entrada, long posicaoRegistro )
{
	ArvoreB_Pagina *novaRaiz;
    ArvoreB_Elemento *elementoSplit;
    elementoSplit = ArvoreB_Insere_Recursiva(arvore, arvore->cabecalho->raiz, Entrada, posicaoRegistro);
    if(elementoSplit)
    {
        novaRaiz = ArvoreB_alocaPagina();
        novaRaiz->quantidadeElementos = 1;
        novaRaiz->elementos[0] = *elementoSplit;
        novaRaiz->paginaEsquerda = arvore->cabecalho->raiz;
        fseek(arvore->f, 0, SEEK_END);
        arvore->cabecalho->raiz = ftell(arvore->f);
        fwrite(novaRaiz,sizeof(ArvoreB_Pagina),1,arvore->f);
        ArvoreB_escreveCabecalho(arvore);
        ArvoreB_desalocaPagina(novaRaiz);
        free(elementoSplit);
    }
}
 
void ArvoreB_PrintDebug(ArvoreB* arvore)
{
    long posicaoPagina;
    int i;
    ArvoreB_Pagina *pagina = ArvoreB_alocaPagina();
    fseek(arvore->f, sizeof(ArvoreB_Cabecalho), SEEK_SET);
    posicaoPagina = ftell(arvore->f);
    while(fread(pagina,sizeof(ArvoreB_Pagina),1,arvore->f))
    {
        if(posicaoPagina == arvore->cabecalho->raiz)
        {
            printf("* ");
        }
        else
        {
            printf("  ");
        }
        printf("%5ld => %5ld|",posicaoPagina, pagina->paginaEsquerda);
        for(i=0; i<pagina->quantidadeElementos; i++)
        {
 
            printf("(%.8s)|%5ld|",pagina->elementos[i].Entrada.nis, pagina->elementos[i].paginaDireita);
        }
        printf("\n");
        posicaoPagina = ftell(arvore->f);
    }
}
 
ArvoreB* ArvoreB_Abre(const char* nomeArquivo){
    ArvoreB* resp = (ArvoreB*) 0;
    ArvoreB_Cabecalho* cabecalho = (ArvoreB_Cabecalho*) 0;
    ArvoreB_Pagina* raiz = (ArvoreB_Pagina*) 0;
    FILE* f = fopen(nomeArquivo, "r");
    if(!f)
    {
        f = fopen(nomeArquivo,"w");
        if(!f)
        {
            fprintf(stderr,"Arquivo %s não pode ser criado\n", nomeArquivo);
            return resp;
        }
        cabecalho = (ArvoreB_Cabecalho*) malloc(sizeof(ArvoreB_Cabecalho));
        cabecalho->raiz = sizeof(ArvoreB_Cabecalho);
        fwrite(cabecalho,sizeof(ArvoreB_Cabecalho),1,f);
        raiz = ArvoreB_alocaPagina();
        fwrite(raiz,sizeof(ArvoreB_Pagina),1,f);
        ArvoreB_desalocaPagina(raiz);
        free(cabecalho);
    }
    fclose(f);
    f = fopen(nomeArquivo, "rb+");
    resp = (ArvoreB*) malloc(sizeof(ArvoreB));
    resp->f = f;
    resp->cabecalho = (ArvoreB_Cabecalho*) malloc(sizeof(ArvoreB_Cabecalho));
    fread(resp->cabecalho,sizeof(ArvoreB_Cabecalho),1,f);
    return resp;
}
 
int main(int argc, char** argv){
    ArvoreB* a = ArvoreB_Abre("arvore.dat");
    char chave[TAM_CHAVE];
    long posicao, j, posicaoOriginal;
    int i;
    FILE* indice = fopen("indice.bin", "r");
    fseek (indice, 0, SEEK_END);
    j = ftell(indice)/sizeof(Entrada);
 	Entrada e;
    for(i=0; i<10; i++){
        
		fread (&e, sizeof(Entrada), 1, indice);
		//strcpy(chave, e.nis);
		//posicaoOriginal = e.ponteiro;
        posicao = i;
        ArvoreB_Insere(a, e, posicao);
    }
 
    ArvoreB_PrintDebug(a);
 
    ArvoreB_Fecha(a);
    return 0;
}
