/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: dma.c 
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
#include "../include/dma.h"

u16 g_DMA1_bit_len = 0;  // DMA1ÿ�����ݴ��ͳ���, ��λbit

/************************************************ 
* ��  ��: SD_SPI_ReadWriteCmd(DMA_Channel_TypeDef *, u32, u32, u16)
* ��  ��: DMA1��ͨ������: 
*         ����Ĵ�����ʽ�ǹ̶���, ���Ҫ���ݲ�ͬ��������޸�;
*         �Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ.
*
* ��  ��: DMA1_Channelx - DMA1ͨ��Channelx
*         des_addr - Ŀ���ַ, �����ַper_addr
*         src_addr - Դ��ַ, �洢����ַmem_addr
*         data_size - ���ݴ�����, sizeof(send_buf)/sizeof(send_buf����������), ���65536
*
* ����ֵ: ��
*************************************************/
void DMA1_Config(DMA_Channel_TypeDef *DMA1_Channelx, u32 des_addr, 
                 u32 src_addr, u16 data_size)
{
  DMA_InitTypeDef DMA_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  // ʹ��DMA����ʱ��
  
  DMA_DeInit(DMA1_Channelx);  // ��DMAͨ��1�ļĴ�������Ϊȱʡֵ
  g_DMA1_bit_len = data_size;
  DMA_InitStructure.DMA_PeripheralBaseAddr = des_addr;  // DMA�������ַ������Ϊ���ڻ���ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = src_addr;  // DMA�ڴ����ַ������Ϊ�����������ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  // ���ݴ��䷽�򣬴��ڴ��ȡ������
  DMA_InitStructure.DMA_BufferSize = data_size; // DMA�Ļ����С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // �����ַ����
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // �ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // ���ݿ��Ϊ8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // ���ݿ��Ϊ8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // ��������������ģʽ�������ѭ��ģʽ����
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // �е����ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  // DMAͨ��x��δ����Ϊ�ڴ浽�ڴ洫��
  DMA_Init(DMA1_Channelx, &DMA_InitStructure);  // ��ʼ��
} // DMA_Config()      

/************************************************ 
* ��  ��: DMA_Enable(DMA_Channel_TypeDef *)
* ��  ��: ����һ��DMA����
*
* ��  ��: DMA1_Channelx - DMA1ͨ��Channelx
*
* ����ֵ: ��
*************************************************/
void DMA_Enable(DMA_Channel_TypeDef *DMA1_Channelx)
{
  DMA_Cmd(DMA1_Channelx, DISABLE);  // �ر�DMA1_Channelxͨ��
  // �趨DMAͨ�������С
  // ����ʹ������USART1��USART1��Ӧ��DMAͨ��ΪDMA1_Channel4
  DMA_SetCurrDataCounter(DMA1_Channelx, g_DMA1_bit_len);  
  DMA_Cmd(DMA1_Channelx, ENABLE); // ����DMA1_Channelxͨ��
} // DMA_Enable()

/************************************************ 
* ��  ��: DMA1_Enable(void)
* ��  ��: ����һ��DMA1����, ʹ��DMA1_Channel4
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void DMA1_Enable(void)
{
  DMA_Enable(DMA1_Channel4);
} // DMA1_Enable()
