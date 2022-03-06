/**
 * @file robot.h
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
#ifndef ROBOT_H_
#define ROBOT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

struct Odom{
    float x;
    float y;
    float z;
    float vel_x;
    float vel_y;
    float vel_z;
};

class Robot{
    public:
        static Robot* get(){
            static Robot dh;
            return &dh;
        }
    private:
        Robot();    
    public:
        void init();
        void roserial_loop();
    private:
        void protocol_process();
        void velocity_control();
        void clear_odom();
        void calc_odom();
        void update_velocity();
        void control_timeout_check();
        void get_odom(struct Odom* odom, float* motor_dis, unsigned long interval);
    private:
//        Odom    odom;
        uint32_t last_control_time;
        uint32_t last_odom_time;
};

#ifdef __cplusplus
}
#endif	
#endif
