#include "util.h"

int queue_init(queue_t *pq, int capacity)
{
	pq->base = (uv_buf_t *) malloc(sizeof (uv_buf_t) * capacity);
	if (pq->base != NULL)
	{
		pq->front = pq->rear = 0;
		pq->size = 0;
		pq->capacity = capacity;
		return 0;
	}
	else
	{
		return -1;
	}
}

int queue_push(queue_t *pq, uv_buf_t value)
{

	if (!queue_is_full(pq))
	{
		pq->base[pq->rear] = value;
		pq->rear = (pq->rear + 1) % pq->capacity;
		pq->size += 1;
		return 0;
	}
	else
	{
		return -1;
	}
}

uv_buf_t queue_pop(queue_t *pq)
{
    uv_buf_t value = {0};
	if (!queue_is_empty(pq))
	{
	    value = pq->base[pq->front];
		pq->front = (pq->front + 1) % pq->capacity;
		pq->size -= 1;
	}
	return value;
}

int queue_is_empty(queue_t *pq)
{
	return pq->size == 0;
}

int queue_is_full(queue_t *pq)
{
	return pq->size >= pq->capacity;
}

void queue_traverse(queue_t *pq)
{
	
}
