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
  int min_num = PG_GETARG_INT32(0);
  int max_num = PG_GETARG_INT32(1);
  int min_qtd = PG_GETARG_INT32(2);
  int max_qtd = PG_GETARG_INT32(3);
  int qtd, nums[MAX_NUMEROS], i;
  int16 int2s[MAX_NUMEROS];
  int2vector *int2Array;

  elog(DEBUG1, "pg_sortear_numeros(%d, %d, %d, %d)", min_num, max_num, min_qtd, max_qtd);

  // TODO: evitar situações de loop infinito (sanity check) - ex: sortear_numeros(1, 2, 3, 4)
  /*
  if (...) {
    elog(ERROR, "xaxaxaxa: %d (máx: %d)", qtd, MAX_NUMEROS);
    PG_RETURN_NULL();
  }
  */

  // sortear números conforme parâmetros
  qtd = sortear(&nums, min_num, max_num, min_qtd, max_qtd);

  /*
  elog(DEBUG1, "qtd = %d", qtd);
  for (i = 0; i < qtd; i++)
    elog(DEBUG1, "  nums[%d] = %d", i, nums[i]);
  */

  memset(int2s, 0, sizeof(int2s));
  for (i = 0; i < qtd; i++)
    int2s[i] = (uint16) nums[i];

  int2Array = buildint2vector(int2s, qtd);

  PG_RETURN_POINTER(int2Array);
}

// function calcular_hash(numeros int2[]): int8
PG_FUNCTION_INFO_V1(pg_calcular_hash);
Datum
pg_calcular_hash(PG_FUNCTION_ARGS)
{
  int2vector *int2Array = (int2vector *) PG_GETARG_POINTER(0);
  const int qtd = int2Array->dim1;
  unsigned int nums[MAX_NUMEROS], i;
  uint64 hash = 0;

  elog(DEBUG1, "pg_calcular_hash(qtd=%d)", qtd);

  elog(DEBUG2, "  [ndim=%d, dataoffset=%d, elemtype=%d, dim1=%d, lbound1=%d]",
    int2Array->ndim, int2Array->dataoffset, int2Array->elemtype, int2Array->dim1, int2Array->lbound1);

/*
  // sanity checks: int2vector must be 1-D, 0-based, no nulls
  if (ARR_NDIM(int2Array) != 1 ||
      ARR_HASNULL(int2Array) ||
      ARR_ELEMTYPE(int2Array) != INT2OID ||
      ARR_LBOUND(int2Array)[0] != 0)
    ereport(ERROR, (errcode(ERRCODE_INVALID_BINARY_REPRESENTATION),
      errmsg("invalid int2vector data")));

  // check length for consistency with int2vectorin()
  if (ARR_DIMS(int2Array)[0] > MAX_NUMEROS)
    ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
      errmsg("oidvector has too many elements")));
*/

  // verificar quantidade máxima de itens
  if (qtd > MAX_NUMEROS) {
    elog(ERROR, "Quantidade de itens ultrapassa o limite: %d (máx: %d)", qtd, MAX_NUMEROS);
    PG_RETURN_NULL();
  }

  memset(nums, 0, sizeof(nums));

  for (i = 0; i < qtd; i++)
    nums[i] = int2Array->values[i];

  // calcular o hash do array de números
  hash = hash_aposta(nums, qtd);

  PG_RETURN_INT64(hash);
}

// function calcular_acertos(hash_sorteio int8, hash_aposta int8): int2
PG_FUNCTION_INFO_V1(pg_calcular_acertos);
Datum
pg_calcular_acertos(PG_FUNCTION_ARGS)
{
  uint64 hash_sorteio = PG_GETARG_INT64(0);
  uint64 hash_aposta = PG_GETARG_INT64(1);
  uint16 qtd_acertos;

  elog(DEBUG1, "pg_calcular_acertos(%llu, %llu)", hash_sorteio, hash_aposta);

  // calcular a quantidade de acertos da aposta no sorteio
  qtd_acertos = acertos(hash_sorteio, hash_aposta);

  PG_RETURN_INT16(qtd_acertos);
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
/*
int
str2nums(int** nums, const char *ns)
{
  char *tk;
  char cns[3 * MAX_NUMEROS];
  int pos = 0, qtd = 1;

  memset(nums, 0, sizeof(int) * MAX_NUMEROS);
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
*/

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

/**
 * Dadas as condições, preenche um array de números inteiros.
 */
int sortear(int** nums,
  const int min_num, const int max_num, const int min_qtd, const int max_qtd)
{
  int i, j, qtd, num = 0;

  memset(nums, 0, sizeof(int) * MAX_NUMEROS);
  //srand(time(NULL));

  // sortear quantidade de itens
  qtd = (max_qtd != min_qtd ? rand() % (max_qtd - min_qtd + 1) + min_qtd : max_qtd);

  // sortear cada um dos números (sem repetição)
  for (i = 0; i < qtd; i++) {
    j = i;
    do {
      if (j == i || (int) nums[j] == num) {
        num = rand() % (max_num - min_num + 1) + min_num;
        j = i;
      }
    } while (--j >= 0);
    nums[i] = num;
  }

  return qtd;
}

