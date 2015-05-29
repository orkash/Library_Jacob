/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: sys.c
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: stm32, NVIC, RCC����, ��IO�ڽ���λ�������ο�����ԭ��Դ����
* ��    ��: NVIC_Configuration() - NVIC�ж�����
*           RCC_Configuration() - ϵͳ�����ⲿʱ��HSE����
* 
* ��ǰ�汾: E0.0.2
* ��    ��: Jacob Peng
* �������: 2015��5��21��
* ��������: E0.0.1, ʵ����NVIC, RCC��������, 2015��4��23��
*           E0.0.2, Ϊ��SD����ʼ��, ������ϵͳʱ��Ƶ��, 2015��5��21��
* 
* ȡ���汾: E0.0.1
* ԭ����  : Jacob Peng
* �������: 2015��4��23��
*/
#include "../include/sys.h"

#include <stm32f10x.h>
#include <stm32f10x_flash.h>  // Declare FLASH_PrefetchBufferCmd(), FLASH_SetLatency().

/************************************************ 
* ��  ��: NVIC_Configuration(void)
* ��  ��: NVIC�ж�����
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/ 
void NVIC_Configuration(void)
{
  // ����NVIC�жϷ���2:2, 2λ��ռ���ȼ�, 2λ��Ӧ���ȼ�
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
} // NVIC_Configuration()

/************************************************ 
* ��  ��: RCC_Configuration(void)
* ��  ��: ϵͳ�����ⲿʱ��HSE����: 
*         Ϊ��SD����ʼ��, ������ϵͳʱ��Ƶ��, 
*         ��RCC_PCLK2Config(RCC_HCLK_Div1),
*         �޸�ΪRCC_PCLK2Config(RCC_HCLK_Div2)
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void RCC_Configuration(void)
{
  /* ������������RCC�����ã����ƼĴ�����λ */
  RCC_DeInit();
  /* ʹ���ⲿ���پ��� */
  RCC_HSEConfig(RCC_HSE_ON);
  /* �ȴ����پ����ȶ� */
  if (SUCCESS == RCC_WaitForHSEStartUp())
  {
    /* ʹ��flashԤ��ȡ������ */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    /* ��Flash���ڵȴ�״̬��2����Ը�Ƶʱ�ӵģ��������RCCûֱ�ӹ�ϵ�����������Թ� */
    FLASH_SetLatency(FLASH_Latency_2);    
    /*AHB, HCLK = SYSCLK ���ø�������ʱ��=ϵͳʱ��*/
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    /*����AHB, PCLK2 = HCLK ���õ�������2ʱ��=��������ʱ��*/
    // RCC_PCLK2Config(RCC_HCLK_Div1);
    RCC_PCLK2Config(RCC_HCLK_Div2); // 72/2=36MHz, Ϊ��SD����ʼ�������ٶ� 
    /* PCLK1 = HCLK/2 ���õ�������1��ʱ��=����ʱ�ӵĶ���Ƶ*/
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /*����AHB, ADCCLK = PCLK2/6 ����ADC����ʱ��=��������2ʱ�ӵ�����Ƶ*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    /* Set PLL clock output to 72MHz using HSE (8MHz) as entry clock */   
    /* �������໷���ⲿ8Mhz����9��Ƶ��72Mhz, ���ܹؼ�*/ 
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
    /* Enable PLL ʹ�����໷*/
    RCC_PLLCmd(ENABLE); 
    /* Wait till PLL is ready �ȴ����໷����ȶ�*/
    while (RESET == RCC_GetFlagStatus(RCC_FLAG_PLLRDY)) continue;
    /* Select PLL as system clock source �����໷�������Ϊϵͳʱ�� */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source �ȴ�У��ɹ�*/
    while (RCC_GetSYSCLKSource() != 0x08) continue;
  } // if
    /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
    // ʹ����Χ�ӿ�����ʱ�ӣ�ע�������������������ͬоƬ�ķ��䲻ͬ����ʱ����ֲ�Ϳ���;
    // Ҫʹ����һ�����Ż���, ��ֻʹ��Ҫʹ�õ����Ż���, �������Խ��͹����������ȶ���
    /*RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |
                           RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);*/ 
} // RCC_Configuration()
