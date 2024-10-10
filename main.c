#include <stdio.h>
#include <stdlib.h>

#include "aux.h"

int main(void){

    // Arquivo de entrada em UTF-8
    FILE* ent8 = fopen("utf8.txt","rb");
    if (ent8 == NULL) exit(1);

    // Arquivo de saída em UTF-32
    FILE* saida32 = fopen("utf8to32.txt","wb");
    if (saida32 == NULL) exit(1);

    // Arquivo de entrada em UTF-32
    FILE* ent32 = fopen("utf32.txt","rb");
    if (ent32 == NULL) exit(1);

    // Arquivo de saída em UTF-8
    FILE* saida8 = fopen("utf32to8.txt","wb");
    if (saida8 == NULL) exit(1);

    // Chamada de funções
    utf8to32(ent8,saida32);
    // utf32to8(ent32,saida8);

    // Fechamento de arquivos
    fclose(ent8);
    fclose(saida32);
    fclose(ent32);
    fclose(saida8);

    return 0;
}