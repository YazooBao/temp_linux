/************************************************************************************
 *@file name  : event_tcp_server.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 *@creat time: 2018年 08月 20日 星期一 16:57:37 CST
************************************************************************************/


#ifndef __EVENT_TCP_SERVER_H__
#define __EVENT_TCP_SERVER_H__

#ifdef __cpluscplus
 extern "C"{
#endif

/*includes-------------------------------------------------------------------------*/
#include "liblist.h"
/*typedef--------------------------------------------------------------------------*/
typedef struct tcp_tag{
    list_t *c_list_header;//表头
    int port;
    void (*call_back)(struct tcp_tag *p);
}tcp_t;

typedef struct{
    struct bufferevent *bev;
    struct evbuffer *buf;
    char ip[16];
    int port;
}client_t;
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
/*function prototype---------------------------------------------------------------*/
void event_tcp_server_start(tcp_t *p);
/*variables------------------------------------------------------------------------*/

#ifdef __cpluscplus
}
#endif

#endif


