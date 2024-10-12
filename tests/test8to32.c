#include <stdio.h>
#include <stdlib.h>

#include "auxTests.h"

int main(void){

    FILE* arq8 = fopen("utf8.dat","rb"
    );
    if (arq8 == NULL) exit(1);
    FILE* arq32 = fopen("ret32.dat","rb");
    if (arq32 == NULL) exit(1);

    printf("\n** DUMP UTF 8 **\n");
    dumpUtf8(arq8);

    printf("\n** DUMP UTF 32 **\n");
    dumpUtf32(arq32);

    fclose(arq8);
    fclose(arq32);

    return 0;
}