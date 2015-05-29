/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: dma.h
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: stm32, DMAֱ�Ӵ洢������, ͨ��DMAʹ��USART1
* ��    ��: DMA1_Config() - DMA1��ͨ������
*           DMA_Enable() - ����һ��DMA����
*           DMA1_Enable() - ����һ��DMA1����
* 
* ��ǰ�汾: A0.0.1
* ��    ��: Jacob Peng
* �������: 2015��4��22��
* ��������: A0.0.1, �����DMA�������ú���, 2015��4��22��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#include <stm32f10x_dma.h>

#ifndef STM32_DMA_H_
#define STM32_DMA_H_

/*----- High layer function -----*/
// DMA1��ͨ������
void DMA1_Config(DMA_Channel_TypeDef *DMA1_Channelx, u32 des_addr, 
                 u32 src_addr, u16 data_bit_size);
// ����һ��DMA1����
void DMA1_Enable(void);

/*----- Medium layer function -----*/
// ����һ��DMA����
void DMA_Enable(DMA_Channel_TypeDef *DMA1_Channelx);

/*----- Low layer function -----*/

#endif  // #ifndef STM32_DMA_H_
