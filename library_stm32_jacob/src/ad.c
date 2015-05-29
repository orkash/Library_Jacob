/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: stm_ad.c
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
#include "../include/ad.h"

/************************************************ 
* 函  数: ADC1_Init(void)
* 功  能: 初始化ADC1
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void ADC1_Init(void)
{
  ADC_InitTypeDef ADC_InitStructure;  // ADC初始化结构体
  GPIO_InitTypeDef GPIO_InitStructure;  // GPIO初始化结构体

  // 使能ADC通道时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1, ENABLE);
  // 设置ADC分频因子为6, 72M/6 =12M, ADC最大时间不能超过14M
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
  // 复位ADC1, 将外设ADC1全部寄存器复位
  ADC_DeInit(ADC1);

  /* Configure PB0 (ADC) as an analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  

  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // 独立工作模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;  // 多通道扫描
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // 连续模数转换
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 转换触发方式：转换由软件触发启动
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // ADC 数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1; // 进行规则转换的 ADC 通道的数目为1
  
	ADC_Init(ADC1, &ADC_InitStructure);
	/* 设置 ADC1 使用8转换通道，采样时间为 55.5 周期 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);
	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* 复位 ADC1 的校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/* 等待 ADC1 校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1)) continue;
	/* 开始 ADC1 校准 */
	ADC_StartCalibration(ADC1);
	/* 等待 ADC1 校准完成 */
	while(ADC_GetCalibrationStatus(ADC1)) continue;  
	/* 启动 ADC1 转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	  
} // InitADC()

/************************************************ 
* 函  数: ADC_Read(ADC_TypeDef *)
* 功  能: 读取指定ADC的值
*
* 参  数: ADCx - ADC编号
*
* 返回值: 读取到的AD值
*************************************************/
unsigned short ADC_Read(ADC_TypeDef *ADCx)   
{
  while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC )) 
    continue;  // 等待转换结束

  return ADC_GetConversionValue(ADCx);  // 返回最近一次ADC1规则组的转换结果
} // ADC_Read()

/************************************************ 
* 函  数: ADC1_Read(void)
* 功  能: 读取ADC1的值
*
* 参  数: 无
*
* 返回值: 读取到的AD值
*************************************************/
unsigned short ADC1_Read(void)
{
  return ADC_Read(ADC1);
} // ADC1_Read()
