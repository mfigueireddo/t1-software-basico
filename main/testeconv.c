/* Matheus Moreira da Silva Figueiredo 2320813 3WA*/

#include <stdio.h>
#include <stdlib.h>

#include "converteutf832.h"

int main(void){

    // Testes UTF-8 -> UTF-32

    // Converte de UTF-8 para UTF-32
    FILE *a = fopen("utf8.dat","rb"); FILE *b = fopen("ret32.dat","wb");
    if (a == NULL || b == NULL) exit(1);
    convUtf8p32(a,b);
    fclose(a); fclose(b);

    // Converte de volta para UTF-8
    a = fopen("ret32.dat","rb"); b = fopen("reverse8.dat","wb");
    if (a == NULL || b == NULL) exit(1);
    convUtf32p8(a,b);
    fclose(a); fclose(b);

    // Dump dos arquivos UTF-8
    a = fopen("utf8.dat","rb"); a = fopen("reverse8.dat","rb");
    if (a == NULL || a == NULL) exit(1);
    printf("** TESTE PARA UTF-8 **\n\n");
    dumpUtf8(a,b);
    fclose(a); fclose(b);

    // Testes UTF-32 -> UTF-8

    // Converte de UTF-32 para UTF-8
    a = fopen("utf32.dat","rb"); b = fopen("ret8.dat","wb");
    if (a == NULL || b == NULL) exit(1);
    convUtf32p8(a,b);
    fclose(a); fclose(b);

    // Converte de volta para UTF-32
    a = fopen("ret8.dat","rb"); b = fopen("reverse32.dat","wb");
    if (a == NULL || b == NULL) exit(1);
    convUtf8p32(a,b);
    fclose(a); fclose(b);

    // Dump dos arquivos UTF-32
    a = fopen("utf32.dat","rb"); b = fopen("reverse32.dat","rb");
    if (a == NULL || b == NULL) exit(1);
    printf("\n\n** TESTE PARA UTF-32 **\n\n");
    dumpUtf32(a,b);
    fclose(a); fclose(b);


    return 0;
}