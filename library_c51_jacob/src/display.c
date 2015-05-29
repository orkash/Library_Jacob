/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: display.c  
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: ������ÿһλ���ΪASCII��ʾ
* ��    ��: ConvertDataTo10Bits() - �����ݲ��Ϊ6��ASCII��ʽ���ݣ����㲹0
*           Display10BitsData() - ͨ��������ʾ6��ASCII��ʽ����, ���λΪ����λ
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��3��2��
* ��������: E0.0.1, ��ɲ��Ϊ10����ASCII��ʾ, 2015��3��2��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#include "../include/display.h"

#if ENABLE_IN_51
#include "../include/uart.h"
#endif // #if ENABLE_IN_51

#if ENABLE_IN_WIN
#include <stdio.h>
#endif // #if ENABLE_IN_WIN

/************************************************ 
* ��  ��: ConvertDataTo10Bits(unsigned char *, int)
* ��  ��: �����ݲ��Ϊ6��ASCII��ʽ����
*
* ��  ��: display_ptr - ת��������
*         displayed_data - ��ת������
*
* ����ֵ: ��
*************************************************/
void ConvertDataTo10Bits(unsigned char *display_ptr, int displayed_data)
{
  // ���λΪ����λ
	if (displayed_data < 0)
	{
		displayed_data = - displayed_data;  // ������ת��Ϊ������
		*display_ptr = '-'; // ASCII 0x2D
	} else {
    *display_ptr = '+';  // ASCII 0x2B
  } // end of else ().
  // ������λ
	*++display_ptr = displayed_data/10000;
  // ����ǧλ
	displayed_data = displayed_data%10000;  // ȡ������
	*++display_ptr = displayed_data/1000;
  // ���ݰ�λ
	displayed_data = displayed_data%1000; // ȡ������
	*++display_ptr = displayed_data/100;
  // ����ʮλ
	displayed_data = displayed_data%100;  // ȡ������
	*++display_ptr = displayed_data/10;
  // ���ݸ�λ
	displayed_data = displayed_data%10; // ȡ������
	*++display_ptr = displayed_data; 	
} // end of ConvertDataTo10Bits().

/************************************************ 
* ��  ��: Display10BitsData(int)
* ��  ��: ������ÿһλ���ΪASCII��ʾ
*
* ��  ��: displayed_data - ����ʾ����
*
* ����ֵ: ��
*************************************************/
#if ENABLE_DISPLAY  // �Ƿ�ʹ��Display10BitsData()����
void Display10BitsData(int displayed_data)
{  
  unsigned char display_buf[6] = {0}; // ����ת��������

	ConvertDataTo10Bits(display_buf, displayed_data);  // ת��������ʾ
  // 6��10λ����, ���λΪ����λ
#if ENABLE_IN_51  
  UART_SendString(display_buf, 6);
#endif // #if ENABLE_IN_51

#if ENABLE_IN_WIN
  for (int i = 0; i < 6; ++i)
  {
    printf("%d ", display_buf[i]);
  } // for
  printf("\n");
#endif // #if ENABLE_IN_WIN
} // end of Display10BitsData().
#endif  // #if ENABLE_DISPLAY
