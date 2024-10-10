#include <stdio.h>
#include <stdlib.h>

void utf8to32(FILE* entrada, FILE* saida);
void utf32to8(FILE* entrada, FILE* saida);
void move_between(unsigned char* byte1, unsigned char* byte2);
void char2bin(unsigned char c);
void int2bin(int n);
void short2bin(int n);
void exibe_bin(unsigned char* a, int b, int tam);
void exibe(unsigned char* a, int b, int tam);
void inverte_int(unsigned int* original);
void zera_vetor(unsigned char* vetor, int tam);