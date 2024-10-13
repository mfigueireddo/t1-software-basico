#include <stdio.h>
#include <stdlib.h>

#include "aux.h"

int main(void){

    // Arquivo de entrada em UTF-8
    FILE* ent8 = fopen("ret8.dat","rb");
    if (ent8 == NULL) exit(1);
    // Arquivo de saída em UTF-32
    FILE* saida32 = fopen("reverse32.dat","wb");
    if (saida32 == NULL) exit(1);
    // Chamada de função
    utf8to32(ent8,saida32);
    // Fechamento de arquivos
    fclose(ent8);
    fclose(saida32);

    // Arquivo de entrada em UTF-32
    FILE* ent32 = fopen("ret32.dat","rb");
    if (ent32 == NULL) exit(1);
    // Arquivo de saída em UTF-8
    FILE* saida8 = fopen("reverse8.dat","wb");
    if (saida8 == NULL) exit(1);
    // Chamada de função
    utf32to8(ent32,saida8);
    // Fechamento de arquivos
    fclose(ent32);
    fclose(saida8);

    return 0;
}