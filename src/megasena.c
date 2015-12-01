/**
 * Mega-Sena
 * Project Home: https://github.com/hjort/mega-sena
 *
 * Author:
 * Rodrigo Hjort <rodrigo.hjort@gmail.com>
 */

#include "postgres.h"
#include "megasena.h"

PG_MODULE_MAGIC;

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

  // evitar situações de loop infinito (sanity check) - ex: sortear_numeros(1, 2, 3, 4)
  if (min_num <= 0 || max_num <= 0 || min_qtd <= 0 || max_qtd <= 0) {
    elog(ERROR, "Os argumentos devem ser todos números inteiros positivos");
  }
  if (min_num > max_num) {
    elog(ERROR, "Número mínimo (%d) deve ser menor ou igual ao número máximo (%d)", min_num, max_num);
  }
  if (min_qtd > max_qtd) {
    elog(ERROR, "Quantidade mínima (%d) deve ser menor ou igual à quantidade máxima (%d)", min_qtd, max_qtd);
  }
  if (max_num - min_num + 1 < max_qtd) {
    elog(ERROR, "Quantidade de números no intervalo ([%d..%d] => %d) deve ser igual ou superior à quantidade máxima (%d)", min_num, max_num, (max_num - min_num + 1), max_qtd);
  }

  // sortear números conforme parâmetros
  qtd = sortear((int**) &nums, min_num, max_num, min_qtd, max_qtd);

  /*
  elog(DEBUG1, "qtd = %d", qtd);
  for (i = 0; i < qtd; i++)
    elog(DEBUG1, "  nums[%d] = %d", i, nums[i]);
  */

  memset(int2s, 0, sizeof(int2s));
  for (i = 0; i < qtd; i++)
    int2s[i] = (uint16) nums[i];

  int2Array = (int2vector*) construir_int2vector(int2s, qtd);

  PG_RETURN_POINTER(int2Array);
}

#define ARRPTR16(x)   ((uint16 *) ARR_DATA_PTR(x))
#define ARRNELEMS(x)  ArrayGetNItems(ARR_NDIM(x), ARR_DIMS(x))
#define ARRISEMPTY(x) (ARRNELEMS(x) == 0)

// function calcular_hash(numeros int2[]): int8
PG_FUNCTION_INFO_V1(pg_calcular_hash);
Datum
pg_calcular_hash(PG_FUNCTION_ARGS)
{
  ArrayType *a = PG_GETARG_ARRAYTYPE_P_COPY(0);
  unsigned int i, qtd, tipo, nums[MAX_NUMEROS];
  uint16 *da;
  uint64 hash = 0;

  qtd = ARRNELEMS(a);
  tipo = ARR_ELEMTYPE(a);
  da = ARRPTR16(a);

  elog(DEBUG1, "pg_calcular_hash(qtd=%d, tipo=%d)", qtd, tipo);

  // sanity checks: int2vector must be 1-D, 0-based, no nulls
  if (ARR_NDIM(a) != 1 || ARR_HASNULL(a) || ARR_ELEMTYPE(a) != INT2OID /*|| ARR_LBOUND(a)[0] != 0*/) {
    elog(ERROR, "Argumento inválido: tipo de dados deve ser smallint[]");
  }

  // se array estiver vazio, retorna 0
  if (ARRISEMPTY(a)) {
    PG_RETURN_INT64(0ULL);
  }

  for (i = 0; i < qtd; i++)
    elog(DEBUG2, "  da[%d] = %d", i, da[i]);

  // verificar quantidade máxima de itens
  if (qtd > MAX_NUMEROS) {
    elog(ERROR, "Quantidade de itens ultrapassa o limite: %d (máx: %d)", qtd, MAX_NUMEROS);
  }

  // verificar todos os itens do array
  for (i = 0; i < qtd; i++) {
    if (da[i] <= 0) {
      elog(ERROR, "Todos os números devem ser inteiros positivos (inválido: %d)", da[i]);
    }
  }

  memset(nums, 0, sizeof(nums));
  for (i = 0; i < qtd; i++)
    nums[i] = da[i];

  // calcular o hash do array de números
  hash = hash_aposta((int*) nums, (int) qtd);

  pfree(a);
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

#define Int2VectorSize(n)  (offsetof(int2vector, values) + (n) * sizeof(int16))

/**
 * Constrói um int2vector a partir de um array de int2s.
 * Retirado de: pgsql/src/backend/utils/adt/int.c
 */
int2vector *
construir_int2vector(const int16 *int2s, const int n)
{
  int2vector *res;

  res = (int2vector *) palloc0(Int2VectorSize(n));

  if (n > 0 && int2s)
    memcpy(res->values, int2s, n * sizeof(int16));

  SET_VARSIZE(res, Int2VectorSize(n));
  res->ndim = 1;           // 1 dimensão
  res->dataoffset = 0;     // sem nulos
  res->elemtype = INT2OID; // tipo int2
  res->dim1 = n;           // n itens
  res->lbound1 = 0;        // índice inicia em 0

  return res;
}

