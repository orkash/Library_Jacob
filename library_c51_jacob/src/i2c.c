/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: i2c.c  
* 文件标识: 见配置管理计划书
* 摘    要: I2C相关函数
* 函    数: I2C_Start() - I2C起始信号
*           I2C_Stop() - I2C起始信号
*           I2C_SendACK() - I2C发送应答信号
*           I2C_RecvACK() - I2C接收应答信号
*           I2C_SendByte() - 向I2C总线发送一个字节数据
*           I2C_RecvByte() - 从I2C总线接收一个字节数据
*           I2C_WriteByteToSlave() - 向I2C设备写入一个字节数据
*           I2C_ReadByteFromSlave() - 从I2C设备读取一个字节数据
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年2月28日
* 更改内容: E0.0.1, 完成了I2C相关8个函数, 2015年2月28日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include "../include/i2c.h"

#include "../include/delay.h"

/************************************************ 
* 函  数: I2C_Start(void)
* 功  能: 发送I2C起始信号
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
#if ENABLE_I2C_Start
void I2C_Start(void) 
{
  I2C_SCL = 1;  // 确保I2C_SCL, I2C_SDA均为高电平
  I2C_SDA = 1;
  DelayUs();
  I2C_SDA = 0;  // 先拉低I2C_SDA
  DelayUs();
  I2C_SCL = 0;  // 再拉低I2C_SCL
} // end of I2C_Start().
#endif // #if ENABLE_I2C_Start

/************************************************ 
* 函  数: I2C_Stop(void)
* 功  能: 发送I2C停止信号
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
#if ENABLE_I2C_Stop
void I2C_Stop(void) 
{
  I2C_SCL = 0;  // 确保I2C_SCL, I2C_SDA均为低电平
  I2C_SDA = 0;
  DelayUs();
  I2C_SCL = 1;  // 先拉高I2C_SDA
  DelayUs();
  I2C_SDA = 1;  // 再拉高I2C_SCL
  DelayUs();
} // end of I2C_Stop().
#endif // #if ENABLE_I2C_Stop

/************************************************ 
* 函  数: I2C_SendACK(bit)
* 功  能: 发送I2C应答信号
*
* 参  数: ack - 应答位
*
* 返回值: 无
*************************************************/
#if ENABLE_I2C_SendACK
void I2C_SendACK(bit ack)
{
  I2C_SDA = ack;  // 写应答信号
  I2C_SCL = 1;  // 拉高时钟线
  DelayUs();  // 延时
  I2C_SCL = 0;  // 拉低时钟线
  DelayUs();  // 延时
} // end of I2C_SendACK().
#endif // #if ENABLE_I2C_SendACK

/************************************************ 
* 函  数: I2C_RecvACK(void)
* 功  能: 接收I2C应答信号
*
* 参  数: ack - 应答位
*
* 返回值: I2C应答信号
*************************************************/
#if ENABLE_I2C_RecvACK
bit I2C_RecvACK(void)
{
  I2C_SCL = 1;  // 拉高时钟线
  DelayUs();  // 延时
  CY = I2C_SDA; // 读应答信号
  I2C_SCL = 0;  // 拉低时钟线
  DelayUs();  // 延时
  return CY;
} // end of I2C_RecvACK().
#endif // #if ENABLE_I2C_RecvACK

/************************************************ 
* 函  数: I2C_SendByte(unsigned char)
* 功  能: 向I2C总线发送一个字节数据
*
* 参  数: dat - 待发送字节数据
*
* 返回值: 无
*************************************************/
#if ENABLE_I2C_SendByte
void I2C_SendByte(unsigned char dat)
{
  unsigned char i = 0;

  for (i = 0; i < 8; ++i) //8位计数器
  {
    dat <<= 1;  // 移出数据的最高位
    I2C_SDA = CY; // 送数据口
    I2C_SCL = 1;  // 拉高时钟线
    DelayUs();  // 延时
    I2C_SCL = 0;  // 拉低时钟线
    DelayUs();  // 延时
  } // end of for ()
  I2C_RecvACK();
} // end of I2C_SendByte().
#endif // #if ENABLE_I2C_SendByte

/************************************************ 
* 函  数: I2C_RecvByte(void)
* 功  能: 从I2C总线接收一个字节数据
*
* 参  数: 无
*
* 返回值: 接收到的字节数据
*************************************************/
#if ENABLE_I2C_RecvByte
unsigned char I2C_RecvByte(void)
{
  unsigned char i = 0;
  unsigned char dat = 0;

  I2C_SDA = 1;  // 使能内部上拉,准备读取数据,
  for (i = 0; i < 8; ++i) // 8位计数器
  {
    dat <<= 1;
    I2C_SCL = 1;  // 拉高时钟线
    DelayUs(); // 延时
    dat |= I2C_SDA; // 读数据               
    I2C_SCL = 0;  // 拉低时钟线
    DelayUs();  // 延时
  } // end of for ()

  return dat;
} // end of I2C_RecvByte().
#endif // #if ENABLE_I2C_RecvByte

/************************************************ 
* 函  数: I2C_WriteByteToSlave(unsigned char, unsigned char)
* 功  能: 向I2C设备写入一个字节数据
*
* 参  数: reg_address - I2C设备内部寄存器地址
*         reg_data - 待写入字节数据
*
* 返回值: 无
*************************************************/
#if ENABLE_I2C_WriteByteToSlave
void I2C_WriteByteToSlave(unsigned char reg_address, unsigned char reg_data)
{
  I2C_Start();  // 起始信号
  I2C_SendByte(SLAVE_ADDRESS);  // 发送设备地址+写信号
  I2C_SendByte(reg_address);  // 内部寄存器地址
  I2C_SendByte(reg_data); // 内部寄存器数据
  I2C_Stop(); // 发送停止信号
} // end of I2C_WriteByteToSlave().
#endif // #if ENABLE_I2C_WriteByteToSlave

/************************************************ 
* 函  数: I2C_ReadByteFromSlave(unsigned char)
* 功  能: 从I2C设备读取一个字节数据
*
* 参  数: reg_address - I2C设备内部寄存器地址
*
* 返回值: 读到的字节数据
*************************************************/
#if ENABLE_I2C_ReadByteFromSlave
unsigned char I2C_ReadByteFromSlave(unsigned char reg_address)
{
  unsigned char reg_data = 0;

  I2C_Start();  // 起始信号
  I2C_SendByte(SLAVE_ADDRESS); // 发送设备地址+写信号
  I2C_SendByte(reg_address);  // 发送存储单元地址，从0开始  
  I2C_Start();  // 起始信号
  I2C_SendByte(SLAVE_ADDRESS + 1);  // 发送设备地址+读信号
  reg_data = I2C_RecvByte();  // 读出寄存器数据
  I2C_SendACK(1); // 接收应答信号
  I2C_Stop(); // 停止信号

  return reg_data;
} // end of I2C_ReadByteFromSlave().
#endif // #if ENABLE_I2C_ReadByteFromSlave
