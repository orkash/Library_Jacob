/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: key.c
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: ������Ӧ����
* ��    ��: KEY_Init() - ��ʼ��������ӦIO��
*           KEY_Scan() - ɨ�谴��, ͨ����ʱ����
*           KEY_Drive() - ��������
*           KEY_Scan2() - ����ɨ�跽ʽ2, ͨ���ж�״̬��־����
*           KEY_Drive2() - ����������ʽ2
* 
* ��ǰ�汾: A0.0.1
* ��    ��: Jacob Peng
* �������: 2015��4��15��
* ��������: A0.0.1, �����ɨ�谴����������������, 2015��4��16��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#include "../include/key.h"

#include "../include/delay.h"
#include "../include/sys.h"
#include "../include/led.h"
#include "../include/usart.h"

/************************************************ 
* ��  ��: KEY_Init(void)
* ��  ��: ��ʼ��������ӦIO��
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void KEY_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  // ʹ��IO��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
} // KEY_Init()

/************************************************ 
* ��  ��: KEY_Scan(unsigned char)
* ��  ��: ɨ�谴��, ͨ����ʱ����
*
* ��  ��: mode - �Ƿ�֧��������������: 1, ֧��; 0, ��֧��
*
* ����ֵ: 0, û�а�������; 1~4, ��Ӧ�����µİ���
*************************************************/
unsigned char KEY_Scan(unsigned char mode)
{
  static unsigned char s_key_up = 1;  // �����ɿ���־
  
  if (1 == mode) s_key_up = 1; // ֧����������
  if (s_key_up && (0 == KEY1 || 0 == KEY2 || 0 == KEY3 || 0 == KEY4))
  {
    DelayMs(2); // ������
    s_key_up = 0; // ��������
    if (0 == KEY1) return 1;
    else if (0 == KEY2) return 2;
    else if (0 == KEY3) return 3;
    else if (0 == KEY4) return 4;
  } else if (1 == KEY1 && 1 == KEY2 && 1 == KEY3 && 1 == KEY4) {
    s_key_up = 1;
  } // else if
  
  return 0;
} // KEY_Scan()

/************************************************ 
* ��  ��: KEY_Drive(void)
* ��  ��: ��������
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void KEY_Drive(void)
{
  unsigned char key_status = 0;
  
  key_status = KEY_Scan(0);  // ��֧����������
  switch (key_status)
  {
    case 1: LED1 = !LED1; SendUsartByte(USART1, 0x01); break;
    case 2: LED2 = !LED2; SendUsartByte(USART1, 0x02); break;
    case 3: LED3 = !LED3; SendUsartByte(USART1, 0x03); break;
    case 4: LED4 = !LED4; SendUsartByte(USART1, 0x04); break;
    default: break; // û�а�������
  } // switch
} // KEY_Drive()

// �����Ƿ����־
unsigned char g_key_up_buf[4] = {1, 1, 1, 1};

/************************************************ 
* ��  ��: KEY_Scan2(void)
* ��  ��: ɨ�谴����ʽ2, ͨ���ж�״̬��־����, ���ж���ɨ��Ч���ܺ�
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void KEY_Scan2(void)
{
  unsigned int i = 0;
  static unsigned char s_key_up_buf[4] = {0xFF, 0xFF, 0xFF, 0xFF};

  s_key_up_buf[0] = (s_key_up_buf[0] << 1) | KEY1;
  s_key_up_buf[1] = (s_key_up_buf[1] << 1) | KEY2;
  s_key_up_buf[2] = (s_key_up_buf[2] << 1) | KEY3;
  s_key_up_buf[3] = (s_key_up_buf[3] << 1) | KEY4;
  for (i = 0; i < 4; ++i)
  {
    // ��������
    if (0x00 == (s_key_up_buf[i] & 0x0F))
    {
      g_key_up_buf[i] = 0;
    } else if (0xFF == (s_key_up_buf[i] | 0xF0)){
      g_key_up_buf[i] = 1;
    } // else
  } // for 
} // KEY_Scan2()

/************************************************ 
* ��  ��: KEY_Drive(void)
* ��  ��: ����������ʽ2, ���ж���ɨ��Ч���ܺ�
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void KEY_Drive2(void)
{
  unsigned int i = 0;
  static unsigned char s_key_up_backup[4] = {1, 1, 1, 1};

  //KEY_Scan2();  // ���ж���ɨ��Ч���ܺ�
  for (i = 0; i < 4; ++i)
  {
    if (g_key_up_buf[i] != s_key_up_backup[i])
    {
      if (0 == s_key_up_backup[i])
      {
        switch (i + 1)
        {
          case 1: LED1 = !LED1; SendUsartByte(USART1, 0x01); break;
          case 2: LED2 = !LED2; SendUsartByte(USART1, 0x02); break;
          case 3: LED3 = !LED3; SendUsartByte(USART1, 0x03); break;
          case 4: LED4 = !LED4; SendUsartByte(USART1, 0x04); break;
          default: break; // û�а�������
        } // switch
      } // if
      s_key_up_backup[i] = g_key_up_buf[i];
    } // if
  } // for
} // KEY_Drive2()
