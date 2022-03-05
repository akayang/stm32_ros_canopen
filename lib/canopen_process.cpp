/**
 * @file canopen_process.cpp
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

#include "canopen_process.h"

static uint8_t CanOpen_OperationBit_Set(STRUCT_OBEJECT struct_obeject){
    switch(struct_obeject.DataType) {
        case(CANOPEN_DATA_T_INTEGER32):
            return CANOPEN_WRITE_DATA_T_INTEGER32;
        case(CANOPEN_DATA_T_UNSIGNED32):
            return CANOPEN_WRITE_DATA_T_UNSIGNED32;
        case(CANOPEN_DATA_T_UNSIGNED16):
            return CANOPEN_WRITE_DATA_T_UNSIGNED16;
        case(CANOPEN_DATA_T_INTEGER8):
            return CANOPEN_WRITE_DATA_T_INTEGER8;
        default:
            return 0;
    }
}

void CanOpen_ProcotolMode_Set(struct Can_Message * can_message, uint8_t Motor_ID) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = CANOPEN_SDO_MASTER + Motor_ID;
    can_message->TxHeader.TransmitGlobalTime = DISABLE;
    can_message->TxHeader.DLC = 8;
    can_message->TxHeader.DLC = 8;
    can_message->TxData[0] = CanOpen_OperationBit_Set(RW_OBJ_MODES_OF_PROTOCOL);
    can_message->TxData[1] = (uint8_t)RW_OBJ_MODES_OF_PROTOCOL.Index;
    can_message->TxData[2] = (uint8_t)(RW_OBJ_MODES_OF_PROTOCOL.Index >> 8);
    can_message->TxData[3] = RW_OBJ_MODES_OF_PROTOCOL.SubIndex;
    can_message->TxData[4] = 0x00;
    can_message->TxData[5] = 0x00;
    can_message->TxData[6] = 0x00;
    can_message->TxData[7] = 0x00;
    while( HAL_CAN_GetTxMailboxesFreeLevel(&Motor_Port) == 0 );
    if(HAL_CAN_AddTxMessage(&Motor_Port, &can_message->TxHeader, can_message->TxData, &can_message->TxMailbox) != HAL_OK){
        Error_Handler();
    }
}

void CanOpen_OperationMode_Set(struct Can_Message * can_message, uint8_t Motor_ID) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = CANOPEN_SDO_MASTER + Motor_ID;
    can_message->TxHeader.TransmitGlobalTime = DISABLE;
    can_message->TxHeader.DLC = 8;
    can_message->TxData[0] = CanOpen_OperationBit_Set(RW_OBJ_MODES_OF_OPERATION);
    can_message->TxData[1] = (uint8_t)RW_OBJ_MODES_OF_OPERATION.Index;
    can_message->TxData[2] = (uint8_t)(RW_OBJ_MODES_OF_OPERATION.Index >> 8);
    can_message->TxData[3] = RW_OBJ_MODES_OF_OPERATION.SubIndex;
    can_message->TxData[4] = OPERATION_MODE_PROFILE_VELOCITY;
    can_message->TxData[5] = 0x00;
    can_message->TxData[6] = 0x00;
    can_message->TxData[7] = 0x00;
    while( HAL_CAN_GetTxMailboxesFreeLevel(&Motor_Port) == 0 );
    if(HAL_CAN_AddTxMessage(&Motor_Port, &can_message->TxHeader, can_message->TxData, &can_message->TxMailbox) != HAL_OK){
        Error_Handler();
    }
}

void CanOpen_AccelerationSpeed_Set(struct Can_Message * can_message, uint8_t Motor_ID, uint32_t value) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = CANOPEN_SDO_MASTER + Motor_ID;
    can_message->TxHeader.TransmitGlobalTime = DISABLE;
    can_message->TxHeader.DLC = 8;
    can_message->TxData[0] = CanOpen_OperationBit_Set(RW_OBJ_PROFILE_ACCELERATION);
    can_message->TxData[1] = (uint8_t)RW_OBJ_PROFILE_ACCELERATION.Index;
    can_message->TxData[2] = (uint8_t)(RW_OBJ_PROFILE_ACCELERATION.Index >> 8);
    can_message->TxData[3] = RW_OBJ_PROFILE_ACCELERATION.SubIndex;
    value *= Motor_SpeedRatio;
    can_message->TxData[4] = (uint8_t)value;
    can_message->TxData[5] = (uint8_t)(value >> 8);
    can_message->TxData[6] = (uint8_t)(value >> 16);
    can_message->TxData[7] = (uint8_t)(value >> 24);
    while( HAL_CAN_GetTxMailboxesFreeLevel(&Motor_Port) == 0 );
    if(HAL_CAN_AddTxMessage(&Motor_Port, &can_message->TxHeader, can_message->TxData, &can_message->TxMailbox) != HAL_OK){
        Error_Handler();
    }
}

void CanOpen_DecelerationSpeed_Set(struct Can_Message * can_message, uint8_t Motor_ID, uint32_t value) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = CANOPEN_SDO_MASTER + Motor_ID;
    can_message->TxHeader.TransmitGlobalTime = DISABLE;
    can_message->TxHeader.DLC = 8;
    can_message->TxData[0] = CanOpen_OperationBit_Set(RW_OBJ_PROFILE_DECELERATION);
    can_message->TxData[1] = (uint8_t)RW_OBJ_PROFILE_DECELERATION.Index;
    can_message->TxData[2] = (uint8_t)(RW_OBJ_PROFILE_DECELERATION.Index >> 8);
    can_message->TxData[3] = RW_OBJ_PROFILE_DECELERATION.SubIndex;
    value *= Motor_SpeedRatio;
    can_message->TxData[4] = (uint8_t)value;
    can_message->TxData[5] = (uint8_t)(value >> 8);
    can_message->TxData[6] = (uint8_t)(value >> 16);
    can_message->TxData[7] = (uint8_t)(value >> 24);
    while( HAL_CAN_GetTxMailboxesFreeLevel(&Motor_Port) == 0 );
    if(HAL_CAN_AddTxMessage(&Motor_Port, &can_message->TxHeader, can_message->TxData, &can_message->TxMailbox) != HAL_OK){
        Error_Handler();
    }
}

void CanOpen_ControlWord_Set(struct Can_Message * can_message, uint8_t Motor_ID, uint8_t controltype) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = CANOPEN_SDO_MASTER + Motor_ID;
    can_message->TxHeader.TransmitGlobalTime = DISABLE;
    can_message->TxHeader.DLC = 8;
    can_message->TxData[0] = CanOpen_OperationBit_Set(RW_OBJ_CTRL_WORD_REGISTER);
    can_message->TxData[1] = (uint8_t)RW_OBJ_CTRL_WORD_REGISTER.Index;
    can_message->TxData[2] = (uint8_t)(RW_OBJ_CTRL_WORD_REGISTER.Index >> 8);
    can_message->TxData[3] = RW_OBJ_CTRL_WORD_REGISTER.SubIndex;
    can_message->TxData[4] = controltype;
    can_message->TxData[5] = 0x00;
    can_message->TxData[6] = 0x00;
    can_message->TxData[7] = 0x00;
    while( HAL_CAN_GetTxMailboxesFreeLevel(&Motor_Port) == 0 );
    if(HAL_CAN_AddTxMessage(&Motor_Port, &can_message->TxHeader, can_message->TxData, &can_message->TxMailbox) != HAL_OK){
        Error_Handler();
    }
}

void CanOpen_ProfileVelocity_Set(struct Can_Message * can_message, uint8_t Motor_ID, int32_t value) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = CANOPEN_SDO_MASTER + Motor_ID;
    can_message->TxHeader.TransmitGlobalTime = DISABLE;
    can_message->TxHeader.DLC = 8;
    can_message->TxData[0] = CanOpen_OperationBit_Set(RO_OBJ_TARGET_VELOCITY);
    can_message->TxData[1] = (uint8_t)RO_OBJ_TARGET_VELOCITY.Index;
    can_message->TxData[2] = (uint8_t)(RO_OBJ_TARGET_VELOCITY.Index >> 8);
    can_message->TxData[3] = RO_OBJ_TARGET_VELOCITY.SubIndex;
    value *= Motor_SpeedRatio;
    can_message->TxData[4] = (int8_t)value;
    can_message->TxData[5] = (int8_t)(value >> 8);
    can_message->TxData[6] = (int8_t)(value >> 16);
    can_message->TxData[7] = (int8_t)(value >> 24);
    while( HAL_CAN_GetTxMailboxesFreeLevel(&Motor_Port) == 0 );
    if(HAL_CAN_AddTxMessage(&Motor_Port, &can_message->TxHeader, can_message->TxData, &can_message->TxMailbox) != HAL_OK){
        Error_Handler();
    }
}
