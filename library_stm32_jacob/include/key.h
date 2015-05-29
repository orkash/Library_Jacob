/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: key.h
* 文件标识: 见配置管理计划书
* 摘    要: 按键相应操作
* 函    数: KEY_Init() - 初始化按键对应IO口
*           KEY_Scan() - 扫描按键, 通过延时消抖
*           KEY_Drive() - 驱动按键
*           KEY_Scan2() - 按键扫描方式2, 通过判断状态标志消抖
*           KEY_Drive2() - 驱动按键方式2
* 
* 当前版本: A0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月15日
* 更改内容: A0.0.1, 完成了扫描按键、驱动按键函数, 2015年4月16日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include <stm32f10x.h>

#include "../include/sys.h"

#ifndef STM32_KEY_H_
#define STM32_KEY_H_

#define KEY1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define KEY3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define KEY4 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)

/*----- High layer function -----*/
// 初始化按键对应IO口
void KEY_Init(void);
// 驱动按键
void KEY_Drive(void);
// 驱动按键方式2, 在中断中扫描效果很好
void KEY_Drive2(void);

/*----- Medium layer function -----*/
// 扫描按键, 通过延时消抖
unsigned char KEY_Scan(unsigned char mode);
// 扫描按键方式2, 通过判断状态标志消抖, 在中断中扫描效果很好
void KEY_Scan2(void);

/*----- Low layer function -----*/

#endif  // #ifndef STM32_KEY_H_
