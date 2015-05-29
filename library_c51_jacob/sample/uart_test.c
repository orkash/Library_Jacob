/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: uart_test.c  
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: C51, uart���Ժ���
* ��    ��: main() - uart��������
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��4��7��
* ��������: E0.0.1, ʵ��uart��������, 2015��4��7��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/

// �������uart.hͷ�ļ�, ����·���ɸ���uart.hλ���޸�
#include "../include/uart.h" 

// ���뱣��C51ͷ�ļ�
#include <reg51.h>

/************************************************ 
* ˵  ��: ������"uart.h"�ļ���, ȷ���Ƿ�ʹ�ô����ж�, ENABLE_UART_INTERRUPT
* ��  ��: UART_Init() - ���ڳ�ʼ��
*         UART_SendByte() - ���ڷ����ֽ�����
*         UART_SendString() - ���ڷ����ַ���
* void UART_Init(unsigned int baud)
* void UART_SendByte(unsigned char data0)
* void UART_SendString(const unsigned char data_buf[], int data_len)
*
* ��  ��: baud - ������
*         data0 - �������ֽ�����
*         data_buf[] - ��������������
*         data_len - �����������ֽ���
*
*����ֵ : ��
*************************************************/

int main(void)
{
  unsigned char send_buf[16] = {0xEF, 0xBE, 0x42, 0x73};
  
  // ���ڳ�ʼ��
  UART_Init(19200); // ���ò�����Ϊ9600, ����ʹ��19200
  // ����Ҫ��ʱ, ���ڳ�ʼ����һ������, 
  // �����������ʹ������ݽ��ղ�����
  // DelayMS(50);
  UART_SendByte(0xAA);
  UART_SendString(send_buf, 4);
  UART_SendByte(0xDD);
  // ���Ҫʹ�ô����ж�, #define ENABLE_UART_INTERRUPT TRUE,
  // �ж���ش���, ��д��"uart.c"�е�UART_Interrupt()������

  return 0;
} // main().
