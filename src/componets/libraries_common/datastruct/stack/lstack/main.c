#include <stdio.h>

#include "lstack.h"

int main(void)
{
	int id[] = {3,1,4,1,5,9,2,6};
	int i;
	lstack_t *me = NULL;	

	me = lstack_init(sizeof(int));

	for (i = 0; i < sizeof(id)/sizeof(*id); i++) {
		lstack_push(me, id+i);
	}

	while (!lstack_empty(me)) {
		lstack_pop(me, &i);
		printf("%d ", i);
	}
	printf("\n");

	lstack_destroy(me);

	return 0;
}

