#ifndef _LSTACK_H
#define _LSTACK_H

#include "llist.h"

typedef llist_t lstack_t;

lstack_t *lstack_init(int size);

int lstack_push(lstack_t *mystack, const void *data);

int lstack_pop(lstack_t *mystack, void *data);

int lstack_empty(lstack_t *mystack);

void lstack_traval(lstack_t *mystack, pri_t pri);

void lstack_destroy(lstack_t *mystack);

#endif

