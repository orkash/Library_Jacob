/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: delay.c  
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
#include "../include/delay.h"

#include <stm32f10x.h>

unsigned char g_fac_us = 0; // us延时倍乘数
unsigned short g_fac_ms = 0;  // ms延时倍乘数

/************************************************ 
* 函  数: InitDelay(void)
* 功  能: 初始化系统滴答时钟
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void InitDelay(void)
{
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); // 外部时钟8分频
  g_fac_us = SystemCoreClock/8000000; // 为系统时钟的1/8
  g_fac_ms = (unsigned short)g_fac_us*1000;
} // InitDelay()

/************************************************ 
* 函  数: DelayUs(unsigned int)
* 功  能: us延时
*
* 参  数: us - 延时微秒数
*
* 返回值: 无
*************************************************/
void DelayUs(unsigned int us)
{
  unsigned int ctrl_temp = 0;
  
  //InitDelay();
  SysTick->LOAD = (unsigned int)us*g_fac_us;  // 计时器加载值
  SysTick->VAL = 0x00;  // 清空计数器
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 开始倒计时
  // 等待时间到达：
  // (ctrl_temp & 0x01)判断倒计时是否开启,
  // (ctrl_temp & (1 << 16))判断倒计时是否到达.
  do {
    ctrl_temp = SysTick->CTRL;
  } while ((ctrl_temp & 0x01) && !(ctrl_temp & (1 << 16)));
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  // 停止倒计时
  SysTick->VAL = 0x00;  // 清空计数器
} // DelayUs()

/************************************************ 
* 函  数: DelayUs(unsigned int)
* 功  能: ms延时
*
* 参  数: ms - 延时毫秒数
*
* 返回值: 无
*************************************************/
void DelayMs(unsigned short ms)
{
  unsigned int ctrl_temp = 0;
  
  //InitDelay();
  SysTick->LOAD = (unsigned int)ms*g_fac_ms;  // 计时器加载值
  SysTick->VAL = 0x00;  // 清空计数器
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 开始倒计时
  // 等待时间到达：
  // (ctrl_temp & 0x01)判断倒计时是否开启,
  // (ctrl_temp & (1 << 16))判断倒计时是否到达.
  do {
    ctrl_temp = SysTick->CTRL;
  } while ((ctrl_temp & 0x01) && !(ctrl_temp & (1 << 16)));
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  // 停止倒计时
  SysTick->VAL = 0x00;  // 清空计数器
} // DelayMs()
