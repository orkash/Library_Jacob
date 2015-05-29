/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: stm_flash.c
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
#include "../include/stm32_flash.h"

#include <stm32f10x.h>  // Declare vu16.
#include <stm32f10x_flash.h>  // Declare FLASH_ProgramHalfWord().

/************************************************ 
* 函  数: StmFlash_ReadHalfWord(u32)
* 功  能: 读半字, 16 bits; addr必须为2的倍数
*
* 参  数: addr - 待读取的地址
*
* 返回值: 读取的半字数据
*************************************************/
u16 StmFlash_ReadHalfWord(u32 addr)
{
  return *(vu16*)addr;
} // StmFlash_ReadHalfWord().

/************************************************ 
* 函  数: StmFlash_ReadBuffer(u32, u16 [], u16)
* 功  能: 从指定地址开始, 读取指定长度的buf数据
*
* 参  数: read_addr - 读取的地址
*         read_buf - 数据缓冲区
*         read_len - 读取的数据长度
*
* 返回值: 读取成功标志: 1, 成功; 0, 失败
*************************************************/
u8 StmFlash_ReadBuffer(u32 read_addr, u16 read_buf[], u16 read_len)
{
  u16 i = 0;
  
  // 传入地址非法
  if (read_addr < STM32_FLASH_BASE || 
      read_addr >= (STM32_FLASH_BASE + 1024*STM32_FLASH_SIZE)) return FALSE;  
  for (i = 0; i < read_len; ++i)
  {
    read_buf[i] = StmFlash_ReadHalfWord(read_addr);
    read_addr += 2; // 偏移2个字节
  } // for
  
  return TRUE;
} // end of StmFlash_ReadBuffer().

/************************************************ 
* 函  数: StmFlash_WriteBuffer_NoCheck(u32, const u16 [], u16)
* 功  能: 在指定地址, 不检查地写入buf数据
*
* 参  数: write_addr - 写入的地址
*         write_buf - 数据缓冲区
*         write_len - 写入的数据长度
*
* 返回值: 无
*************************************************/
#if STM32_FLASH_WREN  // 如果已经使能flash写操作

// 不检查地写入buf数据
void StmFlash_WriteBuffer_NoCheck(u32 write_addr, const u16 write_buf[], u16 write_len)
{
  u16 i = 0;

  for (i = 0; i < write_len; ++i)
  {
    FLASH_ProgramHalfWord(write_addr, write_buf[i]);
    write_addr += 2;  // 地址增加2
  }
} // StmFlash_WriteBuffer_NoCheck().

#if STM32_FLASH_SIZE < 256
#define STM_SECTOR_SIZE 1024  // 单位字节
#else
#define STM_SECTOR_SIZE 2048  // 单位字节
#endif  // #if STM32_FLASH_SIZE

/************************************************ 
* 函  数: StmFlash_WriteBuffer(u32, u16 [], u16)
* 功  能: 在指定地址, 写入指定长度的buf数据,
*         STM32内部flash只能写半字(2个字节),
*         写1个字节会导致stm32内部总线复位
*
* 参  数: write_addr - 写入的地址, 必须为2的倍数
*         write_buf - 数据缓冲区
*         write_len - 写入的数据长度
*
* 返回值: 写入成功标志: 1, 成功; 0, 失败
*************************************************/
u16 g_stmflash_buf[STM_SECTOR_SIZE/2]; // 最大为1024字节

unsigned char StmFlash_WriteBuffer(u32 write_addr, u16 write_buf[], u16 write_len)
{
  u16 *p_write_buf = (u16 *)write_buf;
  u16 write_data_size = write_len;
  u16 i = 0;
  u16 sector_num = 0;  // 扇区编号
  u16 sector_offset_size = 0;  // 扇区偏移地址大小（16位半字计算）
  u16 sector_remain_size = 0;  // 扇区剩余地址大小（16位半字计算）
  u32 offset_addr = 0; // flash去掉0x08000000以后的地址
  
  // 传入地址非法
  if (write_addr < STM32_FLASH_BASE || write_addr >= (STM32_FLASH_BASE + 1024*STM32_FLASH_SIZE)) return FALSE;
  FLASH_Unlock(); // 解锁
  offset_addr = write_addr - STM32_FLASH_BASE; // 实际偏移地址
  sector_num = offset_addr/STM_SECTOR_SIZE; // 扇区编号, 0~127 for STM32F103CBT6
  sector_offset_size = (offset_addr%STM_SECTOR_SIZE)/2;  //  扇区内的偏移量（单位：2个字节）
  sector_remain_size = STM_SECTOR_SIZE/2 - sector_offset_size;  // 扇区剩余空间大小
  // 传入数据长度不超过该扇区范围
  if (write_data_size <= sector_remain_size)
  {
    sector_remain_size = write_data_size;
  }
  while (TRUE)
  {
    // 读取整个扇区的内容
    StmFlash_ReadBuffer(sector_num*STM_SECTOR_SIZE + STM32_FLASH_BASE, 
                        g_stmflash_buf, STM_SECTOR_SIZE/2);
    for (i = 0; i < sector_remain_size; ++i)  // 校验数据是否为空
    {
      if (g_stmflash_buf[sector_offset_size + i] != 0xFFFF)
      {
        break;  // 需要擦除扇区
      }
    }
    if (i >= sector_remain_size)
    { // 已经擦除，直接写入数据
      StmFlash_WriteBuffer_NoCheck(write_addr, p_write_buf, sector_remain_size);
    } else {  // 未擦除
      // 擦除整个扇区
      FLASH_ErasePage(sector_num*STM_SECTOR_SIZE + STM32_FLASH_BASE);
      // 复制待写入数据到缓冲区
      for (i = 0; i < sector_remain_size; ++i)
      {
        g_stmflash_buf[sector_offset_size + i] = p_write_buf[i];
      }
      // 写入整个扇区
      StmFlash_WriteBuffer_NoCheck(sector_num*STM_SECTOR_SIZE + STM32_FLASH_BASE, 
                                   g_stmflash_buf, STM_SECTOR_SIZE/2);
      if (write_data_size == sector_remain_size)
      {
        // 写入结束，传入数据长度不超过该扇区范围
        break;
      } else {  // 写入未结束，传入数据长度超过该扇区范围
        ++sector_num; // 扇区编号地址增加1
        sector_offset_size = 0; // 不偏移
        p_write_buf += sector_remain_size;  // 数据指针偏移
        write_addr += sector_remain_size; // 写地址偏移
        write_data_size -= sector_remain_size;  // 待写入数据按（16位半字）长度减少
        // 如果剩余待写入数据长度大于一个扇区空间大小
        if (write_data_size > STM_SECTOR_SIZE/2)
        {
          // 下一个扇区仍然无法写完
          sector_remain_size = STM_SECTOR_SIZE/2; 
        } else {  // 下一个扇区可以写完
          sector_remain_size = write_data_size;
        } // end of else
      } // end of else
    } // end of else
  } // end of while().
  FLASH_Lock(); // 将flash上锁

  return TRUE;
} // StmFlash_WriteBuffer().

#endif  // #if STM32_FLASH_WREN, 如果已经使能flash写操作
