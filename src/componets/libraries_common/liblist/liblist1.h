/************************************************************************************
 *@file name  : liblist.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 *@creat time : 2018年 09月 21日 星期五 14:44:48 CST
************************************************************************************/


#ifndef __LIBLIST_H__
#define __LIBLIST_H__

#ifdef __cpluscplus
 extern "C"{
#endif

/*includes-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/*typedef--------------------------------------------------------------------------*/
typedef struct _tag_linknode{
    struct _tag_linknode *next;
}linknode;

typedef void* llink;

typedef void(*type_print)(linknode *);
typedef bool(*TYPE_COMPARE)(linknode *, linknode *);
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
/*function prototype---------------------------------------------------------------*/
llink init_linklist();
void insert_linknode(llink head, int pos, linknode *data);
void push_front_linknode(llink head, linknode *data);
void push_back_linknode(llink head, linknode *data);
linknode *read_by_pos_linknode(llink head, int pos);
linknode *remove_by_pos_linknode(llink head, int pos);
linknode *pop_front(llink head);
linknode *popback(llink head);
void remove_by_val_linknode(llink head, linknode *data, TYPE_COMPARE compare);
int get_linklist_len(llink head);
void destroy_linklist(llink head);
void foreach_linklist(llink head, type_print print);

/*variables------------------------------------------------------------------------*/

#ifdef __cpluscplus
}
#endif

#endif
