#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "stm32f4xx_hal.h"

#define DEBUG 1

//存放电池和太阳能板 电压值值 结构体
typedef struct Volage_st
{
    float Vol_Bat;     //电池电压值
    float Vol_Pv;      //太阳能板电压值
    float Vol_Correct; //电压电池电压校正值
    uint32_t Correct_key;
} VOLTAGE_ST;
extern VOLTAGE_ST Vol_st;

#endif
