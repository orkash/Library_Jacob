/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: display_test.c  
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: display���Ժ���, ��׼ANSI C��������, ����C51
* ��    ��: main() - display��������
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��4��7��
* ��������: E0.0.1, ʵ��display��������, 2015��4��7��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/

// �������display.hͷ�ļ�, ����·���ɸ���display.hλ���޸�
#include "../include/display.h" 

// δʹ��ConvertDataTo10Bits()������ʾ,
// ���Լ�������׼���ͷ�ļ��� 
#if !ENABLE_DISPLAY	

#if ENABLE_IN_51
#include "../include/uart.h"
#endif	// #if ENABLE_IN_51
#if ENABLE_IN_WIN
#include <stdio.h>
#endif	// #if ENABLE_IN_WIN

#endif	// #if !ENABLE_DISPLAY

/************************************************ 
* ˵  ��: ������"display.h"�ļ���, ȷ����C51��ʹ��, #define USED_IN_C51, 
*         ������Wondows��ʹ��, #define USED_IN_WIN
* ��  ��: Display10BitsData() - ������ÿһλ���ΪASCII��ʾ
* void ConvertDataTo10Bits(unsigned char *display_ptr, int displayed_data)
* void Display10BitsData(int displayed_data), ��C51
*
* ��  ��: displayed_data - ����ʾ����
*
*����ֵ : ��
*************************************************/

int main(void)
{
  unsigned int i = 0;
  int displayed_data = 4567;

#if ENABLE_DISPLAY  // �Ƿ�ʹ��Display10BitsData()���� 
  // ����Display10BitsData()ֱ����ʾ, ���λΪ����λ
  Display10BitsData(displayed_data);
#endif  // #if ENABLE_DISPLAY

  /*������ת������ConvertDataTo10Bits()*/
/*   unsigned char display_buf[6] = {0}; // ����ת��������
	ConvertDataTo10Bits(display_buf, displayed_data);  // ת��������ʾ
  // �Զ�����ʾ���
  // Windows���
#if ENABLE_IN_WIN
  for (i = 0; i < 6; ++i)
  {
    printf("%d ", display_buf[i]);
  } //
  printf("\n");
#endif  // #if ENABLE_IN_WIN

  // C51�������, 6��10λ����
#if ENABLE_IN_51 
  UART_SendString(display_buf, 6);
#endif  // #if ENABLE_IN_51  */

  return 0;
} // main().
