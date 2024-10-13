#include <stdio.h>
#include <stdlib.h>

#include "converteutf832.h"

int main(void){

    FILE* ent8 = fopen("utf8.dat","rb");
    FILE* saida32 = fopen("ret32.dat","wb");
    if (ent8 == NULL) exit(1);
    if (saida32 == NULL) exit(1);
    convUtf8p32(ent8,saida32);
    fclose(ent8);
    fclose(saida32);

    FILE* ent32 = fopen("utf32.dat","rb");
    FILE* saida8 = fopen("ret8.dat","wb");
    if (ent32 == NULL) exit(1);
    if (saida8 == NULL) exit(1);
    convUtf32p8(ent32,saida8);
    fclose(ent32);
    fclose(saida8);

    return 0;
}