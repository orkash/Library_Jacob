/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: uart.h
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: C51��uart������غ�����ʵ��
* ��    ��: UART_Init() - ���ڳ�ʼ��
*           UART_SendByte() - ���ڷ����ֽ�����
*           UART_SendString() - ���ڷ����ַ���
*           UART_Interrupt() - �����ж�
*
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��2��28��
* ��������: E0.0.1, ����˴��ڳ�ʼ������ѯ����һ���ֽ���
*                   �����ַ����������ж�, 2015��2��28��;
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#ifndef UART_H_
#define UART_H_

#define FOSC 18432000UL // ����18.432Mhz
// �Ƿ�ʹ�ܴ����ж�
#define ENABLE_UART_INTERRUPT FALSE

// ���������Ĵ����ĵ�ַ
sfr AUXR = 0x8E;

// ���ڳ�ʼ��
void UART_Init(unsigned int baud);
// ���ڷ����ֽ�����
void UART_SendByte(unsigned char data0);
// ���ڷ����ַ���
void UART_SendString(const unsigned char data_buf[], int data_len);

#endif  // end of #ifndef UART_H_
