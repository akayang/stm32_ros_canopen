/**
 * @file user_define.h
 * @brief 
 * @author YangDing(502552742@qq.com)
 * @version 1.0
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022  Panda
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author      <th>Description
 * <tr><td>2022-03-04 <td>1.0     <td>YangDing     <td>内容
 * </table>
 */
#ifndef USER_DEFINE_H_
#define USER_DEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* user define area */
#define SYS_MHZ 72
/* port setting */
#define Debug_Port huart1
#define Debug_port_Instance USART1
#define Debug_Hdma hdma_usart1_rx
#define Transfer_Port huart3
#define Transfer_Port_Instance USART3
#define Transfer_Hdma hdma_usart3_rx
#define Motor_Port hcan
#define Motor_Port_Instance CAN1
/* canopen motor setting */
#define ENABLE_TXPDO1   //POSITION_ACTUAL_VALUE(0x6064) for 4 LSB, VELOCITY_ACTUAL_VALUE(0x606C) for 4 MSB, set with NiMotion APP to servo driver eeprom
#define LeftMotor_ID 1
#define RightMotor_ID 2
#define Motor_SpeedRatio    166
#define Motor_Acceleration_Speed 1000
#define Motor_Deceleration_Speed 1000

/* robot setting */
#define WHEEL_NUMBER 2
#define Calculate_Odom_Interval 50
#define Control_Velocity_Interval 150

#define PAI 3.1415926535897932384626433832795

#ifdef __cplusplus
}
#endif
#endif
