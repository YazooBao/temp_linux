#include <stdlib.h>
#include <string.h>

#include "stack.h"

stack_t *stack_init(int size)
{
	stack_t *me;

	me = malloc(sizeof(*me));
	if (me != NULL) {
		me->base = malloc(STACKBASE*size);		
		if (me->base != NULL) {
			me->top = me->base;		
			me->size = size;
			me->maxnmemb = STACKBASE;
		} else {
			free(me);	
			me = NULL;
		}
	}

	return me;
}

int stack_full(stack_t *s)
{
	return ((char *)s->top-(char *)s->base)/s->size\
			 == s->maxnmemb;	
}

int stack_empty(stack_t *s)
{
	return s->top == s->base;
}

int stack_push(stack_t *s, const void *data)
{
	char *ret;

	if (stack_full(s)) {
		if (s->maxnmemb == STACKMAXS)
			return -1;
		ret = realloc(s->base, (s->maxnmemb+STACKINCR)*s->size);	
		if (ret == NULL)
			return  -1;	
		else {
			s->base = ret;
			s->top = (char *)s->base+s->maxnmemb*s->size;
			s->maxnmemb += STACKINCR;
		}
	}	
	memcpy(s->top, data, s->size);
	s->top = (char *)s->top + s->size;

	return 0;
}

int stack_pop(stack_t *s, void *data)
{
	if (stack_empty(s))	
		return -1;
	s->top = (char *)s->top - s->size;
	memcpy(data, s->top, s->size);

	return 0;
}

void stack_destroy(stack_t *s)
{
	free(s->base);
	free(s);
}

