#include "elog.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "udp_server.h"
#include "call_sock.h"
#include "version.h"
// #include "ev_task.h"
// #include "ev_tcp_server.h"
#include "qp_task.h"
#include "event_tcp_server.h"

struct tcp_sv_t sv_me1;
int main(int argc, char **argv){
    extern void elog_config(void);
    extern void pr_config(char *path);

    int svn_ver = 0;
    svn_ver = SVN_VERSION;
    elog_config();

    // elog_i("info", PRJ_NAME);
    // elog_i("info", BUILD_TIME);
    // elog_i("info", BUILD_VERSION);
    // elog_i("info","SVN version : %ld", SVN_VERSION);

    pr_config("./call.xml");
    // pr_config("./bao.xml");

// ev_tcp_server_init();
// ev_task_start();
    phone_sock_init();
    // pecu_sock_init();
event_tcp_server_start();
    qp_task_start();

    for(;;){
        sleep(1);
    }
}
