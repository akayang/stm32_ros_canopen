#ifndef ROBOT_H_
#define ROBOT_H_
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
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

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
};

#ifdef __cplusplus
}
#endif	
#endif