#ifndef FLASH_H_
#define FLASH_H_
/**
 * @file flash.h
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
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief user define
 */
#define FLASH_SAVE_ADDR ((uint32_t)0x800FC00)

void Flash_Write_data(uint32_t Flash_Add, uint16_t temp_data);
uint16_t Flash_read_data(uint32_t Flash_Add);

#ifdef __cplusplus
}
#endif

#endif
