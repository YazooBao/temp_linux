#include <stdlib.h>
#include <string.h>

#include "seqlist.h"

struct slist_st *seqlist_init(int size)
{
	struct slist_st *me = NULL;

	me = malloc(sizeof(*me));
	if (NULL == me)
		return NULL;
	memset(me, '\0', sizeof(*me));
	me->start = NULL;
	me->size = size;
	me->nmemb = 0;

	return me;
}

int seqlist_insert(struct slist_st *ptr, const void *data)
{
	char *new = NULL;	
	//新成员开辟存储空间
	new = malloc(ptr->size * (ptr->nmemb+1));	
	if (new == NULL)
		return -1;
	memcpy(new, ptr->start, ptr->nmemb*ptr->size);
	free(ptr->start);
	ptr->start = new;
	
	//新数据copy到新空间
	memcpy((char *)(ptr->start)+ptr->size*ptr->nmemb,\
			data, ptr->size);
	ptr->nmemb ++;

	return 0;
}

static int seqlist_bsearch(struct slist_st *ptr, const void *data, cmp_t cmp) 
{
	int low, high, mid, ret;
	low = 0;
	high = ptr->nmemb-1;

	while (low < high) {
		mid = (low+high) / 2;
		if ((ret = cmp((char *)(ptr->start)+mid*ptr->size, data)) == 0)
			break;	
		if (ret > 0) 
			high = mid - 1;
		else
			low = mid + 1;
	}
	if (low == high)
		mid = (low+high)/2;

	return mid;
}

int seqlist_sortinsert(struct slist_st *ptr, const void *data, cmp_t cmp)
{
	char *new = NULL;	
	int ind;

	//新成员开辟存储空间
	new = realloc(ptr->start, ptr->size * (ptr->nmemb+1));	
	if (new == NULL)
		return -1;
	ptr->start = new;	

	//新插入元素找到指定的插入位置
	if (ptr->nmemb == 0) {
		ind = 0;
	} else {
		ind = seqlist_bsearch(ptr, data, cmp);
		if (cmp((char *)(ptr->start)+ptr->size*ind, data) < 0)
			ind ++;
	}
	//新元素插在顺序表的ind位置
	memmove((char *)(ptr->start)+(ind+1)*ptr->size,  \
			(char *)(ptr->start)+ind*ptr->size, \
			(ptr->nmemb-ind)*ptr->size);	
	memcpy((char *)(ptr->start)+ind*ptr->size, data, ptr->size);
	ptr->nmemb ++;

	return 0;
}

//找到指定元素的下标
static int seqlist_srh_index(struct slist_st *ptr, const void *key, cmp_t cmp)
{
	int i;

	for (i = 0; i < ptr->nmemb; i++) {
		if (!cmp((char *)(ptr->start)+i*ptr->size, key))
			return i;
	}

	return -1;
}

int seqlist_delete(struct slist_st *ptr, const void *key, cmp_t cmp)
{
	int ret;
	char *new = NULL;

	ret = seqlist_srh_index(ptr, key, cmp);
	if (ret < 0)
		return -1;
	//ret就是要删除元素的下标
	memmove((char *)ptr->start+ret*ptr->size, \
			(char *)ptr->start+(ret+1)*ptr->size,\
			(ptr->nmemb-ret-1)*ptr->size);
	ptr->nmemb --;
	if (ptr->nmemb == 0) {
		ptr->start = NULL;
	} else {
		new = realloc(ptr->start, ptr->nmemb*ptr->size);
		if (new == NULL)
			return -1;
		ptr->start = new;
	}

	return 0;
}

const void *seqlist_search(struct slist_st *ptr, const void *key, cmp_t cmp)
{
	int ret;	

	ret = seqlist_srh_index(ptr, key, cmp);
	if (ret < 0) {
		return NULL;	
	}

	return (char *)(ptr->start)+ret*ptr->size;
}

void seqlist_traval(const struct slist_st *ptr, print_t pri)
{
	int i;

	for (i = 0; i < ptr->nmemb; i++) {
		pri((char *)(ptr->start)+i*ptr->size);
	}
}

void seqlist_sort(struct slist_st *ptr, cmp_t cmp)
{
	qsort(ptr->start, ptr->nmemb, ptr->size, cmp);
}

void seqlist_destroy(struct slist_st *ptr)
{
	free(ptr->start);
	free(ptr);
}

