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
#include "event.h"
#include "listener.h"
#include "bufferevent.h"
#include "thread.h"
// #include "event2/util.h"
// #include "event-internal.h"

#include "liblist.h"
#include "event_tcp_server.h"
/*typedef--------------------------------------------------------------------------*/
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
/*function prototype---------------------------------------------------------------*/
static int tcp_server_init(int port, int listen_num);
static void on_tcp_server(tcp_t *p);
static client_t *client_new(void);
static bool client_delete(client_t *p);

/*variables------------------------------------------------------------------------*/

#if 0
static void accept_cb(int fd, short events, void *arg);
static void socket_read_cb(int fd, short events, void *arg);
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
    else{
        elog_i("info", "server fd:%d!\n",listener);
    }

    int listener1 = tcp_server_init(9998, 10);
    if(listener1 == -1){
        elog_w("warning", "tcp server init error!\n");
        return -1;
    }
    else{
        elog_i("info", "server fd:%d!\n",listener1);
    }

    evthread_use_pthreads();
    struct event_base *base = event_base_new();

    const char *x =  event_base_get_method(base); //查看用了哪个IO多路复用模型，linux一下用epoll
    elog_i("info", "event METHOD:%s\n", x);

    struct event *ev_listen = event_new(base, listener, EV_READ | EV_PERSIST, accept_cb, base);
    event_add(ev_listen, NULL);

    struct event *ev_listen1 = event_new(base, listener1, EV_READ | EV_PERSIST, accept_cb, base);
    event_add(ev_listen1, NULL);

    event_base_dispatch(base);

    event_base_free(base);
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
    struct skt_addr cl_add;

    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    sockfd = accept(fd, (struct sockaddr*)&client, &len);
    evutil_make_socket_nonblocking(sockfd);
    elog_i("info", "ip:%s, port:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

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
    sin.sin_family      = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port        = htons(port);

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
#else
static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, 
                        struct sockaddr *sock, int socklen, void *arg);
static void socket_read_cb(struct bufferevent *bev, void *arg);
static void socket_event_cb(struct bufferevent  *bev, short events, void *arg);
static void timeout_cb(evutil_socket_t fd, short event, void *arg);
/*! \brief tcp server task
 *
 *  tcp server task
 *
 * \return none
 */
static void on_tcp_server(tcp_t *p)
{
    struct event timeout;
    struct timeval tv;

    evthread_use_pthreads();//enable threads

    struct event_base *base = event_base_new();

    /* Initalize one event */
    event_assign(&timeout, base, -1, EV_PERSIST, timeout_cb, (void*) &timeout);
    evutil_timerclear(&tv);
    tv.tv_sec = 1;
    event_add(&timeout, &tv);

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(p->port);

    struct evconnlistener *listener = \
    evconnlistener_new_bind(base, listener_cb, base, LEV_OPT_REUSEABLE |
                            LEV_OPT_CLOSE_ON_FREE, 10, (struct sockaddr_in*)&sin, 
                            sizeof(struct sockaddr_in));

elog_i("info","port:%d, header:%lu,call%lu",p->port, p->c_list_header,p->call_back);
    // lisen_arg->tcp_arg = p;
    // lisen_arg->evbs_arg = base;
    evconnlistener_set_cb(listener, listener_cb, p);
    //TODO: change ev arg;
    event_base_dispatch(base);
    evconnlistener_free(listener);
    event_base_free(base);

    return 0;
}

/*! \brief listener call back
 *
 *  listener call back
 *
 * \param listener : listener
 * \param fd       : socket fd
 * \param *sock    : socket addr
 * \param socklen  : socket len
 * \param arg      : user data
 * \return none
 */
static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, 
                        struct sockaddr *sock, int socklen, void *arg)
{
    client_t cl;
    tcp_t *temp;

    char ip[30];
    struct sockaddr_in *client_addr = (struct sockaddr_in *)sock;
    // ip = inet_ntoa(client_addr->sin_addr);
    int port = ntohs(client_addr->sin_port);

    elog_i("info", "accecp a client %d, ip:%x, ip_string:%s, port:%d\n", 
            fd, client_addr->sin_addr, inet_ntoa(client_addr->sin_addr), port);

    struct event_base *base = evconnlistener_get_base(listener);

    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    // struct evbuffer *buf = evbuffer_new();

    temp = (tcp_t *)arg;

    bufferevent_setcb(bev, NULL, NULL,socket_event_cb, arg);
    bufferevent_enable(bev, EV_READ | EV_PERSIST);

    cl.bev  = bev;
    // cl.buf  = buf;
    cl.port = port;
    strcpy(cl.ip, ip);

    //TODO: insert to list
    list_insert(temp->c_list_header, &cl, TAIL);
}

/*! \brief tcp server task
 *
 *  tcp server task creat
 *
 * \return none
 */
static int id_cmp(const void *data, const void *key){
    client_t *d = (void *)data;
    struct bufferevent *bev = (void *)key;

    if(d->bev == bev){
        return 0;
    }
    else{
        return -1;
    }
}

/*! \brief socket read call back
 *
 *  read call back
 *
 * \param *bev buffer event
 * \param *arg :user data
 * \return none
 */
static void socket_read_cb(struct bufferevent *bev, void *arg)
{
    tcp_t *temp = (tcp_t *)arg;
    client_t * client;

    char msg[1024];

    size_t len = bufferevent_read(bev, msg, sizeof(msg));

    client = (void *)list_search(temp->c_list_header, bev, id_cmp);
    if(client == NULL){
        elog_w("warning","list element not find!\n");
        return;
    }
    elog_i("info","recv data!\n");
    evbuffer_add(client->buf, msg, len);
    bufferevent_write(bev, msg, len);
}

/*! \brief event call back
 *
 *  socket event call back
 *
 * \param e *bev, T buffer event
 * \param nts Param events
 * \param  Parameter description
 * \return none
 */
static void socket_event_cb(struct bufferevent *bev, short events, void *arg)
{
    tcp_t *temp = (tcp_t *)arg;;
    client_t * client;

    //TODO:remove frome list; free client_t
    client = (void *)list_search(temp->c_list_header, bev, id_cmp);
    if(client == NULL){
        elog_w("warning","list element not find!\n");
        return;
    }

    if(events & BEV_EVENT_EOF){
        elog_w("warning", "connection closed!---ip:%x, port:%d",
                client->ip, client->port);
    }
    else if(events & BEV_EVENT_ERROR){
        elog_w("warning", "some other error\n");
    }
    
    // evbuffer_free(client->buf);
    bufferevent_free(bev);
    if(0==list_delete(temp->c_list_header, bev, id_cmp)){
        elog_i("info", "list deleted");
    }
}
#endif

/*! \brief timer out call back
 *
 *  timer out call back
 *
 * \param fd    : listener
 * \param event : socket fd
 * \param arg   : user data
 * \return none
 */
static void timeout_cb(evutil_socket_t fd, short event, void *arg)
{
    struct event *timeout = (struct event *)arg;

    struct timeval tv;
    evutil_timerclear(&tv);
    tv.tv_sec = 1;
    event_add(timeout, &tv);

    elog_i("info", "timer out!\n");
}

static void on_parse(tcp_t *p){
    for(;;){
        if(p->call_back){
            p->call_back(p);
        }
        sleep(1);
        // list_len = get_linklist_len(p->c_list_header);
        // if(len){
        //     for (int i = 0; i < len; i++) {
        //         temp = (client_t *)read_by_pos_linknode(p->c_list_header, i);
        //         read_size = evbuffer_remove(temp->buf, buf, sizeof(buf));
        //         if(read_size){
        //             if(p->call_back){
        //                 p->call_back(buf, read_size);
        //             }
        //         }
        //     }
        // }
    }
}
/*! \brief tcp server task
 *
 *  tcp server task creat
 *
 * \return none
 */
void event_tcp_server_start(tcp_t *p)
{
    int err = -1;
    pthread_t thread_id;

    if(list_init(sizeof(client_t), &p->c_list_header) < 0){
        elog_w("warning","list_init failed!\n");
    }

    err = pthread_create(&thread_id, NULL, (void *)on_tcp_server, (void *)p);
    if(err!=0){
        elog_d("debug", "creat event tcp server task thread error!err=%d",err);
    }
    elog_i("info", "event tcp server task thread success!thread id=%lu\n", thread_id);

    err = pthread_create(&thread_id, NULL, (void *)on_parse, (void *)p);
    if(err!=0){
        elog_d("debug", "creat event tcp server task thread error!err=%d",err);
    }
    elog_i("info", "event tcp server task thread success!thread id=%lu\n", thread_id);
    return err;
}
