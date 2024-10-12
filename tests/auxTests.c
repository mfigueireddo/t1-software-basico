#include <stdio.h>
#include <stdlib.h>

#include "auxTests.h"

void dumpUtf8(FILE* arq){

    unsigned char byte[4];  

    while (!feof(arq)){
        fread(&byte[0], sizeof(char), 1, arq);
        // 1 byte
        if (!(byte[0] & 128)){
            printf("\n1 byte\n");
            char2bin(byte[0]); printf("\n");
        }
        // 2 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && !(byte[0] & 32)){
            fread(&byte[1], sizeof(char), 1, arq);

            printf("\n2 bytes\n");
            char2bin(byte[0]); char2bin(byte[1]); printf("\n");
        }
        // 3 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && (byte[0] & 32) && !(byte[0] & 16)){
            fread(&byte[1], sizeof(char), 1, arq);
            fread(&byte[2], sizeof(char), 1, arq);

            printf("\n3 bytes\n");
            char2bin(byte[0]); char2bin(byte[1]); char2bin(byte[2]); printf("\n");
        }
        // 4 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && (byte[0] & 32) && (byte[0] & 16)){
            fread(&byte[1], sizeof(char), 1, arq);
            fread(&byte[2], sizeof(char), 1, arq);
            fread(&byte[3], sizeof(char), 1, arq);

            printf("\n4 bytes\n");
            char2bin(byte[0]); char2bin(byte[1]); char2bin(byte[2]); char2bin(byte[3]); printf("\n");
        }
    }

}

void dumpUtf32(FILE* arq){

    short bom;
    unsigned int aux;

    fread(&bom, sizeof(short), 1, arq);
    printf("\n0x%04x BOM\n\n", bom & 0xFFFF);

    while(!feof(arq)){
        fread(&aux, sizeof(int), 1, arq);
        //inverte_int(&aux);
        if (aux < 127) printf("1 byte\n");
        else if (aux < 2047 && aux > 127 ) printf("2 bytes\n");
        else if (aux < 65535 && aux > 2047) printf("3 bytes\n");
        else if (aux < 1114111 && aux > 65535) printf("4 bytes\n");
        int2bin(aux);
        printf("\n\n");
    }

}

void dumpUtf32Inv(FILE* arq){

    short bom;
    unsigned int aux;

    fread(&bom, sizeof(short), 1, arq);
    printf("\n0x%04x BOM\n\n", bom & 0xFFFF);

    while(!feof(arq)){
        fread(&aux, sizeof(int), 1, arq);
        inverte_int(&aux);
        if (aux < 127) printf("1 byte\n");
        else if (aux < 2047 && aux > 127 ) printf("2 bytes\n");
        else if (aux < 65535 && aux > 2047) printf("3 bytes\n");
        else if (aux < 1114111 && aux > 65535) printf("4 bytes\n");
        int2bin(aux);
        printf("\n\n");
    }

}

void char2bin(unsigned char c) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (c >> i) & 1);
        if (i%4==0) printf(" ");
    }
}

void int2bin(unsigned int n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i%4==0) printf(" ");
    }
}

void inverte_int(unsigned int *original){

    // Cópia do inteiro original
    unsigned int copia = *(original);

    // Armazena 1 bit do original
    unsigned char aux;

    // Inteiro invertido
    unsigned int final;

    for(int i=0; i<32; i++){
        aux = copia & 1;
        copia = copia >> 1;
        final = final | aux;
        final = final << 1;
    }

    *(original) = final;
}