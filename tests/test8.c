#include <stdio.h>
#include <stdlib.h>

#include "auxTests.h"

int main(void){

    FILE* original8 = fopen("utf8.dat","rb");
    FILE* reverse8 = fopen("reverse8.dat","rb");
    if (original8 == NULL) exit(1);
    if (reverse8 == NULL) exit(1);

    printf("\n** DUMP UTF 8 ORIGINAL **\n");
    dumpUtf8(original8);

    printf("\n\n** DUMP UTF 8 REVERSO **\n");
    dumpUtf8(reverse8);

    fclose(original8);
    fclose(reverse8);

    return 0;
}