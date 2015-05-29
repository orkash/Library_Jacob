/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: wwdg.c 
* 文件标识: 见配置管理计划书
* 摘    要: 窗口看门狗操作
* 函    数: WWDG_NVIC_Init() - 窗口看门狗中断初始化
*           WWDG_Init() - 初始化窗口看门狗
*           WWDG_Feed() - 窗口看门狗喂狗
* 
* 当前版本: A0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月17日
* 更改内容: A0.0.1, 完成了窗口看门狗初始化, 2015年4月17日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include "../include/wwdg.h"

#include <stm32f10x.h>

#include "../include/led.h"

u8 g_wwdg_cnt = 0x7F;  // WWDG中间窗口值, 默认为最大值

/************************************************ 
* 函  数: WWDG_NVIC_Init(void)
* 功  能: 窗口看门狗中断初始化
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void WWDG_NVIC_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn; // WWDG中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  // 抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // 子优先级0

  NVIC_Init(&NVIC_InitStructure); // 初始化NVIC
} // WWDG_NVIC_Init()

/************************************************ 
* 函  数: WWDG_Init(u8, u8, u32)
* 功  能: 初始化窗口看门狗, Fwwdg = PCLK1/(4096*2^prescaler)
*         看门狗复位大概时间: time_out = (4*2^prescaler)*reload/40(ms)
*
* 参  数: timer_cnt - T[6:0], 计数器值, 最大0x7F, 最小0x40
          window_cnt - W[6:0], 窗口值. 上窗口值, 最大0x7F, 最小0x40
*         prescaler - 分频系数(WDGTB), 仅最低2位有效
*
* 返回值: 无
*************************************************/
void WWDG_Init(u8 timer_cnt, u8 window_cnt, u32 prescaler)
{
  // 使能WWDG时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
  
  g_wwdg_cnt = timer_cnt & 0x7F;
  // 设置WWDG预分频数
  WWDG_SetPrescaler(prescaler);
  // 设置窗口值
  WWDG_SetWindowValue(window_cnt);
  // 使能看门狗，设置计数器初值
  // timer_cnt & 0x7F, 限制计数器值最大为WWDG 7位计数器的最大值: 0x7F
  WWDG_Enable(g_wwdg_cnt);  
  // 清除提前唤醒中断标志位
  WWDG_ClearFlag();
  // 初始化窗口看门狗NVIC
  WWDG_NVIC_Init();
  // 开启窗口看门狗中断
  WWDG_EnableIT();
} // WWDG_Init()

/************************************************ 
* 函  数: WWDG_Feed(u8)
* 功  能: 窗口看门狗喂狗
*
* 参  数: cnt - 重新待装载的计数器值
*
* 返回值: 无
*************************************************/
void WWDG_Feed(u8 cnt)
{
  // 重新设置计数器值
  //WWDG_Enable(cnt & 0x7F);
  WWDG_Enable(cnt);
} // WWDG_Feed()

/************************************************ 
* 函  数: WWDG_IRQHandler(void)
* 功  能: 窗口看门狗中断, 此中断里面必须有其他用户操作, 否则系统将复位
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void WWDG_IRQHandler(void)
{
  // 窗口看门狗喂狗
  // 当禁掉此句后, 窗口看门狗将产生复位
  WWDG_Feed(g_wwdg_cnt);
  // WWDG_Feed(0x7F);
  // 清除提前唤醒中断标志
  WWDG_ClearFlag();
  // 此中断里面必须有其他用户操作, 否则系统将复位;
  // 不知道为什么会这样!
  LED1 = !LED1;
  //SendUsartByte(USART1, 0xEE);
} // WWDG_IRQHandler()
