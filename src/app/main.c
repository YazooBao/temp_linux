#include "qpc.h"
#include "blinky.h"
#include "bsp.h"
#include "elog.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "udp_server.h"
#include "version.h"

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

    struct tcp_sv_t sv_me1;
    struct tcp_sv_t sv_me2;
    struct tcp_clt_t clt_me1;
    struct tcp_clt_t clt_me2;
    struct udp_sv_t udp_me1;

    sv_me1.max_fd = 20;
    sv_me1.max_listen = 20;
    sv_me1.port = 8889;
    sv_me1.call_back = NULL;
    tcp_server_init(&sv_me1);
    sv_me2.port = 8887;
    sv_me2.max_fd = 21;
    sv_me2.max_listen = 21;
    sv_me2.call_back = NULL;
    tcp_server_init(&sv_me2);

    clt_me1.ipaddr = "192.168.180.1";
    clt_me1.port = 8885;
    clt_me1.call_back = NULL;
    tcp_client_init(&clt_me1);

    clt_me2.port = 8884;
    clt_me2.ipaddr = "192.168.180.1";
    clt_me2.call_back = NULL;
    tcp_client_init(&clt_me2);

    udp_me1.call_back = NULL;
    udp_me1.ipaddr = "192.168.180.1";
    udp_me1.mode = 0;
    udp_me1.port = 8888;
    udp_sv_init(&udp_me1);

    static QEvt const *l_blinkyQSto[10]; /* Event queue storage for Blinky */

        QF_init();  /* initialize the framework and the underlying RT kernel */
        BSP_init(); /* initialize the Board Support Package */

        /* publish-subscribe not used, no call to QF_psInit() */
        /* dynamic event allocation not used, no call to QF_poolInit() */

        /* instantiate and start the active objects... */
        Blinky_ctor();
        QACTIVE_START(AO_Blinky,      /* AO pointer to start */
                      1U,             /* unique QP priority of the AO */
                      l_blinkyQSto,   /* storage for the AO's queue */
                      Q_DIM(l_blinkyQSto), /* lenght of the queue [entries] */
                      (void *)0,      /* stack storage (not used in QK) */
                      0U,             /* stack size [bytes] (not used in QK) */
                      (QEvt *)0);     /* initial event (or 0) */

        return QF_run(); /* run the QF application */

}
