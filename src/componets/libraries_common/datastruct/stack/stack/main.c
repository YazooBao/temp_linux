#include <stdio.h>

#include "stack.h"

int main(void)
{
	int id[] = {3,1,4,1,5,9,2,6};
	int i;
	stack_t *me = NULL;	

	me = stack_init(sizeof(int));

	for (i = 0; i < sizeof(id)/sizeof(*id); i++) {
		stack_push(me, id+i);
	}

	while (!stack_empty(me)) {
		stack_pop(me, &i);
		printf("%d ", i);
	}
	printf("\n");

	stack_destroy(me);

	return 0;
}






