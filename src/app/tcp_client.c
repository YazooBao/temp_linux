/************************************************************************************
 *@file name  : tcp_client.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 #*@creat time: 2018年 07月 23日 星期五 16:55:37 CST
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
#include "tcp_client.h"
#include "elog.h"
/*typedef--------------------------------------------------------------------------*/
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
#define SELECT_MODE 1
/*variables------------------------------------------------------------------------*/
/*function prototype---------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
static int on_tcp_client(void *p);
static int creat_tcp_client(void *p);

/*! \brief 处理TCP接收的数据
 *
 *  on_process
 *
 * \param 客户端socket id
 * \param 接收的数据
 * \param 接收的数据个数
 */
int tcp_client_init(struct tcp_clt_t *me){
    elog_d("debug", "server ip addr = %s, port = %d\n", me->ipaddr, me->port);
    return creat_tcp_client(me);
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
int on_tcp_client(void *p)
{
    int sockfd;
    int err;
    int i;
    int connfd;
    char buffer[1024] = {0};
    struct tcp_clt_t *me = (struct tcp_clt_t *)p;

    size_t size = 0;
    //下面两个备份原因是select调用后，会发生变化，再次调用select前会重新赋值
    fd_set fd_read; //FD_SET数据备份

    struct sockaddr_in serv_addr; //服务器地址
    struct sockaddr_in cli_addr;  //客户端地址

    socklen_t serv_len;
    socklen_t cli_len;

    //创建TCP套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
    serv_addr.sin_port = htons(me->port);//端口
    inet_pton(AF_INET, me->ipaddr, &serv_addr.sin_addr.s_addr);

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("connect error");
    }
    else{
        elog_i("info", "client: connect OK\n");
    }

    for(;;){
        //调用connect指定服务器的ip
        if(sockfd < 0){
            //创建TCP套接字
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if(sockfd<0){
                perror("fail to creat socket!\n");
                exit(1);
            }
            else{
                elog_d("debug", "creat socket success!\n");
            }
            if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
                perror("connect error");
            }
            else{
                elog_i("info", "client: connect OK\n");
            }
        }
        else{
            if((size = read(sockfd, buffer, sizeof(buffer))) < 0){
                perror("read error");
            }
            else{
                if(me->call_back==NULL){
                    elog_i("info", "call back function is null!\n");
                    exit(1);
                }
                else{
                    me->call_back(sockfd, buffer, size);
                }
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
static int creat_tcp_client(void *p)
{
    int err = -1;
    pthread_t thread_id;

    err = pthread_create(&thread_id, NULL, (void *)on_tcp_client,p);
    if(err!=0){
        elog_i("info", "creat tcp client thread error!err=%d",err);
    }
    elog_i("info", "tcp client thread success!thread id = %lu\n", thread_id);
    return err;
}

