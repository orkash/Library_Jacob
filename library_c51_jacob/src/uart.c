/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: uart.c  
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
#include "../include/uart.h"

#include <reg51.h>  // 申明SCON, TMOD, TH1, TL1, TR1, ES, EA, SBUF，TI

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // end of #ifndef TRUE

/************************************************ 
* 函  数: UART_Init(unsigned int)
* 功  能: 初始化串口
*
* 参  数: baud- 波特率
*
* 返回值: 无
*************************************************/
void UART_Init(unsigned int baud)
{
  SCON = 0x50;  // UART工作方式1、10位异步收发、定时器控制波特率
  TMOD &= 0x0F;
  TMOD |= 0x20; // timer 1, mode 2
  AUXR = 0x40;  // timer1在1T模式下工作，速度*12
  TH1 = 256 - FOSC/32/baud;
  TL1 = TH1;
  ET1 = FALSE;  // timer 1此时只能用于uart
  TR1 = TRUE;
#if ENABLE_UART_INTERRUPT
  ES = TRUE;  // 允许串行口中断
  EA = TRUE;  // 允许中断
#endif  // end of #if ENABLE_UART_INTERRUPT
} // end of UART_Init().

/************************************************ 
* 函  数: UART_SendByte(unsigned char)
* 功  能: 串口发送字节数据
*
* 参  数: data0 - 待发送字节数据
*
* 返回值: 无
*************************************************/
void UART_SendByte(unsigned char data0)
{
  SBUF = data0;  // 数据传给SBUF后自动发送
  
  while(!TI)
    continue;
  TI = 0;
} // end of UART_SendByte().

/************************************************ 
* 函  数: UART_SendString(const unsigned char [], int)
* 功  能: 串口发送字节数据
*
* 参  数: data_buf[] - 待发送数据数组
*         data_len - 待发送数据字节数
*
* 返回值: 无
*************************************************/
void UART_SendString(const unsigned char data_buf[], int data_len)
{
  unsigned char i = 0;
  
  for (i = 0; i < data_len; ++i)
  {
    UART_SendByte(data_buf[i]);
  } // end of for ().
} // end of UART_SendString().

/************************************************ 
* 函  数: UART_Interrupt(void) interrupt 4 using 3
* 功  能: 串口中断
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
#if ENABLE_UART_INTERRUPT // 是否允许串口中断
void UART_Interrupt(void) interrupt 4 using 3
{
  if (RI)
  {  
    RI = FALSE; // 清除UART接收中断标志位
    
    /*code here*/
  } // end of if ().
} // end of UART_Interrupt().
#endif  // end of #if ENABLE_UART_INTERRUPT
