/************************************************************************************
 *@file name  : ev_tcp_server.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 *@creat time: 2018年 08月 17日 星期五 15:57:08 CST
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
#include "ev.h"
#include "ev_task.h"

/*typedef--------------------------------------------------------------------------*/
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
#define MAX_CLIENT 100
#define BUFFER_SIZE 1024
#define PORT 8866
#define MAX_LISTEN 100
/*variables------------------------------------------------------------------------*/
 struct ev_io *libevlist[MAX_CLIENT] = {NULL};
/*function prototype---------------------------------------------------------------*/
static int freelibev(struct ev_loop *loop, int fd);
static void on_ev_task(void);
static void timer_beat(struct ev_loop *loop, struct ev_io *watcher, int revents);
static void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
static void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);

/*variables------------------------------------------------------------------------*/

/*! \brief 处理TCP接收的数�?
 *
 *  on_process
 *
 * \param 客户端socket id
 * \param 接收的数�?
 * \param 接收的数�?�?�?
 */
static void on_ev_task(void){
    struct ev_loop *loop = ev_loop_new(EVFLAG_AUTO);
    int sd;
    struct sockaddr_in addr;
    int addr_len = sizeof(addr);

    struct ev_io socket_accept;
    struct ev_timer timeout_w;

    sd = socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        elog_w("warning", "socket creat faild!\n");
        return -1;
    }

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0){
        elog_w("warning", "bind error!\n");
        return -1;
    }

    if(listen(sd, MAX_LISTEN) < 0){
        elog_w("warning", "listen error!\n");
        return -1;
    }

    //
    int bReuseaddr = 1;
    if(setsockopt(sd,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(bReuseaddr)) != 0){
        elog_w("warning", "setsockopt error in reuseaddr[%d]\n", sd);
        return -1;
    }

    ev_io_init(&socket_accept, accept_cb, sd, EV_READ);
    ev_io_start(loop, &socket_accept);

    // ev_timer_init(&timeout_w, timer_beat, 1., 0);
    // ev_timer_start(loop, &timeout_w);

    ev_run(loop, 0);

    return 1;
}


static void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents){
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sd;

    struct ev_io *w_client = (struct ev_io *)malloc(sizeof(struct ev_io));

    if(w_client == NULL){
        elog_w("warning", "malloc error in accept_cb");
        return;
    }

    if(EV_ERROR &revents){
        elog_w("warning", "error event in accept_cb");
    }

    client_sd = accept(watcher->fd, (struct sockaddr*)&client_addr, &client_len);
    if(client_sd < 0){
        elog_w("warning", "accept error!\n");
        return;
    }

    if(client_sd > MAX_CLIENT){
        elog_w("warning", "fd to large!\n");
        close(client_sd);
        return;
    }

    if(libevlist[client_sd] != NULL){
        elog_w("warning", "client_sd not NULL fd is [%d]\n", client_sd);
        return;
    }

    elog_i("info", "client connected!\n");

    ev_io_init(w_client, read_cb, client_sd, EV_READ);
    ev_io_start(loop, w_client);

    libevlist[client_sd] = w_client;
}

static void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents){
    char buffer[BUFFER_SIZE] = {0};

    ssize_t read = 0;

    if(EV_ERROR & revents){
        elog_w("warning", "error event in read!\n");
        return;
    }

    read = recv(watcher->fd, buffer, sizeof(buffer), 0);

    if(read < 0){
        elog_w("warning", "read error!\n");
        return;
    }

    if(read == 0){
        elog_w("warning", "client disconnected!\n");
        freelibev(loop, watcher->fd);
        return;
    }
    else{
        elog_i("info", "recv");
    }
}

static void timer_beat(struct ev_loop *loop, struct ev_io *watcher, int revents){
    if(EV_ERROR & revents)
    {
        elog_w("warning", "error event in timer_beat\n");
        return ;
    }
    // ev_timer_set(watcher, 1.,0.);
    // ev_timer_start(loop, watcher);
}

static int freelibev(struct ev_loop *loop, int fd)
{  
    /*if(fd > MAX_ALLOWED_CLIENT)
    { 
        printf("more than MAX_ALLOWED_CLIENT[%d]", fd);
        return -1;
    }*/
    //清理相关资源
      
    if(libevlist[fd] == NULL)
    {
        printf("the fd already freed[%d]\n", fd);
        return -1;
    }
      
    close(fd);
    ev_io_stop(loop, libevlist[fd]);
    free(libevlist[fd]);
    libevlist[fd] = NULL;
    return 1;
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
int ev_tcp_server_init(void)
{
    int err = -1;
    pthread_t thread_id;
    err = pthread_create(&thread_id, NULL, (void *)on_ev_task, NULL);
    if(err!=0){
        elog_d("debug", "creat ev task thread error!err=%d",err);
    }
    elog_i("info", "ev task thread success!thread id=%lu\n", thread_id);

    return err;
}


