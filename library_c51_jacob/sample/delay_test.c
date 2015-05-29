/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: delay_test.c  
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: delay���Ժ���, ��׼ANSI C��������, ����C51
* ��    ��: main() - delay��������
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��4��7��
* ��������: E0.0.1, ʵ��delay��������, 2015��4��7��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/

// �������delay.hͷ�ļ�, ����·���ɸ���delay.hλ���޸�
#include "../include/delay.h" 

#include <reg51.h> 

/************************************************ 
* ˵  ��: ����crc˵��
* ��  ��: DelayUs() - us��ʱ
*         DelayMs() - ms��ʱ
* void DelayUs(void)
* void DelayMs(unsigned int ms)
*
* ��  ��: ms - ��ʱms��
*
*����ֵ : ��
*************************************************/

int main(void)
{
  DelayUs();
  
  while (1)
  {
    DelayMs(100);
    /*code here*/
  }; // while 

  return 0;
} // main().
