/**
 * @file flash.c
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
#include"flash.h"

#define FLASH_SAVE_ADDR ((uint32_t)0x800FC00)

/**
 * @brief 
 * @param  Flash_Add        My Param doc
 * @param  temp_data        My Param doc
 */
void Flash_Write_data(uint32_t Flash_Add, uint16_t temp_data) {
    uint32_t PageError = 0;                         //设置PageError,如果出现错误这个变量会被设置为出错的FLASH地址	
    FLASH_EraseInitTypeDef My_Flash;                //声明 FLASH_EraseInitTypeDef 结构体为 My_Flash
//    HAL_FLASH_Unlock();                             //解锁Flash
        
    My_Flash.TypeErase = FLASH_TYPEERASE_PAGES;     //标明Flash执行页面只做擦除操作
    My_Flash.PageAddress = FLASH_SAVE_ADDR + Flash_Add;               //声明要擦除的地址
    My_Flash.NbPages = 1;                           //说明要擦除的页数，此参数必须是Min_Data = 1和Max_Data =(最大页数-初始页的值)之间的值
    
//    HAL_FLASHEx_Erase(&My_Flash, &PageError);       //调用擦除函数擦除
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Flash_Add, temp_data);   //对Flash进行烧写，FLASH_TYPEPROGRAM_HALFWORD 声明操作的Flash地址的16位的，此外还有32位跟64位的操作，自行翻查HAL库的定义即可         
//    HAL_FLASH_Lock();                               //锁住Flash    
}

/**
 * @brief 
 * @param  Flash_Add        My Param doc
 * @return uint16_t 
 */
uint16_t Flash_read_data(uint32_t Flash_Add) {
    return *(__IO uint16_t*)( FLASH_SAVE_ADDR + Flash_Add ); //*(__IO uint16_t *)是读取该地址的参数值,其值为16位数据,一次读取两个字节，*(__IO uint32_t *)就一次读4个字节
}
