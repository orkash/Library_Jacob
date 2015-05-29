/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: delay.c 
* 文件标识: 见配置管理计划书
* 摘    要: C51粗略延时函数的实现
* 函    数: DelayUs() - us延时
*           DelayMs() - ms延时
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年2月28日
* 更改内容: E0.0.1, 完成C51粗略延时函数, 2015年2月28日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include "../include/delay.h"

#include <intrins.h>  // call _nop_()

/************************************************ 
* 函  数: DelayUs(void)
* 功  能: us短延时
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
#if USE_DELAYUS
void DelayUs(void)
{
  _nop_();
  _nop_();
  _nop_();
  _nop_();
} // end of DelayUs().
#endif  // #if USE_DELAYUS

/************************************************ 
* 函  数: DelayMs(unsigned int)
* 功  能: ms延时
*
* 参  数: ms - 延时ms数
*
* 返回值: 无
*************************************************/
#if USE_DELAYMS
void DelayMs(unsigned int ms)
{
  unsigned int count0;

  while (ms--)
  {
    for (count0 = 0; count0 < 125; ++count0)  // 延时约1ms
      continue;
  } // end of while ().
} // end of DelayMs().
#endif  // #if USE_DELAYMS
