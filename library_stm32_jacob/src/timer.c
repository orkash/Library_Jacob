/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: timer.c 
* 文件标识: 见配置管理计划书
* 摘    要: 通用定时器操作
* 函    数: TIMER3_Init() - 通用定时器3初始化
* 
* 当前版本: A0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月17日
* 更改内容: A0.0.1, 完成了定时器3初始化, 2015年4月17日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include "../include/timer.h"

#include <stm32f10x_tim.h>

#include "../include/key.h"

/************************************************ 
* 函  数: TIMER3_Init(u16, u16)
* 功  能: 通用定时器3初始化, 这里时钟选择为APB1的2倍, 而APB1为36M
*
* 参  数: arr - 自动重装值
*         psc - 时钟预分频数
*
* 返回值: 无, 调用示例: TIMER3_Init(199, 7199); // 20ms左右, 10Khz的计数频率
*************************************************/
void TIMER3_Init(u16 arr, u16 psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  // 定时器3时钟使能
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  TIM_TimeBaseStructure.TIM_Period = arr; // 设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_TimeBaseStructure.TIM_Prescaler = psc;  // 设置用来作为TIMx时钟频率除数的预分频值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 设置时钟分割: TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数模式
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); // 根据指定的参数初始化TIMx的时间基数单位
  
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  // 使能指定的TIM3中断, 允许更新中断
  
  // NVIC中断优先级设置
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; // TIM3中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;  // 抢占优先级0级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; // 从优先级3级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQ通道被使能
  NVIC_Init(&NVIC_InitStructure);
  
  TIM_Cmd(TIM3, ENABLE);  // 使能TIM3
} // TIMER3_Init()

/************************************************ 
* 函  数: TIM3_IRQHandler(void)
* 功  能: 通用定时器3中断
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void TIM3_IRQHandler(void)
{
  // 检查是否发生TIM3更新中断
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除TIM3更新中断标志
    KEY_Scan2();
  } // if
} // TIM3_IRQHandler()
