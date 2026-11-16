#ifndef ENTRADAS_H
#define ENTRADAS_H
#include <stdbool.h>
int entradaINT(int *variable);
int entradaSHORT(short *variable);
int entradaFLOAT(float *variable);
int entradaDOUBLE(double *variable);
int entradaCHAR(char *variable);
int entradaBOOL(bool *variable, char *text, unsigned int estado);
int entradaLONG(long *variable);
int entradaTEXTO(char *text, unsigned int longitud);
int entradaUINT(unsigned int *variable);
int entradaUSHORT(unsigned short *variable);
int entradaLDOUBLE(long double *variable);
int entradaUCHAR(unsigned char *variable, unsigned int longitud);
int entradaULONG(unsigned long *variable);
int entradaLLONG(long long *variable);
int entradaULLong(unsigned long long *variable);
int entradaBYTE(unsigned char *variable);
#endif