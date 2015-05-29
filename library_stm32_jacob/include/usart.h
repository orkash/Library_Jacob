/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: usart.h  
* 文件标识: 见配置管理计划书
* 摘    要: stm32, 串口配置
* 函    数: USART1_Init() - 初始化串口1
*           SendUsartByte() - 串口发送字节数据
*           SendUsartString() - 串口发送字符串数据
*           USART1_SendByte() - 串口1发送单字节数据
*           USART1_SendString() - 串口1发送字符串数据
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月15日
* 更改内容: E0.0.1, 完成了串口1初始化、发送, 2015年4月15日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include <stm32f10x.h>

#ifndef STM32_USART_H_
#define STM32_USART_H_ 

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define EN_USART1_RX TRUE // 是否使能串口中断接收
// #define EN_USART2_RX TRUE
// #define EN_USART3_RX TRUE

#define USART_RXD_LEN 64 // 定义最大接收字节数128

/*----- High layer function -----*/
// 初始化串口
void USART1_Init(u32 baud);
// 串口1发送单字节数据
void USART1_SendByte(unsigned char dat);
// 串口1发送字符串数据
void USART1_SendString(const unsigned char buf[], unsigned short send_bytes);

/*----- Medium layer function -----*/
// 串口发送单字节数据
void SendUsartByte(USART_TypeDef* USARTx, unsigned char dat);
// 串口发送字符串数据
void SendUsartString(USART_TypeDef* USARTx, const unsigned char buf[], unsigned short buf_cnt);

/*----- Low layer function -----*/

#endif  // #ifndef STM32_USART_H_
