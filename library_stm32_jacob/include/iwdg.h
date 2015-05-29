/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: iwdg.h  
* 文件标识: 见配置管理计划书
* 摘    要: stm32, 独立看门狗
* 函    数: IWDG_Init() - 初始化独立看门狗
*           IWDG_Feed() - 独立看门狗喂狗
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月17日
* 更改内容: E0.0.1, 完成了独立看门狗初始化, 2015年4月17日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#ifndef STM32_IWDG_H_
#define STM32_IWDG_H_

#include <stm32f10x.h>

/*----- High layer function -----*/
// 初始化独立看门狗
void IWDG_Init(u8 prescaler, u16 reload);
// 独立看门狗喂狗
void IWDG_Feed(void);

/*----- Medium layer function -----*/

/*----- Low layer function -----*/


#endif  // #ifndef STM32_IWDG_H_
