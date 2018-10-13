#include <stdlib.h>
#include <string.h>

#include "llist.h"

struct node_st {
	struct node_st *prev;
	struct node_st *next;
	char data[1];
}__attribute__((packed));

typedef struct {
	struct node_st head;
	int size;
}head_t;

llist_t *llist_init(int size)
{
	head_t *me = malloc(sizeof(head_t));
	if (NULL == me)
		return NULL;
	memset(me, '\0', sizeof(head_t));
	
	me->head.prev = me->head.next = &me->head;
	me->size = size;

	return me;
}

//将一个结点插入两个结点之间
static int _llist_insert(struct node_st *prev, struct node_st *next, struct node_st *new)
{
	new->prev = prev;
	new->next = next;
	prev->next = new;
	next->prev = new;
}


int llist_insert(llist_t *ptr, const void *data, int way)
{
	struct node_st *new;
	head_t *list = ptr;

	new = malloc(sizeof(*new)+list->size-1);
	if (NULL == new)
		return -1;
	memcpy(new->data,  data, list->size);

	if (way == FRONT)
		_llist_insert(&list->head, list->head.next, new);
	else if (way == TAIL)
		_llist_insert(list->head.prev, &list->head, new);
	else {
		free(new->data);
		free(new);
		return -2;
	}

	return 0;
}

static struct node_st *_llist_search(llist_t *ptr, const void *key, cmp_t cmp)
{
	struct node_st *cur = NULL;
	head_t *list = ptr;

	for (cur = list->head.next; cur != &list->head; cur = cur->next) {
		if (!cmp(cur->data, key)) {
			return cur;
		}
	}

	return NULL;
}

int llist_delete(llist_t *list, const void *key, cmp_t *cmp)
{
	struct node_st *ret = NULL;

	ret = _llist_search(list, key, cmp);
	if (ret == NULL)
		return -1;
	ret->prev->next = ret->next;
	ret->next->prev = ret->prev;
	ret->prev = ret->next = NULL;

	free(ret);

	return 0;
}

const void *llist_search(llist_t *list, const void *key, cmp_t *cmp)
{
	struct node_st *ret = NULL;

	ret = _llist_search(list, key, cmp);
	if (ret == NULL)
		return NULL;
	return (const void *)(ret->data);
}

void llist_traval(llist_t *ptr, pri_t pri)
{
	struct node_st *cur;
	head_t *list = ptr;

	for (cur = list->head.next; cur != &list->head; cur = cur->next)
		pri(cur->data);
}

int llist_empty(llist_t *ptr)
{
	head_t *list = ptr;

	return list->head.next == &list->head && list->head.prev == &list->head;
}

int llist_fetch(llist_t *ptr, const void *key, cmp_t *cmp, void *data)
{
	head_t *list = ptr;	
	struct node_st *cur;

	cur = _llist_search(list, key, cmp);
	if (cur == NULL) {
		return -1;
	}
	memcpy(data, cur->data, list->size);

	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	cur->prev = cur->next = NULL;

	free(cur);

	return 0;
}

void llist_destroy(llist_t *ptr)
{
	struct node_st *cur, *step;	
	head_t *list = ptr;

	cur = list->head.next;

	while (cur != &list->head) {
		step = cur->next;
		free(cur);
		cur = step;
	}

	free(list);
}

