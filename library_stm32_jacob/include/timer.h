/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: timer.h 
* 文件标识: 见配置管理计划书
* 摘    要: 通用定时器操作
* 函    数: TIMER3_Init() - 通用定时器3初始化
* 
* 当前版本: A0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月17日
* 更改内容: A0.0.1, 完成了定时器3初始化, 2015年4月17日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#ifndef STM32_TIMER_H_
#define STM32_TIMER_H_

#include <stm32f10x.h>

/*----- High layer function -----*/
// 通用定时器3初始化
void TIMER3_Init(u16 arr, u16 psc);

/*----- Medium layer function -----*/

/*----- Low layer function -----*/


#endif  // #ifndef STM32_TIMER_H_
