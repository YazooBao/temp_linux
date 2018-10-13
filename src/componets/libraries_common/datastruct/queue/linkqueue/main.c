#include <stdio.h>

#include "lqueue.h"

int main(int argc, char **argv)
{
	lqueue_t *lq;
	int i;

	if (argc < 2)
		return 1;

	lq = lqueue_init(sizeof(char));
	//if error

	for (i = 0; argv[1][i] != 0; i++) {
		lqueue_enq(lq, argv[1]+i);	
	}

	while (!lqueue_empty(lq)) {
		lqueue_deq(lq, &i);
		putchar(i);
		putchar(' ');
	}
	printf("\n");

	lqueue_destroy(lq);

	return 0;
}

