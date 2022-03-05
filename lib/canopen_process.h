#ifndef CANOPEN_PROCESS_H_
#define CANOPEN_PROCESS_H_
/**
 * @file canopen_process.h
 * @brief 
 * @author YangDing(502552742@qq.com)
 * @version 1.0
 * @date 2022-03-05
 * 
 * @copyright Copyright (c) 2022  Panda
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author      <th>Description
 * <tr><td>2022-03-05 <td>1.0     <td>YangDing     <td>内容
 * </table>
 */
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "canopen_objdic.h"

static uint8_t CanOpen_OperationBit_Set(STRUCT_OBEJECT struct_obeject);
void CanOpen_ProcotolMode_Set(struct Can_Message * can_message, uint8_t Motor_ID);
void CanOpen_OperationMode_Set(struct Can_Message * can_message, uint8_t Motor_ID);
void CanOpen_AccelerationSpeed_Set(struct Can_Message * can_message, uint8_t Motor_ID, uint32_t value);
void CanOpen_DecelerationSpeed_Set(struct Can_Message * can_message, uint8_t Motor_ID, uint32_t value);
void CanOpen_ControlWord_Set(struct Can_Message * can_message, uint8_t Motor_ID, uint8_t controltype);
void CanOpen_ProfileVelocity_Set(struct Can_Message * can_message, uint8_t Motor_ID, int32_t value);

#ifdef __cplusplus
}
#endif	
#endif
