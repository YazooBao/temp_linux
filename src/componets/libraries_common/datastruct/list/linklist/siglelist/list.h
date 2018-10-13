#ifndef _LIST_H
#define _LIST_H

/***********ADT************/
//结点的数据类型
struct node_st {
	void *data;//存储数据域的地址
	struct node_st *next;//存储后继结点的地址
};
//头结点数据类型
typedef struct {
	struct node_st head;
	int size;//存储结点数据字节数
}list_t; 

enum {
	FRONT = 1,
	TAIL
};

typedef void (*pri_t)(const void *data);
typedef int (*cmp_t)(const void *data, const void *key);

/************OPERATION**************/
int list_init(int size, list_t **head);

int list_insert(list_t *head, const void *data, int way);

//homework
int list_delete(list_t *head, const void *key, cmp_t cmp);

const void *list_search(list_t *head, const void *key, cmp_t cmp);

void list_traval(list_t *head, pri_t pri);

void list_destroy(list_t *head);

#endif

