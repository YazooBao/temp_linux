/************************************************************************************
 *@file name  : liblist.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 *@creat time : 2018年 09月 21日 星期五 14:43:16 CST
************************************************************************************/
/*includes-------------------------------------------------------------------------*/
#include "liblist.h"
/*typedef--------------------------------------------------------------------------*/
typedef struct _tag_linklist{
    linknode header;
    int size;
}linklist;
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
llink init_linklist()
{
    linklist *list = (linklist *)malloc(sizeof(linklist));
    if(list == NULL){
        return NULL;
    }
    list->header.next = NULL;
    list->size = 0;
    return list;
}

/*! \brief insert elemnet
 *
 *  insert element
 *
 * \param *head :list header
 * \param pos :position
 * \param *data :insert data
 * \return :none
 */
void insert_linknode(llink head, int pos, linknode *data)
{
    if(head == NULL || data == NULL){
        return;
    }

    linklist *temp = (linklist *)head;

    if(pos < 0 || pos > temp->size){
        pos = temp->size;
    }

    linknode *pcurrent = &(temp->header);

    for(int i = 0; i < pos; i++){
        pcurrent = pcurrent->next;
    }

    data->next = pcurrent->next;
    pcurrent->next = data;
    temp->size++;
}

/*! \brief push
 *
 *  push
 *
 * \param (llink *h:header
 * \param nknode *d Parameter description
 * \returnata == NUarameter description
 */
void push_front_linknode(llink head, linknode *data)
{
    if(head == NULL || data == NULL){
        return;
    }

    insert_linknode(head, 0, data);
}

/*! \brief push back
 *
 *  push back
 *
 * \param *head :header
 * \param *data :data
 * \return none
 */
void push_back_linknode(llink head, linknode *data)
{
    if(head == NULL || data == NULL){
        return;
    }
    linklist *temp = (linklist *)head;
    insert_linknode(head, temp->size, data);
}

/*! \brief remove
 *
 *  remove
 *
 * \param *head :header
 * \param pos :position
 * \return linknode
 */
linknode* remove_by_pos_linknode(llink head, int pos)
{
    if(head == NULL){
        return NULL;
    }

    linklist *temp = (linklist *)head;
    if(pos < 0 || pos > temp->size){
        return NULL;
    }

    linknode *pcurrent = &(temp->header);
    for (int i = 0; i < pos; i++) {
        pcurrent = pcurrent->next;
    }

    linknode *delnode = pcurrent->next;
    pcurrent->next = delnode->next;
    temp->size--;

    return delnode;
}

/*! \brief remove
 *
 *  remove
 *
 * \param *head :header
 * \param *data :pos
 * \return linknode
 */
linknode *read_by_pos_linknode(llink head, int pos){
    if(head == NULL){
        return NULL;
    }

    linklist *temp = (linklist *)head;

    if(pos < 0 || pos > temp->size){
        pos = temp->size;
    }

    linknode *pcurrent = &(temp->header);

    for (int i = 0; i < pos; i++) {
        pcurrent = pcurrent->next;
    }

    return pcurrent;
}

/*! \brief remove
 *
 *  remove
 *
 * \param *head :header
 * \param *data :data
 * \param compare :call back
 * \return none
 */
void remove_by_val_linknode(llink head, linknode *data, TYPE_COMPARE compare)
{
    if(head == NULL || data == NULL || compare == NULL){
        return;
    }

    linklist *temp = (linklist *)head;
    if (temp->size == 0) {
        return;
    }

    linknode *pprev = &(temp->header);
    linknode *pcurrent = temp->header.next;

    while(pcurrent){
        if (compare(pcurrent, data)) {
            pprev->next = pcurrent->next;
            temp->size--;
            break;
        }
        pprev = pcurrent;
        pcurrent = pcurrent->next;
    }

    if (pcurrent == NULL) {
        return;
    }
}

/*! \brief del header
 *
 *  delete header
 *
 * \param k *head) :header
 * \return linknode *
 */
linknode* pop_front(llink head)
{
    if (head == NULL) {
        return NULL;
    }

    return remove_by_pos_linknode(head, 0);
}

/*! \brief del tail
 *
 *  delete tail
 *
 * \param *head :header
 * \return linknode*
 */
linknode* pop_back(llink head)
{
    if (head == NULL) {
        return NULL;
    }

    linklist *temp = (linklist*)head;

    return remove_by_pos_linknode(head, temp->size-1);
}

/*! \brief get len
 *
 *  get list len
 *
 * \param *head :header
 * \return int
 */
int get_linklist_len(llink head)
{
    if (head == NULL) {
        return 0;
    }

    linklist *temp = (linklist *)head;

    return temp->size;
}

/*! \brief destroy list
 *
 *  destroy list
 *
 * \param *head :header
 * \return none
 */
void destroy_linklist(llink head)
{
    if (head == NULL) {
        return;
    }

    free(head);
    head = NULL;
}

