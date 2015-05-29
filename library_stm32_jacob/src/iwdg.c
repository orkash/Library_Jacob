/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: iwdg.c  
* 文件标识: 见配置管理计划书
* 摘    要: stm32, 独立看门狗
* 函    数: IWDG_Init() - 初始化独立看门狗
*           IWDG_Feed() - 独立看门狗喂狗
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月17日
* 更改内容: E0.0.1, 完成了独立看门狗初始化, 2015年4月17日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include "../include/iwdg.h"

#include <stm32f10x.h>
#include <stm32f10x_iwdg.h>

/************************************************ 
* 函  数: IWDG_Init(u8, u16)
* 功  能: 初始化独立看门狗, 预分频数 = 4*2^prescaler, 最大256;
*         看门狗复位大概时间: time_out = (4*2^prescaler)*reload/40(ms)
*
* 参  数: prescaler - 分频因子(0~7), 仅低3位有效
*         reload - 重装载寄存器值, 低11位有效
*
* 返回值: 无, 调用示例: IWDG_Init(4, 62); // 独立看门狗, 预分频数64，重装载值62，溢出时间约为100ms 
*************************************************/
void IWDG_Init(u8 prescaler, u16 reload)
{
  // 使能对寄存器IWDG_PR和IWDG_RLR的写操作
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  // 设置IWDG预分频数
  IWDG_SetPrescaler(prescaler);
  // 设置IWDG重新装载值
  IWDG_SetReload(reload);
  // 根据IWDG重新装载寄存器值重新装载IWDG计数器
  IWDG_ReloadCounter();
  // 使能IWDG
  IWDG_Enable();
} // IWDG_Init()

/************************************************ 
* 函  数: IWDG_Feed(void)
* 功  能: 独立看门狗喂狗
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void IWDG_Feed(void)
{
  IWDG_ReloadCounter();
} // IWDG_Feed()
