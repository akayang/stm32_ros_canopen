#ifndef DATA_PROCESS_H_
#define DATA_PROCESS_H_
/**
 * @file data_process.h
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
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void USER_UART_IDLECallback(UART_HandleTypeDef *huart);
uint8_t CRC_Calculate(uint8_t *data, uint8_t data_length);
void Response_Messgae_process(enum MESSAGE_ID id_num);

#ifdef __cplusplus
}
#endif	
#endif
