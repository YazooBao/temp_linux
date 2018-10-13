#include <stdio.h>

#include "lqueue.h"

lqueue_t *lqueue_init(int size)
{
	return llist_init(size);
}

int lqueue_empty(lqueue_t *lq)
{
	return llist_empty(lq);
}

int lqueue_enq(lqueue_t *lq, const void *data)
{
	return llist_insert(lq, data, TAIL);
}

static int alway_cmp(const void *data, const void *key)
{
	return 0;
}

int lqueue_deq(lqueue_t *lq, void *data)
{
	return llist_fetch(lq, NULL, alway_cmp, data);
}

void lqueue_destroy(lqueue_t *lq)
{
	llist_destroy(lq);
}

