/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: stm_ad.h
* 文件标识: 见配置管理计划书
* 摘    要: 使用stm32内部12位AD
* 函    数: ADC1_Init() - 初始化ADC1
*           ADC_Read() - 读取指定ADC的值
*           ADC1_Read() - 读取ADC1的值
*           
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月15日
* 更改内容: E0.0.1, 完成了stm32 flash读取与写操作, 2015年4月15日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#ifndef STM32_AD_H
#define STM32_AD_H

#include <stm32f10x_adc.h>

/*----- High layer function -----*/
// 初始化ADC1
void ADC1_Init(void);
// 读取ADC1的值
unsigned short ADC1_Read(void);

/*----- Medium layer function -----*/
// 读取指定ADC的值
unsigned short ADC_Read(ADC_TypeDef *ADCx);

/*----- Low layer function -----*/

#endif  // end of #ifndef STM32_AD_H
