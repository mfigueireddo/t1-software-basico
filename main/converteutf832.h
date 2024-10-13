#include <stdio.h>
#include <stdlib.h>

// Funções principais
int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida);
int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida);

// Move bits entre bytes separados
void move_between(unsigned char *byte1, unsigned char *byte2);

// Monta um byte nas com base na quantidade de bits disponíveis pra uso
void utf8ByteFormat(unsigned int *num, unsigned char *destiny, int capacity);

// Transformação entre Big Endian e Little Endian
void little2big(unsigned int *num);
void big2little(unsigned int *num);

// Zera N vetores
void zera_vetor(unsigned char *vetor, int tam);

// Funções de dump
void dumpUtf8(FILE* arq1, FILE* arq2);
void dumpUtf32(FILE *arq1, FILE *arq2);
void char2bin(unsigned char c);
void int2bin(unsigned int n);