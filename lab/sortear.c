#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_NUMEROS 15

/**
 * Dadas as condições, preenche um array de números inteiros.
 */
int sortear_numeros(int nums[],
  const int min_num, const int max_num, const int min_qtd, const int max_qtd)
{
  int i, j, qtd, num;

  printf("min_num = %d, max_num = %d, min_qtd = %d, max_qtd = %d\n",
    min_num, max_num, min_qtd, max_qtd);

  memset(nums, 0, sizeof(int) * MAX_NUMEROS);

  srand(time(NULL));

  // sortear quantidade de itens
  qtd = (max_qtd != min_qtd ? rand() % (max_qtd - min_qtd + 1) + min_qtd : max_qtd);

  // sortear cada um dos números (sem repetição)
  for (i = 0; i < qtd; i++) {
    j = i;
    do {
      if (j == i || nums[j] == num) {
        num = rand() % (max_num - min_num + 1) + min_num;
        j = i;
        //printf("\ti = %d, j = %d, num = %d\n", i, j, num);
      }
    } while (--j >= 0);
    nums[i] = num;
  }

  return qtd;
}

int main()
{
  int i, qtd;
  int nums[MAX_NUMEROS];

  //qtd = sortear_numeros((int**) &nums, 1, 6, 1, 2);
  //qtd = sortear_numeros((int**) &nums, 1, 60, 6, 6);
  qtd = sortear_numeros(nums, 1, 60, 6, 15);

  printf("qtd = %d\n", qtd);
  for (i = 0; i < qtd; i++)
    printf("\tnums[%d] = %d\n", i, nums[i]);

  return 0;
}

