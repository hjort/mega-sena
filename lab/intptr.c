#include <stdio.h>
#include <string.h>

int mudar(int* p) {
  *p = 123;
  printf("sizeof(p) = %d\n", sizeof(p));
}

int main()
{
  int n = 0;

  printf("sizeof(n) = %d\n", sizeof(n));
  printf("n = %d\n", n);

  mudar(&n);

  printf("n = %d\n", n);

  return 0;
}
