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

/**
 * @brief 
 * @param  canHandle        My Param doc
 */
void CAN_Filter_Init(CAN_HandleTypeDef* canHandle) { 
    CAN_FilterTypeDef sFilterConfig;

    sFilterConfig.FilterBank = 13;//过滤器0 这里可设0-13
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;//采用掩码模式
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;//采用16位掩码模式
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;//采用FIFO0
    sFilterConfig.FilterActivation = ENABLE;//打开过滤器
#ifdef ENABLE_TXPDO1
    sFilterConfig.FilterIdHigh = ((CANOPEN_TXPDO1 + RightMotor_ID) << 5); //设置过滤器ID高16位
    sFilterConfig.FilterIdLow = ((CANOPEN_TXPDO1 + LeftMotor_ID) << 5);//设置过滤器ID低16位
    sFilterConfig.FilterMaskIdHigh = ((CANOPEN_TXPDO1 + RightMotor_ID) << 5);//设置过滤器掩码高16位
    sFilterConfig.FilterMaskIdLow = ((CANOPEN_TXPDO1 + LeftMotor_ID) << 5);//设置过滤器掩码低16位
#endif
    if(HAL_CAN_ConfigFilter(canHandle,&sFilterConfig) != HAL_OK)//初始化过滤器
    {    
        Error_Handler();
    }
    if(HAL_CAN_Start(canHandle) != HAL_OK)//打开can
    { 
        Error_Handler();
    }
    if(HAL_CAN_ActivateNotification(canHandle,CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)//开启接收中断
    { 
        Error_Handler();
    }
}

/**
 * @brief 
 * @param  hcan             My Param doc
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];
	if(hcan->Instance == CAN1)
	{
        HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&RxHeader,RxData);//获取数据
        if(RxHeader.StdId == (CANOPEN_TXPDO1 + LeftMotor_ID)) {
            memcpy(&Data_holder::get()->can_message[LeftMotor_ID-1].RxHeader, &RxHeader, sizeof(CAN_RxHeaderTypeDef));
            memcpy(Data_holder::get()->can_message[LeftMotor_ID-1].RxData, RxData, sizeof(RxData));
        }
        else if(RxHeader.StdId == (CANOPEN_TXPDO1 + RightMotor_ID)) {
            memcpy(&Data_holder::get()->can_message[RightMotor_ID-1].RxHeader, &RxHeader, sizeof(CAN_RxHeaderTypeDef));
            memcpy(Data_holder::get()->can_message[RightMotor_ID-1].RxData, RxData, sizeof(RxData));
        }
        else;
		HAL_CAN_ActivateNotification(hcan,CAN_IT_RX_FIFO0_MSG_PENDING);//再次开启接收中断 
    }	
}

/**
 * @brief 
 * @param  struct_obeject   My Param doc
 * @return uint8_t 
 */
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

/**
 * @brief 
 * @param  can_message      My Param doc
 */
void CanOpen_NMT_Set(struct Can_Message * can_message, uint8_t Motor_ID, uint8_t value) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = 0;
    can_message->TxHeader.TransmitGlobalTime = DISABLE;
    can_message->TxHeader.DLC = 2;
    can_message->TxData[0] = value;
    can_message->TxData[1] = Motor_ID;
    while( HAL_CAN_GetTxMailboxesFreeLevel(&Motor_Port) == 0 );
    if(HAL_CAN_AddTxMessage(&Motor_Port, &can_message->TxHeader, can_message->TxData, &can_message->TxMailbox) != HAL_OK){
        Error_Handler();
    }
}

/**
 * @brief 
 * @param  can_message      My Param doc
 * @param  Motor_ID         My Param doc
 */
void CanOpen_ProcotolMode_Set(struct Can_Message * can_message, uint8_t Motor_ID) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = CANOPEN_SDO_REQUEST + Motor_ID;
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

/**
 * @brief 
 * @param  can_message      My Param doc
 * @param  Motor_ID         My Param doc
 */
void CanOpen_OperationMode_Set(struct Can_Message * can_message, uint8_t Motor_ID) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = CANOPEN_SDO_REQUEST + Motor_ID;
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

/**
 * @brief 
 * @param  can_message      My Param doc
 * @param  Motor_ID         My Param doc
 * @param  value            My Param doc
 */
void CanOpen_AccelerationSpeed_Set(struct Can_Message * can_message, uint8_t Motor_ID, uint32_t value) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = CANOPEN_SDO_REQUEST + Motor_ID;
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

/**
 * @brief 
 * @param  can_message      My Param doc
 * @param  Motor_ID         My Param doc
 * @param  value            My Param doc
 */
void CanOpen_DecelerationSpeed_Set(struct Can_Message * can_message, uint8_t Motor_ID, uint32_t value) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = CANOPEN_SDO_REQUEST + Motor_ID;
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

/**
 * @brief 
 * @param  can_message      My Param doc
 * @param  Motor_ID         My Param doc
 * @param  controltype      My Param doc
 */
void CanOpen_ControlWord_Set(struct Can_Message * can_message, uint8_t Motor_ID, uint8_t controltype) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = CANOPEN_SDO_REQUEST + Motor_ID;
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

/**
 * @brief 
 * @param  can_message      My Param doc
 * @param  Motor_ID         My Param doc
 * @param  value            My Param doc
 */
void CanOpen_ProfileVelocity_Set(struct Can_Message * can_message, uint8_t Motor_ID, int32_t value) {
    can_message->TxHeader.RTR = CAN_RTR_DATA;
    can_message->TxHeader.IDE = CAN_ID_STD;
    can_message->TxHeader.StdId = CANOPEN_SDO_REQUEST + Motor_ID;
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
