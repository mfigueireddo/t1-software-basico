#include <stdio.h>
#include <stdlib.h>

#include "auxTests.h"

int main(void){

    FILE* arq32 = fopen("utf32.dat","rb");
    if (arq32 == NULL) exit(1);

    FILE* arq8 = fopen("ret8.dat","rb");
    if (arq8 == NULL) exit(1);

    printf("\n** DUMP UTF 32 **\n");
    dumpUtf32Inv(arq32);

    printf("\n** DUMP UTF 8 **\n");
    dumpUtf8(arq8);

    fclose(arq32);
    fclose(arq8);

    return 0;
}