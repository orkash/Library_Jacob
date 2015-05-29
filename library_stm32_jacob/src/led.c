/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: led.c  
* 文件标识: 见配置管理计划书
* 摘    要: stm32, led相应操作
* 函    数: LED_Init() - 初始化LED相应IO口
* 
* 当前版本: A0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月15日
* 更改内容: A0.0.1, 完成了LED初始化, 2015年4月16日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include "../include/led.h"

/************************************************ 
* 函  数: LED_Init(mode)
* 功  能: 初始化LED相应IO口
*
* 参  数: mode - 所有LED灯亮或灭模式: 1, 亮; 0, 灭
*
* 返回值: 无
*************************************************/
void LED_Init(unsigned char mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  if (mode !=0 && mode != 1) return;
  // 使能IO口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  LED1 = mode;
  LED2 = mode;
  LED3 = mode;
  LED4 = mode;
} // LED_Init()
