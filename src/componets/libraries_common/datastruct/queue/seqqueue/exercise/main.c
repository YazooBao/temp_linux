#include <stdio.h>
#include "sequeue.h"
#include "stack.h"

#define BALLNR	27

static int is_ok(queue_t *que)
{
	int i;	
	char before, after;

	for (i = 1; i < BALLNR; i++) {
		queue_getnmemb(que, i, &before);				
		queue_getnmemb(que, i+1, &after);
		if (before > after)
			return 1;
	}

	return 0; 
}

int main(void)
{
	stack_t *onemin;
	stack_t *fivemin;
	stack_t *onehour;
	queue_t *ball;
	char deqball, popball; 
	char i;
	int sumdays = 0;

	onemin = stack_init(sizeof(char));
	fivemin = stack_init(sizeof(char));
	onehour = stack_init(sizeof(char));
	//if error
	
	ball = queue_init(sizeof(char));
	//if error
	
	for (i = 1; i <= BALLNR; i++) {
		queue_enq(ball, &i);

	}

	while (1) {
		queue_deq(ball, &deqball);
		if (stack_getnum(onemin) < 4) {
			stack_push(onemin, &deqball);
		} else {
			while (!stack_empty(onemin)) {
				stack_pop(onemin, &popball);	
				queue_enq(ball, &popball);
			}
			if (stack_getnum(fivemin) < 11) {
				stack_push(fivemin, &deqball);
			} else {
				while (!stack_empty(fivemin)) {
					stack_pop(fivemin, &popball);	
					queue_enq(ball, &popball);
				}
				if (stack_getnum(onehour) < 11) {
					stack_push(onehour, &deqball);
				} else {
					while (!stack_empty(onehour)) {
						stack_pop(onehour, &popball);	
						queue_enq(ball, &popball);
					}
					queue_enq(ball, &deqball);
					sumdays++;
					//全部球都在队列中
					if (!is_ok(ball))
						break;
				}	
			}
		}	
	}

	printf("sumdays = %d\n", sumdays);
	while (!queue_empty(ball)) {
		queue_deq(ball, &popball);
		printf("%d ", popball);
	}
	printf("\n");

	stack_destroy(onemin);		
	stack_destroy(fivemin);		
	stack_destroy(onehour);		
	queue_destroy(ball);

	return 0;
}

