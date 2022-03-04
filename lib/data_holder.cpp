/**
 * @file data_holder.cpp
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

#include "data_holder.h"

#define MOTOR1_REVERSE 1
#define MOTOR2_REVERSE 1
#define MOTOR3_REVERSE 1
#define MOTOR4_REVERSE 1
#define ENCODER1_REVERSE 1
#define ENCODER2_REVERSE 1
#define ENCODER3_REVERSE 1
#define ENCODER4_REVERSE 1

/**
 * @brief Construct a new Data_holder::Data_holder object
 */
Data_holder::Data_holder() {
    memset(&message_rec, 0, sizeof(struct Robot_Message));
    memset(&parameter, 0, sizeof(struct Robot_parameter));
    memset(&velocity, 0, sizeof(struct Robot_velocity));
    memset(&odom, 0, sizeof(struct Robot_odom));
    memset(&pid_data, 0, sizeof(struct Robot_pid_data));
    memset(imu_data, 0, sizeof(imu_data));

    parameter.wheel_diameter=150;
    parameter.wheel_track=440;
    parameter.encoder_resolution=10000;
    parameter.do_pid_interval=0;
    parameter.kp=0;
    parameter.ki=0;
    parameter.kd=0;
    parameter.ko=0;
    parameter.cmd_last_time=10;
    parameter.max_v_liner_x=0;
    parameter.max_v_liner_y=0;
    parameter.max_v_angular_z=0;
    parameter.imu_type=0;
    parameter.motor_ratio=40;
    parameter.model_type=1;
    parameter.motor_nonexchange_flag= 
                MOTOR4_REVERSE<<3 | MOTOR3_REVERSE<<2 | MOTOR2_REVERSE<<1 | MOTOR1_REVERSE;
    parameter.encoder_nonexchange_flag=
                ENCODER4_REVERSE<<3 | ENCODER3_REVERSE<<2 | ENCODER2_REVERSE<<1 | ENCODER1_REVERSE;

    strncpy((char*)&firmware_info.version, "v1.1.1", strlen("v1.1.1"));
    sprintf(firmware_info.time, "%s-m%de%d", "20220301",
    parameter.motor_nonexchange_flag, parameter.encoder_nonexchange_flag);
}

/**
 * @brief TODO:to be test
 */
void Data_holder::load_parameter() {
    // if(sizeof(parameter)%2==0)
    // {
    //     for(int i=0;i<sizeof(parameter)/2;i++)
    //     {
    //         uint16_t a = Flash_read_data(i*2);
    //         memcpy((unsigned char*)&parameter+i*2,&a,2);
    //     }
    // }    
}

/**
 * @brief TODO:to be test
 */
void Data_holder::save_parameter() {
    if(sizeof(parameter)%2==0)
    {
        HAL_FLASH_Unlock();
        for(int i = 0; i < sizeof(parameter)/2; i++)
        {
            uint16_t a = 0;
            memcpy(&a,(unsigned char*)&parameter+i*2,2);
            Flash_Write_data(i*2, a);
            delay_us(10);
        }
   HAL_FLASH_Lock();
  }
}
    

