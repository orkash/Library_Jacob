/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: stm_flash.h
* 文件标识: 见配置管理计划书
* 摘    要: 掉电存储, 对STM32自带flash的相关操作, STM32内部flash只能写半字(2个字节),
*           写1个字节会导致stm32内部总线复位
* 函    数: StmFlash_ReadHalfWord() - 读半字
*           StmFlash_ReadBuffer() - 从指定地址开始, 读取指定长度的buf数据
*           StmFlash_WriteBuffer_NoCheck() - 不检查地写入buf数据
*           StmFlash_WriteBuffer() - 指定地址开始, 写入指定长度的buf数据
*           
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月15日
* 更改内容: E0.0.1, 完成了stm32 flash读取与写操作, 2015年4月15日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#ifndef STM32_FLASH_H
#define STM32_FLASH_H

#include <stm32f10x.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define STM32_FLASH_SIZE 256  // 所选stm32的flash容量大小(单位为K)
#define STM32_FLASH_WREN TRUE // flash写入使能（0, 不使能; 1, 使能） 
// stm32 flash起始地址
#define STM32_FLASH_BASE 0x08000000
// stm32 用户存储数据起始地址
#define USER_SAVE_BASE_ADDR 0x08020000 

/*----- High layer function -----*/
// 从指定地址开始，读取指定长度的buf数据, 返回是否读取成功标志
u8 StmFlash_ReadBuffer(u32 read_addr, u16 read_buf[], u16 read_len);
// 从指定地址开始，写入指定长度的buf数据，返回是否写入成功标志
u8 StmFlash_WriteBuffer(u32 write_addr, u16 write_buf[], u16 write_len);

/*----- Medium layer function -----*/
// 读半字，16 bits
static u16 StmFlash_ReadHalfWord(u32 addr);
// 不检查地写入buf数据
static void StmFlash_WriteBuffer_NoCheck(u32 write_addr, const u16 write_buf[], 
                                         u16 write_len);

/*----- Low layer function -----*/


#endif  // #ifndef STM32_FLASH_H
