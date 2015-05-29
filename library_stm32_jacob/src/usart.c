/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: usart.c  
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: stm32, ��������
* ��    ��: USART1_Init() - ��ʼ������1
*           SendUsartByte() - ���ڷ����ֽ�����
*           SendUsartString() - ���ڷ����ַ�������
*           USART1_SendByte() - ����1���͵��ֽ�����
*           USART1_SendString() - ����1�����ַ�������
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��4��15��
* ��������: E0.0.1, ����˴���1��ʼ��������, 2015��4��15��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#include "../include/usart.h"

#include <stdio.h>  // Declare FILE and fputc().

// ���ʹ��ucos, ����������ͷ�ļ�����
#if SYSTEM_SUPPORT_UCOS
#include "includes.h" // ʹ��ucos 	  
#endif

// ���ջ����������USART_REC_LEN���ֽڡ�
unsigned char g_usart_rx_buf[USART_RXD_LEN];
unsigned int g_uart_count = 0;

// �������´���, ֧��printf����, ������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
// ��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
// ����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
// �ض���fputc���� 
int fputc(int ch, FILE *f)
{
	while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC))
    continue;
  // ����д��while()�ȴ�������Խ����һ���ֽڷ��Ͳ�����������
  USART_SendData(USART1, (u8)ch);
  
	return ch;
}
#endif  // #if 1

// ʹ��microLib�ķ���
 /* int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) 
    continue;	
   
  return ch;
}
int GetKey(void) { 
  // ע�⣬��ȡUSARTx->SR�ܱ���Ī������Ĵ���   
  while (!(USART1->SR & USART_FLAG_RXNE))
    continue;

  return ((int)(USART1->DR & 0x1FF));
}
*/

/************************************************ 
* ��  ��: USART1_Init(u32)
* ��  ��: ��ʼ������1
*
* ��  ��: baud - ��ʼ��������
*
* ����ֵ: ��
*************************************************/
void USART1_Init(u32 baud)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  // Enable USART1, GPIOA clock.���︴����PA.9, PA.10��USART1����
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
  // Reset USART1.
  USART_DeInit(USART1);
  // Set the mode of GPIOA, full duplex.
  // USART1_TX PA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���츴�������
  GPIO_Init(GPIOA, &GPIO_InitStructure);  // Initialize PA.9.
  // USART1_RX PA.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // �������롣
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
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // ��ռ���ȼ�Ϊ3.
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  // �����ȼ�Ϊ3.
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // Enable IRQ channel.

  NVIC_Init(&NVIC_InitStructure); // Initialize NVIC.
  // Enable USART1 interrupt if receive buffer isn't empty.
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
#endif  // #if EN_USART1_RX 
  USART_Cmd(USART1, ENABLE);  // Enable USART1.

  return;
} // USART1_Init()

/************************************************ 
* ��  ��: SendUsartByte(USART_TypeDef *, unsigned char)
* ��  ��: ���ڷ��͵��ֽ�����
*
* ��  ��: USARTx - ��ʹ�õĴ���
*         dat - �����͵��ֽ�����
*
* ����ֵ: ��
*************************************************/
void SendUsartByte(USART_TypeDef *USARTx, unsigned char dat)
{
  USART_SendData(USARTx, dat);

  // Wait till all data are transimited completely.
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET)
    continue;
  // ���������ɱ�־
  USART_ClearFlag(USARTx, USART_FLAG_TC); 
} // SendUsartByte()

/************************************************ 
* ��  ��: SendUsartString(USART_TypeDef *, const unsigned char[], unsigned short)
* ��  ��: ���ڷ����ַ�������
*
* ��  ��: USARTx - ��ʹ�õĴ���
*         buf - �����͵��ַ��������׵�ַ
*         send_bytes - ���������ݵ��ֽ���
*
* ����ֵ: ��
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
* ��  ��: USART1_SendByte(unsigned char)
* ��  ��: ����1���͵��ֽ�����
*
* ��  ��: dat - �����͵��ֽ�����
*
* ����ֵ: ��
*************************************************/
void USART1_SendByte(unsigned char dat)
{
  SendUsartByte(USART1, dat);
} // USART1_SendByte()

/************************************************ 
* ��  ��: USART1_SendString(const unsigned char[], unsigned short)
* ��  ��: ����1�����ַ�������
*
* ��  ��: buf - �����͵��ַ��������׵�ַ
*         send_bytes - ���������ݵ��ֽ���
*
* ����ֵ: ��
*************************************************/
void USART1_SendString(const unsigned char buf[], unsigned short send_bytes)
{
  SendUsartString(USART1, buf, send_bytes);
} // USART1_SendString()

// ����1�жϷ������
#if EN_USART1_RX  // ���ʹ���˴��ڽ���

// ����1�жϷ����ӳ���
void USART1_IRQHandler(void)                  
{
#ifdef OS_TICKS_PER_SEC   //���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
  OSIntEnter();    
#endif
  if(SET == USART_GetITStatus(USART1, USART_IT_RXNE)) // ���յ�����
  {
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    if (g_uart_count >= USART_RXD_LEN) g_uart_count = 0;
    g_usart_rx_buf[g_uart_count++] = USART_ReceiveData(USART1);
  } // end of if
#ifdef OS_TICKS_PER_SEC // ���ʱ�ӽ�����������, ˵��Ҫʹ��ucosII��.
  OSIntExit();                         
#endif  // #ifdef OS_TICKS_PER_SEC
} // USART1_IRQHandler()
#ifdef OS_TICKS_PER_SEC // ���ʱ�ӽ�����������, ˵��Ҫʹ��ucosII��.
  OSIntExit();                         
#endif  // #ifdef OS_TICKS_PER_SEC 

#endif  // #if EN_USART1_RX 	
