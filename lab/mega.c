#include <stdio.h>
#include <string.h>

#include "mega.h"

#define APOSTA(NUMS) {\
  qtd = str2nums(&nums, NUMS);\
  hash = hash_aposta(nums, qtd);\
  ace = acertos(sorteado, hash);\
  printf("{%s} qtd = %d, hash = %llu => %d acertos\n", NUMS, qtd, hash, ace);\
}

#define SORTEADO "1,2,3,4,5,6"

int
main()
{
  int qtd, ace;
  int8 hash, sorteado;
  int sorteio[6], nums[MAX_NUMEROS];

  // sorteio
  qtd = str2nums(&sorteio, SORTEADO);
  sorteado = hash_aposta(sorteio, qtd);
  printf("{%s} sorteio, hash = %d\n\n", SORTEADO, sorteado);

  // sena
  APOSTA("1,2,3,4,5,6");
  APOSTA("1,2,3,4,5,6,7");
  APOSTA("1,2,3,4,5,6,7,8");
  APOSTA("1,2,3,4,5,6,7,8,9");
  APOSTA("1,2,3,4,5,6,7,8,9,10");
  APOSTA("1,2,3,4,5,6,51,52,53,54,56,57,58,59,60");
  printf("\n");

  // quina
  APOSTA("2,3,4,5,6,7");
  APOSTA("2,3,4,5,6,7,8");
  APOSTA("2,3,4,5,6,7,8,9");
  APOSTA("1,3,4,5,6,7");
  APOSTA("1,3,4,5,6,7,8");
  APOSTA("1,3,4,5,6,7,8,9");
  APOSTA("1,2,4,5,6,7");
  APOSTA("1,2,3,5,6,7");
  APOSTA("1,2,3,4,6,7");
  APOSTA("1,2,3,4,6,8");
  APOSTA("1,2,3,4,5,7");
  APOSTA("1,2,3,4,5,8");
  APOSTA("1,2,3,4,5,50,51,52,53,54,56,57,58,59,60");
  printf("\n");

  // quadra
  APOSTA("3,4,5,6,7,8");
  APOSTA("3,4,5,6,7,8,9");
  APOSTA("3,4,5,6,7,8,10");
  APOSTA("2,3,4,5,7,8");
  APOSTA("2,3,4,5,7,8,9");
  APOSTA("2,3,4,5,7,8,9,10");
  APOSTA("1,3,4,5,7,8");
  APOSTA("1,3,4,5,7,8,10,11");
  APOSTA("1,3,4,5,7,8,10,11,12");
  APOSTA("1,2,3,4,49,50,51,52,53,54,56,57,58,59,60");
  printf("\n");

  // trinca
  APOSTA("1,2,3,13,25,46");
  APOSTA("1,2,3,24,33,48,55");
  APOSTA("1,2,3,17,22,36,49,59");
  APOSTA("3,4,5,16,22,31");
  APOSTA("3,4,5,28,39,51,60");
  APOSTA("3,4,5,18,29,32,44,56");
  APOSTA("1,2,3,48,49,50,51,52,53,54,56,57,58,59,60");
  printf("\n");

  // zero
  APOSTA("45,46,47,48,49,50,51,52,53,54,56,57,58,59,60");
  printf("\n");

  return 0;
}

