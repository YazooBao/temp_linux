/************************************************************************************
 *@file name  : ev_task.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 #*@creat time: 2018�? 07�? 23�? 星期�? 16:55:37 CST
************************************************************************************/

/*includes-------------------------------------------------------------------------*/
#include "elog.h"
#include "ev.h"
/*typedef--------------------------------------------------------------------------*/
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
/*function prototype---------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
ev_timer timeout_watcher;

/*! \brief 处理TCP接收的数�?
 *
 *  on_process
 *
 * \param 客户端socket id
 * \param 接收的数�?
 * \param 接收的数�?�?�?
 */
static void timeout_cb(EV_P_ ev_timer *w, int intrevents){
    elog_d("debug", "ev timer out!\n");
    // ev_break (EV_A_ EVBREAK_ONE);
}

/*! \brief 处理TCP接收的数�?
 *
 *  on_process
 *
 * \param 客户端socket id
 * \param 接收的数�?
 * \param 接收的数�?�?�?
 */
static void on_ev_task(void){
    struct ev_loog *loop = ev_loop_new(EVFLAG_AUTO);

    ev_timer_init(&timeout_watcher, timeout_cb,5.5, 3.);
    ev_timer_start(loop, &timeout_watcher);

    elog_d("debug", "ev loop run!\n");
    ev_run(loop, 0);

    elog_d("debug", "ev loop stop!\n");
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
int ev_task_start(void)
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
