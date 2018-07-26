/************************************************************************************
 *@file name  : tcp_server.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 #*@creat time: 2018年 07月 26日 星期四 14:13:35 CST
************************************************************************************/
#ifndef _TCP_SERVER__H__
#define _TCP_SERVER__H__

#ifdef __cpluscplus
 extern "C"{
#endif

/*includes-------------------------------------------------------------------------*/
/*typedef--------------------------------------------------------------------------*/
struct tcp_sv_t{
    int port;
    int max_fd;
    int max_listen;
    void (*call_back)(int fd, char *buffer, int num);
};
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
/*function prototype---------------------------------------------------------------*/

int tcp_server_init(struct tcp_sv_t *me);
/*variables------------------------------------------------------------------------*/

#ifdef __cpluscplus
}
#endif

#endif