#include "myuart.h"
#include "usart.h"
#include "uartidle.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

UART_DRIVER myuartDriver;

UART_DRIVER* uart_Driver_Init(void)
{
	myuartDriver.Uart_idle_init=User_uart_init;
	myuartDriver.UsartReceive_IDLE=UsartReceive_IDLE;
	myuartDriver.user_send_nbyte=user_send_nbyte;
	return &myuartDriver;
}
