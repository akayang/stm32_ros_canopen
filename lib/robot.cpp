/**
 * @file robot.cpp
 * @brief 
 * @author YangDing(502552742@qq.com)
 * @version 1.0
 * @date 2022-03-03
 * 
 * @copyright Copyright (c) 2022  Panda
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author      <th>Description
 * <tr><td>2022-03-03 <td>1.0     <td>YangDing     <td>内容
 * </table>
 */

#include "robot.h"

#define FIX_HEAD 0x5A

Robot::Robot(){

}

/**
 * @brief 
 */
void Robot::init() {
    /* enable serial port idle interrupt */
    __HAL_UART_ENABLE_IT(&Debug_Port, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&Debug_Port, (uint8_t *)&Data_holder::get()->message_rec.data, sizeof(Data_holder::get()->message_rec.data));
	__HAL_UART_ENABLE_IT(&Transfer_Port, UART_IT_IDLE);
    //HAL_UART_Receive_DMA(&Transfer_Port, (uint8_t *)&Data_holder->get().message_rec.data, sizeof(Data_holder::get()->message_rec.data));
    
    /* load parameter saved in flash memory */
    //Data_holder::get()->load_parameter();

    /* canopen motor enable operation */
}

/**
 * @brief 
 */
void Robot::clear_odom() {

}

/**
 * @brief 
 */
void Robot::calc_odom() {

}

void Robot::velocity_control() {

}

/**
 * @brief 
 */
void Robot::protocol_process() {
    /* justify if data_holder get correct package */
    if(Data_holder::get()->message_flag != 1) return;
    if(Data_holder::get()->message_rec.head.flag != FIX_HEAD) return;
    /* justify command number */
    switch(Data_holder::get()->message_rec.head.msg_id) {
        case ID_GET_VERSION:
            Response_Messgae_process(ID_GET_VERSION);
			break;
		case ID_SET_ROBOT_PARAMTER:
            Data_holder::get()->save_parameter();
			break;
		case ID_GET_ROBOT_PARAMTER:
            Data_holder::get()->load_parameter();
            Response_Messgae_process(ID_GET_ROBOT_PARAMTER);
			break;
		case ID_CLEAR_ODOM:
            clear_odom();
			break;
		case ID_SET_VELOCITY:
            velocity_control();
			break;
		case ID_GET_ODOM:
            calc_odom();
            Response_Messgae_process(ID_GET_ODOM);
			break;
		case ID_GET_PID_DATA: 	//WARN:This is not used in canopen motor version
			break;
		case ID_GET_IMU_DATA:	//TODO:merge imu data process function
			break;
		case ID_MESSGAE_MAX:	//process as default
		default:
			break;
    }
    /* clear serial port received message */
    memset(Data_holder::get()->message_rec.data, 0, Data_holder::get()->data_length);
    Data_holder::get()->message_flag = 0;
    Data_holder::get()->data_length = 0;
}

/**
 * @brief 
 */
void Robot::roserial_loop(){
    protocol_process();
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}
