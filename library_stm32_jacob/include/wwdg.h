/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: wwdg.h
* 文件标识: 见配置管理计划书
* 摘    要: 窗口看门狗操作
* 函    数: WWDG_NVIC_Init() - 窗口看门狗中断初始化
*           WWDG_Init() - 初始化窗口看门狗
*           WWDG_Feed() - 窗口看门狗喂狗
* 
* 当前版本: A0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月17日
* 更改内容: A0.0.1, 完成了窗口看门狗初始化, 2015年4月17日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#ifndef STM32_WWDG_H_
#define STM32_WWDG_H_

#include "stm32f10x_wwdg.h"
#include <stm32f10x.h>

/*----- High layer function -----*/
// 初始化窗口看门狗 
void WWDG_Init(u8 timer_cnt, u8 window_cnt, u32 prescaler);
// 窗口看门狗喂狗
void WWDG_Feed(u8 cnt);

/*----- Medium layer function -----*/
// 窗口看门狗中断初始化
void WWDG_NVIC_Init(void);

/*----- Low layer function -----*/


#endif  // #ifndef STM32_WWDG_H_
