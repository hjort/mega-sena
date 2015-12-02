/**
 * Mega-Sena
 * Project Home: https://github.com/hjort/mega-sena
 *
 * Author:
 * Rodrigo Hjort <rodrigo.hjort@gmail.com>
 */

#ifndef MEGASENA_H
#define MEGASENA_H

#include <time.h>
//#include <stdio.h>
//#include <unistd.h>

#include "fmgr.h"
#include "utils/array.h"
#include "catalog/pg_type.h"
//#include "utils/builtins.h"

// 8 bytes
#define int8	unsigned long long int

#define MIN_NUMEROS  6
#define MAX_NUMEROS 15

// function sortear_numeros(min_num int, max_num int, min_qtd int, max_qtd int): int2[]
extern Datum pg_sortear_numeros(PG_FUNCTION_ARGS);

// function calcular_hash(numeros int2[]): int8
extern Datum pg_calcular_hash(PG_FUNCTION_ARGS);

// function calcular_acertos(hash_sorteio int8, hash_aposta int8): int2
extern Datum pg_calcular_acertos(PG_FUNCTION_ARGS);

// funções internas
int8 pow2(const int8);
int8 hash_aposta(const int[], const int);
//int str2nums(int**, const char*);
int acertos(const int8, const int8);
int sortear(int[], const int, const int, const int, const int);
int2vector *construir_int2vector(const int16*, const int);

#endif   /* MEGASENA_H */

