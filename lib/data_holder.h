#ifndef DATA_HOLDER_H_
#define DATA_HOLDER_H_
/**
 * @file data_holder.h
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
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#pragma anon_unions
#pragma pack(1)

#define WHEEL_NUMBER 2
#define UserDefine_Rx_Buffer_Size 255

struct Robot_head{
    uint8_t flag;// 头部标记,固定帧头:0X5A
    uint8_t msg_id;// 消息ID,表示消息具体作用,决定消息体具体格式
    uint8_t length;// 消息体长度
};

struct Robot_firmware{
    char version[16]; //固件版本
    char time[16];  //构建时间
};

struct Robot_parameter{
    union{ 
        char buff[64];
        struct
        {   
            uint16_t wheel_diameter;      //轮子直径  mm
            uint16_t wheel_track;         //差分：轮距， 三全向轮：直径，四全向：前后轮距+左右轮距 mm
            uint16_t encoder_resolution;  //编码器分辨率
            uint8_t do_pid_interval;      //pid间隔 (ms)
            uint16_t kp;
            uint16_t ki;
            uint16_t kd;
            uint16_t ko;                  //pid参数比例
            uint16_t cmd_last_time;       //命令持久时间ms 超过该时间会自动停止运动
            uint16_t max_v_liner_x;       // 最大x线速度
            uint16_t max_v_liner_y;       // 最大y线速度
            uint16_t max_v_angular_z;     // 最大角速度
            uint8_t imu_type;             // imu类型 gy85: 69, gy87:71,gy65:49
            uint16_t motor_ratio;         // 电机减速比
            uint8_t model_type;           // 运动模型类型 2wd-diff:1, 4wd-diff:2, 3wd-omni:101, 4wd-omni:102,4wd-mecanum:201
            uint8_t motor_nonexchange_flag;      // 电机标志参数        1 正接      0 反接(相当于电机线交换)
            uint8_t encoder_nonexchange_flag;    // 编码器标志参数      1 正接      0 反接(相当于编码器ab相交换)
        };
    };
};

struct Robot_velocity{
    int16_t v_liner_x; //线速度 前>0 cm/s
    int16_t v_liner_y; //差分轮 为0  cm/s
    int16_t v_angular_z; //角速度 左>0 0.01rad/s  100 means 1 rad/s
};

struct Robot_encoder{
    int32_t last_value[WHEEL_NUMBER];
    int32_t now_value[WHEEL_NUMBER];
};

struct Robot_odom{
    int16_t v_liner_x;          //线速度 前>0 后<0  cm/s
    int16_t v_liner_y;          //差分轮 为0        cm/s
    int16_t v_angular_z;        //角速度 左>0 右<0  0.01rad/s   100 means 1 rad/s
    long x;                     //里程计坐标x       cm (这里long为4字节，下同)
    long y;                     //里程计坐标y       cm
    int16_t yaw;                //里程计航角        0.01rad     100 means 1 rad
};

enum RECEIVE_STATE{
    STATE_RECV_FIX=0,
    STATE_RECV_ID,
    STATE_RECV_LEN,
    STATE_RECV_DATA,
    STATE_RECV_CHECK
};

enum MESSAGE_ID{
    ID_GET_VERSION = 0,
    ID_SET_ROBOT_PARAMTER = 1,
    ID_GET_ROBOT_PARAMTER = 2,
    ID_CLEAR_ODOM = 3,
    ID_SET_VELOCITY = 4,
    ID_GET_ODOM = 5,
    ID_GET_PID_DATA = 6,
    ID_GET_IMU_DATA = 7,
    ID_MESSGAE_MAX
};

struct Robot_Message{
    union{
        uint8_t data[UserDefine_Rx_Buffer_Size];
        struct Robot_head head;
    };
};

struct Can_Message{
    CAN_TxHeaderTypeDef TxHeader;
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t TxData[8];
    uint8_t RxData[8];
    uint32_t TxMailbox;
};

class Data_holder{
    public:
        static Data_holder* get(){
            static Data_holder dh;
            return &dh;
        }

        void load_parameter();
        void save_parameter();
    
    private:
        Data_holder();
    public:
        struct Robot_Message message_rec;
        struct Robot_head      head;
        struct Robot_firmware  firmware_info;
        struct  Robot_parameter parameter;
        struct Robot_velocity  velocity;
        struct Robot_odom      odom;
        struct Robot_encoder   encoder;
        struct Can_Message  can_message[2];
        float imu_data[9];
    public:
        uint32_t data_length;   //the valid data bytes of uart package
        uint32_t message_flag;    //
};

#ifdef __cplusplus
}
#endif	
#endif
