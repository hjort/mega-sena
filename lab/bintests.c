#include <stdio.h>

void pand(int a, int b)
{
	printf("%d & %d = %d\n", a, b, a & b);
}

int main()
{
	int i = 63, j = 59, k = 62, l = 64;

	pand(i, 59);
	pand(i, 62);
	pand(i, 64);

	pand(i, 126);
	pand(i, 190);
	pand(i, 318);
	pand(i, 254);
	pand(i, 382);

	pand(i, 187);
	pand(i, 183);

	return 0;
}
