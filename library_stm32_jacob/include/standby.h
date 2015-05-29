/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: standby.h
* 文件标识: 见配置管理计划书
* 摘    要: stm32, 低功耗相应操作, 使用待机模式
* 函    数: Standby_SysEnter() - 系统进入待机模式
*           Standby_Check() - 检测待机WAKEUP引脚的信号
*           Standby_Init() - 通过PA0引脚唤醒功能初始化
* 
* 当前版本: A0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月15日
* 更改内容: A0.0.1, 完成了, 2015年4月20日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#ifndef STM32_STANDBY_H_
#define STM32_STANDBY_H_

#include "../include/sys.h"

#define WAKEUP PAin(0)  // PA0 检测外部唤醒按键是否按下

/*----- High layer function -----*/
// 系统进入待机模式
void Standby_SysEnter(void);
// 检测待机WAKEUP引脚的信号
u8 Standby_Check(void);
// 通过PA0引脚唤醒功能初始化
void Standby_Init(void);

/*----- Medium layer function -----*/

/*----- Low layer function -----*/

#endif  // #ifndef STM32_STANDBY_H_
