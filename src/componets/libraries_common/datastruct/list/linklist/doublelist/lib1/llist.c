#include <stdlib.h>
#include <string.h>

#include "llist.h"

llist_t *llist_init(int size)
{
	llist_t *me = malloc(sizeof(llist_t));
	if (NULL == me)
		return NULL;
	memset(me, '\0', sizeof(llist_t));
	
	me->head.data = NULL;
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


int llist_insert(llist_t *list, const void *data, int way)
{
	struct node_st *new;

	new = malloc(sizeof(*new));
	if (NULL == new)
		return -1;
	new->data = malloc(list->size);
	if (NULL == new->data) {
		free(new);
		return -1;
	}
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

static struct node_st *_llist_search(llist_t *list, const void *key, cmp_t cmp)
{
	struct node_st *cur = NULL;

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

	free(ret->data);
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

void llist_traval(llist_t *list, pri_t pri)
{
	struct node_st *cur;

	for (cur = list->head.next; cur != &list->head; cur = cur->next)
		pri(cur->data);
}

void llist_destroy(llist_t *list)
{
	struct node_st *cur, *step;	

	cur = list->head.next;

	while (cur != &list->head) {
		step = cur->next;
		free(cur->data);	
		free(cur);
		cur = step;
	}

	free(list);
}

