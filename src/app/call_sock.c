#include "event_tcp_server.h"
#include "elog.h"
#include "bufferevent.h"
// tcp_t sv_me1;
tcp_t sv_me2;
static void on_phone_msg(tcp_t *p);
static void on_pecu_msg(tcp_t *p);

void phone_sock_init(void){
    tcp_t *sv_me1 = (tcp_t *)malloc(sizeof(tcp_t));
    sv_me1->port       = 8003;
    sv_me1->call_back  = on_phone_msg;
    sv_me1->c_list_header = NULL;
    event_tcp_server_start(sv_me1);
    
}

void pecu_sock_init(void){
    sv_me2.port       = 8004;
    sv_me2.call_back  = on_pecu_msg;
    event_tcp_server_start(&sv_me2);
}

static void on_phone_msg(tcp_t *p){
    int list_len = 0;
    client_t *temp;
    char buf[1024] = {0};
    int read_size = 0;
    struct evbuffer *input, *output;

    list_len = list_size(p->c_list_header);
    elog_i("info","list_len=%d",list_len);
    if(list_len){
        for (int i = 0; i < list_len; i++) {
            temp = (void *)list_read(p->c_list_header, i);

            // input = bufferevent_get_input(temp->bev);
            // output = bufferevent_get_output(temp->bev);

            // read_size = evbuffer_remove(input, buf, sizeof(buf));
            // if(read_size){
            //     bufferevent_write(temp->bev, buf, read_size);
            //     elog_d("debug", "reciev pecu data");
            // }
        }
    }
}

static void on_pecu_msg(tcp_t *p){
    elog_d("debug", "reciev pecu data");
}