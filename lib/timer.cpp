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
