#include <stdio.h>
#include <limits.h>
#include <string.h>

#define int8	unsigned long long int

int8
exp2(const int8 n)
{
  return (n > 0 ? 2 * exp2(n - 1) : 1);
}

int main()
{
  int i, j;
  unsigned long long int num = 0;
  char s[110], s2[10];

  // max: 1.844674407×10¹⁹
  printf("\nmax: %20llu (%d bytes)\n\n", ULLONG_MAX, sizeof(ULLONG_MAX));

  for (i = 64; i >= 55; i--) {
    num = exp2(i);
    printf("num: %20llu = 2^%d\n", num, i);
  }

  // 2^60 = 1.152921505×10¹⁸
  //num = 1073741824ULL * 1073741824ULL; // 2^30 * 2^30
  num = exp2(60);
  printf("num: %20llu = 2^60\n", num);

  // 2^60 + 2^59 = 1.729382257×10¹⁸
  //num = 1152921504606846976ULL + 576460752303423488ULL; // 2^60 + 2^59
  num = exp2(60) + exp2(59);
  printf("num: %20llu = 2^60 + 2^59\n", num);

  num = exp2(60) + exp2(59) + exp2(58);
  printf("num: %20llu = 2^60 + 2^59 + 2^58\n", num);
 
  num = exp2(60) + exp2(59) + exp2(58) + exp2(57);
  printf("num: %20llu = 2^60 + 2^59 + 2^58 + 2^57\n", num);

  num = exp2(60) + exp2(59) + exp2(58) + exp2(57) + exp2(56);
  printf("num: %20llu = 2^60 + 2^59 + 2^58 + 2^57 + 2^56\n", num);

  num = exp2(60) + exp2(59) + exp2(58) + exp2(57) + exp2(56) + exp2(55);
  printf("num: %20llu = 2^60 + 2^59 + 2^58 + 2^57 + 2^56 + 2^55\n", num);

  for (j = 6; j <= 15; j++) {
    printf("\n%d números\n", j);
    num = 0;
    strcpy(s, "2^60");
    strcpy(s2, "");
    for (i = 60; i > 60 - j; i--) {
      if (num > 0) {
        sprintf(s2, " + 2^%d", i);
        strcat(s, s2);
      }
      num += exp2(i);
      //printf("i=%d, num=%llu, s=%s, s2=%s\n", i, num, s, s2);
    }
    printf("num: %20llu = %s\n", num, s);
  }
 
  return 0;
}
