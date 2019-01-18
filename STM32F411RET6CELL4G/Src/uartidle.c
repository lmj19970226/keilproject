#include "uartidle.h"
#include "usart.h"
#include "ringBuffer.h"
#include "stm32f4xx_hal.h"

UART_REV_ST Uart1_Rev_Dat_st; //串口 的接收缓冲结构体
UART_REV_ST Uart6_Rev_Dat_st;
rb_t pRb1;
rb_t pRb6;

extern uint16_t Samples_In[16];

/**@} printf 函数重定向*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/* USART init function */
void User_uart_init()
{
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);

  HAL_UART_Receive_DMA(&huart1, Uart1_Rev_Dat_st.Rev_Buf, RECIVE_BUF_SIZE);
  HAL_UART_Receive_DMA(&huart6, Uart6_Rev_Dat_st.Rev_Buf, RECIVE_BUF_SIZE);
  rbInit(&pRb1, Uart1_Rev_Dat_st.Rev_Buf, RECIVE_BUF_SIZE);
  rbInit(&pRb6, Uart6_Rev_Dat_st.Rev_Buf, RECIVE_BUF_SIZE);
}

/*串口发送数据
1：huart--串口号
2：pData--数据指针
3：Size--数据大小
*/
void user_send_nbyte(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  HAL_UART_Transmit(huart, pData, Size, 0xffff);
}
/*空闲中断接收函数
1:huart--选择串口号
2：对应串口建立的环形缓冲区
3:x选择对应串口号发送出去可选 1/6
*/
void UsartReceive_IDLE(UART_HandleTypeDef *huart, rb_t *pRbx, uint8_t x)
{
  unsigned char buf[1024] = {0};
  uint16_t len = 0;

  if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
  {
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    rbSetPtr(pRbx, RECIVE_BUF_SIZE - huart->hdmarx->Instance->NDTR);
    len = rbCanRead(pRbx);
    if (len)
    {
      rbRead(pRbx, buf, len);
      if (x == 1)
      {
        user_send_nbyte(&huart6, buf, len);
      }
      else if (x == 6)
      {
        user_send_nbyte(&huart1, buf, len);
      }
    }
  }
}
