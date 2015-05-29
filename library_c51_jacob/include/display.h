/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: display.h
* 文件标识: 见配置管理计划书
* 摘    要: 将数据每一位拆分为ASCII显示
* 函    数: ConvertDataTo10Bits() - 将数据拆分为6个ASCII格式数据，不足补0
*           Display10BitsData() - 通过串口显示6个ASCII格式数据, 最高位为符号位
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年3月2日
* 更改内容: E0.0.1, 完成拆分为10进制ASCII显示, 2015年3月2日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#ifndef DISPLAY_H_
#define DISPLAY_H_

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // end of #ifndef

// 是否使用Display10BitsData()函数, 
// 可以消除编译器未使用某一个函数的警告。
#define ENABLE_DISPLAY TRUE

// 在C51上使用还是Windows上使用
// USED_IN_C51, USED_IN_WIN
#define USED_IN_WIN

#if defined(USED_IN_C51)
#define ENABLE_IN_51 TRUE
#define ENABLE_IN_WIN FALSE
#elif defined(USED_IN_WIN)
#define ENABLE_IN_51 FALSE
#define ENABLE_IN_WIN TRUE
#else
#error "Neither USED_IN_C51 nor USED_IN_WIN was defined!!"
#endif  // #if defined()

// 将数据拆分为6个10位格式显示
void ConvertDataTo10Bits(unsigned char *display_ptr, int displayed_data);
#if ENABLE_DISPLAY  // 是否使用Display10BitsData()函数
// 显示10位数据
void Display10BitsData(int displayed_data);
#endif // #if ENABLE_DISPLAY

#endif  // end of #ifndef DISPLAY_H_
