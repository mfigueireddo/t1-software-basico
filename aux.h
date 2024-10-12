#include <stdio.h>
#include <stdlib.h>

// Funções principais
void utf8to32(FILE* entrada, FILE* saida);
void utf32to8(FILE* entrada, FILE* saida);
// Move bits entre bytes separados
void move_between(unsigned char* byte1, unsigned char* byte2);
// Exibe variáveis em binário
void char2bin(unsigned char c);
void short2bin(short n);
void int2bin(int n);
// Operações com vetores
void inverte_int(unsigned int* original);
void zera_vetor(unsigned char* vetor, int tam);