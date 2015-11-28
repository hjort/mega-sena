#define int8	unsigned long long int

#define MIN_NUMEROS  6
#define MAX_NUMEROS 15

/**
 * Retorna o resultado do cálculo de 2^n.
 */
int8
exp2(const int8 n)
{
  return (n > 0 ? 2 * exp2(n - 1) : 1);
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
    hash += exp2(nums[i] - 1);
  return hash;
}

/**
 * Converte uma string em um array de números inteiros.
 * Retorna a quantidade de elementos detectados.
 */
int
str2nums(int *nums[], const char *ns)
{
  char *tk, *ps;
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
  const int8 conjuncao = hash_sorteado & hash_aposta;
  int i, qtd = 0;
  int8 binario = 1;
  for (i = 0; i < 60; i++, binario *= 2)
    if (binario & conjuncao)
      qtd++;
  return qtd;
}

