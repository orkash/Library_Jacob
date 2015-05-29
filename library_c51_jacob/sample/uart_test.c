/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: uart_test.c  
* 文件标识: 见配置管理计划书
* 摘    要: C51, uart测试函数
* 函    数: main() - uart调用例程
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月7日
* 更改内容: E0.0.1, 实现uart调用例程, 2015年4月7日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/

// 必须包含uart.h头文件, 包含路径可根据uart.h位置修改
#include "../include/uart.h" 

// 必须保护C51头文集
#include <reg51.h>

/************************************************ 
* 说  明: 必须在"uart.h"文件中, 确认是否使用串口中断, ENABLE_UART_INTERRUPT
* 函  数: UART_Init() - 串口初始化
*         UART_SendByte() - 串口发送字节数据
*         UART_SendString() - 串口发送字符串
* void UART_Init(unsigned int baud)
* void UART_SendByte(unsigned char data0)
* void UART_SendString(const unsigned char data_buf[], int data_len)
*
* 参  数: baud - 波特率
*         data0 - 待发送字节数据
*         data_buf[] - 待发送数据数组
*         data_len - 待发送数据字节数
*
*返回值 : 无
*************************************************/

int main(void)
{
  unsigned char send_buf[16] = {0xEF, 0xBE, 0x42, 0x73};
  
  // 串口初始化
  UART_Init(19200); // 常用波特率为9600, 这里使用19200
  // 这里要延时, 串口初始化有一个过程, 
  // 否则立即发送串口数据接收不到。
  // DelayMS(50);
  UART_SendByte(0xAA);
  UART_SendString(send_buf, 4);
  UART_SendByte(0xDD);
  // 如果要使用串口中断, #define ENABLE_UART_INTERRUPT TRUE,
  // 中断相关代码, 请写在"uart.c"中的UART_Interrupt()函数中

  return 0;
} // main().
