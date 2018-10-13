#include <stdio.h>

#include "sequeue.h"

int main(void)
{
	int id[] = {2,0,1,6,8,1,9,5,7,3};	
	int i, j, n;
	queue_t *q;
	
	q = queue_init(sizeof(int));
	//if error

	for (j = 0; j < 15; j++) {
		for (i = 0; i < sizeof(id)/sizeof(*id); i++) {
			if (queue_full(q))
				break;
			queue_enq(q, id+i);	
		}
		for (i = 0; i < 3; i++) {
			queue_deq(q, &n);
			printf("%d ", n);
		}
		printf("\n");
	}


	queue_destroy(q);

	return 0;
}

