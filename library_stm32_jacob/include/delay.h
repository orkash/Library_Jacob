/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: delay.h  
* 文件标识: 见配置管理计划书
* 摘    要: stm32粗略延时函数
* 函    数: InitDelay() - 初始化系统滴答时钟
*           DelayUs() - us延时
*           DelayMs() - ms延时
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月16日
* 更改内容: E0.0.1, 完成了us, ms延时, 2015年4月16日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#ifndef STM32_DELAY_H_
#define STM32_DELAY_H_

/*----- High layer function -----*/
// 初始化系统滴答时钟
void InitDelay(void);
// us延时
void DelayUs(unsigned int us);
// ms延时
void DelayMs(unsigned short ms);

/*----- Medium layer function -----*/

/*----- Low layer function -----*/

#endif  // #ifndef STM32_DELAY_H_
