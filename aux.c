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
    printf("0x%04x BOM\n\n", bom & 0xFFFF);

    while (!feof(entrada)){

        fread(&byte[0], sizeof(char), 1, entrada);

        // 1 byte
        if (!(byte[0] & 128)){
            extended = extended | byte[0];
            fwrite(&extended,sizeof(char),1,saida);
            // exibe(byte,extended,1);
            printf("\n\n1 byte\n");
            exibe_bin(byte,extended,1);
            extended = 0;

        }
        // 2 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && !(byte[0] & 32)){
            fread(&byte[1], sizeof(char), 1, entrada);
            
            printf("\n\n2 bytes\n");
            printf("Recebido: ");
            for(int i=0; i<2; i++){
                char2bin(byte[i]);
            }

            byte[0] = byte[0] & 63; // esvazia os 2 primeiros bits do primeiro byte
            byte[1] = byte[1] & 127; // esvazia o primeiro bit do segundo byte

            move_between(&byte[0],&byte[1]); // move os 2 últimos bits do primeiro byte para o segundo

            // Copia os 2 bytes separados para uma variável de 4 bytes
            for(int i=0; i<2; i++){
                extended = extended | byte[i];
                if (i!=1) extended = extended << 8;
            }
            
            fwrite(&extended,sizeof(int),1,saida);
            printf("\nResultado: ");
            int2bin(extended);
            extended = 0;

        }
        // 3 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && (byte[0] & 32) && !(byte[0] & 16)){
            fread(&byte[1], sizeof(char), 1, entrada);
            fread(&byte[2], sizeof(char), 1, entrada);

            printf("\n\n3 bytes\n");
            printf("Recebido: ");
            for(int i=0; i<3; i++){
                char2bin(byte[i]);
            }

            byte[0] = byte[0] & 31; // esvazia os 3 primeiros bits do primeiro byte
            
            // Esvazia o primeiro bit do segundo e terceiro bytes
            for (int i=1; i<3; i++){
                byte[i] = byte[i] & 127;
            }

            move_between(&byte[0],&byte[1]); // move os 2 últimos bits do primeiro byte para o segundo
            move_between(&byte[1],&byte[2]); // move os 2 últimos bits do segundo byte para o terceiro

            // Copia os 3 bytes separados para uma variável de 4 bytes
            for(int i=0; i<3; i++){
                extended = extended | byte[i];
                if (i!=2) extended = extended << 8;
            }

            fwrite(&extended,sizeof(int),1,saida);
            printf("\nResultado: ");
            int2bin(extended);
            extended = 0;

        }
        // 4 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && (byte[0] & 32) && (byte[0] & 16)){
            fread(&byte[1], sizeof(char), 1, entrada);
            fread(&byte[2], sizeof(char), 1, entrada);
            fread(&byte[3], sizeof(char), 1, entrada);

            printf("\n\n4 bytes\n");
            printf("Recebido: ");
            for(int i=0; i<4; i++){
                char2bin(byte[i]);
            }

            byte[0] = byte[0] & 15; // esvazia os 4 primeiros bits do primeiro byte

            // esvazia o primeiro bit do segundo, terceiro e quarto bytes
            for (int i=1; i<4; i++){
                byte[i] = byte[i] & 127;
            }

            move_between(&byte[0],&byte[1]); // move os 2 últimos bits do primeiro byte para o segundo
            move_between(&byte[1],&byte[2]); // move os 2 últimos bits do segundo byte para o terceiro
            move_between(&byte[2],&byte[3]); // move os 2 últimos bits do terceiro byte para o quarto

            // Copia os 4 bytes separados para uma variável de 4 bytes
            for(int i=0; i<4; i++){
                extended = extended| byte[i];
                if (i!=3) extended = extended << 8;
            }

            fwrite(&extended,sizeof(int),1,saida);
            printf("\nResultado: ");
            int2bin(extended);
            extended = 0;
        }

    }

}

// Converte um arquivo UTF-32 para UTF-8
void utf32to8(FILE* entrada, FILE* saida){

    // Armazena o valor BOM
    short bom;

    // Armazena os inteiros lidos
    unsigned int aux;

    // Cópia de um bit do inteiro
    char copy;

    // Versão final do byte
    unsigned char final[] = {0,0,0,0};

    // Armazena a potência de 2 sendo trabalhada
    int potencia;

    fread(&bom, sizeof(short), 1, entrada);
    printf("0x%04x BOM\n\n", bom & 0xFFFF);
    
    while (!feof(entrada)){

        fread(&aux, sizeof(int), 1, entrada);
        inverte_int(&aux);

        // 1 byte
        if (aux < 127){
            printf("\n\n1 byte\n");
            printf("Recebido: ");
            int2bin(aux);
            // Monta o byte
            potencia = 0;
            // Preenche os bits disponíveis
            for(int i=0; i<7; i++){
                copy = aux & 1;
                aux = aux >> 1;
                copy = copy << potencia++;
                final[0] = final[0] | copy;
            }
            
            printf("\nResultado: ");
            char2bin(final[0]);

            fwrite(&final[0],sizeof(char),1,saida);
            zera_vetor(final, 4);
        }
        
        // 2 bytes
        else if (aux < 2047 && aux > 127 ){
            printf("\n\n2 bytes\n");
            printf("Recebido: ");
            int2bin(aux);
            // Monta o segundo byte
            potencia = 0;
            // Preenche os bits disponíveis
            for(int i=0; i<7; i++){
                copy = aux & 1;
                aux = aux >> 1;
                copy = copy << potencia++;
                final[1] = final[1] | copy;
            }
            // Preenche as flags
            final[1] = final[1] | 128;

            // Monta o primeiro byte
            potencia = 0;
            // Preenche os bits disponíveis
            for(int i=0; i<5; i++){
                copy = aux & 1;
                aux = aux >> 1;
                copy = copy << potencia++;
                final[0] = final[0] | copy;
            }
            // Preenche as flags
            final[0] = final[0] | 192;

            printf("\nResultado: ");
            for(int i=0; i<2; i++){
                char2bin(final[i]);
            }

            fwrite(&final[0],sizeof(char),1,saida);
            fwrite(&final[1],sizeof(char),1,saida);
            zera_vetor(final, 4);
        }
        
        // 3 bytes
        else if (aux < 65535 && aux > 2047){

            printf("\n\n3 bytes\n");
            printf("Recebido: ");
            int2bin(aux);

            // Monta o terceiro byte
            // Preenche os bits disponíveis
            potencia = 0;
            for(int i=0; i<7; i++){
                copy = aux & 1;
                aux = aux >> 1;
                copy = copy << potencia++;
                final[2] = final[2] | copy;
            }
            // Preenche as flags
            final[2] = final[2] | 128;

            // Monta o segundo byte
            potencia = 0;
            // Preenche os bits disponíveis
            for(int i=0; i<7; i++){
                copy = aux & 1;
                aux = aux >> 1;
                copy = copy << potencia++;
                final[1] = final[1] | copy;
            }
            // Preenche as flags
            final[1] = final[1] | 128;

            // Monta o primeiro byte
            potencia = 0;
            // Preenche os bits disponíveis
            for(int i=0; i<4; i++){
                copy = aux & 1;
                aux = aux >> 1;
                copy = copy << potencia++;
                final[0] = final[0] | copy;
            }
            // Preenche as flags
            final[0] = final[0] | 224;

            printf("\nResultado: ");
            for(int i=0; i<3; i++){
                char2bin(final[i]);
            }

            fwrite(&final[0],sizeof(char),1,saida);
            fwrite(&final[1],sizeof(char),1,saida);
            fwrite(&final[2],sizeof(char),1,saida);
            zera_vetor(final, 4);
        }
        
        // 4 bytes
        else if (aux < 1114111 && aux > 65535){

            printf("\n\n4 bytes\n");
            printf("Recebido: ");
            int2bin(aux);

            // Monta o quarto byte
            // Preenche os bits disponíveis
            potencia = 0;
            for(int i=0; i<7; i++){
                copy = aux & 1;
                aux = aux >> 1;
                copy = copy << potencia++;
                final[3] = final[3] | copy;
            }
            // Preenche as flags
            final[3] = final[3] | 128;

            // Monta o terceiro byte
            potencia = 0;
            // Preenche os bits disponíveis
            for(int i=0; i<7; i++){
                copy = aux & 1;
                aux = aux >> 1;
                copy = copy << potencia++;
                final[2] = final[2] | copy;
            }
            // Preenche as flags
            final[2] = final[2] | 128;

            // Monta o segundo byte
            potencia = 0;
            // Preenche os bits disponíveis
            for(int i=0; i<3; i++){
                copy = aux & 1;
                aux = aux >> 1;
                copy = copy << potencia++;
                final[1] = final[1] | copy;
            }
            // Preenche as flags
            final[0] = final[0] | 240;

            printf("\nResultado: ");
            for(int i=0; i<4; i++){
                char2bin(final[i]);
            }

            fwrite(&final[0],sizeof(char),1,saida);
            fwrite(&final[1],sizeof(char),1,saida);
            fwrite(&final[2],sizeof(char),1,saida);
            fwrite(&final[3],sizeof(char),1,saida);
            zera_vetor(final, 4);
        }

    }
}

// Move os 2 últimos bits de um char para os 2 primeiros de outro
void move_between(unsigned char* byte1, unsigned char* byte2){
    char copy;
    copy = *(byte1) & 3;
    if (copy == 1) *(byte2) = *(byte2) | 64; // joga 01 para o segundo byte
    else if (copy == 2) *(byte2) = *(byte2) | 128; // joga 10 para o segundo byte
    else if (copy == 3) *(byte2) = *(byte2) | 192; // joga 11 para o segundo byte
    *byte1 = *byte1 >> 2;
}

// Exibe 8 bits
void char2bin(unsigned char c) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (c >> i) & 1);
        if (i%4==0) printf(" ");
    }
}

void short2bin(int n) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i%4==0) printf(" ");
    }
    printf("\n\n");
}

// Exibe 32 bits
void int2bin(int n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i%4==0) printf(" ");
    }
}

// Exibe antes e depois da UTF-8 para UTF-32 em binário
void exibe_bin(unsigned char* a, int b, int tam){
    printf("Recebido: ");
    for(int i=0; i<tam; i++){
        char2bin(a[i]); 
        printf("\t");
    }
    printf("\nResultado: ");
    int2bin(b);
}

// Exibe antes e depois da UTF-8 para UTF-32
void exibe(unsigned char* a, int b, int tam){
    for(int i=0; i<tam; i++){
        printf("Recebido: %d\n", a[i]);
    }
    printf("Resultado: %d\n\n", b);
}

// Inverte os bits de um número inteiro
void inverte_int(unsigned int* original){

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

void zera_vetor(unsigned char* vetor, int tam){
    for(int i=0; i<tam; i++){
        vetor[i] = 0;
    }
}