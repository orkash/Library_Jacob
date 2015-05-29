/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: display.c  
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
#include "../include/display.h"

#if ENABLE_IN_51
#include "../include/uart.h"
#endif // #if ENABLE_IN_51

#if ENABLE_IN_WIN
#include <stdio.h>
#endif // #if ENABLE_IN_WIN

/************************************************ 
* 函  数: ConvertDataTo10Bits(unsigned char *, int)
* 功  能: 将数据拆分为6个ASCII格式数据
*
* 参  数: display_ptr - 转换缓冲区
*         displayed_data - 待转换数据
*
* 返回值: 无
*************************************************/
void ConvertDataTo10Bits(unsigned char *display_ptr, int displayed_data)
{
  // 最高位为符号位
	if (displayed_data < 0)
	{
		displayed_data = - displayed_data;  // 将数据转换为正整数
		*display_ptr = '-'; // ASCII 0x2D
	} else {
    *display_ptr = '+';  // ASCII 0x2B
  } // end of else ().
  // 数据万位
	*++display_ptr = displayed_data/10000;
  // 数据千位
	displayed_data = displayed_data%10000;  // 取余运算
	*++display_ptr = displayed_data/1000;
  // 数据百位
	displayed_data = displayed_data%1000; // 取余运算
	*++display_ptr = displayed_data/100;
  // 数据十位
	displayed_data = displayed_data%100;  // 取余运算
	*++display_ptr = displayed_data/10;
  // 数据个位
	displayed_data = displayed_data%10; // 取余运算
	*++display_ptr = displayed_data; 	
} // end of ConvertDataTo10Bits().

/************************************************ 
* 函  数: Display10BitsData(int)
* 功  能: 将数据每一位拆分为ASCII显示
*
* 参  数: displayed_data - 待显示数据
*
* 返回值: 无
*************************************************/
#if ENABLE_DISPLAY  // 是否使用Display10BitsData()函数
void Display10BitsData(int displayed_data)
{  
  unsigned char display_buf[6] = {0}; // 数据转换缓冲区

	ConvertDataTo10Bits(display_buf, displayed_data);  // 转换数据显示
  // 6个10位数据, 最高位为符号位
#if ENABLE_IN_51  
  UART_SendString(display_buf, 6);
#endif // #if ENABLE_IN_51

#if ENABLE_IN_WIN
  for (int i = 0; i < 6; ++i)
  {
    printf("%d ", display_buf[i]);
  } // for
  printf("\n");
#endif // #if ENABLE_IN_WIN
} // end of Display10BitsData().
#endif  // #if ENABLE_DISPLAY
