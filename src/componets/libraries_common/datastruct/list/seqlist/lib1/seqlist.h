#ifndef _SEQLIST_H
#define _SEQLIST_H
//条件编译:防止头文件重复包含
//主要：类型的定义以及函数的声明

typedef void (*print_t)(const void *data);
typedef int (*cmp_t)(const void *data, const void *key);

/*****************ADT*******************/
struct slist_st {
	void *start;//顺序表的首地址
	int size;//顺序表中每个成员的字节数
	int nmemb;//顺序表中成员个数
};

/****************OPERATOR*******************/
//初始化表头结构
/*seqlist_init:初始化一个表头结构
 *@size:顺序表中存储元素的字节数
 *return:
 *		表头结构的地址
 * */
struct slist_st *seqlist_init(int size);

//表的插入
/*seqlist_insert:向顺序表中插入一个元素
 *@ptr:表头的地址
 *@data:待插入元素的地址
 *return:
 *		插入是否成功
 * */

int seqlist_insert(struct slist_st *ptr, const void *data);

/*表的删除
 *@ptr:表头地址
 *@key:要删除元素的关键字
 *@cmp:key和顺序表中元素的比较函数
 * */
int seqlist_delete(struct slist_st *ptr, const void *key, cmp_t cmp);

/*表中元素的搜索
 *@ptr:表头地址
 *@key:要删除元素的关键字
 *@cmp:key和顺序表中元素的比较函数
 *return:搜索元素的地址
 * */
const void *seqlist_search(struct slist_st *ptr, const void *key, cmp_t cmp);

//表的遍历
/*seqlist_traval:打印输出表中每个成员
 *@ptr:表头地址
 *@pri:打印一个成员的函数
 * */
void seqlist_traval(const struct slist_st *ptr, print_t pri);

//表的销毁
void seqlist_destroy(struct slist_st *ptr);

//为顺序表排序
void seqlist_sort(struct slist_st *ptr, cmp_t cmp);

//homework 
/*
 * 使顺序表插入新元素的同时是有序的
seqlist_insert_sort();
*/

#endif

