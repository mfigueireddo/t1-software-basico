#include <stdio.h>
#include <stdlib.h>

void dumpUtf8(FILE* arq);
void dumpUtf32(FILE* arq);
void char2bin(unsigned char c);
void int2bin(unsigned int n);
void little2big(unsigned int *num);