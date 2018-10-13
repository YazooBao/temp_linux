#include <stdio.h>

#include "operation.h"

int main(int argc, char **argv)
{
	int a, b;

	if (argc < 3) {
		return 1;
	}

	a = atoi(argv[1]);
	b = atoi(argv[2]);

	printf("%d + %d\n", a, b, add(a, b));
	printf("%d - %d\n", a, b, sub(a, b));
	printf("%d * %d\n", a, b, mul(a, b));


	return 0;
}
