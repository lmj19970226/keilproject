#ifndef __uartidle_H
#define __uartidle_H

#include "usart.h"
#include "ringBuffer.h"

#define RECIVE_BUF_SIZE  1024

/*串口接收缓存*/
typedef struct 
{
	uint8_t Rev_Buf[RECIVE_BUF_SIZE];
	uint16_t len;
}UART_REV_ST;

extern UART_REV_ST Uart1_Rev_Dat_st;  //串口 的接收缓冲结构体
extern UART_REV_ST Uart6_Rev_Dat_st;  //串口 的接收缓冲结构体
extern rb_t pRb1; 
extern rb_t pRb6;

void UsartReceive_IDLE(UART_HandleTypeDef *huart, rb_t *pRbx, uint8_t x);
void User_uart_init(void);
void user_send_nbyte(UART_HandleTypeDef *huart,uint8_t* pData,uint16_t size);

#endif
