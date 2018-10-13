#ifndef _LLIST_H
#define _LLIST_H

typedef int cmp_t(const void *data, const void *key);
typedef void (*pri_t)(const void *data);

typedef void llist_t; 

enum {
	FRONT = 1,
	TAIL
};

llist_t *llist_init(int size);

int llist_insert(llist_t *list, const void *data, int way);

int llist_delete(llist_t *list, const void *key, cmp_t *cmp);

const void *llist_search(llist_t *list, const void *key, cmp_t *cmp);

int llist_empty(llist_t *list);

int llist_fetch(llist_t *ptr, const void *key, cmp_t *cmp, void *data);

void llist_traval(llist_t *list, pri_t pri);

void llist_destroy(llist_t *list);

#endif

