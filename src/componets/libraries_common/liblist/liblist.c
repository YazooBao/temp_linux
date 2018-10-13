/************************************************************************************
 *@file name  : liblist.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 *@creat time : 2018年 09月 21日 星期五 14:43:16 CST
************************************************************************************/
/*includes-------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "liblist.h"
/*typedef--------------------------------------------------------------------------*/
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
/*function prototype---------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/

/*! \brief :init linklist
 *
 *  init linklist
 *
 * \return :list header
 */

int list_init(int size, list_t **head)
{
	list_t *me = NULL;

	me = malloc(sizeof(list_t));
	if (me == NULL) {
		*head = NULL;
		return -1;
	}
	memset(me, '\0', sizeof(list_t));
	me->head.data = NULL;
	me->head.next = &me->head;
	me->size = size;
	pthread_mutex_init(&me->lock, NULL);

	*head = me;

	return 0;
}

int list_insert(list_t *list, const void *data, int way)
{		
	struct node_st *new;
	struct node_st *cur;

	//1.为新结点结构开辟存储空间
	new = malloc(sizeof(*new));	
	if (new == NULL)
		return -1;
	//2.为新结点数据域开辟存储空即那
	pthread_mutex_lock(&list->lock);
	new->data = malloc(list->size);
	if (NULL == new->data) {
		free(new);
		return -1;
	}
	memcpy(new->data, data, list->size);
	
	//2.安排新结点的指针域

	if (way == FRONT) {
		new->next = list->head.next;	
		list->head.next = new;	
	} else if (way == TAIL) {
		for (cur = list->head.next; cur->next != &list->head; cur = cur->next)
			;
        cur->next = new;
		new->next = cur->next;
		
	} else {
		free(new->data);	
		free(new);
		return -2;
	}	
	pthread_mutex_unlock(&list->lock);
	return 0;
}

static struct node_st *list_fnodepre(list_t *list, const void *key, cmp_t cmp)
{
	struct node_st *pre, *cur;
	pthread_mutex_lock(&list->lock);
	for (pre = &list->head, cur = pre->next; cur != &list->head; pre=cur, cur=cur->next) {
		if (!cmp(cur->data, key)) {
			pthread_mutex_unlock(&list->lock);
			return pre;
		}
	}
	pthread_mutex_unlock(&list->lock);
	return NULL;
}

int list_delete(list_t *head, const void *key, cmp_t cmp)
{
	struct node_st *findpre, *cur;	

	findpre = list_fnodepre(head, key, cmp);
	if (findpre == NULL)
		return -1;

	pthread_mutex_lock(&head->lock);
	cur = findpre->next;
	findpre->next = cur->next;
	cur->next = NULL;
	free(cur->data);
	free(cur);
	pthread_mutex_unlock(&head->lock);

	return 0;
}

const void *list_search(list_t *head, const void *key, cmp_t cmp)
{
	struct node_st *findpre;

	findpre = list_fnodepre(head, key, cmp);
	if (findpre == NULL)
		return NULL;
	return findpre->next->data;
}

void list_traval(list_t *list, pri_t pri)
{
	struct node_st *cur;	

	for (cur = list->head.next; cur != &list->head; cur = cur->next)
		pri(cur->data);
}

void *list_read(list_t *list, int pos){
    struct node_st *cur;
    int i;

	pthread_mutex_lock(&list->lock);
    for(cur = list->head.next,i = 0;cur != &list->head; cur = cur->next, i++){
        if(i<=pos){
			pthread_mutex_unlock(&list->lock);
            return cur->next->data;
        }
    }
	pthread_mutex_unlock(&list->lock);
    return NULL;
}

int list_size(list_t *list){
    struct node_st *cur;
    int i = 0;

	pthread_mutex_lock(&list->lock);
    for(cur = list->head.next;cur != &list->head; cur = cur->next){
        i++;
    }
	pthread_mutex_unlock(&list->lock);
    return i;
}

void list_destroy(list_t *list)
{
	struct node_st *cur, *step;	

	pthread_mutex_lock(&list->lock);
	for (cur = list->head.next; cur != &list->head; cur = step) {
		step = cur->next;
		free(cur->data);
		free(cur);
	}

	free(list);
	pthread_mutex_unlock(&list->lock);
}



