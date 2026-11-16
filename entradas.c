#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include <errno.h>

#include "entradas.h"
/*
-1 significa fuera del rango
-2 significa tipo de entrada incorrecta
-3 para *variable como nula
*/

static int limpiarBuffer(char *text){
    //Si el texto no contiene '\n' es invalido y no entra
    if(strchr(text, '\n') == NULL){
        int c;
        while((c = getchar()) != '\n' && c != EOF); //Limpiamos los residuos
        return -1;  //Si es demasiado largo de una vez lo cancelamos
    } else{
        return 0;   //Sin error
    }
}

static int verificacion(char *text, char *result){
    if(result == text){
        return -2;
    }
    while(isspace((unsigned char)*result)){
        result++;
    }
    if(*result != '\0'){
        return -1;
    }
    return 0;
}

static int verificacionUNS(char *text){
    char *inicio = text;
    while(isspace((unsigned char)*inicio)){
        inicio++;
    }
    if(*inicio == '-'){
        return -1;
    }
    return 0;
}

static int longitudVariables(int largo){
    //Dependiendo del tipo pedimos una cantidad de espacios
    switch(largo){
        case 0:{ // int
            int P = snprintf(NULL, 0, "%d", INT_MAX);
            int N = snprintf(NULL, 0, "%d", INT_MIN);
            if(P < 0 || N < 0){
                return -1;
            }
            return (P > N ? P : N) + 2;
        }
        case 1:{
            int P = snprintf(NULL, 0, "%hd", SHRT_MAX);
            int N = snprintf(NULL, 0, "%hd", SHRT_MIN);
            if(P < 0 || N < 0){
                return -1;
            }
            return (P > N ? P : N) + 2;
        }
        case 2:{
            int comparador1, comparador2, comparador3, comparador4;
            int maxP = snprintf(NULL, 0, "%.*g", FLT_DECIMAL_DIG, FLT_MAX);
            int maxN = snprintf(NULL, 0, "%.*g", FLT_DECIMAL_DIG, -FLT_MAX);
            int minP = snprintf(NULL, 0, "%.*g", FLT_DECIMAL_DIG, FLT_MIN);
            int minN = snprintf(NULL, 0, "%.*g", FLT_DECIMAL_DIG, -FLT_MIN);
            int subP = snprintf(NULL, 0, "%.*g", FLT_DECIMAL_DIG, FLT_TRUE_MIN);
            int subN = snprintf(NULL, 0, "%.*g", FLT_DECIMAL_DIG, -FLT_TRUE_MIN);
            if(maxP < 0 || maxN < 0 || minP < 0 || minN < 0 || subP < 0 || subN < 0){
                return -1;
            }
            comparador1 = maxP > maxN ? maxP : maxN;
            comparador2 = minP > minN ? minP : minN;
            comparador3 = subP > subN ? subP : subN;
            comparador4 = comparador1 > comparador2 ? comparador1 : comparador2;
            return (comparador4 > comparador3 ? comparador4 : comparador3) + 2;
        }
        case 3:{
            int comparador1, comparador2, comparador3, comparador4;
            int maxP = snprintf(NULL, 0, "%.*g", DBL_DECIMAL_DIG, DBL_MAX);
            int maxN = snprintf(NULL, 0, "%.*g", DBL_DECIMAL_DIG, -DBL_MAX);
            int minP = snprintf(NULL, 0, "%.*g", DBL_DECIMAL_DIG, DBL_MIN);
            int minN = snprintf(NULL, 0, "%.*g", DBL_DECIMAL_DIG, -DBL_MIN);
            int subP = snprintf(NULL, 0, "%.*g", DBL_DECIMAL_DIG, DBL_TRUE_MIN);
            int subN = snprintf(NULL, 0, "%.*g", DBL_DECIMAL_DIG, -DBL_TRUE_MIN);
            if(maxP < 0 || maxN < 0 || minP < 0 || minN < 0 || subP < 0 || subN < 0){
                return -1;
            }
            comparador1 = maxP > maxN ? maxP : maxN;
            comparador2 = minP > minN ? minP : minN;
            comparador3 = subP > subN ? subP : subN;
            comparador4 = comparador1 > comparador2 ? comparador1 : comparador2;
            return (comparador4 > comparador3 ? comparador4 : comparador3) + 2;
        }
        case 4:{
            int P = snprintf(NULL, 0, "%ld", LONG_MAX);
            int N = snprintf(NULL, 0, "%ld", LONG_MIN);
            if(P < 0 || N < 0){
                return -1;
            }
            return (P > N ? P : N) + 2;
        }
        case 5:{
            int P = snprintf(NULL, 0, "%u", UINT_MAX);
            if(P < 0){
                return -1;
            }
            return P + 2;
        }
        case 6:{
            int P = snprintf(NULL, 0, "%hu", USHRT_MAX);
            if(P < 0){
                return -1;
            }
            return P + 2;
        }
        case 7:{
            int comparador1, comparador2, comparador3, comparador4;
            int maxP = snprintf(NULL, 0, "%.*Lg", LDBL_DECIMAL_DIG, LDBL_MAX);
            int maxN = snprintf(NULL, 0, "%.*Lg", LDBL_DECIMAL_DIG, -LDBL_MAX);
            int minP = snprintf(NULL, 0, "%.*Lg", LDBL_DECIMAL_DIG, LDBL_MIN);
            int minN = snprintf(NULL, 0, "%.*Lg", LDBL_DECIMAL_DIG, -LDBL_MIN);
            int subP = snprintf(NULL, 0, "%.*Lg", LDBL_DECIMAL_DIG, LDBL_TRUE_MIN);
            int subN = snprintf(NULL, 0, "%.*Lg", LDBL_DECIMAL_DIG, -LDBL_TRUE_MIN);
            if(maxP < 0 || maxN < 0 || minP < 0 || minN < 0 || subP < 0 || subN < 0){
                return -1;
            }
            comparador1 = maxP > maxN ? maxP : maxN;
            comparador2 = minP > minN ? minP : minN;
            comparador3 = subP > subN ? subP : subN;
            comparador4 = comparador1 > comparador2 ? comparador1 : comparador2;
            return (comparador4 > comparador3 ? comparador4 : comparador3) + 2;
        }
        case 8:{
            int P = snprintf(NULL, 0, "%lu", ULONG_MAX);
            if(P < 0){
                return -1;
            }
            return P + 2;
        }
        case 9:{
            int P = snprintf(NULL, 0, "%lld", LLONG_MAX);
            int N = snprintf(NULL, 0, "%lld", LLONG_MIN);
            if(P < 0 || N < 0){
                return -1;
            }
            return (P > N ? P : N) + 2;
        }
        case 10:{
            int P = snprintf(NULL, 0, "%llu", ULLONG_MAX);
            if(P < 0){
                return -1;
            }
            return P + 2;
        }
        default :{
            return -1;
        }
    }
}

int entradaINT(int *variable){
    if(variable == NULL){
        return -3;
    }

    int cantidad = longitudVariables(0);
    if(cantidad <= 0){
        *variable = 0;
        return -1;
    }
    size_t espacios = (size_t)cantidad; //Manejamos mejor la longitud de la memoria

    char *entrada = malloc(espacios);   //Pedimos espacio
    if(entrada == NULL){
        *variable = 0;
        return -1;
    }

    if(fgets(entrada, espacios, stdin) == NULL){
        *variable = 0;
        free(entrada);
        return -1;
    }

    //Si el numero es muy grande no deja pasar
    if(limpiarBuffer(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    char *resultado;
    errno = 0;
    long valor = strtol(entrada, &resultado, 10);

    //Ultima puerta antes de retornar un valor
    int veric = verificacion(entrada, resultado);
    if(veric == -2){
        *variable = 0;
        free(entrada);
        return -2;
    }

    if(veric == -1 || errno == ERANGE || valor < INT_MIN || valor > INT_MAX){
        *variable = 0;
        free(entrada);
        return -1;
    }

    *variable = (int)valor;
    free(entrada);
    return 0;   //Si todo salio bien llegamos limpios
}

int entradaSHORT(short *variable){
    if(variable == NULL){
        return -3;
    }

    int cantidad = longitudVariables(1);
    if(cantidad <= 0){
        *variable = 0;
        return -1;
    }
    size_t espacios = (size_t)cantidad; //Manejamos mejor la longitud de la memoria

    char *entrada = malloc(espacios);   //Pedimos espacio
    if(entrada == NULL){
        *variable = 0;
        return -1;
    }

    if(fgets(entrada, espacios, stdin) == NULL){
        *variable = 0;
        free(entrada);
        return -1;
    }

    //Si el numero es muy grande no deja pasar
    if(limpiarBuffer(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    char *resultado;
    errno = 0;
    long valor = strtol(entrada, &resultado, 10);

    //Ultima puerta antes de retornar un valor
    int veric = verificacion(entrada, resultado);
    if(veric == -2){
        *variable = 0;
        free(entrada);
        return -2;
    }

    if(veric == -1 || errno == ERANGE || valor < SHRT_MIN || valor > SHRT_MAX){
        *variable = 0;
        free(entrada);
        return -1;
    }

    *variable = (short)valor;
    free(entrada);
    return 0;   //Si todo salio bien llegamos limpios
}

int entradaFLOAT(float *variable){
    if(variable == NULL){
        return -3;
    }

    int cantidad = longitudVariables(2);
    if(cantidad <= 0){
        *variable = 0;
        return -1;
    }
    size_t espacios = (size_t)cantidad; //Manejamos mejor la longitud de la memoria

    char *entrada = malloc(espacios);   //Pedimos espacio
    if(entrada == NULL){
        *variable = 0;
        return -1;
    }

    if(fgets(entrada, espacios, stdin) == NULL){
        *variable = 0;
        free(entrada);
        return -1;
    }

    //Si el numero es muy grande no deja pasar
    if(limpiarBuffer(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    char *resultado;
    errno = 0;
    float valor = strtof(entrada, &resultado);

    //Desbordamiento
    if(errno == ERANGE && (valor > FLT_MAX || valor < -FLT_MAX)){
        *variable = 0;
        free(entrada);
        return -1;
    }  

    //Ultima puerta antes de retornar un valor
    int veric = verificacion(entrada, resultado);
    if(veric == -2){
        *variable = 0;
        free(entrada);
        return -2;
    }
    if(veric == -1 || valor < -FLT_MAX || valor > FLT_MAX){
        *variable = 0;
        free(entrada);
        return -1;
    }

    *variable = valor;
    free(entrada);
    return 0;   //Si todo salio bien llegamos limpios
}

int entradaDOUBLE(double *variable){
    if(variable == NULL){
        return -3;
    }

    int cantidad = longitudVariables(3);
    if(cantidad <= 0){
        *variable = 0;
        return -1;
    }
    size_t espacios = (size_t)cantidad; //Manejamos mejor la longitud de la memoria

    char *entrada = malloc(espacios);   //Pedimos espacio
    if(entrada == NULL){
        *variable = 0;
        return -1;
    }

    if(fgets(entrada, espacios, stdin) == NULL){
        *variable = 0;
        free(entrada);
        return -1;
    }

    //Si el numero es muy grande no deja pasar
    if(limpiarBuffer(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    char *resultado;
    errno = 0;
    double valor = strtod(entrada, &resultado);

    //Desbordamiento
    if(errno == ERANGE && (valor > DBL_MAX || valor < -DBL_MAX)){
        *variable = 0;
        free(entrada);
        return -1;
    }  

    //Ultima puerta antes de retornar un valor
    int veric = verificacion(entrada, resultado);
    if(veric == -2){
        *variable = 0;
        free(entrada);
        return -2;
    }
    if(veric == -1 || valor < -DBL_MAX || valor > DBL_MAX){
        *variable = 0;
        free(entrada);
        return -1;
    }

    *variable = valor;
    free(entrada);
    return 0;   //Si todo salio bien llegamos limpios
}

int entradaCHAR(char *variable){
    if(variable == NULL){
        return -3;  //Nos aseguramos que no sea nulo
    }

    char entrada[3];
    if(fgets(entrada, sizeof(entrada), stdin) == NULL){
        *variable = '\0';
        return -1;
    }

    if(limpiarBuffer(entrada) < 0){
        *variable = '\0';
        return -1;
    }

    int veric = verificacion(entrada, entrada + 1); //Solo usamos la entrada para verificar

    //Pasa los filtros
    if(veric == -2){
        *variable = '\0';
        return -2;
    }
    if(veric == -1){
        *variable = '\0';
        return -1;
    }

    *variable = entrada[0];
    return 0;
}

int entradaBOOL(bool *variable, char *text, unsigned int estado){
    if(variable == NULL){
        return -1;  //Confirmamos que variable puede modificarse
    }
    //text representa caracteres permitidos
    if(text == NULL){
        *variable = (estado > 0 ? false : true);    //Confirmamos que text tiene los caracteres permitidos
        return -1;
    }

    size_t espacios = strlen(text) + 2; //text representa la longitud de la entrada mas '\0' y '\n'
    char *entrada = malloc(espacios);
    if(entrada == NULL){
        *variable = (estado > 0 ? false : true);
        return -1;
    }

    if(fgets(entrada, espacios, stdin) == NULL){
        *variable = (estado > 0 ? false : true);
        free(entrada);
        return -1;
    }

    if(limpiarBuffer(entrada) < 0){
        *variable = (estado > 0 ? false : true);
        free(entrada);
        return -1;
    }

    //estado representa una entrada normal si es mayor a 0 o invetida si es 0
    size_t result = strspn(entrada, text);
    bool condicion = (result == strlen(entrada) && result > 0); //Dependiendo de estado la variable sale normal o invertida
    *variable = (estado > 0) ? condicion : !condicion;  //Si se cumple la condicion sale equivalente a estado

    free(entrada);
    return 0;
}

int entradaLONG(long *variable){
    if(variable == NULL){
        return -3;
    }

    int cantidad = longitudVariables(4);
    if(cantidad <= 0){
        *variable = 0;
        return -1;
    }
    size_t espacios = (size_t)cantidad; //Manejamos mejor la longitud de la memoria

    char *entrada = malloc(espacios);   //Pedimos espacio
    if(entrada == NULL){
        *variable = 0;
        return -1;
    }

    if(fgets(entrada, espacios, stdin) == NULL){
        *variable = 0;
        free(entrada);
        return -1;
    }

    //Si el numero es muy grande no deja pasar
    if(limpiarBuffer(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    char *resultado;
    errno = 0;
    long valor = strtol(entrada, &resultado, 10);

    //Ultima puerta antes de retornar un valor
    int veric = verificacion(entrada, resultado);
    if(veric == -2){
        *variable = 0;
        free(entrada);
        return -2;
    }

    if(veric == -1 || errno == ERANGE){
        *variable = 0;
        free(entrada);
        return -1;
    }

    *variable = valor;
    free(entrada);
    return 0;   //Si todo salio bien llegamos limpios
}

int entradaTEXTO(char *text, unsigned int longitud){
    if(text == NULL || longitud < 2){
        return -3;
    }

    if(fgets(text, longitud, stdin) == NULL){
        *text = '\0';
        return -1;
    }

    if(limpiarBuffer(text) < 0){
        *text = '\0';
        return -1;
    }

    return 0;
}

int entradaUINT(unsigned int *variable){
    if(variable == NULL){
        return -3;
    }

    int cantidad = longitudVariables(5);
    if(cantidad <= 0){
        *variable = 0;
        return -1;
    }
    size_t espacios = (size_t)cantidad; //Manejamos mejor la longitud de la memoria

    char *entrada = malloc(espacios);   //Pedimos espacio
    if(entrada == NULL){
        *variable = 0;
        return -1;
    }

    if(fgets(entrada, espacios, stdin) == NULL){
        *variable = 0;
        free(entrada);
        return -1;
    }

    //Si el numero es muy grande no deja pasar
    if(limpiarBuffer(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    if(verificacionUNS(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    char *resultado;
    errno = 0;
    unsigned long valor = strtoul(entrada, &resultado, 10);

    //Ultima puerta antes de retornar un valor
    int veric = verificacion(entrada, resultado);
    if(veric == -2){
        *variable = 0;
        free(entrada);
        return -2;
    }

    if(veric == -1 || errno == ERANGE || valor > UINT_MAX){
        *variable = 0;
        free(entrada);
        return -1;
    }

    *variable = (unsigned int)valor;
    free(entrada);
    return 0;   //Si todo salio bien llegamos limpios
}

int entradaUSHORT(unsigned short *variable){
    if(variable == NULL){
        return -3;
    }

    int cantidad = longitudVariables(6);
    if(cantidad <= 0){
        *variable = 0;
        return -1;
    }
    size_t espacios = (size_t)cantidad; //Manejamos mejor la longitud de la memoria

    char *entrada = malloc(espacios);   //Pedimos espacio
    if(entrada == NULL){
        *variable = 0;
        return -1;
    }

    if(fgets(entrada, espacios, stdin) == NULL){
        *variable = 0;
        free(entrada);
        return -1;
    }

    //Si el numero es muy grande no deja pasar
    if(limpiarBuffer(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    if(verificacionUNS(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    char *resultado;
    errno = 0;
    unsigned long valor = strtoul(entrada, &resultado, 10);

    //Ultima puerta antes de retornar un valor
    int veric = verificacion(entrada, resultado);
    if(veric == -2){
        *variable = 0;
        free(entrada);
        return -2;
    }

    if(veric == -1 || errno == ERANGE || valor > USHRT_MAX){
        *variable = 0;
        free(entrada);
        return -1;
    }

    *variable = (unsigned short)valor;
    free(entrada);
    return 0;   //Si todo salio bien llegamos limpios
}

int entradaLDOUBLE(long double *variable){
    if(variable == NULL){
        return -3;
    }

    int cantidad = longitudVariables(7);
    if(cantidad <= 0){
        *variable = 0;
        return -1;
    }
    size_t espacios = (size_t)cantidad; //Manejamos mejor la longitud de la memoria

    char *entrada = malloc(espacios);   //Pedimos espacio
    if(entrada == NULL){
        *variable = 0;
        return -1;
    }

    if(fgets(entrada, espacios, stdin) == NULL){
        *variable = 0;
        free(entrada);
        return -1;
    }

    //Si el numero es muy grande no deja pasar
    if(limpiarBuffer(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    char *resultado;
    errno = 0;
    long double valor = strtold(entrada, &resultado);

    //Ultima puerta antes de retornar un valor
    int veric = verificacion(entrada, resultado);
    if(veric == -2){
        *variable = 0;
        free(entrada);
        return -2;
    }
    if(veric == -1 || valor < -LDBL_MAX || valor > LDBL_MAX){
        *variable = 0;
        free(entrada);
        return -1;
    }

    *variable = valor;
    free(entrada);
    return 0;   //Si todo salio bien llegamos limpios
}

int entradaUCHAR(unsigned char *variable, unsigned int longitud){
    if(variable == NULL || longitud <= 1){
        return -3;
    }

    char *entrada = (char *)variable;

    if(fgets(entrada, longitud, stdin) == NULL){
        *variable = '\0';
        return -1;
    }

    if(limpiarBuffer(entrada) < 0){
        *variable = '\0';
        return -1;
    }

    return 0;
}

int entradaULONG(unsigned long *variable){
    if(variable == NULL){
        return -3;
    }

    int cantidad = longitudVariables(8);
    if(cantidad <= 0){
        *variable = 0;
        return -1;
    }
    size_t espacios = (size_t)cantidad; //Manejamos mejor la longitud de la memoria

    char *entrada = malloc(espacios);   //Pedimos espacio
    if(entrada == NULL){
        *variable = 0;
        return -1;
    }

    if(fgets(entrada, espacios, stdin) == NULL){
        *variable = 0;
        free(entrada);
        return -1;
    }

    //Si el numero es muy grande no deja pasar
    if(limpiarBuffer(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    if(verificacionUNS(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    char *resultado;
    errno = 0;
    unsigned long valor = strtoul(entrada, &resultado, 10);

    //Ultima puerta antes de retornar un valor
    int veric = verificacion(entrada, resultado);
    if(veric == -2){
        *variable = 0;
        free(entrada);
        return -2;
    }

    if(veric == -1 || errno == ERANGE){
        *variable = 0;
        free(entrada);
        return -1;
    }

    *variable = valor;
    free(entrada);
    return 0;   //Si todo salio bien llegamos limpios
}

int entradaLLONG(long long *variable){
    if(variable == NULL){
        return -3;
    }

    int cantidad = longitudVariables(9);
    if(cantidad <= 0){
        *variable = 0;
        return -1;
    }
    size_t espacios = (size_t)cantidad; //Manejamos mejor la longitud de la memoria

    char *entrada = malloc(espacios);   //Pedimos espacio
    if(entrada == NULL){
        *variable = 0;
        return -1;
    }

    if(fgets(entrada, espacios, stdin) == NULL){
        *variable = 0;
        free(entrada);
        return -1;
    }

    //Si el numero es muy grande no deja pasar
    if(limpiarBuffer(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    char *resultado;
    errno = 0;
    long long valor = strtoll(entrada, &resultado, 10);

    //Ultima puerta antes de retornar un valor
    int veric = verificacion(entrada, resultado);
    if(veric == -2){
        *variable = 0;
        free(entrada);
        return -2;
    }

    if(veric == -1 || errno == ERANGE){
        *variable = 0;
        free(entrada);
        return -1;
    }

    *variable = valor;
    free(entrada);
    return 0;   //Si todo salio bien llegamos limpios
}

int entradaULLong(unsigned long long *variable){
    if(variable == NULL){
        return -3;
    }

    int cantidad = longitudVariables(10);
    if(cantidad <= 0){
        *variable = 0;
        return -1;
    }
    size_t espacios = (size_t)cantidad; //Manejamos mejor la longitud de la memoria

    char *entrada = malloc(espacios);   //Pedimos espacio
    if(entrada == NULL){
        *variable = 0;
        return -1;
    }

    if(fgets(entrada, espacios, stdin) == NULL){
        *variable = 0;
        free(entrada);
        return -1;
    }

    //Si el numero es muy grande no deja pasar
    if(limpiarBuffer(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    if(verificacionUNS(entrada) < 0){
        *variable = 0;
        free(entrada);
        return -1;
    }

    char *resultado;
    errno = 0;
    unsigned long long valor = strtoull(entrada, &resultado, 10);

    //Ultima puerta antes de retornar un valor
    int veric = verificacion(entrada, resultado);
    if(veric == -2){
        *variable = 0;
        free(entrada);
        return -2;
    }

    if(veric == -1 || errno == ERANGE){
        *variable = 0;
        free(entrada);
        return -1;
    }

    *variable = valor;
    free(entrada);
    return 0;   //Si todo salio bien llegamos limpios
}

int entradaBYTE(unsigned char *variable){
    if(variable == NULL){
        return -3;
    }

    char entrada[5];
    char *resultado;
    errno = 0;

    if(fgets(entrada, sizeof(entrada), stdin) == NULL){
        *variable = 0;
        return -1;
    }

    if(limpiarBuffer(entrada) < 0){
        *variable = 0;
        return -1;
    }

    unsigned long valor = strtoul(entrada, &resultado, 10);

    int veric = verificacion(entrada, resultado);

    if(veric == -2){
        *variable = 0;
        return -2;
    }

    if(veric == -1 || verificacionUNS(entrada) < 0 ||
        errno == ERANGE || valor > UCHAR_MAX){
        *variable = 0;
        return -1;
    }

    *variable = (unsigned char)valor;
    return 0;
}
