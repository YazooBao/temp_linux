#ifndef _LQUEUE_H
#define _LQUEUE_H

#include "llist.h"

typedef llist_t lqueue_t;

lqueue_t *lqueue_init(int size);

int lqueue_empty(lqueue_t *lq);

int lqueue_enq(lqueue_t *lq, const void *data);

int lqueue_deq(lqueue_t *lq, void *data);

void lqueue_destroy(lqueue_t *lq);

#endif

