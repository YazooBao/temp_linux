#include "tcp_server.h"
#include "elog.h"

struct tcp_sv_t sv_me1;
struct tcp_sv_t sv_me2;

static void on_phone_msg(int fd, char *buffer, int num);
static void on_pecu_msg(int fd, char *buffer, int num);

void phone_sock_init(void){
    

    sv_me1.max_fd = 20;
    sv_me1.max_listen = 20;
    sv_me1.port = 8003;
    sv_me1.call_back = on_phone_msg;
    tcp_server_init(&sv_me1);
}

void pecu_sock_init(void){
    

    sv_me2.max_fd = 20;
    sv_me2.max_listen = 20;
    sv_me2.port = 8004;
    sv_me2.call_back = on_pecu_msg;
    tcp_server_init(&sv_me2);
}

static void on_phone_msg(int fd, char *buffer, int num){
    elog_d("debug", "reciev phone data");
}

static void on_pecu_msg(int fd, char *buffer, int num){
    elog_d("debug", "reciev pecu data");
}