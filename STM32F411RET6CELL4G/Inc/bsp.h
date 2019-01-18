#ifndef BSP_H
#define BSP_H

#include "qpc.h"
#include "dpp.h"
#include "stm32f4xx.h"

#define BSP_TICKS_PER_SEC 1000U

void BSP_init(void);

extern QActive *the_Ticker0;

#endif /* BSP_H */
