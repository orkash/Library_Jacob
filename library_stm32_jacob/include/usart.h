/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: usart.h  
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
#include <stm32f10x.h>

#ifndef STM32_USART_H_
#define STM32_USART_H_ 

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define EN_USART1_RX TRUE // �Ƿ�ʹ�ܴ����жϽ���
// #define EN_USART2_RX TRUE
// #define EN_USART3_RX TRUE

#define USART_RXD_LEN 64 // �����������ֽ���128

/*----- High layer function -----*/
// ��ʼ������
void USART1_Init(u32 baud);
// ����1���͵��ֽ�����
void USART1_SendByte(unsigned char dat);
// ����1�����ַ�������
void USART1_SendString(const unsigned char buf[], unsigned short send_bytes);

/*----- Medium layer function -----*/
// ���ڷ��͵��ֽ�����
void SendUsartByte(USART_TypeDef* USARTx, unsigned char dat);
// ���ڷ����ַ�������
void SendUsartString(USART_TypeDef* USARTx, const unsigned char buf[], unsigned short buf_cnt);

/*----- Low layer function -----*/

#endif  // #ifndef STM32_USART_H_
