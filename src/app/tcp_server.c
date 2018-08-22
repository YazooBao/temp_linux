/************************************************************************************
 *@file name  : tcp_server.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 #*@creat time: 2018年 07月 20日 星期五 15:22:37 CST
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

#include "tcp_server.h"
#include "elog.h"
/*typedef--------------------------------------------------------------------------*/
struct cli_attr_t{
    int fd;
    char ip[INET_ADDRSTRLEN];
    int port;
};
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
#define SELECT_MODE 1
/*variables------------------------------------------------------------------------*/
static struct cli_attr_t cli_attr[100];
/*function prototype---------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
static int creat_tcp_server(void *p);
static int on_tcp_server(void *p);
/*! \brief 处理TCP接收的数据
 *
 *  on_process
 *
 * \param 客户端socket id
 * \param 接收的数据
 * \param 接收的数据个数
 */;
int tcp_server_init(struct tcp_sv_t *me){
    int i = 0;
    for(i = 0; i<sizeof(cli_attr)/sizeof(cli_attr[0]);i++){
        cli_attr[i].fd = -1;
        cli_attr[i].port = -1;
    }
    return creat_tcp_server(me);
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
#if (SELECT_MODE==1)
static int on_tcp_server(void *p)
{
    int sockfd;
    int err;
    int i;
    int connfd;
    struct tcp_sv_t me;// = (struct tcp_sv_t *)p;
    memcpy(&me, (struct tcp_sv_t *)p, sizeof(me));
    int fd_all[me.max_fd];    //保存所有描述符，用于select调用后判断哪个可读
    elog_d("debug", "port = %d, max_fd = %d, max_listen = %d\n", me.port, me.max_fd, me.max_listen);

    //下面两个备份原因是select调用后，会发生变化，再次调用select前会重新赋值
    fd_set fd_read; //FD_SET数据备份
    fd_set fd_select; //用于select

    struct timeval timeout; //超时备份
    struct timeval timeout_select; //用于select

    struct sockaddr_in serv_addr; //服务器地址
    struct sockaddr_in cli_addr;  //客户端地址

    socklen_t serv_len;
    socklen_t cli_len;

    //超时时间设置
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    //创建TCP套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        perror("fail to creat socket!\n");
        exit(1);
    }
    else{
        elog_d("debug", "creat socket success!\n");
    }

    int iSockOptVal = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &iSockOptVal, sizeof(iSockOptVal)) == -1) {
        perror("setsockopt fail");
        close(sockfd);
        exit(1);
    }
    //配置本地地址
    memset(&serv_addr, 0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; //ip_v4
    serv_addr.sin_port = htons(me.port);//端口
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);//ip

    serv_len = sizeof(serv_addr);

// int b_on = 1;
// ioctl (sockfd, FIONBIO, &b_on);
    //绑定
    err = bind(sockfd, (struct sockaddr * )&serv_addr, serv_len);
    if(err<0){
        perror("fail to bind!\n");
        exit(1);
    }
    else{
        elog_d("debug", "bind success!\n");
    }

    //监听
    err = listen(sockfd, me.max_listen);
    if(err<0){
        perror("fail to listen!\n");
        exit(1);
    }
    else{
        elog_d("debug", "listen success!\n");
    }

    //初始化fd_all数组
    memset(&fd_all, -1, sizeof(fd_all));

    fd_all[0] = sockfd;//第一个为监听套接字

    FD_ZERO(&fd_read);//清空
    FD_SET(sockfd, &fd_read);//将监听套接字加入fd_read

    int maxfd;
    maxfd = fd_all[0];//监听的最大套接字

    for(;;){
        // 每次都需要重新赋值，fd_select，timeout_select每次都会变
        fd_select = fd_read;
        timeout_select = timeout;
        // 检测监听套接字是否可读，没有可读，此函数会阻塞
        // 只要有客户连接，或断开连接，select()都会往下执行
        err = select(maxfd+1, &fd_select, NULL, NULL, NULL);
        //err = select(maxfd+1, &fd_select, NULL, NULL, (struct timeval *)&timeout_select);
        if(err < 0)
        {
            perror("fail to select");
            exit(1);
        }
        if(err == 0){
            elog_d("debug","timeout\n");
        }

        // 检测监听套接字是否可读
        if( FD_ISSET(sockfd, &fd_select) ){//可读，证明有新客户端连接服务器

            cli_len = sizeof(cli_addr);

            // 取出已经完成的连接
            connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
            if(connfd < 0)
            {
                perror("fail to accept");
                exit(1);
            }

            // 打印客户端的 ip 和端口
            char cli_ip[INET_ADDRSTRLEN] = {0};
            inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
            elog_i("info","----------------------------------------------\n");
            elog_i("info","client ip=%s,port=%d\n", cli_ip,ntohs(cli_addr.sin_port));
            cli_attr[i].fd = connfd;
            memcpy(cli_attr[i].ip, cli_ip, INET_ADDRSTRLEN);
            cli_attr[i].port = ntohs(cli_addr.sin_port);
            // 将新连接套接字加入 fd_all 及 fd_read
            for(i=0; i < me.max_fd; i++){
                if(fd_all[i] != -1){
                    continue;
                }else{
                    fd_all[i] = connfd;
                    elog_i("info","client fd_all[%d] join\n", i);
                    break;
                }
            }

            FD_SET(connfd, &fd_read);

            if(maxfd < connfd)
            {
                maxfd = connfd;  //更新maxfd
            }

        }

        //从1开始查看连接套接字是否可读，因为上面已经处理过0（sockfd）
        for(i = 1; i < maxfd; i++){
            if(FD_ISSET(fd_all[i], &fd_select)){
                elog_d("debug","fd_all[%d] is ok\n", i);

                char buf[1024] = {0};  //读写缓冲区
                int num        = read(fd_all[i], buf, 1024);
                if(num > 0){
                    //TODO:处理客户端数据
                    if(me.call_back == NULL){
                        elog_i("info", "server call back is null!\n");
                    }
                    else{
                        me.call_back(fd_all[i], buf, num);
                    }
                }else if(0 == num){ // 客户端断开时

                    //客户端退出，关闭套接字，并从监听集合清除
                    elog_i("info","client:fd_all[%d] exit\n", i);
                    FD_CLR(fd_all[i], &fd_read);
                    close(fd_all[i]);
                    fd_all[i] = -1;
                    cli_attr[i].fd = -1;
                    continue;
                }
            }else {
                //printf("no data\n");
            }
        }
    }
}
#else
void on_tcp_server(void){
    //1.创建tcp监听套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        perror("fail to creat socket!\n");
        exit(1);
    }
    else{
        elog_d("debug", "creat socket success!\n");
    }
    //2.绑定sockfd
    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(8000);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));

    //3.监听listen
    listen(sockfd, 10);

    //4.poll相应参数准备
    struct pollfd client[OPEN_MAX];
    int i = 0, maxi = 0;
    for(;i<OPEN_MAX; i++)
        client[i].fd = -1;//初始化poll结构中的文件描述符fd

    client[0].fd = sockfd;//需要监测的描述符
    client[0].events = POLLIN;//普通或优先级带数据可读

    //5.对已连接的客户端的数据处理
    while(1)
    {
        int ret = poll(client, maxi+1, -1);//对加入poll结构体数组所有元素进行监测

        //5.1监测sockfd(监听套接字)是否存在连接
        if((client[0].revents & POLLIN) == POLLIN )
        {
            struct sockaddr_in cli_addr;
            int clilen = sizeof(cli_addr);
            int connfd = 0;
            //5.1.1 从tcp完成连接中提取客户端
            connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

            //5.1.2 将提取到的connfd放入poll结构体数组中，以便于poll函数监测
            for(i=1; i<OPEN_MAX; i++)
            {
                if(client[i].fd < 0)
                {
                    client[i].fd = connfd;
                    client[i].events = POLLIN;
                    break;
                }
            }

            //5.1.3 maxi更新
            if(i > maxi)
                maxi = i;

            //5.1.4 如果没有就绪的描述符，就继续poll监测，否则继续向下看
            if(--ret <= 0)
                continue;
        }

        //5.2继续响应就绪的描述符
        for(i=1; i<=maxi; i++)
        {
            if(client[i].fd < 0)
                continue;

            if(client[i].revents & (POLLIN | POLLERR))
            {
                int len = 0;
                char buf[128] = "";

                //5.2.1接受客户端数据
                if((len = recv(client[i].fd, buf, sizeof(buf), 0)) < 0)
                {
                    if(errno == ECONNRESET)//tcp连接超时、RST
                    {
                        close(client[i].fd);
                        client[i].fd = -1;
                    }
                    else
                        perror("read error:");

                }
                else if(len == 0)//客户端关闭连接
                {
                    close(client[i].fd);
                    client[i].fd = -1;
                }
                else//正常接收到服务器的数据
                    send(client[i].fd, buf, len, 0);

                //5.2.2所有的就绪描述符处理完了，就退出当前的for循环，继续poll监测
                if(--ret <= 0)
                    break;

            }
        }
    }
}
#endif
/*! \brief creat tcp server thread
 *
 *  creat tcp server thread
 *
 * \param int port Parameter description
 * \param Parameter Parameter description
 * \param Parameter Parameter description
 * \return Return parameter description
 */
static int creat_tcp_server(void *p)
{
    int err = -1;
    pthread_t thread_id;
    err = pthread_create(&thread_id, NULL, (void *)on_tcp_server, p);
    if(err!=0){
        elog_i("info", "creat tcp server thread error!err=%d",err);
    }
    elog_i("info", "tcp server thread success!thread id=%lu\n", thread_id);

    return err;
}

