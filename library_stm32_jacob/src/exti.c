/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: exti.c 
* 文件标识: 见配置管理计划书
* 摘    要: stm32, exti外部中断
* 函    数: EXTIMY_Init() - 外部中断初始化
* 
* 当前版本: A0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月17日
* 更改内容: A0.0.1, 完成了外部中断初始化等几个函数, 2015年4月17日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include "../include/exti.h"

#include <stm32f10x.h>
#include <stm32f10x_exti.h>
#include <stdio.h>  // Declare printf().

#include "../include/key.h" // Use keys.
#include "../include/led.h" // Use LEDs.
#include "../include/delay.h" // Use DelayMs().
#include "../include/usart.h" // Use UASRT.
#include "../include/standby.h" // Use standby mode.

/************************************************ 
* 函  数: EXTIMY_Init(void)
* 功  能: 外部中断初始化
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void EXTIMY_Init(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  KEY_Init();  // 按键端口初始化
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // 使能中断复用功能
  
  // GPIOA.0, 中断线以及中断初始化配置, 下降沿触发, KEY1
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); // 根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
  
  // GPIOA.1, 中断线以及中断初始化配置, 下降沿触发, KEY2
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_Init(&EXTI_InitStructure);
  
  // GPIOA.2, 中断线以及中断初始化配置, 下降沿触发, KEY3
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_Init(&EXTI_InitStructure);

  // GPIOC.13, 中断线以及中断初始化配置, 下降沿触发, KEY4
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  // 使能KEY1所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  // 抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; // 子优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能外部中断通道
  NVIC_Init(&NVIC_InitStructure); // 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;  // 使能KEY2所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  // 抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; // 子优先级1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能外部中断通道
  NVIC_Init(&NVIC_InitStructure); // 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;  // 使能KEY3所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  // 抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; // 子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能外部中断通道
  NVIC_Init(&NVIC_InitStructure); // 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  // 使能KEY4所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  // 抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; // 子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能外部中断通道
  NVIC_Init(&NVIC_InitStructure); // 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
} // EXTIMY_Init()

/************************************************ 
* 函  数: EXTI0_IRQHandler(void)
* 功  能: 外部中断0
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void EXTI0_IRQHandler(void)
{
  // DelayMs(10); // 消抖
  // if (0 == KEY1) LED1 = !LED1;
  EXTI_ClearITPendingBit(EXTI_Line0); // 清除Line0上的中断标志位
  //SendUsartByte(USART1, 0x00);
  printf("0x%02x ", 0x00);
  if(0 == Standby_Check()) Standby_SysEnter(); 
} // EXTI0_IRQHandler()

/************************************************ 
* 函  数: EXTI1_IRQHandler(void)
* 功  能: 外部中断1
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void EXTI1_IRQHandler(void)
{
  printf("0x%02x ", 0x01);
  //SendUsartByte(USART1, 0x01);
  DelayMs(20); // 消抖
  if (0 == KEY2) LED2 = !LED2;
  EXTI_ClearITPendingBit(EXTI_Line1); // 清除Line1上的中断标志位
} // EXTI1_IRQHandler()

/************************************************ 
* 函  数: EXTI2_IRQHandler(void)
* 功  能: 外部中断2
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void EXTI2_IRQHandler(void)
{
  printf("0x%02x ", 0x02);
  //SendUsartByte(USART1, 0x02);
  DelayMs(30); // 消抖
  if (0 == KEY3) LED3 = !LED3;
  EXTI_ClearITPendingBit(EXTI_Line2); // 清除Line2上的中断标志位  
} // EXTI2_IRQHandler()

/************************************************ 
* 函  数: EXTI15_10_IRQHandler(void)
* 功  能: 外部中断10~15
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void EXTI15_10_IRQHandler(void)
{
  printf("0x%02x ", 0x13);
 // SendUsartByte(USART1, 0x13);
  DelayMs(140); // 消抖
  if (0 == KEY4) LED4 = !LED4;
  EXTI_ClearITPendingBit(EXTI_Line13); // 清除Line13上的中断标志位
} // EXTI15_10_IRQHandler()
