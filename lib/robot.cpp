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
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

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

    HAL_Delay(5000);    //wait motor system upload

    /* canopen motor enable operation */
    HAL_CAN_Start(&Motor_Port);
    /* protocol mode setting */
    CanOpen_ProcotolMode_Set(&Data_holder::get()->can_message[LeftMotor_ID -1], LeftMotor_ID);
    CanOpen_ProcotolMode_Set(&Data_holder::get()->can_message[RightMotor_ID -1], RightMotor_ID);
    HAL_Delay(1);
    /* operation mode setting */
    CanOpen_OperationMode_Set(&Data_holder::get()->can_message[LeftMotor_ID -1], LeftMotor_ID);
    CanOpen_OperationMode_Set(&Data_holder::get()->can_message[RightMotor_ID -1], RightMotor_ID);
    HAL_Delay(1);    
    /* acceleration speed setting */
    CanOpen_AccelerationSpeed_Set(&Data_holder::get()->can_message[LeftMotor_ID -1], LeftMotor_ID, Motor_Acceleration_Speed);
    CanOpen_AccelerationSpeed_Set(&Data_holder::get()->can_message[RightMotor_ID -1], RightMotor_ID, Motor_Deceleration_Speed);
    HAL_Delay(1);    
    /* deceleration speed setting */
    CanOpen_DecelerationSpeed_Set(&Data_holder::get()->can_message[LeftMotor_ID -1], LeftMotor_ID, Motor_Deceleration_Speed);
    CanOpen_DecelerationSpeed_Set(&Data_holder::get()->can_message[RightMotor_ID -1], RightMotor_ID, Motor_Deceleration_Speed);
    HAL_Delay(1);
    /* control word setting */
    CanOpen_ControlWord_Set(&Data_holder::get()->can_message[LeftMotor_ID -1], LeftMotor_ID, CTRL_WORD_READY);
    CanOpen_ControlWord_Set(&Data_holder::get()->can_message[RightMotor_ID -1], RightMotor_ID, CTRL_WORD_READY);
    HAL_Delay(1);
    CanOpen_ControlWord_Set(&Data_holder::get()->can_message[LeftMotor_ID -1], LeftMotor_ID, CTRL_WORD_SWITCH_ON);
    CanOpen_ControlWord_Set(&Data_holder::get()->can_message[RightMotor_ID -1], RightMotor_ID, CTRL_WORD_SWITCH_ON);
    HAL_Delay(1);
    CanOpen_ControlWord_Set(&Data_holder::get()->can_message[LeftMotor_ID -1], LeftMotor_ID, CTRL_WORD_ENABLE_OPERATION);
    CanOpen_ControlWord_Set(&Data_holder::get()->can_message[RightMotor_ID -1], RightMotor_ID, CTRL_WORD_ENABLE_OPERATION);        
    HAL_Delay(1);
}

/**
 * @brief 
 */
void Robot::clear_odom() {
    memset(&Data_holder::get()->odom, 0, sizeof(Data_holder::get()->odom));
    memset(&Data_holder::get()->encoder, 0, sizeof(Data_holder::get()->encoder));
}

/**
 * @brief 
 */
void Robot::calc_odom() {
    int32_t distance[WHEEL_NUMBER];
    static unsigned long last_millis = 0;

    if(getCurrentMicros()-last_millis > Calculate_Odom_Interval)
        last_millis = getCurrentMicros(); 
    /* pdo获取速度值 */
    /* 计算线速度角速度 */
    /* 获取编码器数据 */
    /* 计算里程计数据 */
    //Data_holder::get()->encoder.last_value[0] = Data_holder::get()->encoder.now_value[0];
    //Data_holder::get()->encoder.last_value[1] = Data_holder::get()->encoder.now_value[1];
    distance[0] = Data_holder::get()->encoder.now_value[0] - Data_holder::get()->encoder.last_value[0];
    distance[1] = Data_holder::get()->encoder.now_value[1] - Data_holder::get()->encoder.last_value[1];
    /* 计算里程计xy及yaw角 */
    
}

void Robot::velocity_control() {
    int16_t vx = min(max(Data_holder::get()->velocity.v_liner_x, -(int16_t(Data_holder::get()->parameter.max_v_liner_x))), int16_t(Data_holder::get()->parameter.max_v_liner_x));
    int16_t vy = min(max(Data_holder::get()->velocity.v_liner_y, -(int16_t(Data_holder::get()->parameter.max_v_liner_y))), int16_t(Data_holder::get()->parameter.max_v_liner_y));
    int16_t vz = min(max(Data_holder::get()->velocity.v_angular_z, -(int16_t(Data_holder::get()->parameter.max_v_angular_z))), int16_t(Data_holder::get()->parameter.max_v_angular_z));
    double robot_speed[3]={vx/100.0, vy/100.0, vz/100.0};	
    int32_t motor_speed[2];
    motor_speed[0] = (robot_speed[0] + robot_speed[2] * Data_holder::get()->parameter.wheel_track * 0.0005)  * 60 * Data_holder::get()->parameter.motor_ratio / PAI \
                    / (Data_holder::get()->parameter.wheel_diameter * 0.001);
    motor_speed[1] = (-robot_speed[0] + robot_speed[2] * Data_holder::get()->parameter.wheel_track * 0.0005)  * 60 * Data_holder::get()->parameter.motor_ratio / PAI \
                    / (Data_holder::get()->parameter.wheel_diameter * 0.001);
    CanOpen_ProfileVelocity_Set(&Data_holder::get()->can_message[LeftMotor_ID -1], LeftMotor_ID, motor_speed[0]);
    CanOpen_ProfileVelocity_Set(&Data_holder::get()->can_message[RightMotor_ID -1], RightMotor_ID, motor_speed[1]);

    last_control_time = getCurrentMicros() / 1000;
}   

void Robot::control_timeout_check() {
    if((getCurrentMicros() / 1000 - last_control_time) > Control_Velocity_Interval) {
        CanOpen_ProfileVelocity_Set(&Data_holder::get()->can_message[LeftMotor_ID -1], LeftMotor_ID, 0);
        CanOpen_ProfileVelocity_Set(&Data_holder::get()->can_message[RightMotor_ID -1], RightMotor_ID, 0);   
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    }
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
            //Data_holder::get()->save_parameter();
			break;
		case ID_GET_ROBOT_PARAMTER:
            Data_holder::get()->load_parameter();
            Response_Messgae_process(ID_GET_ROBOT_PARAMTER);
			break;
		case ID_CLEAR_ODOM:
            clear_odom();
			break;
		case ID_SET_VELOCITY:
			Response_Messgae_process(ID_SET_VELOCITY);
            velocity_control();
			break;
		case ID_GET_ODOM:
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
    calc_odom();
    /* clear serial port received message */
    memset(&Data_holder::get()->message_rec, 0, sizeof(Robot_Message));
    Data_holder::get()->message_flag = 0;
    Data_holder::get()->data_length = 0;
}

/**
 * @brief 
 */
void Robot::roserial_loop(){
    protocol_process();
    control_timeout_check();
}
