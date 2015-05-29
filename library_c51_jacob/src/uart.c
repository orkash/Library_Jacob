/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: uart.c  
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
#include "../include/uart.h"

#include <reg51.h>  // ����SCON, TMOD, TH1, TL1, TR1, ES, EA, SBUF��TI

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // end of #ifndef TRUE

/************************************************ 
* ��  ��: UART_Init(unsigned int)
* ��  ��: ��ʼ������
*
* ��  ��: baud- ������
*
* ����ֵ: ��
*************************************************/
void UART_Init(unsigned int baud)
{
  SCON = 0x50;  // UART������ʽ1��10λ�첽�շ�����ʱ�����Ʋ�����
  TMOD &= 0x0F;
  TMOD |= 0x20; // timer 1, mode 2
  AUXR = 0x40;  // timer1��1Tģʽ�¹������ٶ�*12
  TH1 = 256 - FOSC/32/baud;
  TL1 = TH1;
  ET1 = FALSE;  // timer 1��ʱֻ������uart
  TR1 = TRUE;
#if ENABLE_UART_INTERRUPT
  ES = TRUE;  // �����п��ж�
  EA = TRUE;  // �����ж�
#endif  // end of #if ENABLE_UART_INTERRUPT
} // end of UART_Init().

/************************************************ 
* ��  ��: UART_SendByte(unsigned char)
* ��  ��: ���ڷ����ֽ�����
*
* ��  ��: data0 - �������ֽ�����
*
* ����ֵ: ��
*************************************************/
void UART_SendByte(unsigned char data0)
{
  SBUF = data0;  // ���ݴ���SBUF���Զ�����
  
  while(!TI)
    continue;
  TI = 0;
} // end of UART_SendByte().

/************************************************ 
* ��  ��: UART_SendString(const unsigned char [], int)
* ��  ��: ���ڷ����ֽ�����
*
* ��  ��: data_buf[] - ��������������
*         data_len - �����������ֽ���
*
* ����ֵ: ��
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
* ��  ��: UART_Interrupt(void) interrupt 4 using 3
* ��  ��: �����ж�
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
#if ENABLE_UART_INTERRUPT // �Ƿ��������ж�
void UART_Interrupt(void) interrupt 4 using 3
{
  if (RI)
  {  
    RI = FALSE; // ���UART�����жϱ�־λ
    
    /*code here*/
  } // end of if ().
} // end of UART_Interrupt().
#endif  // end of #if ENABLE_UART_INTERRUPT
