#include <stdio.h>
#include <stdlib.h>

#include "aux.h"

// Converte um arquivo UTF-8 para UTF-32
void utf8to32(FILE* entrada, FILE* saida){

    unsigned char byte[4]; // armazena os byte lidos
    unsigned int extended = 0; // armazena o caractere convertido para UTF-32

    // Escreve o BOM em Big Endian
    unsigned int bom = 0x0000FEFF;
    fwrite(&bom, sizeof(int), 1, saida);

    while (!feof(entrada)){

        fread(&byte[0], sizeof(char), 1, entrada); // lê 1 byte do arquivo

        // 1 byte
        if (!(byte[0] & 128)){
            extended = extended | byte[0]; // copia o byte para um inteiro
            fwrite(&extended,sizeof(int),1,saida); // escreve o inteiro
            extended = 0; // zera o inteiro
        }
        // 2 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && !(byte[0] & 32)){
            fread(&byte[1], sizeof(char), 1, entrada); // lê +1 byte do arquivo

            byte[0] = byte[0] & 63; // esvazia os 2 primeiros bits do 1o byte
            byte[1] = byte[1] & 127; // esvazia o primeiro bit do 2o byte

            // Move os 2 últimos bits do 1o byte para o 2o + Anda com o 1o byte
            move_between(&byte[0],&byte[1]); byte[0] = byte[0] >> 2;

            // Copia os 2 bytes separados para um inteiro
            for(int i=0; i<2; i++){
                extended = extended | byte[i];
                if (i!=1) extended = extended << 8;
            }
            
            fwrite(&extended,sizeof(int),1,saida); // escreve o inteiro        
            extended = 0; // zera o inteiro
        }
        // 3 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && (byte[0] & 32) && !(byte[0] & 16)){
            // Lê mais 2 bytes
            for(int i=1; i<3; i++){ fread(&byte[i], sizeof(char), 1, entrada); }

            byte[0] = byte[0] & 31; // esvazia os 3 1os bits do 1o byte
            
            // Esvazia o primeiro bit do 2o e 3o bytes
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

            extended = 0;
        }
        // 4 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && (byte[0] & 32) && (byte[0] & 16)){
            // Lê mais 3 bytes
            for(int i=1; i<4; i++){ fread(&byte[i], sizeof(char), 1, entrada); }

            byte[0] = byte[0] & 15; // esvazia os 4 1os bits do 1o byte

            // Esvazia o primeiro bit do 2o, 3o e 4o bytes
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
            
            extended = 0;
        }
    }

}

// Converte um arquivo UTF-32 para UTF-8
void utf32to8(FILE* entrada, FILE* saida){

    unsigned int bom; // armazena o valor BOM
    unsigned int aux; // armazena o inteiro lido
    unsigned char final[] = {0,0,0,0}; // cada byte montado

    fread(&bom, sizeof(int), 1, entrada); // lê o BOM
    
    while (!feof(entrada)){
        fread(&aux, sizeof(int), 1, entrada); // lê 1 inteiro

        // Inverte os bits do inteiro se ele estiver em Little Endian
        if (bom == 0xFFFE0000) little2big(&aux);

        // 1 byte
        if (aux < 127){
            utf8ByteFormat(&aux,&final[0],7); // aciona a função que formatará o byte
            fwrite(&final[0],sizeof(char),1,saida); // escreve o byte
            zera_vetor(final, 4); // zera os bytes auxiliares
        }
        // 2 bytes
        else if (aux < 2047 && aux > 127 ){
            utf8ByteFormat(&aux,&final[1],6); // aciona a função que formatará o 2o byte
            final[1] = final[1] | 128; // preenche as flags do 2o byte

            utf8ByteFormat(&aux,&final[0],5); // aciona a função pra formatará o 1o byte
            final[0] = final[0] | 192; // preenche as flags do 1o byte

            for(int i=0; i<2; i++){
                fwrite(&final[i],sizeof(char),1,saida); // escreve o 1o e 2o bytes
            }

            zera_vetor(final, 4); // zera os bytes auxiliares
        }
        // 3 bytes
        else if (aux < 65535 && aux > 2047){
            for(int i=2; i>0; i--){
                utf8ByteFormat(&aux,&final[i],6); // aciona a função pra formatará o 2o e 3o bytes
                final[i] = final[i] | 128; // preenche as flags do 2o e 3o bytes
            }

            utf8ByteFormat(&aux,&final[0],4); // aciona a função pra formatará o 1o byte
            final[0] = final[0] | 224; // preenche as flags do 1o byte

            for(int i=0; i<3; i++){
                fwrite(&final[i],sizeof(char),1,saida); // escreve o 1o, 2o e 3o bytes
            }

            zera_vetor(final, 4); // zera os bytes auxiliares
        }
        // 4 bytes
        else if (aux < 1114111 && aux > 65535){
            for(int i=3; i>0; i--){
                utf8ByteFormat(&aux,&final[i],6); // aciona a função pra formatará o 2o, 3o e 4o bytes
                final[i] = final[i] | 128; // preenche as flags do 2o, 3o e 4o bytes
            }

            utf8ByteFormat(&aux,&final[0],4); // aciona a função pra formatar o 1o byte
            final[0] = final[0] | 240; // preenche as flags do 1o byte

            for(int i=0; i<4; i++){
                fwrite(&final[i],sizeof(char),1,saida); // escreve o 1o, 2o, 3o e 4o bytes
            }

            zera_vetor(final, 4); // zera os bytes auxiliares
        }

    }
}

// Move os 2 últimos bits de um char para os 2 primeiros de outro
void move_between(unsigned char *byte1, unsigned char *byte2){
    char copy;
    copy = *(byte1) & 3;
    if (copy == 1) *(byte2) = *(byte2) | 64; // joga 01 para o 2o byte
    else if (copy == 2) *(byte2) = *(byte2) | 128; // joga 10 para o 2o byte
    else if (copy == 3) *(byte2) = *(byte2) | 192; // joga 11 para o 2o byte
}

// Monta um byte nas com base na quantidade de bits disponíveis pra uso
void utf8ByteFormat(unsigned int *num, unsigned char *destiny, int capacity){
    unsigned char copy = 0; // armazenará o bit copiado
    int potencia = 0; // auxiliar para mexer na posição do bit
    for(int i=0; i<capacity; i++){
        copy = *num & 1; // copia o último bit
        *num = *num >> 1; // move o número para direita
        copy = copy << potencia++; // move o bit copiado para a posição certa
        *destiny = *destiny | copy; // joga o bit copiado o destino
        copy = 0; // zera o bit copiado
    }
}

// Inverte os bits de um número inteiro
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

// Zera um vetor
void zera_vetor(unsigned char *vetor, int tam){
    for(int i=0; i<tam; i++){
        vetor[i] = 0;
    }
}