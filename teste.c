#include <stdio.h>
#include <string.h>

typedef struct Entrada {
    char nis[14];
    long ponteiro;
} Entrada;

int main(){
    Entrada e;
    FILE *f;
    f = fopen("entradas_0", "r");
    for (int i = 0; i < 3; i++){
        fread(&e, sizeof(Entrada), 1, f);
        printf("%.14s\n%ld\n",e.nis, e.ponteiro);
    }
}