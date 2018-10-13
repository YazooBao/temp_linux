#ifndef _STACK_H
#define _STACK_H

#define STACKBASE	10
#define STACKINCR	5
#define STACKMAXS	50

typedef struct {
	void *top;
	void *base;	
	int size;
	int maxnmemb;
}stack_t;

stack_t *stack_init(int size);

int stack_full(stack_t *s);

int stack_empty(stack_t *s);

int stack_push(stack_t *s, const void *data);

int stack_pop(stack_t *s, void *data);

void stack_destroy(stack_t *s);

#endif

