#include "bsp.h" /* Board Support Package interface */

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
* DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
*/
enum KernelUnawareISRs
{ /* see NOTE00 */
  USART2_PRIO,
  /* ... */
  MAX_KERNEL_UNAWARE_CMSIS_PRI /* keep always last */
};
/* "kernel-unaware" interrupts can't overlap "kernel-aware" interrupts */
Q_ASSERT_COMPILE(MAX_KERNEL_UNAWARE_CMSIS_PRI <= QF_AWARE_ISR_CMSIS_PRI);

enum KernelAwareISRs
{
    SYSTICK_PRIO = QF_AWARE_ISR_CMSIS_PRI, /* see NOTE00 */
    /* ... */
    MAX_KERNEL_AWARE_CMSIS_PRI /* keep always last */
};
/* "kernel-aware" interrupts should not overlap the PendSV priority */
Q_ASSERT_COMPILE(MAX_KERNEL_AWARE_CMSIS_PRI <= (0xFF >> (8 - __NVIC_PRIO_BITS)));

/* callback functions needed by the framework ------------------------------*/
void BSP_init(void)
{
}
void QF_onStartup(void)
{
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);
}

void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
    /* USER CODE END SysTick_IRQn 0 */

    /* USER CODE BEGIN SysTick_IRQn 1 */
    QF_TICK_X(0U, (void *)0);
    /* USER CODE END SysTick_IRQn 1 */
}

void QF_onCleanup(void) {}

void QF_onClockTick(void)
{
    /* QF clock tick processing for rate 0 */
    QF_TICK((void *)0);
}

void Q_onAssert(char const *module, int loc)
{
}
void QV_onIdle(void)
{

    QF_INT_ENABLE(); /* just enable interrupts */
}
