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

    unsigned int bom;
    unsigned int aux;

    fread(&bom, sizeof(int), 1, arq);
    printf("BOM: "); int2bin(bom); printf("\n\n");

    while(!feof(arq)){
        fread(&aux, sizeof(int), 1, arq);
        if (bom == 0xFFFE0000) little2big(&aux);
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

void little2big(unsigned int *num) {
    // Reorganizando os bytes do número
    unsigned int byte0, byte1, byte2, byte3;
    unsigned int value = *num;  // Obtém o valor original do ponteiro

    byte0 = (value & 0x000000FF) << 24;  // Move o byte 0 para a posição 3
    byte1 = (value & 0x0000FF00) << 8;   // Move o byte 1 para a posição 2
    byte2 = (value & 0x00FF0000) >> 8;   // Move o byte 2 para a posição 1
    byte3 = (value & 0xFF000000) >> 24;  // Move o byte 3 para a posição 0

    *num = byte0 | byte1 | byte2 | byte3; // Atualiza o valor original com a nova ordem de bytes
}