/**
 * Mega-Sena
 * Project Home: https://github.com/hjort/mega-sena
 *
 * Author:
 * Rodrigo Hjort <rodrigo.hjort@gmail.com>
 */

#include "postgres.h"
#include "megasena.h"

//#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
//#endif

// function sortear_numeros(min_num int, max_num int, min_qtd int, max_qtd int): int2[]
PG_FUNCTION_INFO_V1(pg_sortear_numeros);
Datum
pg_sortear_numeros(PG_FUNCTION_ARGS)
{
  PG_RETURN_NULL();
}

// function calcular_hash(numeros int2[]): int8
PG_FUNCTION_INFO_V1(pg_calcular_hash);
Datum
pg_calcular_hash(PG_FUNCTION_ARGS)
{
  PG_RETURN_NULL();
}

// function calcular_acertos(hash_sorteio int8, hash_aposta int8): int2
PG_FUNCTION_INFO_V1(pg_calcular_acertos);
Datum
pg_calcular_acertos(PG_FUNCTION_ARGS)
{
  uint64 a = PG_GETARG_INT64(0);
  uint64 b = PG_GETARG_INT64(1);
  uint16 qtd;

  qtd = acertos(a, b);

  PG_RETURN_INT16(qtd);
}

/**
 * Retorna o resultado do cálculo de 2^n.
 */
int8
pow2(const int8 n)
{
  return (n > 0 ? 2 * pow2(n - 1) : 1);
}

/**
 * Retorna um hash: resultado da soma binária de cada número apostado.
 */
int8
hash_aposta(const int nums[], const int qtd)
{
  int i;
  int8 hash = 0;

  for (i = 0; i < qtd; i++)
    hash += pow2(nums[i] - 1);

  return hash;
}

/**
 * Converte uma string em um array de números inteiros.
 * Retorna a quantidade de elementos detectados.
 */
int
str2nums(int *nums[], const char *ns)
{
  char *tk;
  char cns[3 * MAX_NUMEROS];
  int pos = 0, qtd = 1;

  memset(nums, 0, MAX_NUMEROS);
  strcpy(cns, ns);

  if (!(tk = strtok(cns, ",")))
    return 0;

  nums[0] = atoi(tk);

  while ((tk = strtok(NULL, ",")))
  {
    nums[++pos] = atoi(tk);
    qtd++;
  }

  return qtd;
}

/**
 * Retorna a quantidade de acertos da aposta com relação ao número sorteado.
 */
int
acertos(const int8 hash_sorteado, const int8 hash_aposta)
{
  int i, qtd = 0;
  int8 binario = 1;
  const int8 conjuncao = hash_sorteado & hash_aposta;

  for (i = 0; i < 60; i++, binario *= 2)
    if (binario & conjuncao)
      qtd++;

  return qtd;
}

