/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: 24cxx.c
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
#include "../include/24cxx.h"

#include <string.h> // Declare memset().

#include "../include/i2c.h"
#include "../include/delay.h"

/************************************************ 
* 函  数: AT24CXX_Init(void)
* 功  能: 初始化AT24CXX对应IO口
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void AT24CXX_Init(void)
{
  I2C_GPIO_Init();
} // AT24CXX_Init()

/************************************************ 
* 函  数: AT24CXX_CheckIsFailed(void)
* 功  能: 检查AT24CXX是否正常
*
* 参  数: addr - 待读取数据的目的地址
*         dat_bytes - 待读取数据的字节数, 2 or 4
*
* 返回值: 1, 检测失败; 0, 检测成功
*************************************************/
#define AT24CXX_CHECKED 0X55  // AT24CXX检查正常标志

u8 AT24CXX_CheckIsFailed(void)
{
  u8 verify_data = 0x00;

  // 先读取AT24CXX是否检查成功标志,
  // 避免每次开机都写AT24CXX.
  verify_data = AT24CXX_ReadByte(AT24CXX);         
  if (AT24CXX_CHECKED == verify_data) 
  { // 判断是否是第一次初始化
    return 0;
  } else {  // 排除第一次初始化的情况
    // 这里使用AT24CXX的最后一个地址(AT24CXX)来存储标志字
    AT24CXX_WriteByte(AT24CXX, AT24CXX_CHECKED);
    // 重新读取一次, 判断是否写入成功
    verify_data = AT24CXX_ReadByte(AT24CXX);    
    if (AT24CXX_CHECKED == verify_data) return 0;
  } // if else

  return 1;                       
} // AT24CXX_CheckIsFailed()

/************************************************ 
* 函  数: AT24CXX_ReadByte(u16)
* 功  能: 从AT24CXX指定地址读出一个字节数据
*
* 参  数: addr - 开始读取的地址
*
* 返回值: 读取到的字节数据
*************************************************/
u8 AT24CXX_ReadByte(u16 addr)
{
  u8 ret_data = 0x00; 

  I2C_Start();  
  if (AT24CXX > AT24C16)
  {
    I2C_Send_Byte(0XA0); // 发送写命令
    I2C_Wait_Ack();
    I2C_Send_Byte(addr>>8); // 发送高地址
    I2C_Wait_Ack();    
  } else {  // 发送器件地址0XA0, 写数据   
    I2C_Send_Byte(0XA0 + ((addr/256) << 1));
  } // if else
  I2C_Wait_Ack(); 
  I2C_Send_Byte(addr % 256);  // 发送低地址
  I2C_Wait_Ack();     
  I2C_Start();
  I2C_Send_Byte(0XA1);  // 进入接收模式        
  I2C_Wait_Ack();
  ret_data = I2C_Read_Byte(0);       
  I2C_Stop(); // 产生一个停止条件   

  return ret_data;
} // AT24CXX_ReadByte()

/************************************************ 
* 函  数: AT24CXX_WriteByte(u16, u8)
* 功  能: 向AT24CXX指定地址写入一个字节数据
*
* 参  数: addr - 写入数据的目的地址
*         dat - 待写入的字节数据
*
* 返回值: 读取到的字节数据
*************************************************/
void AT24CXX_WriteByte(u16 addr, u8 dat)
{
  I2C_Start();  
  if (AT24CXX > AT24C16)
  {
    I2C_Send_Byte(0XA0);  // 发送写命令
    I2C_Wait_Ack();
    I2C_Send_Byte(addr >> 8); // 发送高地址
  } else { // 发送器件地址0XA0, 写数据 
    I2C_Send_Byte(0XA0 + ((addr/256) << 1));   
  } // if else
  I2C_Wait_Ack();    
  I2C_Send_Byte(addr % 256);  // 发送低地址
  I2C_Wait_Ack();                                
  I2C_Send_Byte(dat); // 发送字节                 
  I2C_Wait_Ack();
  I2C_Stop(); // 产生一个停止条件 
  DelayUs(5);
} // AT24CXX_WriteByte()

/************************************************ 
* 函  数: AT24CXX_WriteLenByte(u16, u32, u8)
* 功  能: 向AT24CXX里面的指定地址开始写入长度为dat_bytes的数据,
*         该函数用于写入16bit或者32bit的数据, 比如: float
*
* 参  数: addr - 写入数据的目的地址
*         dat - 待写入的16bit或32bit数据
*         dat_bytes - 待写入数据的字节数, 2 or 4
*
* 返回值: 无
*************************************************/
void AT24CXX_WriteLenBytes(u16 addr, u32 dat, u8 dat_bytes)
{   
  u8 i = 0;

  for (i = 0; i < dat_bytes; ++i)
  {
    AT24CXX_WriteByte(addr + i, (dat >> (8*i)) & 0xFF);
  } // for
} // AT24CXX_WriteLenBytes()

/************************************************ 
* 函  数: AT24CXX_ReadLenBytes(u16, u8)
* 功  能: 从AT24CXX里面的指定地址开始读取长度为dat_bytes的数据,
*         该函数用于读取16bit或者32bit的数据, 比如: float
*
* 参  数: addr - 待读取数据的目的地址
*         dat_bytes - 待读取数据的字节数, 2 or 4
*
* 返回值: 读取到的数据
*************************************************/
u32 AT24CXX_ReadLenBytes(u16 addr, u8 len)
{
  u8 i = 0;
  u32 ret_data = 0;

  for (i = 0; i < len; ++i)
  {
    ret_data <<= 8;
    ret_data += AT24CXX_ReadByte(addr + len - i - 1);             
  } // for

  return ret_data;                            
} // AT24CXX_ReadLenBytes()

/************************************************ 
* 函  数: AT24CXX_ReadBuf(u16, u8, u16)
* 功  能: 从AT24CXX指定地址开始读出指定字节数据
*
* 参  数: addr - 待读取数据的目的地址
*         read_out_buf - 读出数据存储的缓冲区首地址
*         read_bytes - 待读取数据的字节数
*
* 返回值: 无
*************************************************/
void AT24CXX_ReadBuf(u16 addr, u8 read_out_buf[], u16 read_bytes)
{
  u16 i = 0;

  for (i = 0; i < read_bytes; ++i)
  {
    read_out_buf[i] = AT24CXX_ReadByte(addr++);
  } // for
} // AT24CXX_ReadBuf()

/************************************************ 
* 函  数: AT24CXX_ReadBuf(u16, u8, u16)
* 功  能: 向AT24CXX指定地址写入指定字节的数据
*
* 参  数: addr - 待写入数据的目的地址
*         read_out_buf - 待写入数据的首地址
*         write_bytes - 待写入数据的字节数
*
* 返回值: 无
*************************************************/
void AT24CXX_WriteBuf(u16 addr, const u8 to_write_buf[], u16 write_bytes)
{
  u16 i = 0;

  for (i = 0; i < write_bytes; ++i)
  {
    AT24CXX_WriteByte(addr++, to_write_buf[i]);
  } // for
} // AT24CXX_WriteBuf()

/************************************************ 
* 函  数: AT24CXX_WriteAllTheSameData(u8)
* 功  能: 将AT24CXX所有空间写入同一个数据
*
* 参  数: dat - 待填充的数据
*
* 返回值: 无
*************************************************/
void AT24CXX_WriteAllTheSameData(u8 dat)
{
  u16 i = 0;
  
  for (i = 0; i <= AT24CXX; ++i)
  {
    AT24CXX_WriteByte(i, dat);
  } // for
} // AT24CXX_WriteAllTheSameData()

/************************************************ 
* 函  数: AT24CXX_Empty(void)
* 功  能: 清空AT24CXX存储的内容
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void AT24CXX_Empty(void)
{
  // 向AT24CXX全部写入0, 即为清空
  AT24CXX_WriteAllTheSameData(0x00);
} // AT24CXX_Empty()
