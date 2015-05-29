/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: dma.h
* 文件标识: 见配置管理计划书
* 摘    要: stm32, DMA直接存储器访问, 通过DMA使用USART1
* 函    数: DMA1_Config() - DMA1的通道配置
*           DMA_Enable() - 开启一次DMA传输
*           DMA1_Enable() - 开启一次DMA1传输
* 
* 当前版本: A0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月22日
* 更改内容: A0.0.1, 完成了DMA基本设置函数, 2015年4月22日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include <stm32f10x_dma.h>

#ifndef STM32_DMA_H_
#define STM32_DMA_H_

/*----- High layer function -----*/
// DMA1的通道配置
void DMA1_Config(DMA_Channel_TypeDef *DMA1_Channelx, u32 des_addr, 
                 u32 src_addr, u16 data_bit_size);
// 开启一次DMA1传输
void DMA1_Enable(void);

/*----- Medium layer function -----*/
// 开启一次DMA传输
void DMA_Enable(DMA_Channel_TypeDef *DMA1_Channelx);

/*----- Low layer function -----*/

#endif  // #ifndef STM32_DMA_H_
