/* Matheus Moreira da Silva Figueiredo 2320813 3WA*/

#include <stdio.h>
#include <stdlib.h>

#include "converteutf832.h"

// Converte um arquivo UTF-8 para UTF-32
int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida){

    unsigned char byte[4]; // armazena os byte lidos
    unsigned int extended = 0; // armazena o caractere convertido para UTF-32
    int end; // variável para identificar o fim do arquivo

    // Escreve o BOM em Big Endian
    unsigned int bom = 0xFFFE0000;
    
    if (fwrite(&bom, sizeof(int), 1, arquivo_saida) != 1){
        fprintf(stderr, "Erro: Falha na escrita.");
        return -1;
    }

    while (1){
        // Lê 1 byte do arquivo
        end = fread(&byte[0], sizeof(char), 1, arquivo_entrada);

        if (end != 1){
            if (feof(arquivo_entrada)) break;
            else {
                fprintf(stderr, "Erro: Falha na leitura.");
                return -1;
            }
        }

        // 1 byte
        if (!(byte[0] & 128)){
            extended = extended | byte[0]; // copia o byte para um inteiro
            big2little(&extended); // transforma de big para little endian
            // Escreve o inteiro
            if (fwrite(&extended,sizeof(int),1,arquivo_saida) != 1){
                fprintf(stderr, "Erro: Falha na escrita.");
                return -1;
            }
            extended = 0; // zera o inteiro
        }
        // 2 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && !(byte[0] & 32)){
            // Lê +1 byte do arquivo
            if (fread(&byte[1], sizeof(char), 1, arquivo_entrada) != 1){
                fprintf(stderr, "Erro: Falha na leitura.");
                return -1;
            }

            byte[0] = byte[0] & 63; // esvazia os 2 primeiros bits do 1o byte
            byte[1] = byte[1] & 127; // esvazia o primeiro bit do 2o byte

            // Move os 2 últimos bits do 1o byte para o 2o + Anda com o 1o byte
            move_between(&byte[0],&byte[1]); byte[0] = byte[0] >> 2;

            // Copia os 2 bytes separados para um inteiro
            for(int i=0; i<2; i++){
                extended = extended | byte[i];
                if (i!=1) extended = extended << 8;
            }
            
            big2little(&extended); // transforma de big para little endian
            // Escreve o inteiro
            if (fwrite(&extended,sizeof(int),1,arquivo_saida) != 1){
                fprintf(stderr, "Erro: Falha na escrita.");
                return -1;
            }       
            extended = 0; // zera o inteiro
        }
        // 3 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && (byte[0] & 32) && !(byte[0] & 16)){
            // Lê mais 2 bytes
            for(int i=1; i<3; i++){ 
                if (fread(&byte[i], sizeof(char), 1, arquivo_entrada) != 1){
                    fprintf(stderr, "Erro: Falha na leitura.");
                    return -1;
                } 
            }

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

            big2little(&extended); // transforma de big para little endian

            // Escreve o inteiro
            if (fwrite(&extended,sizeof(int),1,arquivo_saida) != 1){
                fprintf(stderr, "Erro: Falha na escrita.");
                return -1;
            }
            extended = 0;
        }
        // 4 bytes
        else if ((byte[0] & 128) && (byte[0] & 64) && (byte[0] & 32) && (byte[0] & 16)){
            // Lê mais 3 bytes
            for(int i=1; i<4; i++){ 
                if (fread(&byte[i], sizeof(char), 1, arquivo_entrada) != 1){
                    fprintf(stderr, "Erro: Falha na leitura.");
                    return -1;
                } 
            }

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

            big2little(&extended); // transforma de big para little endian
            if (fwrite(&extended,sizeof(int),1,arquivo_saida) != 1){
                fprintf(stderr, "Erro: Falha na escrita.");
                return -1;
            }
            extended = 0;
        }
    
    }

    return 0;
}

// Converte um arquivo UTF-32 para UTF-8
int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida){

    unsigned int bom; // armazena o valor BOM
    unsigned int aux; // armazena o inteiro lido
    unsigned char final[] = {0,0,0,0}; // cada byte montado
    int end; // variável para identificar o fim do arquivo

    // Lê o BOM
    if (fread(&bom, sizeof(int), 1, arquivo_entrada) != 1){
        fprintf(stderr, "Erro: Falha na leitura.");
        return -1;
    }

    if (bom != 0xFFFE0000 && bom != 0x0000FEFF){
        fprintf(stderr, "Erro: Caractere BOM inválido.");
        return -1;
    }
    
    while (1){
        // Lê 1 inteiro
        end = fread(&aux, sizeof(int), 1, arquivo_entrada);
        
        if (end != 1){
            if (feof(arquivo_entrada)) break;
            else{
                fprintf(stderr, "Erro: Falha na leitura.");
                return -1;
            }
        };

        // Transforma o inteiro em Big Endian se ele estiver em Little Endian
        if (bom == 0xFFFE0000) little2big(&aux);

        // 1 byte
        if (aux < 127){
            utf8ByteFormat(&aux,&final[0],7); // aciona a função que formatará o byte
            // Escreve o byte
            if (fwrite(&final[0],sizeof(char),1,arquivo_saida) != 1){
                fprintf(stderr, "Erro: Falha na escrita.");
                return -1;
            }
            zera_vetor(final, 4); // zera os bytes auxiliares
        }
        // 2 bytes
        else if (aux < 2047 && aux > 127 ){
            utf8ByteFormat(&aux,&final[1],6); // aciona a função que formatará o 2o byte
            final[1] = final[1] | 128; // preenche as flags do 2o byte

            utf8ByteFormat(&aux,&final[0],5); // aciona a função pra formatará o 1o byte
            final[0] = final[0] | 192; // preenche as flags do 1o byte

            // Escreve o 1o e 2o bytes
            for(int i=0; i<2; i++){
                if (fwrite(&final[i],sizeof(char),1,arquivo_saida) != 1){
                    fprintf(stderr, "Erro: Falha na escrita.");
                    return -1;
                }
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

            // Escreve o 1o, 2o e 3o bytes
            for(int i=0; i<3; i++){
                if (fwrite(&final[i],sizeof(char),1,arquivo_saida) != 1){
                    fprintf(stderr, "Erro: Falha na escrita.");
                    return -1;
                }
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

            // Escreve o 1o, 2o, 3o e 4o bytes
            for(int i=0; i<4; i++){
                if (fwrite(&final[i],sizeof(char),1,arquivo_saida) != 1){
                    fprintf(stderr, "Erro: Falha na escrita.");
                    return -1;
                }
            }

            zera_vetor(final, 4); // zera os bytes auxiliares
        }
    }

    return 0;
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

// Converte de Little Endian para Big Endian
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

// Converte de Big Endian para Little Endian
void big2little(unsigned int *num) {
    // Reorganizando os bytes do número
    unsigned int byte0, byte1, byte2, byte3;
    unsigned int value = *num;  // Obtém o valor original do ponteiro

    byte0 = (value & 0xFF000000) >> 24;  // Move o byte 3 para a posição 0
    byte1 = (value & 0x00FF0000) >> 8;   // Move o byte 2 para a posição 1
    byte2 = (value & 0x0000FF00) << 8;   // Move o byte 1 para a posição 2
    byte3 = (value & 0x000000FF) << 24;  // Move o byte 0 para a posição 3

    *num = byte0 | byte1 | byte2 | byte3; // Atualiza o valor original com a nova ordem de bytes
}

// Zera um vetor
void zera_vetor(unsigned char *vetor, int tam){
    for(int i=0; i<tam; i++){
        vetor[i] = 0;
    }
}

// Lê um arquivo UTF-8 em binário
void dumpUtf8(FILE *arq1, FILE *arq2){

    unsigned char byte1[4];  
    unsigned char byte2[4];
    int end1, end2;

    while (1){
        end1 = fread(&byte1[0], sizeof(char), 1, arq1);
        end2 = fread(&byte2[0], sizeof(char), 1, arq2);

        if (end1 != 1 || end2 != 1) break;

        printf("Original:");
        // 1 byte
        if (!(byte1[0] & 128)){
            printf(" 1 byte ");
            char2bin(byte1[0]); printf("\n");
        }
        // 2 bytes
        else if ((byte1[0] & 128) && (byte1[0] & 64) && !(byte1[0] & 32)){
            fread(&byte1[1], sizeof(char), 1, arq1);

            printf(" 2 bytes ");
            char2bin(byte1[0]); char2bin(byte1[1]); printf("\n");
        }
        // 3 bytes
        else if ((byte1[0] & 128) && (byte1[0] & 64) && (byte1[0] & 32) && !(byte1[0] & 16)){
            fread(&byte1[1], sizeof(char), 1, arq1);
            fread(&byte1[2], sizeof(char), 1, arq1);

            printf(" 3 bytes ");
            char2bin(byte1[0]); char2bin(byte1[1]); char2bin(byte1[2]); printf("\n");
        }
        // 4 bytes
        else if ((byte1[0] & 128) && (byte1[0] & 64) && (byte1[0] & 32) && (byte1[0] & 16)){
            fread(&byte1[1], sizeof(char), 1, arq1);
            fread(&byte1[2], sizeof(char), 1, arq1);
            fread(&byte1[3], sizeof(char), 1, arq1);

            printf(" 4 bytes ");
            char2bin(byte1[0]); char2bin(byte1[1]); char2bin(byte1[2]); char2bin(byte1[3]); printf("\n");
        }
    
        printf("Resultado:");
        // 1 byte
        if (!(byte2[0] & 128)){
            printf(" 1 byte ");
            char2bin(byte2[0]); printf("\n");
        }
        // 2 bytes
        else if ((byte2[0] & 128) && (byte2[0] & 64) && !(byte2[0] & 32)){
            fread(&byte2[1], sizeof(char), 1, arq2);

            printf(" 2 bytes ");
            char2bin(byte1[0]); char2bin(byte1[1]); printf("\n");
        }
        // 3 bytes
        else if ((byte2[0] & 128) && (byte2[0] & 64) && (byte2[0] & 32) && !(byte2[0] & 16)){
            fread(&byte2[1], sizeof(char), 1, arq2);
            fread(&byte2[2], sizeof(char), 1, arq2);

            printf(" 3 bytes ");
            char2bin(byte2[0]); char2bin(byte2[1]); char2bin(byte2[2]); printf("\n");
        }
        // 4 bytes
        else if ((byte2[0] & 128) && (byte2[0] & 64) && (byte2[0] & 32) && (byte2[0] & 16)){
            fread(&byte2[1], sizeof(char), 1, arq2);
            fread(&byte2[2], sizeof(char), 1, arq2);
            fread(&byte2[3], sizeof(char), 1, arq2);

            printf(" 4 bytes ");
            char2bin(byte2[0]); char2bin(byte2[1]); char2bin(byte2[2]); char2bin(byte2[3]); printf("\n");
        }
    
        printf("\n");
    }

}

// Lê um arquivo UTF-32 em binário
void dumpUtf32(FILE *arq1, FILE *arq2){

    unsigned int bom1, bom2, aux1, aux2;
    int end1, end2;

    fread(&bom1, sizeof(int), 1, arq1);
    printf("BOM Original: "); int2bin(bom1);

    fread(&bom2 , sizeof(int), 1, arq2);
    printf("\nBOM Resultante: "); int2bin(bom2); 
    printf("\nDisclaimer: o resultado sera impresso em Big Endian para melhor visualização"); printf("\n\n");

    while(1){
        end1 = fread(&aux1, sizeof(int), 1, arq1);
        end2 = fread(&aux2, sizeof(int), 1, arq2);
        
        if (end1 != 1 || end2 != 1) break;

        if (bom1 == 0xFFFE0000) little2big(&aux1);
        if (bom2 == 0xFFFE0000) little2big(&aux2);

        printf("Original: ");
        if (aux1 < 127) printf("1 byte ");
        else if (aux1 < 2047 && aux1 > 127 ) printf("2 bytes ");
        else if (aux1 < 65535 && aux1 > 2047) printf("3 bytes ");
        else if (aux1 < 1114111 && aux1 > 65535) printf("4 bytes ");
        int2bin(aux1);
        
        printf("\nResultado: ");
        if (aux2 < 127) printf("1 byte ");
        else if (aux2 < 2047 && aux2 > 127 ) printf("2 bytes ");
        else if (aux2 < 65535 && aux2 > 2047) printf("3 bytes ");
        else if (aux2 < 1114111 && aux2 > 65535) printf("4 bytes ");
        int2bin(aux2);
        printf("\n\n");
    }

}

// Lê um char em binário
void char2bin(unsigned char c) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (c >> i) & 1);
        if (i%4==0) printf(" ");
    }
}

// Lê um int em binário
void int2bin(unsigned int n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i%4==0) printf(" ");
    }
}