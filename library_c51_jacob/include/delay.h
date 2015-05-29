/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: delay.h
* 文件标识: 见配置管理计划书
* 摘    要: C51粗略延时函数的声明
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
#ifndef DELAY_H_
#define DELAY_H_

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // end of #ifndef

// 是否使用us延时函数与ms延时函数,
// 可以消除编译器未使用某一个函数的警告。
#define USE_DELAYUS TRUE
#define USE_DELAYMS TRUE

#if USE_DELAYUS
// _nop_()短延时
void DelayUs(void);
#endif  // #if USE_DELAYUS

#if USE_DELAYMS
// ms延时
void DelayMs(unsigned int ms);
#endif  // #if USE_DELAYMS

#endif  // end of #ifndef DELAY_H_