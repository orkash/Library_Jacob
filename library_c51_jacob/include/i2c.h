/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: i2c.h
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

#ifndef I2C_H_
#define I2C_H_

#include <reg51.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // end of #ifndef

// 是否使用其中一个I2C函数,
// 可以消除编译器未使用某一个函数的警告。
#define ENABLE_I2C_Start TRUE
#define ENABLE_I2C_Stop TRUE
#define ENABLE_I2C_SendACK TRUE
#define ENABLE_I2C_RecvACK TRUE
#define ENABLE_I2C_SendByte TRUE
#define ENABLE_I2C_RecvByte TRUE
#define ENABLE_I2C_WriteByteToSlave TRUE
#define ENABLE_I2C_ReadByteFromSlave TRUE

#define SLAVE_ADDRESS 0xD0  // IIC写入时的地址，+1为读取

sbit I2C_SDA = P3^4;  // IIC数据引脚定义
sbit I2C_SCL = P3^5;  // IIC时钟引脚定义

// I2C起始信号
#if ENABLE_I2C_Start
static void I2C_Start(void);
#endif // #if ENABLE_I2C_Start
// I2C停止信号
#if ENABLE_I2C_Stop
static void I2C_Stop(void);
#endif // #if ENABLE_I2C_Stop
// I2C发送应答信号
#if ENABLE_I2C_SendACK
static void I2C_SendACK(bit ack);
#endif // #if ENABLE_I2C_SendACK
// I2C接收应答信号
#if ENABLE_I2C_RecvACK
static bit I2C_RecvACK(void);
#endif // #if ENABLE_I2C_RecvACK
// 向I2C总线发送一个字节数据
#if ENABLE_I2C_SendByte
void I2C_SendByte(unsigned char dat);
#endif // #if ENABLE_I2C_SendByte
// 从I2C总线接收一个字节数据
#if ENABLE_I2C_RecvByte
unsigned char I2C_RecvByte(void);
#endif // #if ENABLE_I2C_RecvByte
// 向I2C设备写入一个字节数据
#if ENABLE_I2C_WriteByteToSlave
void I2C_WriteByteToSlave(unsigned char reg_address, unsigned char reg_data);
#endif // #if ENABLE_I2C_WriteByteToSlave
// 从I2C设备读取一个字节数据
#if ENABLE_I2C_ReadByteFromSlave
unsigned char I2C_ReadByteFromSlave(unsigned char reg_address);
#endif // #if ENABLE_I2C_ReadByteFromSlave

#endif  // end of #ifndef I2C_H_
