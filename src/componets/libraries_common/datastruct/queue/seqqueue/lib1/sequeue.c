#include <stdlib.h>
#include <string.h>

#include "sequeue.h"

queue_t *queue_init(int size)
{
	queue_t *me;

	me = malloc(sizeof(*me));	
	if (NULL == me)
		return NULL;
	me->base = malloc(MAXQUEUE*size);
	if (me->base == NULL) {
		free(me);
		return NULL;
	}
	me->size = size;
	me->front = me->tail = 0;

	return me;
}

int queue_empty(queue_t *que)
{
	return que->front == que->tail;
}

int queue_full(queue_t *que)
{
	return (que->tail+1)%MAXQUEUE == que->front;
}

int queue_enq(queue_t *que, const void *data)
{
	if (queue_full(que))
		return -1;
	memcpy((char *)que->base+que->tail*que->size, data, que->size);
	que->tail = (que->tail+1)%MAXQUEUE;

	return 0;
}

int queue_deq(queue_t *que, void *data)
{
	if (queue_empty(que))
		return -1;
	memcpy(data, (char *)que->base+que->front*que->size, que->size);
	que->front = (que->front+1)%MAXQUEUE;

	return 0;
}

void queue_destroy(queue_t *que)
{
	free(que->base);
	free(que);
}

