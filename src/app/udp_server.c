/************************************************************************************
 *@file name  : udp_server.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 #*@creat time: 2018年 07月 26日 星期四 15:34:44 CST
************************************************************************************/
/*includes-------------------------------------------------------------------------*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "udp_server.h"
#include "elog.h"
/*typedef--------------------------------------------------------------------------*/
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
/*function prototype---------------------------------------------------------------*/
static creat_udp_sv(void *p);
/*variables------------------------------------------------------------------------*/

/*! \brief 处理TCP接收的数据
 *
 *  on_process
 *
 * \param 客户端socket id
 * \param 接收的数据
 * \param 接收的数据个数
 */
int udp_sv_init(struct udp_sv_t *me){
    elog_d("debug", "udp server ip addr = %s, port = %d, mode = %d\n", me->ipaddr, me->port, me->mode);
    return creat_udp_sv(me);
}


/*! \brief creat socket
 *
 *  creat socket
 *
 * \param int s_port server port
 * \param Parameter Parameter description
 * \param Parameter Parameter description
 * \return socket fd
 */
static int on_udp_sv(void *p)
{
    int sockfd;
    int err;
    int i;
    int connfd;
    char buffer[1024] = {0};
    struct udp_sv_t *me = (struct udp_sv_t *)p;
    int yes;
    size_t size = 0;
    //下面两个备份原因是select调用后，会发生变化，再次调用select前会重新赋值
    fd_set fd_read; //FD_SET数据备份

    struct sockaddr_in serv_addr; //服务器地址
    struct sockaddr_in cli_addr;  //客户端地址
    struct ip_mreq mreq;

    socklen_t serv_len;
    socklen_t cli_len;

    //创建TCP套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0){
        perror("fail to creat socket!\n");
        exit(1);
    }
    else{
        elog_d("debug", "creat socket success!\n");
    }

    //配置本地地址
    memset(&serv_addr, 0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; //ip_v4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(me->port);//端口

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        perror("bind faild!\n");
        exit(1);
    }

    if(me->mode){
        elog_i("info", "udp multicast mode!\n");
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0){
            perror("reusing addr failed!\n");
            exit(1);
        }
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        mreq.imr_multiaddr.s_addr = inet_addr(me->ipaddr);
        if(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0){
            perror("setsockopt failed!\n");
            exit(1);
        }
    }
    else{
        elog_i("info", "udp unicast mode!\n");
    }



    for(;;){
//        if((size = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&serv_addr, &serv_len)) < 0){
            if((size = recv(sockfd, buffer, 1024, 0)) < 0){
            perror("recvfrom faild!\n");
            exit(1);
        }
        else{
            if(me->call_back == NULL){
                elog_w("warning", "call_back is NULL!\n");
            }
            else{
                me->call_back(sockfd, buffer, size);
            }
        }
    }
}

/*! \brief creat tcp server thread
 *
 *  creat tcp server thread
 *
 * \param int port Parameter description
 * \param Parameter Parameter description
 * \param Parameter Parameter description
 * \return Return parameter description
 */
static int creat_udp_sv(void *p)
{
    int err = -1;
    pthread_t thread_id;

    err = pthread_create(&thread_id, NULL, (void *)on_udp_sv,p);
    if(err!=0){
        elog_i("info", "creat udp server thread error!err=%d",err);
    }
    elog_i("info", "udp server thread success!thread id = %lu\n", thread_id);
    return err;
}
