#include <stdio.h>
#include <math.h>

int
myexp2(int n)
{
  return (n > 0 ? 2 * myexp2(n - 1) : 1);
}

int
main(void)
{
  double x, y;

  x = 3.0;
  y = exp2(x);

  printf("2^%d = %d\n", (int) x, (int) y);
  printf("2^%d = %d\n", 8, myexp2(8));

  return 0;
}
