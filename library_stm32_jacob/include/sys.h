/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: sys.h
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
#ifndef STM32_SYS_H_
#define STM32_SYS_H_

#include <stm32f10x.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // #ifndef TRUE

// �Ƿ�֧��UCOS����ϵͳ: 0, ��֧��ucos; 1, ֧��ucos
#define SYSTEM_SUPPORT_UCOS FALSE 

// λ������, ʵ��51���Ƶ�GPIO���ƹ���;
// ����ʵ��˼��, �ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ);
// IO�ڲ����궨��.
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2)) 
#define MEM_ADDR(addr) *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum)) 
// IO�ڵ�ַӳ��
// IO���
#define GPIOA_ODR_Addr  (GPIOA_BASE+12) // 0x4001080C 
#define GPIOB_ODR_Addr  (GPIOB_BASE+12) // 0x40010C0C 
#define GPIOC_ODR_Addr  (GPIOC_BASE+12) // 0x4001100C 
#define GPIOD_ODR_Addr  (GPIOD_BASE+12) // 0x4001140C 
#define GPIOE_ODR_Addr  (GPIOE_BASE+12) // 0x4001180C 
#define GPIOF_ODR_Addr  (GPIOF_BASE+12) // 0x40011A0C    
#define GPIOG_ODR_Addr  (GPIOG_BASE+12) // 0x40011E0C    
// IO����
#define GPIOA_IDR_Addr  (GPIOA_BASE+8)  // 0x40010808 
#define GPIOB_IDR_Addr  (GPIOB_BASE+8)  // 0x40010C08 
#define GPIOC_IDR_Addr  (GPIOC_BASE+8)  // 0x40011008 
#define GPIOD_IDR_Addr  (GPIOD_BASE+8)  // 0x40011408 
#define GPIOE_IDR_Addr  (GPIOE_BASE+8)  // 0x40011808 
#define GPIOF_IDR_Addr  (GPIOF_BASE+8)  // 0x40011A08 
#define GPIOG_IDR_Addr  (GPIOG_BASE+8)  // 0x40011E08 
// IO�ڲ���, ֻ��Ե�һ��IO��, ȷ��n��ֵС��16
// IO�����
#define PAout(n)  BIT_ADDR(GPIOA_ODR_Addr, n) // ���
#define PBout(n)  BIT_ADDR(GPIOB_ODR_Addr, n) // ��� 
#define PCout(n)  BIT_ADDR(GPIOC_ODR_Addr, n) // ��� 
#define PDout(n)  BIT_ADDR(GPIOD_ODR_Addr, n) // ���
#define PEout(n)  BIT_ADDR(GPIOE_ODR_Addr, n) // ��� 
#define PFout(n)  BIT_ADDR(GPIOF_ODR_Addr, n) // ��� 
#define PGout(n)  BIT_ADDR(GPIOG_ODR_Addr, n) // ���
// IO������
#define PAin(n)   BIT_ADDR(GPIOA_IDR_Addr, n) // ���� 
#define PBin(n)   BIT_ADDR(GPIOB_IDR_Addr, n) // ���� 
#define PCin(n)   BIT_ADDR(GPIOC_IDR_Addr, n) // ���� 
#define PDin(n)   BIT_ADDR(GPIOD_IDR_Addr, n) // ���� 
#define PEin(n)   BIT_ADDR(GPIOE_IDR_Addr, n) // ����
#define PFin(n)   BIT_ADDR(GPIOF_IDR_Addr, n) // ����
#define PGin(n)   BIT_ADDR(GPIOG_IDR_Addr, n) // ����

/*----- High layer function -----*/
// NVIC�ж�����
void NVIC_Configuration(void);
// ϵͳ�����ⲿʱ��HSE����
void RCC_Configuration(void);

/*----- Medium layer function -----*/

/*----- Low layer function -----*/

#endif  // #ifndef STM32_SYS_H_
