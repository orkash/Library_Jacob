/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: uart.h
* 文件标识: 见配置管理计划书
* 摘    要: C51的uart串口相关函数的实现
* 函    数: UART_Init() - 串口初始化
*           UART_SendByte() - 串口发送字节数据
*           UART_SendString() - 串口发送字符串
*           UART_Interrupt() - 串口中断
*
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年2月28日
* 更改内容: E0.0.1, 完成了串口初始化、查询发送一个字节与
*                   整个字符串、串口中断, 2015年2月28日;
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#ifndef UART_H_
#define UART_H_

#define FOSC 18432000UL // 晶振18.432Mhz
// 是否使能串口中断
#define ENABLE_UART_INTERRUPT FALSE

// 声明辅助寄存器的地址
sfr AUXR = 0x8E;

// 串口初始化
void UART_Init(unsigned int baud);
// 串口发送字节数据
void UART_SendByte(unsigned char data0);
// 串口发送字符串
void UART_SendString(const unsigned char data_buf[], int data_len);

#endif  // end of #ifndef UART_H_
