/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: wwdg.c 
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: ���ڿ��Ź�����
* ��    ��: WWDG_NVIC_Init() - ���ڿ��Ź��жϳ�ʼ��
*           WWDG_Init() - ��ʼ�����ڿ��Ź�
*           WWDG_Feed() - ���ڿ��Ź�ι��
* 
* ��ǰ�汾: A0.0.1
* ��    ��: Jacob Peng
* �������: 2015��4��17��
* ��������: A0.0.1, ����˴��ڿ��Ź���ʼ��, 2015��4��17��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#include "../include/wwdg.h"

#include <stm32f10x.h>

#include "../include/led.h"

u8 g_wwdg_cnt = 0x7F;  // WWDG�м䴰��ֵ, Ĭ��Ϊ���ֵ

/************************************************ 
* ��  ��: WWDG_NVIC_Init(void)
* ��  ��: ���ڿ��Ź��жϳ�ʼ��
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void WWDG_NVIC_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn; // WWDG�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  // ��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // �����ȼ�0

  NVIC_Init(&NVIC_InitStructure); // ��ʼ��NVIC
} // WWDG_NVIC_Init()

/************************************************ 
* ��  ��: WWDG_Init(u8, u8, u32)
* ��  ��: ��ʼ�����ڿ��Ź�, Fwwdg = PCLK1/(4096*2^prescaler)
*         ���Ź���λ���ʱ��: time_out = (4*2^prescaler)*reload/40(ms)
*
* ��  ��: timer_cnt - T[6:0], ������ֵ, ���0x7F, ��С0x40
          window_cnt - W[6:0], ����ֵ. �ϴ���ֵ, ���0x7F, ��С0x40
*         prescaler - ��Ƶϵ��(WDGTB), �����2λ��Ч
*
* ����ֵ: ��
*************************************************/
void WWDG_Init(u8 timer_cnt, u8 window_cnt, u32 prescaler)
{
  // ʹ��WWDGʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
  
  g_wwdg_cnt = timer_cnt & 0x7F;
  // ����WWDGԤ��Ƶ��
  WWDG_SetPrescaler(prescaler);
  // ���ô���ֵ
  WWDG_SetWindowValue(window_cnt);
  // ʹ�ܿ��Ź������ü�������ֵ
  // timer_cnt & 0x7F, ���Ƽ�����ֵ���ΪWWDG 7λ�����������ֵ: 0x7F
  WWDG_Enable(g_wwdg_cnt);  
  // �����ǰ�����жϱ�־λ
  WWDG_ClearFlag();
  // ��ʼ�����ڿ��Ź�NVIC
  WWDG_NVIC_Init();
  // �������ڿ��Ź��ж�
  WWDG_EnableIT();
} // WWDG_Init()

/************************************************ 
* ��  ��: WWDG_Feed(u8)
* ��  ��: ���ڿ��Ź�ι��
*
* ��  ��: cnt - ���´�װ�صļ�����ֵ
*
* ����ֵ: ��
*************************************************/
void WWDG_Feed(u8 cnt)
{
  // �������ü�����ֵ
  //WWDG_Enable(cnt & 0x7F);
  WWDG_Enable(cnt);
} // WWDG_Feed()

/************************************************ 
* ��  ��: WWDG_IRQHandler(void)
* ��  ��: ���ڿ��Ź��ж�, ���ж���������������û�����, ����ϵͳ����λ
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void WWDG_IRQHandler(void)
{
  // ���ڿ��Ź�ι��
  // �������˾��, ���ڿ��Ź���������λ
  WWDG_Feed(g_wwdg_cnt);
  // WWDG_Feed(0x7F);
  // �����ǰ�����жϱ�־
  WWDG_ClearFlag();
  // ���ж���������������û�����, ����ϵͳ����λ;
  // ��֪��Ϊʲô������!
  LED1 = !LED1;
  //SendUsartByte(USART1, 0xEE);
} // WWDG_IRQHandler()
