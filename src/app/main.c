#include "qpc.h"
#include "blinky.h"
#include "bsp.h"
#include "elog.h"

int main(int argc, char **argv){
    extern void elog_config(void);
    extern int creat_tcp_server(void);
    elog_config();
    log_a("elog_a print!\n");
    log_e("elog_e print!\n");
    log_w("elog_w print!\n");
    log_i("elog_i print!\n");
    log_d("elog_d print!\n");
    log_v("elog_v print!\n");
    creat_tcp_server();
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
