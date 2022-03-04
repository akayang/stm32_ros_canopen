/**
 * @file data_process.cpp
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
#include "data_process.h"

/**
 * @brief 
 * @param  huart            My Param doc
 */
void USER_UART_IDLECallback(UART_HandleTypeDef *huart) {
	if(Debug_port_Instance == huart->Instance) {
        HAL_UART_DMAStop(&Debug_Port);
        Data_holder::get()->data_length = UserDefine_Rx_Buffer_Size - __HAL_DMA_GET_COUNTER(&Debug_Hdma);
        //HAL_UART_Transmit(&Debug_Port, (uint8_t *)Data_holder::get()->message_rec.data, sizeof(Data_holder::get()->message_rec.data), 10); //TODO:tobe placed by setting a flag
		Data_holder::get()->message_flag = 1;
		HAL_UART_Receive_DMA(&Debug_Port, (uint8_t *)&Data_holder::get()->message_rec.data, sizeof(Data_holder::get()->message_rec.data));;
    }
}

uint8_t CRC_Calculate(uint8_t *data, uint8_t data_length) {
	uint32_t sum_value = 0;
	for(int i = 0; i < data_length; i++) {
		sum_value += (uint8_t)*(data+i);
	}
	return (uint8_t)sum_value;
}

/**
 * @brief 
 * @param  id_num           My Param doc
 */
void Response_Messgae_process(enum MESSAGE_ID id_num) {
	uint8_t *data = NULL;
	uint8_t data_length;
	switch(id_num) {
		case ID_GET_VERSION:
			data_length = sizeof(struct Robot_head) + sizeof(struct Robot_firmware) + 1;  //get message length, 1 stand of CRC bit
			data = (uint8_t *)malloc(data_length);	//malloc space
			memcpy(data, &Data_holder::get()->message_rec.head, sizeof(struct Robot_head));	//set message head
			*(data + sizeof(struct Robot_head) - 1) = sizeof(struct Robot_firmware);	//set message length
			memcpy(data + sizeof(struct Robot_head), &Data_holder::get()->firmware_info, sizeof(struct Robot_firmware));  //set message body
			*(data + data_length - 1) = CRC_Calculate(data, data_length - 1);	//set message CRC bit
			break;
		case ID_GET_ROBOT_PARAMTER:
			data_length = sizeof(struct Robot_head) + sizeof(struct Robot_parameter) + 1;
			data = (uint8_t *)malloc(data_length);
			memcpy(data, &Data_holder::get()->message_rec.head, sizeof(struct Robot_head));
			*(data + sizeof(struct Robot_head) - 1) = sizeof(struct Robot_parameter);
			memcpy(data + sizeof(struct Robot_head), &Data_holder::get()->parameter, sizeof(struct Robot_parameter));
			*(data + data_length - 1) = CRC_Calculate(data, data_length - 1);
			break;
		case ID_GET_ODOM:
			data_length = sizeof(struct Robot_head) + sizeof(struct Robot_odom) + 1;
			data = (uint8_t *)malloc(data_length);
			memcpy(data, &Data_holder::get()->message_rec.head, sizeof(struct Robot_head));
			*(data + sizeof(struct Robot_head) - 1) = sizeof(struct Robot_odom);
			memcpy(data + sizeof(struct Robot_head), &Data_holder::get()->odom, sizeof(struct Robot_odom));
			*(data + data_length - 1) = CRC_Calculate(data, data_length - 1);
			break;
		default:break;
	}
	HAL_UART_Transmit(&Debug_Port, (uint8_t *)data, data_length,10);
	free(data);
	data = NULL;
}



