#ifndef DPP_H
#define DPP_H
#include "qpc.h"
enum DPPSignals
{
  GET_NTP_SIG = Q_USER_SIG,
  TIMEOUT_SIG,
  TIMEOUT_1MS_SIG,
  TIMEOUT_60S_SIG,
  MAX_SIG
};

typedef struct
{
  QEvt super;
} MyEvt;

void User_ctor(void);
extern QActive *const AO_User;

#endif
