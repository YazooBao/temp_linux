#include <stdio.h>

#include "lstack.h"

lstack_t *lstack_init(int size)
{
	return llist_init(size);	
}

int lstack_push(lstack_t *mystack, const void *data)
{
	return llist_insert(mystack, data, FRONT);
}

static int always_cmp(const void *data, const void *key)
{
	return 0;
}

int lstack_pop(lstack_t *mystack, void *data)
{
	return llist_fetch(mystack, NULL, always_cmp, data);
}

int lstack_empty(lstack_t *mystack)
{
	return llist_empty(mystack);
}

void lstack_traval(lstack_t *mystack, pri_t pri)
{
	llist_traval(mystack, pri);
}

void lstack_destroy(lstack_t *mystack)
{
	llist_destroy(mystack);
}

