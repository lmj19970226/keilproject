#include "AO_user.h"

typedef struct
{
  QActive super;
  QTimeEvt timeEvt;
} User;

static QState User_initial(User *const me, QEvt const *const e);

static QState User_TimeEvt(User *const me, QEvt const *const e);

static User l_User;
QActive *const AO_User = &l_User.super;

//构造函数
void User_ctor(void)
{
  User *me = (User *)AO_User;

  QActive_ctor(&me->super, Q_STATE_CAST(&User_initial));

  QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U); //定时器
}

//时间事件初始化
static QState User_initial(User *const me, QEvt const *const e)
{
  QTimeEvt_armX(&me->timeEvt, BSP_TICKS_PER_SEC * 10, BSP_TICKS_PER_SEC * 10); //开启定时器
  return Q_TRAN(&User_TimeEvt);
}

static QState User_TimeEvt(User *const me, QEvt const *const e)
{
  QState status_;
  char str[14];
  switch (e->sig)
  {
  case Q_ENTRY_SIG:
    status_ = Q_HANDLED();
    break;
  case TIMEOUT_SIG:
  {
    Vol_st.Vol_Bat = get_rank_adc(2, 5, 9.8);
    sprintf(str, "BAT_Vol:%0.2f\r\n", Vol_st.Vol_Bat);
    user_send_nbyte(&huart6, (uint8_t *)str, sizeof(str));
    //			#if DEBUG
    //			printf("BAT_Vol:%0.2f\r\n", Vol_st.Vol_Bat);
    //			#endif
    status_ = Q_HANDLED();
  }
  break;
  case Q_EXIT_SIG:
    status_ = Q_HANDLED();
    break;
  default:
  {
    status_ = Q_SUPER(&QHsm_top);
    break;
  }
  }
  return status_;
}
