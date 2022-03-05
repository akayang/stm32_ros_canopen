/**
 * @file timer.c
 * @brief 
 * @author YangDing(502552742@qq.com)
 * @version 1.0
 * @date 2022-03-01
 * 
 * @copyright Copyright (c) 2022  Panda
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author      <th>Description
 * <tr><td>2022-03-01 <td>1.0     <td>YangDing     <td>内容
 * </table>
 */
#include "timer.h"

/**
 * @brief 
 * @param  udelay           My Param doc
 */
void delay_us(uint32_t udelay) {
    uint32_t startval,tickn,delays,wait;
 
    startval = SysTick->VAL;
    tickn = HAL_GetTick();
    delays =udelay * SYS_MHZ; //sysc / 1000 * udelay;
    if(delays > startval)
    {
        while(HAL_GetTick() == tickn)
        {
 
        }
        wait = SYS_MHZ * 1000 + startval - delays;
        while(wait < SysTick->VAL)
        {
 
        }
    }
    else
    {
        wait = startval - delays;
        while(wait < SysTick->VAL && HAL_GetTick() == tickn)
        {
 
        }
    }
}

/**
 * @brief 
 * @return __STATIC_INLINE 
 */
__STATIC_INLINE uint32_t LL_SYSTICK_IsActiveCounterFlag(void)
{
  //判断COUNTFLAG位是否为1，1则计数器已经递减到0了至少一次。读取该位后该位自动清零。
  return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}

/**
 * @brief Get the Current Micros object
 * @return uint32_t 
 */
uint32_t getCurrentMicros()
{
  /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
  LL_SYSTICK_IsActiveCounterFlag();           //清除计数器"溢出"标志位
  uint32_t m = HAL_GetTick();
  const uint32_t tms = SysTick->LOAD + 1;
  __IO uint32_t u = tms - SysTick->VAL;
  if (LL_SYSTICK_IsActiveCounterFlag()) {
  m = HAL_GetTick();
  u = tms - SysTick->VAL;
  }
  return (m * 1000 + (u * 1000) / tms);
}

/**
 * @brief 
 * @param  us               My Param doc
 */
void delayMicroseconds(uint32_t us) {
  //方法一
  __IO uint32_t currentTicks = SysTick->VAL;
  /* Number of ticks per millisecond */
  const uint32_t tickPerMs = SysTick->LOAD + 1;
  /* Number of ticks to count */
  const uint32_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint32_t elapsedTicks = 0;
  __IO uint32_t oldTicks = currentTicks;
  do {
    currentTicks = SysTick->VAL;
    elapsedTicks += (oldTicks < currentTicks) ? tickPerMs + oldTicks - currentTicks :
                    oldTicks - currentTicks;
    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);
}
