/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: usart.c  
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
#include "../include/usart.h"

#include <stdio.h>  // Declare FILE and fputc().

// 如果使用ucos, 则包含下面的头文件即可
#if SYSTEM_SUPPORT_UCOS
#include "includes.h" // 使用ucos 	  
#endif

// 接收缓冲区，最大USART_REC_LEN个字节。
unsigned char g_usart_rx_buf[USART_RXD_LEN];
unsigned int g_uart_count = 0;

// 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
// 标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
// 定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
// 重定义fputc函数 
int fputc(int ch, FILE *f)
{
	while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC))
    continue;
  // 发送写在while()等待后面可以解决第一个字节发送不出来的问题
  USART_SendData(USART1, (u8)ch);
  
	return ch;
}
#endif  // #if 1

// 使用microLib的方法
 /* int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) 
    continue;	
   
  return ch;
}
int GetKey(void) { 
  // 注意，读取USARTx->SR能避免莫名其妙的错误   
  while (!(USART1->SR & USART_FLAG_RXNE))
    continue;

  return ((int)(USART1->DR & 0x1FF));
}
*/

/************************************************ 
* 函  数: USART1_Init(u32)
* 功  能: 初始化串口1
*
* 参  数: baud - 初始化波特率
*
* 返回值: 无
*************************************************/
void USART1_Init(u32 baud)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  // Enable USART1, GPIOA clock.这里复用了PA.9, PA.10的USART1功能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
  // Reset USART1.
  USART_DeInit(USART1);
  // Set the mode of GPIOA, full duplex.
  // USART1_TX PA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推挽复用输出。
  GPIO_Init(GPIOA, &GPIO_InitStructure);  // Initialize PA.9.
  // USART1_RX PA.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入。
  GPIO_Init(GPIOA, &GPIO_InitStructure);  // Initialize PA.10.
  // Settings of USART1.
  USART_InitStructure.USART_BaudRate = baud;  // Recommend 19200.
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  // No hardware flow control.
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  // USART mode: TXD and RXD.
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

  USART_Init(USART1, &USART_InitStructure); // Initialize USART1.
#if EN_USART1_RX  // If USART1 rxd is enabled.
  // Settings of NVIC.
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级为3.
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  // 子优先级为3.
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // Enable IRQ channel.

  NVIC_Init(&NVIC_InitStructure); // Initialize NVIC.
  // Enable USART1 interrupt if receive buffer isn't empty.
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
#endif  // #if EN_USART1_RX 
  USART_Cmd(USART1, ENABLE);  // Enable USART1.

  return;
} // USART1_Init()

/************************************************ 
* 函  数: SendUsartByte(USART_TypeDef *, unsigned char)
* 功  能: 串口发送单字节数据
*
* 参  数: USARTx - 待使用的串口
*         dat - 待发送的字节数据
*
* 返回值: 无
*************************************************/
void SendUsartByte(USART_TypeDef *USARTx, unsigned char dat)
{
  USART_SendData(USARTx, dat);

  // Wait till all data are transimited completely.
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET)
    continue;
  // 清除发送完成标志
  USART_ClearFlag(USARTx, USART_FLAG_TC); 
} // SendUsartByte()

/************************************************ 
* 函  数: SendUsartString(USART_TypeDef *, const unsigned char[], unsigned short)
* 功  能: 串口发送字符串数据
*
* 参  数: USARTx - 待使用的串口
*         buf - 待发送的字符串数据首地址
*         send_bytes - 待发送数据的字节数
*
* 返回值: 无
*************************************************/
void SendUsartString(USART_TypeDef* USARTx, const unsigned char buf[], unsigned short send_bytes)
{
  unsigned short i = 0;
  
  for (i = 0; i < send_bytes; ++i)
  {
    SendUsartByte(USARTx, buf[i]);
  } // for
} // SendUsartString()

/************************************************ 
* 函  数: USART1_SendByte(unsigned char)
* 功  能: 串口1发送单字节数据
*
* 参  数: dat - 待发送的字节数据
*
* 返回值: 无
*************************************************/
void USART1_SendByte(unsigned char dat)
{
  SendUsartByte(USART1, dat);
} // USART1_SendByte()

/************************************************ 
* 函  数: USART1_SendString(const unsigned char[], unsigned short)
* 功  能: 串口1发送字符串数据
*
* 参  数: buf - 待发送的字符串数据首地址
*         send_bytes - 待发送数据的字节数
*
* 返回值: 无
*************************************************/
void USART1_SendString(const unsigned char buf[], unsigned short send_bytes)
{
  SendUsartString(USART1, buf, send_bytes);
} // USART1_SendString()

// 串口1中断服务程序
#if EN_USART1_RX  // 如果使能了串口接收

// 串口1中断服务子程序
void USART1_IRQHandler(void)                  
{
#ifdef OS_TICKS_PER_SEC   //如果时钟节拍数定义了,说明要使用ucosII了.
  OSIntEnter();    
#endif
  if(SET == USART_GetITStatus(USART1, USART_IT_RXNE)) // 接收到数据
  {
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    if (g_uart_count >= USART_RXD_LEN) g_uart_count = 0;
    g_usart_rx_buf[g_uart_count++] = USART_ReceiveData(USART1);
  } // end of if
#ifdef OS_TICKS_PER_SEC // 如果时钟节拍数定义了, 说明要使用ucosII了.
  OSIntExit();                         
#endif  // #ifdef OS_TICKS_PER_SEC
} // USART1_IRQHandler()
#ifdef OS_TICKS_PER_SEC // 如果时钟节拍数定义了, 说明要使用ucosII了.
  OSIntExit();                         
#endif  // #ifdef OS_TICKS_PER_SEC 

#endif  // #if EN_USART1_RX 	
