/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: led.h 
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
#include "../include/sys.h"

#ifndef STM32_LED_H_
#define STM32_LED_H_

#define LED1 PAout(4)
#define LED2 PAout(5)
#define LED3 PAout(6)
#define LED4 PAout(7)

/*----- High layer function -----*/
// 初始化LED相应IO口
void LED_Init(unsigned char mode);

/*----- Medium layer function -----*/

/*----- Low layer function -----*/


#endif  // #ifndef STM32_LED_H_
