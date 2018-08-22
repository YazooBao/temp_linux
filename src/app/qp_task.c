/************************************************************************************
 *@file name  : qp_task.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 *@creat time: 2018年 08月 20日 星期一 14:24:44 CST
************************************************************************************/
/*includes-------------------------------------------------------------------------*/
#include "elog.h"
#include "qpc.h"
#include "blinky.h"
#include "bsp.h"
/*typedef--------------------------------------------------------------------------*/
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
/*function prototype---------------------------------------------------------------*/
static void on_qp_task(void);
/*variables------------------------------------------------------------------------*/

/*! \brief creat qp task
 *
 *  creat qp task
 *
 * \return err
 */
int qp_task_start(void)
{
    int err = -1;
    pthread_t thread_id;
    err = pthread_create(&thread_id, NULL, (void *)on_qp_task, NULL);
    if(err!=0){
        elog_d("debug", "creat qp task thread error!err=%d",err);
    }
    elog_i("info", "qp task thread success!thread id=%lu\n", thread_id);

    return err;

}

/*! \brief on qp task
 *
 *  on qp task
 *
 * \return none
 */
static void on_qp_task(void)
{
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





