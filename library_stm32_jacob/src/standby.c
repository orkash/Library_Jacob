/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: standby.c 
* 文件标识: 见配置管理计划书
* 摘    要: stm32, 低功耗相应操作, 使用待机模式
* 函    数: Standby_SysEnter() - 系统进入待机模式
*           Standby_Check() - 检测待机WAKEUP引脚的信号
*           Standby_Init() - 通过PA0引脚唤醒功能初始化
* 
* 当前版本: A0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月15日
* 更改内容: A0.0.1, 完成了, 2015年4月20日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include "../include/standby.h"

#include <stm32f10x.h>
#include <stm32f10x_pwr.h>
#include <stm32f10x_exti.h>

#include "../include/led.h"
#include "../include/delay.h"

/************************************************ 
* 函  数: Standby_SysEnter(void)
* 功  能: 系统进入待机模式
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void Standby_SysEnter(void)
{
  RCC_APB2PeriphResetCmd(0x01FC, DISABLE);  // 复位所有IO口
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // 使能PWR外设时钟
  PWR_WakeUpPinCmd(ENABLE); // 使能唤醒引脚唤醒功能
  PWR_EnterSTANDBYMode(); // 进入待机模式
} // Standby_SysEnter()

/************************************************ 
* 函  数: Standby_Check(void)
* 功  能: 检测待机WAKEUP引脚的信号
*
* 参  数: 无
*
* 返回值: 0, 错误的触发; 1, 连续按下3s以上
*************************************************/
u8 Standby_Check(void)
{
  u8 key_down_time = 0; // 按键按下时间
  u8 key_up_times = 0;  // 按键松开次数
  
  while (1)
  {
    // 唤醒按键按下
    if (0 == WAKEUP)
    {
      ++key_down_time;
      key_up_times = 0;
    } else {
      ++key_up_times;
      // 超过300ms没有唤醒信号
      if (key_up_times > 3)
      {
        LED1 = 0; // 熄灭LED1
        return 0; // 错误的触发, 按下时间不够
      } // if
    } // if else
    DelayMs(30);
    // 按下时间超过100 * 30ms
    if (key_down_time >= 100)
    {
      LED1 = 1; // 点亮LED1
      return 1;
    } // if
  } // while
} // Standby_Check()

/************************************************ 
* 函  数: Standby_Init(void)
* 功  能: 通过PA0引脚唤醒功能初始化
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void Standby_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  
  // 使能GPIOA和复用功能时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // PA0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);  // 初始化IO口
  
  // 使用外部中断
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); // 中断线0连接GPIOA.0
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;  // 中断线0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 外部中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); // 初始化外部中断
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  // 使能按键所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  // 子优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能外部中断通道
  NVIC_Init(&NVIC_InitStructure); // 初始化外设NVIC寄存器
  
  //if (0 == Standby_Check()) Standby_SysEnter();  // 不是开机, 进入待机模式
} // Standby_Init()
