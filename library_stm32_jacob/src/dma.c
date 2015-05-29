/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: dma.c 
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
#include "../include/dma.h"

u16 g_DMA1_bit_len = 0;  // DMA1每次数据传送长度, 单位bit

/************************************************ 
* 函  数: SD_SPI_ReadWriteCmd(DMA_Channel_TypeDef *, u32, u32, u16)
* 功  能: DMA1的通道配置: 
*         这里的传输形式是固定的, 这点要根据不同的情况来修改;
*         从存储器->外设模式/8位数据宽度/存储器增量模式.
*
* 参  数: DMA1_Channelx - DMA1通道Channelx
*         des_addr - 目标地址, 外设地址per_addr
*         src_addr - 源地址, 存储器地址mem_addr
*         data_size - 数据传输量, sizeof(send_buf)/sizeof(send_buf的数据类型), 最大65536
*
* 返回值: 无
*************************************************/
void DMA1_Config(DMA_Channel_TypeDef *DMA1_Channelx, u32 des_addr, 
                 u32 src_addr, u16 data_size)
{
  DMA_InitTypeDef DMA_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  // 使能DMA传输时钟
  
  DMA_DeInit(DMA1_Channelx);  // 将DMA通道1的寄存器设置为缺省值
  g_DMA1_bit_len = data_size;
  DMA_InitStructure.DMA_PeripheralBaseAddr = des_addr;  // DMA外设基地址，这里为串口基地址
  DMA_InitStructure.DMA_MemoryBaseAddr = src_addr;  // DMA内存基地址，这里为待发送数组地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  // 数据传输方向，从内存读取到外设
  DMA_InitStructure.DMA_BufferSize = data_size; // DMA的缓存大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // 外设地址不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 内存地址寄存器递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 数据宽度为8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // 数据宽度为8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // 工作在正常缓存模式，相对于循环模式而言
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // 中等优先级
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  // DMA通道x并未设置为内存到内存传输
  DMA_Init(DMA1_Channelx, &DMA_InitStructure);  // 初始化
} // DMA_Config()      

/************************************************ 
* 函  数: DMA_Enable(DMA_Channel_TypeDef *)
* 功  能: 开启一次DMA传输
*
* 参  数: DMA1_Channelx - DMA1通道Channelx
*
* 返回值: 无
*************************************************/
void DMA_Enable(DMA_Channel_TypeDef *DMA1_Channelx)
{
  DMA_Cmd(DMA1_Channelx, DISABLE);  // 关闭DMA1_Channelx通道
  // 设定DMA通道缓存大小
  // 这里使用外设USART1，USART1对应的DMA通道为DMA1_Channel4
  DMA_SetCurrDataCounter(DMA1_Channelx, g_DMA1_bit_len);  
  DMA_Cmd(DMA1_Channelx, ENABLE); // 开启DMA1_Channelx通道
} // DMA_Enable()

/************************************************ 
* 函  数: DMA1_Enable(void)
* 功  能: 开启一次DMA1传输, 使用DMA1_Channel4
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void DMA1_Enable(void)
{
  DMA_Enable(DMA1_Channel4);
} // DMA1_Enable()
