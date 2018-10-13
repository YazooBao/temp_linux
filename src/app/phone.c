/************************************************************************************
 *@file name  : phone.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 *@creat time: 2018年 08月 24日 星期五 15:44:27 CST
************************************************************************************/
/*includes-------------------------------------------------------------------------*/
#include "qpc.h"
#include "libvector.h"
/*typedef--------------------------------------------------------------------------*/
typedef struct {
    QActive super;

    uint8_t cab;
    uint8_t id;
    QTimeEvt timeEvt;
}Phone;
/*define---------------------------------------------------------------------------*/
Q_DEFINE_THIS_FILE
/*macro----------------------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
static Phone l_phone[100];
QActive *const AO_phone[100] = {0};
vector_t *v_phone = NULL;
/*function prototype---------------------------------------------------------------*/
static QState idle(Phone * const me, QEvt const * const e);
static QState calling(Phone * const me, QEvt const * const e);
static QState ring(Phone * const me, QEvt const * const e);
/*variables------------------------------------------------------------------------*/

/*! \brief phone ctor
 *
 *  phone ctor
 *
 * \return none
 */
void Phone_ctor()
{
    for(int i=0; i<sizeof(AO_phone); i++){
        AO_phone[i] = l_phone[i].super;
    }

    Phone *me;
    for(int i=0;i<sizeof(AO_phone); i++){
        me = &l_phone[i].super;
        QActive_ctor(&me->super, Q_STATE_CAST(&Phone_initial));
        QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0u);
    }
    v_phone = vector_create(QActive *);
}

phone_new(){
    Phone *me = NULL;
    Phone *l_phone = calloc(1, sizeof(Phone));
    QActive *const *AO_phone = calloc(1, sizeof(QActive *));

    AO_phone = l_phone.super;
    me = &l_phone.super;
    QActive_ctor(&me->super, Q_STATE_CAST(&Phone_initial));
    QTimeEvt_ctorX(me->timeEvt, &me->super, TIMEOUT_SIG, 0u);

    vector_push_back(v_phone, AO_phone);


}
/*! \brief phone initial
 *
 *  phone active initial
 *
 * \param me :AO
 * \param e :event
 * \return AO state
 */
static QState Phone_initial(Phone * const me, QEvt const * const e)
{
    (void)e;

    QTimeEvt_armX(&me->timeEvt, BSP_TICKS_PER_SEC, BSP_TICKS_PER_SEC);
    return Q_TRAN(idle);
}


/*! \brief phone idle
 *
 *  phone idle
 *
 * \param me :AO
 * \param e :event
 * \return AO state
 */
static QState top(Phone * const me, QEvt const * const e)
{
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG:

            status_ = Q_HANDLED();
            break;
        case Q_EXIT_SIG:{

            status_ = Q_HANDLED();
            break;
        }
        case TIMEOUT_SIG:{
            elog_i("info", "send heart!\n");
            status_ = Q_HANDLED();
            break;
        }
        case HEART_SIG:{

            elog_i("info", "reciv heart!\n");
            status_ = Q_HANDLED();
            break;
        }
        default:{
            status_ = Q_SUPER(QHsm_top);
            break;
        }

    }
}
/*! \brief phone idle
 *
 *  phone idle
 *
 * \param me :AO
 * \param e :event
 * \return AO state
 */
static QState idle(Phone * const me, QEvt const * const e)
{
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG:

            status_ = Q_HANDLED();
            break;
        case Q_EXIT_SIG:{

            status_ = Q_HANDLED();
        }
        default:{
            status_ = Q_SUPER(top);
        }

    }
}

/*! \brief phone calling
 *
 *  phone idle
 *
 * \param me :AO
 * \param e :event
 * \return AO state
 */
static QState calling(Phone * const me, QEvt const * const e)
{
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG:

            status_ = Q_HANDLED();
            break;
        case Q_EXIT_SIG:{

            status_ = Q_HANDLED();
        }
        default:{
            status_ = Q_SUPER(top);
                }

    }
}

/*! \brief phone ring
 *
 *  phone idle
 *
 * \param me :AO
 * \param e :event
 * \return AO state
 */
static QState ring(Phone * const me, QEvt const * const e)
{
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG:

            status_ = Q_HANDLED();
            break;
        case Q_EXIT_SIG:{

            status_ = Q_HANDLED();
        }
        default:{
            status_ = Q_SUPER(top);
                }

    }
}

/*! \brief phone idle
 *
 *  phone idle
 *
 * \param me :AO
 * \param e :event
 * \return AO state
 */
static QState idle(Phone * const me, QEvt const * const e)
{
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG:

            status_ = Q_HANDLED();
            break;
        case Q_EXIT_SIG:{

            status_ = Q_HANDLED();
        }
        default:{
            status_ = Q_SUPER(QHsm_top);
                }

    }
}

/*! \brief phone idle
 *
 *  phone idle
 *
 * \param me :AO
 * \param e :event
 * \return AO state
 */
static QState idle(Phone * const me, QEvt const * const e)
{
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG:

            status_ = Q_HANDLED();
            break;
        case Q_EXIT_SIG:{

            status_ = Q_HANDLED();
        }
        default:{
            status_ = Q_SUPER(QHsm_top);
                }

    }
}
