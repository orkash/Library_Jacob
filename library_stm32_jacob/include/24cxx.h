/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: 24cxx.h
* 文件标识: 见配置管理计划书
* 摘    要: stm32, 24cxx驱动, 支持AT24C01~AT24C256各种型号
* 函    数: AT24CXX_Init() - 初始化I2C接口
*           AT24CXX_ReadByte() - 在AT24CXX指定地址读出一个字节数据
*           AT24CXX_WriteByte() - 向AT24CXX指定地址写入一个字节数据
*           AT24CXX_WriteLenBytes() - 向AT24CXX里面的指定地址开始写入长度为dat_bytes的数据
*           AT24CXX_ReadLenBytes() - 从AT24CXX里面的指定地址开始读取长度为dat_bytes的数据
*           AT24CXX_CheckIsFailed() - 检查AT24CXX是否正常
*           AT24CXX_ReadBuf() - 在AT24CXX里面的指定地址开始读出指定个数的数据
*           AT24CXX_WriteBuf() - 向AT24CXX指定地址写入指定字节的数据
*           AT24CXX_WriteAllTheSameData() - 将AT24CXX所有空间写入同一个数据
*           AT24CXX_Empty() - 清空AT24CXX存储的内容
* 
* 当前版本: A0.0.2
* 作    者: Jacob Peng
* 完成日期: 2015年4月24日
* 更改内容: E0.0.1, 完成了AT24CXX数组数据读取与数组数据写入, 2015年4月23日
*           A0.0.2, 增加AT24CXX整个EEPROM清空函数, 2015年4月24日
* 
* 取代版本: E0.0.1
* 原作者  : Jacob Peng
* 完成日期: 2015年4月23日
*/
#ifndef STM32_AT24CXX_H_
#define STM32_AT24CXX_H_

#include <stm32f10x.h>

// AT24CXX系列存储空间, 单位byte
#define AT24C01   127
#define AT24C02   255
#define AT24C04   511
#define AT24C08   1023
#define AT24C16   2047
#define AT24C32   4095
#define AT24C64   8191
#define AT24C128  16383
#define AT24C256  32767

// 这里使用的是AT24C02
#define AT24CXX AT24C02

/*----- High layer function -----*/
// 初始化AT24CXX对应IO口
void AT24CXX_Init(void);
// 检查AT24CXX是否正常
u8 AT24CXX_CheckIsFailed(void);
// 向AT24CXX里面的指定地址开始写入长度为dat_bytes的数据
void AT24CXX_WriteLenBytes(u16 addr, u32 dat, u8 dat_bytes);
// 从AT24CXX里面的指定地址开始读取长度为dat_bytes的数据
u32 AT24CXX_ReadLenBytes(u16 addr, u8 len);
// 在AT24CXX里面的指定地址开始读出指定个数的数据
void AT24CXX_ReadBuf(u16 addr, u8 read_out_buf[], u16 read_bytes);
// 向AT24CXX指定地址写入指定字节的数据
void AT24CXX_WriteBuf(u16 addr, const u8 to_write_buf[], u16 write_bytes);
// 将AT24CXX所有空间写入同一个数据
void AT24CXX_WriteAllTheSameData(u8 dat);
// 清空AT24CXX存储的内容
void AT24CXX_Empty(void);

/*----- Medium layer function -----*/
// 从AT24CXX指定地址读出一个字节数据
u8 AT24CXX_ReadByte(u16 addr);
// 向AT24CXX指定地址写入一个字节数据
void AT24CXX_WriteByte(u16 addr, u8 dat);

/*----- Low layer function -----*/

#endif  // #ifndef STM32_AT24CXX_H_
