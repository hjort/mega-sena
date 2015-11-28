#include <stdio.h>

#include "mega.h"

#define HASH(NUMS) {\
  qtd = str2nums(&nums, NUMS);\
  hash = hash_aposta(nums, qtd);\
  printf("qtd = %2d, hash = %llu\n", qtd, hash);\
}

int main()
{
  int8 hash = 0;
  int nums[MAX_NUMEROS], qtd = 0;

  HASH("1");
  HASH("2");
  HASH("59");
  HASH("60");

  HASH("31,32");
  HASH("60,59");

  HASH("1,2,3,4,5,6");
  HASH("55,56,57,58,59,60");
  HASH("46,47,48,49,50,51,52,53,54,55,56,57,58,59,60");

  return 0;
}
