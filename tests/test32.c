#include <stdio.h>
#include <stdlib.h>

#include "auxTests.h"

int main(void){

    FILE* original32 = fopen("utf32.dat","rb");
    FILE* reverse32 = fopen("utf32.dat","rb");
    if (original32 == NULL) exit(1);
    if (reverse32 == NULL) exit(1);

    printf("\n** DUMP UTF 32 ORIGINAL **\n");
    dumpUtf32(original32);

    printf("\n\n** DUMP UTF 32 REVERSO **\n");
    dumpUtf32(reverse32);

    fclose(original32);
    fclose(reverse32);

    return 0;
}