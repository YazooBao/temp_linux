/************************************************************************************
 *@file name  : event_tcp_server.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 *@creat time: 2018年 08月 20日 星期一 16:57:25 CST
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

#include "elog.h"
// #include <event2/bufferevent.h>
// #include <event2/buffer.h>
// #include <event2/listener.h>
#include "event2/event.h"
// #include "event2/util.h"
// #include "event-internal.h"

/*typedef--------------------------------------------------------------------------*/
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
/*function prototype---------------------------------------------------------------*/
static void accept_cb(int fd, short events, void *arg);
static void socket_read_cb(int fd, short events, void *arg);

static int tcp_server_init(int port, int listen_num);
static void on_tcp_server(void);
/*variables------------------------------------------------------------------------*/

/*! \brief tcp server task
 *
 *  tcp server task
 *
 * \return none
 */
static void on_tcp_server(void)
{
    int listener = tcp_server_init(9999, 10);
    if(listener == -1){
        elog_w("warning", "tcp server init error!\n");
        return -1;
    }

    struct event_base *base = event_base_new();

    struct event *ev_listen = event_new(base, listener, EV_READ | EV_PERSIST, accept_cb, base);
    event_add(ev_listen, NULL);

    event_base_dispatch(base);

    return 0;

}

/*! \brief accept callback function
 *
 *  accept callback function
 *
 * \param int fd socket fd
 * \param event event
 * \param arg arg
 * \return none
 */
static void accept_cb(int fd, short event, void* arg)
{
    evutil_socket_t sockfd;

    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    sockfd = accept(fd, (struct sockaddr*)&client, &len);
    evutil_make_socket_nonblocking(sockfd);

    elog_i("info", "accept a client %d\n", sockfd);

    struct event_base* base = (struct event_base*)arg;

    struct event *ev = event_new(NULL, -1, 0, NULL, NULL);
    event_assign(ev, base, sockfd, EV_READ | EV_PERSIST, socket_read_cb, (void*)ev);

    event_add(ev, NULL);
}

/*! \brief read callback
 *
 *  read callback
 *
 * \param fd :socke fd
 * \param events events
 * \param *arg arg
 * \return none
 */
static void socket_read_cb(int fd, short events, void *arg)
{
    char msg[4096];
    struct event *ev = (struct event*)arg;
    int len = read(fd, msg, sizeof(msg)-1);

    if(len <= 0){
        elog_w("warning", "some error happen when read\n");
        event_free(ev);
        close(fd);
        return;
    }

    elog_i("info", "read!\n");
}

/*! \brief tcp server init
 *
 *  tcp server init
 *
 * \param port tcp server port
 * \param listen_num :max listen num
 * \return error num
 */
static int tcp_server_init(int port, int listen_num)
{
    int errno_save;
    evutil_socket_t listener;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener == -1){
        elog_w("warning", "creat socket failed!\n");
        return -1;
    }

    evutil_make_listen_socket_reuseable(listener);

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(port);

    if(bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0){
        goto error;
    }

    if(listen(listener, listen_num) < 0){
        goto error;
    }

    evutil_make_socket_nonblocking(listener);

    return listener;

error:
    errno_save = errno;
    evutil_closesocket(listener);
    errno = errno_save;

    return -1;
}

/*! \brief tcp server task
 *
 *  tcp server task creat
 *
 * \return none
 */
void event_tcp_server_start(void)
{
    int err = -1;
    pthread_t thread_id;
    err = pthread_create(&thread_id, NULL, (void *)on_tcp_server, NULL);
    if(err!=0){
        elog_d("debug", "creat event tcp server task thread error!err=%d",err);
    }
    elog_i("info", "event tcp server task thread success!thread id=%lu\n", thread_id);

    return err;
}
