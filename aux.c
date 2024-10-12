#include <stdio.h>
#include <stdlib.h>

#include "aux.h"

// Converte um arquivo UTF-8 para UTF-32
void utf8to32(FILE* entrada, FILE* saida){

    // Vetor que vai armazenar, no máximo, 4 bytes de memória
    unsigned char byte[4];

    // Inteiro auxiliar para realizar a conversão
    unsigned int extended = 0;

    // Escreve o BOM em Big Endian
    short bom = 0x0000FEFF;
    fwrite(&bom, sizeof(short), 1, saida);
    // printf("0x%04x BOM\n\n", bom & 0xFFFF);

    while (!feof(entrada)){

        fread(&byte[0], sizeof(char), 1, entrada);

        // 1 byte
        if (!(byte[0] & 128)){
            extended = extended | byte[0]; // copia o byte para um inteiro
            fwrite(&extended,sizeof(char),1,saida);

            // printf("\n1 byte\n");
            // printf("Obtido: "); char2bin(byte[0]); printf("\n");
            // printf("Retornado: "); int2bin(extended); printf("\n");

            extended = 0;
        }
        // 2 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && !(byte[0] & 32)){
            fread(&byte[1], sizeof(char), 1, entrada);

            // printf("\n2 bytes\n");
            // printf("Obtido: "); char2bin(byte[0]); char2bin(byte[1]); printf("\n");

            byte[0] = byte[0] & 63; // esvazia os 2 1os bits do 1o byte
            byte[1] = byte[1] & 127; // esvazia o 1o bit do 2o byte

            // Move os 2 últimos bits do 1o byte para o 2o + Anda com o 1o
            move_between(&byte[0],&byte[1]); byte[0] = byte[0] >> 2;

            // Copia os 2 bytes separados para uma variável de 4 bytes
            for(int i=0; i<2; i++){
                extended = extended | byte[i];
                if (i!=1) extended = extended << 8;
            }
            
            fwrite(&extended,sizeof(int),1,saida);

            // printf("Retornado: "); int2bin(extended); printf("\n");
            
            extended = 0;
        }
        // 3 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && (byte[0] & 32) && !(byte[0] & 16)){
            fread(&byte[1], sizeof(char), 1, entrada);
            fread(&byte[2], sizeof(char), 1, entrada);

            //printf("\n3 bytes\n");
            //printf("Obtido: "); char2bin(byte[0]); char2bin(byte[1]); char2bin(byte[2]); printf("\n");

            byte[0] = byte[0] & 31; // esvazia os 3 1os bits do 1o byte
            
            // Esvazia o 1o bit do 2o e 3o bytes
            for (int i=1; i<3; i++){ byte[i] = byte[i] & 127; }

            // Move os 2 últimos bits do 1o byte para o 2o + Anda com o 1o byte
            move_between(&byte[0],&byte[1]); byte[0] = byte[0] >> 2;

            // Move os 2 últimos bits do 2o byte para o 3o + Anda com o 2o byte
            move_between(&byte[1],&byte[2]); byte[1] = byte[1] >> 2;

            // Move os 2 últimos bits do 1o byte para o 2o + Anda com o 1o byte
            move_between(&byte[0],&byte[1]); byte[0] = byte[0] >> 2;

            // Copia os 3 bytes separados para uma variável de 4 bytes
            for(int i=0; i<3; i++){
                extended = extended | byte[i];
                if (i!=2) extended = extended << 8;
            }

            fwrite(&extended,sizeof(int),1,saida);
            
            //printf("Retornado: "); int2bin(extended); printf("\n");

            extended = 0;
        }
        // 4 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && (byte[0] & 32) && (byte[0] & 16)){
            fread(&byte[1], sizeof(char), 1, entrada);
            fread(&byte[2], sizeof(char), 1, entrada);
            fread(&byte[3], sizeof(char), 1, entrada);

            //printf("\n4 bytes\n");
            //printf("Obtido: "); char2bin(byte[0]); char2bin(byte[1]); char2bin(byte[2]); char2bin(byte[3]); printf("\n");

            byte[0] = byte[0] & 15; // esvazia os 4 1os bits do 1o byte

            // Esvazia o 1o bit do 2o, 3o e 4o bytes
            for (int i=1; i<4; i++){
                byte[i] = byte[i] & 127;
            }

            // Move os 2 últimos bits do 1o byte para o 2o + Anda com o 1o byte
            move_between(&byte[0],&byte[1]); byte[0] = byte[0] >> 2;

            // Move os 2 últimos bits do 2o byte para o 3o + Anda com o 2o byte
            move_between(&byte[1],&byte[2]); byte[1] = byte[1] >> 2;

            // Move os 2 últimos bits do 1o byte para o 2o + Anda com o 1o byte
            move_between(&byte[0],&byte[1]); byte[0] = byte[0] >> 2;

            // Move os 2 últimos bits do 3o byte para o 4o + Anda com o 3o byte
            move_between(&byte[2], &byte[3]); byte[2] = byte[2] >> 2;

            // Move os 2 últimos bits do 2o byte para o 3o + Anda com o 2o byte
            move_between(&byte[1],&byte[2]); byte[1] = byte[1] >> 2;

            // Move os 2 últimos bits do 1o byte para o 2o + Anda com o 1o byte
            move_between(&byte[0],&byte[1]); byte[0] = byte[0] >> 2;

            // Copia os 4 bytes separados para uma variável de 4 bytes
            for(int i=0; i<4; i++){
                extended = extended| byte[i];
                if (i!=3) extended = extended << 8;
            }

            fwrite(&extended,sizeof(int),1,saida);

            //printf("Retornado: "); int2bin(extended); printf("\n");

            extended = 0;
        }
    }

}

// Converte um arquivo UTF-32 para UTF-8
void utf32to8(FILE* entrada, FILE* saida){

    short bom; // Armazena o valor BOM
    unsigned int aux; // Armazena os inteiros lidos
    unsigned char final[] = {0,0,0,0}; // Versão final do byte

    fread(&bom, sizeof(short), 1, entrada);
    //printf("0x%04x BOM\n\n", bom & 0xFFFF);
    
    while (!feof(entrada)){
        fread(&aux, sizeof(int), 1, entrada);

        inverte_int(&aux); // Inverte os bits do inteiro

        // 1 byte
        if (aux < 127){

            //printf("1 byte\n");
            //printf("Obtido: "); int2bin(aux);

            // Aciona a função que formata o byte
            utf8ByteFormat(&aux,&final[0],7);

            //printf("\nRetornado: "); char2bin(final[0]); printf("\n\n");

            fwrite(&final[0],sizeof(char),1,saida);
            zera_vetor(final, 4); // Zera os bytes auxiliares
        }
        
        // 2 bytes
        else if (aux < 2047 && aux > 127 ){

            //printf("2 bytes\n");
            //printf("Obtido: "); int2bin(aux);

            utf8ByteFormat(&aux,&final[1],6); // Aciona a função pra formatar o segundo byte
            final[1] = final[1] | 128; // Preenche as flags do segundo byte

            utf8ByteFormat(&aux,&final[0],5); // Aciona a função pra formatar o primeiro byte
            final[0] = final[0] | 192; // Preenche as flags do primeiro byte

            //printf("\nRetornado: "); char2bin(final[0]); char2bin(final[1]); printf("\n\n");

            fwrite(&final[0],sizeof(char),1,saida);
            fwrite(&final[1],sizeof(char),1,saida);

            zera_vetor(final, 4); // Zera os bytes auxiliares
        }
        
        // 3 bytes
        else if (aux < 65535 && aux > 2047){

            //printf("3 bytes\n");
            //printf("Obtido: "); int2bin(aux);

            utf8ByteFormat(&aux,&final[2],6); // Aciona a função pra formatar o terceiro byte
            final[2] = final[2] | 128; // Preenche as flags do terceiro byte

            utf8ByteFormat(&aux,&final[1],6); // Aciona a função pra formatar o segundo byte
            final[1] = final[1] | 128; // Preenche as flags do segundo byte

            utf8ByteFormat(&aux,&final[0],4); // Aciona a função pra formatar o primeiro byte
            final[0] = final[0] | 224; // Preenche as flags do primeiro byte

            //printf("\nRetornado: "); char2bin(final[0]); char2bin(final[1]); char2bin(final[2]); printf("\n\n");

            fwrite(&final[0],sizeof(char),1,saida);
            fwrite(&final[1],sizeof(char),1,saida);
            fwrite(&final[2],sizeof(char),1,saida);

            zera_vetor(final, 4); // Zera os bytes auxiliares
        }
        
        // 4 bytes
        else if (aux < 1114111 && aux > 65535){

            //printf("4 bytes\n");
            //printf("Obtido: "); int2bin(aux);

            utf8ByteFormat(&aux,&final[3],6); // Aciona a função pra formatar o terceiro byte
            final[3] = final[3] | 128; // Preenche as flags do terceiro byte

            utf8ByteFormat(&aux,&final[2],6); // Aciona a função pra formatar o segundo byte
            final[2] = final[2] | 128; // Preenche as flags do segundo byte

            utf8ByteFormat(&aux,&final[1],6); // Aciona a função pra formatar o primeiro byte
            final[1] = final[1] | 128; // Preenche as flags do primeiro byte

            utf8ByteFormat(&aux,&final[0],4); // Aciona a função pra formatar o primeiro byte
            final[0] = final[0] | 240; // Preenche as flags do primeiro byte

            //printf("\nRetornado: "); char2bin(final[0]); char2bin(final[1]); char2bin(final[2]); char2bin(final[3]); printf("\n\n");

            fwrite(&final[0],sizeof(char),1,saida);
            fwrite(&final[1],sizeof(char),1,saida);
            fwrite(&final[2],sizeof(char),1,saida);
            fwrite(&final[3],sizeof(char),1,saida);

            zera_vetor(final, 4); // Zera os bytes auxiliares
        }

    }
}

// Move os 2 últimos bits de um char para os 2 1os de outro
void move_between(unsigned char *byte1, unsigned char *byte2){
    char copy;
    copy = *(byte1) & 3;
    if (copy == 1) *(byte2) = *(byte2) | 64; // joga 01 para o 2o byte
    else if (copy == 2) *(byte2) = *(byte2) | 128; // joga 10 para o 2o byte
    else if (copy == 3) *(byte2) = *(byte2) | 192; // joga 11 para o 2o byte
}

// Monta um byte nas com base na quantidade de bits disponíveis pra uso
void utf8ByteFormat(unsigned int *num, unsigned char *destiny, int capacity){
    unsigned char copy = 0;
    int potencia = 0;
    for(int i=0; i<capacity; i++){
        copy = *num & 1;
        *num = *num >> 1;
        copy = copy << potencia++;
        *destiny = *destiny | copy;
        copy = 0;
    }
}

// Exibe 8 bits
void char2bin(unsigned char c) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (c >> i) & 1);
        if (i%4==0) printf(" ");
    }
}

// Exibe 16 bits
void short2bin(short n) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i%4==0) printf(" ");
    }
}

// Exibe 32 bits
void int2bin(int n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i%4==0) printf(" ");
    }
}

// Inverte os bits de um número inteiro
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

// Zera um vetor
void zera_vetor(unsigned char *vetor, int tam){
    for(int i=0; i<tam; i++){
        vetor[i] = 0;
    }
}