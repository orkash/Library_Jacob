/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: key.c
* 文件标识: 见配置管理计划书
* 摘    要: 按键相应操作
* 函    数: KEY_Init() - 初始化按键对应IO口
*           KEY_Scan() - 扫描按键, 通过延时消抖
*           KEY_Drive() - 驱动按键
*           KEY_Scan2() - 按键扫描方式2, 通过判断状态标志消抖
*           KEY_Drive2() - 驱动按键方式2
* 
* 当前版本: A0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月15日
* 更改内容: A0.0.1, 完成了扫描按键、驱动按键函数, 2015年4月16日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include "../include/key.h"

#include "../include/delay.h"
#include "../include/sys.h"
#include "../include/led.h"
#include "../include/usart.h"

/************************************************ 
* 函  数: KEY_Init(void)
* 功  能: 初始化按键对应IO口
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void KEY_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  // 使能IO口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);
} // KEY_Init()

/************************************************ 
* 函  数: KEY_Scan(unsigned char)
* 功  能: 扫描按键, 通过延时消抖
*
* 参  数: mode - 是否支持连续按键按下: 1, 支持; 0, 不支持
*
* 返回值: 0, 没有按键按下; 1~4, 对应被按下的按键
*************************************************/
unsigned char KEY_Scan(unsigned char mode)
{
  static unsigned char s_key_up = 1;  // 按键松开标志
  
  if (1 == mode) s_key_up = 1; // 支持连续按键
  if (s_key_up && (0 == KEY1 || 0 == KEY2 || 0 == KEY3 || 0 == KEY4))
  {
    DelayMs(2); // 消抖动
    s_key_up = 0; // 按键按下
    if (0 == KEY1) return 1;
    else if (0 == KEY2) return 2;
    else if (0 == KEY3) return 3;
    else if (0 == KEY4) return 4;
  } else if (1 == KEY1 && 1 == KEY2 && 1 == KEY3 && 1 == KEY4) {
    s_key_up = 1;
  } // else if
  
  return 0;
} // KEY_Scan()

/************************************************ 
* 函  数: KEY_Drive(void)
* 功  能: 驱动按键
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void KEY_Drive(void)
{
  unsigned char key_status = 0;
  
  key_status = KEY_Scan(0);  // 不支持连续按下
  switch (key_status)
  {
    case 1: LED1 = !LED1; SendUsartByte(USART1, 0x01); break;
    case 2: LED2 = !LED2; SendUsartByte(USART1, 0x02); break;
    case 3: LED3 = !LED3; SendUsartByte(USART1, 0x03); break;
    case 4: LED4 = !LED4; SendUsartByte(USART1, 0x04); break;
    default: break; // 没有按键按下
  } // switch
} // KEY_Drive()

// 按键是否弹起标志
unsigned char g_key_up_buf[4] = {1, 1, 1, 1};

/************************************************ 
* 函  数: KEY_Scan2(void)
* 功  能: 扫描按键方式2, 通过判断状态标志消抖, 在中断中扫描效果很好
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void KEY_Scan2(void)
{
  unsigned int i = 0;
  static unsigned char s_key_up_buf[4] = {0xFF, 0xFF, 0xFF, 0xFF};

  s_key_up_buf[0] = (s_key_up_buf[0] << 1) | KEY1;
  s_key_up_buf[1] = (s_key_up_buf[1] << 1) | KEY2;
  s_key_up_buf[2] = (s_key_up_buf[2] << 1) | KEY3;
  s_key_up_buf[3] = (s_key_up_buf[3] << 1) | KEY4;
  for (i = 0; i < 4; ++i)
  {
    // 按键按下
    if (0x00 == (s_key_up_buf[i] & 0x0F))
    {
      g_key_up_buf[i] = 0;
    } else if (0xFF == (s_key_up_buf[i] | 0xF0)){
      g_key_up_buf[i] = 1;
    } // else
  } // for 
} // KEY_Scan2()

/************************************************ 
* 函  数: KEY_Drive(void)
* 功  能: 驱动按键方式2, 在中断中扫描效果很好
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void KEY_Drive2(void)
{
  unsigned int i = 0;
  static unsigned char s_key_up_backup[4] = {1, 1, 1, 1};

  //KEY_Scan2();  // 在中断中扫描效果很好
  for (i = 0; i < 4; ++i)
  {
    if (g_key_up_buf[i] != s_key_up_backup[i])
    {
      if (0 == s_key_up_backup[i])
      {
        switch (i + 1)
        {
          case 1: LED1 = !LED1; SendUsartByte(USART1, 0x01); break;
          case 2: LED2 = !LED2; SendUsartByte(USART1, 0x02); break;
          case 3: LED3 = !LED3; SendUsartByte(USART1, 0x03); break;
          case 4: LED4 = !LED4; SendUsartByte(USART1, 0x04); break;
          default: break; // 没有按键按下
        } // switch
      } // if
      s_key_up_backup[i] = g_key_up_buf[i];
    } // if
  } // for
} // KEY_Drive2()
