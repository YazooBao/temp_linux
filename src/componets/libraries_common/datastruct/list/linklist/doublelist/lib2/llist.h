#ifndef _LLIST_H
#define _LLIST_H

struct node_st {
	struct node_st *prev;
	struct node_st *next;
	char data[1];
}__attribute__((packed));

typedef struct {
	struct node_st head;
	int size;
}llist_t;

typedef int cmp_t(const void *data, const void *key);
typedef void (*pri_t)(const void *data);

enum {
	FRONT = 1,
	TAIL
};

llist_t *llist_init(int size);

int llist_insert(llist_t *list, const void *data, int way);

int llist_delete(llist_t *list, const void *key, cmp_t *cmp);

const void *llist_search(llist_t *list, const void *key, cmp_t *cmp);

void llist_traval(llist_t *list, pri_t pri);

void llist_destroy(llist_t *list);

#endif

