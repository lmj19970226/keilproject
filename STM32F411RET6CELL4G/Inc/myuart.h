#ifndef __myuart_H
#define __myuart_H

#include "stm32f4xx_hal.h"
#include "usart.h"
#include "uartidle.h"

typedef struct 
{ 
	void (*Uart_idle_init)(void);
	void (*UsartReceive_IDLE)(UART_HandleTypeDef *huart, rb_t *pRbx, uint8_t x);
	void (*user_send_nbyte)(UART_HandleTypeDef *huart,uint8_t* pData,uint16_t size);
}UART_DRIVER;

UART_DRIVER* uart_Driver_Init(void);

#endif
