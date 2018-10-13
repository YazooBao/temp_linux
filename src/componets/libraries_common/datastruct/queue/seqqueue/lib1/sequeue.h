#ifndef _SEQQUEUE_H
#define	_SEQQUEUE_H

#define MAXQUEUE	10

typedef struct sequeue_st {
	void *base;
	int size;
	int front;
	int tail;
}queue_t;

queue_t *queue_init(int size);

int queue_empty(queue_t *que);

int queue_full(queue_t *que);

int queue_enq(queue_t *que, const void *data);

int queue_deq(queue_t *que, void *data);

void queue_destroy(queue_t *que);

#endif

