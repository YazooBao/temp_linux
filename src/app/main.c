#include "qpc.h"
#include "blinky.h"
#include "bsp.h"
#include "elog.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "udp_server.h"
#include "call_sock.h"
#include "version.h"

int main(int argc, char **argv){
    extern void elog_config(void);
    extern void pr_config(char *path);
struct tcp_sv_t sv_me1;
    int svn_ver = 0;
    svn_ver = SVN_VERSION;
    elog_config();

    QF_init();  /* initialize the framework and the underlying RT kernel */
    BSP_init(); /* initialize the Board Support Package */

    // elog_i("info", PRJ_NAME);
    // elog_i("info", BUILD_TIME);
    // elog_i("info", BUILD_VERSION);
    // elog_i("info","SVN version : %ld", SVN_VERSION);

    pr_config("./call.xml");
    // pr_config("./bao.xml");

sv_me1.max_fd = 20;
    sv_me1.max_listen = 20;
    sv_me1.port = 8003;
    sv_me1.call_back = NULL;
    tcp_server_init(&sv_me1);
    // phone_sock_init();
    // pecu_sock_init();
    static QEvt const *l_blinkyQSto[10]; /* Event queue storage for Blinky */

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
