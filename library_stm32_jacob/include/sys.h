/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: sys.h
* 文件标识: 见配置管理计划书
* 摘    要: stm32, NVIC, RCC配置, 对IO口进行位带操作参考正点原子源代码
* 函    数: NVIC_Configuration() - NVIC中断设置
*           RCC_Configuration() - 系统高速外部时钟HSE配置
* 
* 当前版本: E0.0.2
* 作    者: Jacob Peng
* 完成日期: 2015年5月21日
* 更改内容: E0.0.1, 实现了NVIC, RCC基本设置, 2015年4月23日
*           E0.0.2, 为了SD卡初始化, 降低了系统时钟频率, 2015年5月21日
* 
* 取代版本: E0.0.1
* 原作者  : Jacob Peng
* 完成日期: 2015年4月23日
*/
#ifndef STM32_SYS_H_
#define STM32_SYS_H_

#include <stm32f10x.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // #ifndef TRUE

// 是否支持UCOS操作系统: 0, 不支持ucos; 1, 支持ucos
#define SYSTEM_SUPPORT_UCOS FALSE 

// 位带操作, 实现51类似的GPIO控制功能;
// 具体实现思想, 参考<<CM3权威指南>>第五章(87页~92页);
// IO口操作宏定义.
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2)) 
#define MEM_ADDR(addr) *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum)) 
// IO口地址映射
// IO输出
#define GPIOA_ODR_Addr  (GPIOA_BASE+12) // 0x4001080C 
#define GPIOB_ODR_Addr  (GPIOB_BASE+12) // 0x40010C0C 
#define GPIOC_ODR_Addr  (GPIOC_BASE+12) // 0x4001100C 
#define GPIOD_ODR_Addr  (GPIOD_BASE+12) // 0x4001140C 
#define GPIOE_ODR_Addr  (GPIOE_BASE+12) // 0x4001180C 
#define GPIOF_ODR_Addr  (GPIOF_BASE+12) // 0x40011A0C    
#define GPIOG_ODR_Addr  (GPIOG_BASE+12) // 0x40011E0C    
// IO输入
#define GPIOA_IDR_Addr  (GPIOA_BASE+8)  // 0x40010808 
#define GPIOB_IDR_Addr  (GPIOB_BASE+8)  // 0x40010C08 
#define GPIOC_IDR_Addr  (GPIOC_BASE+8)  // 0x40011008 
#define GPIOD_IDR_Addr  (GPIOD_BASE+8)  // 0x40011408 
#define GPIOE_IDR_Addr  (GPIOE_BASE+8)  // 0x40011808 
#define GPIOF_IDR_Addr  (GPIOF_BASE+8)  // 0x40011A08 
#define GPIOG_IDR_Addr  (GPIOG_BASE+8)  // 0x40011E08 
// IO口操作, 只针对单一的IO口, 确保n的值小于16
// IO口输出
#define PAout(n)  BIT_ADDR(GPIOA_ODR_Addr, n) // 输出
#define PBout(n)  BIT_ADDR(GPIOB_ODR_Addr, n) // 输出 
#define PCout(n)  BIT_ADDR(GPIOC_ODR_Addr, n) // 输出 
#define PDout(n)  BIT_ADDR(GPIOD_ODR_Addr, n) // 输出
#define PEout(n)  BIT_ADDR(GPIOE_ODR_Addr, n) // 输出 
#define PFout(n)  BIT_ADDR(GPIOF_ODR_Addr, n) // 输出 
#define PGout(n)  BIT_ADDR(GPIOG_ODR_Addr, n) // 输出
// IO口输入
#define PAin(n)   BIT_ADDR(GPIOA_IDR_Addr, n) // 输入 
#define PBin(n)   BIT_ADDR(GPIOB_IDR_Addr, n) // 输入 
#define PCin(n)   BIT_ADDR(GPIOC_IDR_Addr, n) // 输入 
#define PDin(n)   BIT_ADDR(GPIOD_IDR_Addr, n) // 输入 
#define PEin(n)   BIT_ADDR(GPIOE_IDR_Addr, n) // 输入
#define PFin(n)   BIT_ADDR(GPIOF_IDR_Addr, n) // 输入
#define PGin(n)   BIT_ADDR(GPIOG_IDR_Addr, n) // 输入

/*----- High layer function -----*/
// NVIC中断设置
void NVIC_Configuration(void);
// 系统高速外部时钟HSE配置
void RCC_Configuration(void);

/*----- Medium layer function -----*/

/*----- Low layer function -----*/

#endif  // #ifndef STM32_SYS_H_
