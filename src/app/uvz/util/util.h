/* 
 * File:   util.h
 * Author: sundq
 *
 * Created on 2016年2月5日, 下午1:36
 */

#ifndef UTIL_H
#define	UTIL_H

#ifdef	__cplusplus
extern "C"
{
#endif
    
#include <uv.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
    
typedef struct queue_s queue_t;


int queue_init(queue_t *pq, int cap);
int queue_push(queue_t *pq , uv_buf_t value);
int queue_is_empty(queue_t *pq);
int queue_is_full(queue_t *pq);
uv_buf_t queue_pop(queue_t *pq);
void queue_traverse(queue_t *pq);

struct queue_s
{
    uv_buf_t *base;
    int capacity;
    int size;
    int front;
    int rear;
};


#ifdef	__cplusplus
}
#endif

#endif	/* UTIL_H */

