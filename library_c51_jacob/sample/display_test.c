/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: display_test.c  
* 文件标识: 见配置管理计划书
* 摘    要: display测试函数, 标准ANSI C环境即可, 比如C51
* 函    数: main() - display调用例程
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月7日
* 更改内容: E0.0.1, 实现display调用例程, 2015年4月7日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/

// 必须包含display.h头文件, 包含路径可根据display.h位置修改
#include "../include/display.h" 

// 未使用ConvertDataTo10Bits()函数显示,
// 需自己包含标准输出头文件。 
#if !ENABLE_DISPLAY	

#if ENABLE_IN_51
#include "../include/uart.h"
#endif	// #if ENABLE_IN_51
#if ENABLE_IN_WIN
#include <stdio.h>
#endif	// #if ENABLE_IN_WIN

#endif	// #if !ENABLE_DISPLAY

/************************************************ 
* 说  明: 必须在"display.h"文件中, 确认在C51下使用, #define USED_IN_C51, 
*         还是在Wondows下使用, #define USED_IN_WIN
* 函  数: Display10BitsData() - 将数据每一位拆分为ASCII显示
* void ConvertDataTo10Bits(unsigned char *display_ptr, int displayed_data)
* void Display10BitsData(int displayed_data), 限C51
*
* 参  数: displayed_data - 待显示数据
*
*返回值 : 无
*************************************************/

int main(void)
{
  unsigned int i = 0;
  int displayed_data = 4567;

#if ENABLE_DISPLAY  // 是否使用Display10BitsData()函数 
  // 调用Display10BitsData()直接显示, 最高位为符号位
  Display10BitsData(displayed_data);
#endif  // #if ENABLE_DISPLAY

  /*仅调用转换函数ConvertDataTo10Bits()*/
/*   unsigned char display_buf[6] = {0}; // 数据转换缓冲区
	ConvertDataTo10Bits(display_buf, displayed_data);  // 转换数据显示
  // 自定义显示输出
  // Windows输出
#if ENABLE_IN_WIN
  for (i = 0; i < 6; ++i)
  {
    printf("%d ", display_buf[i]);
  } //
  printf("\n");
#endif  // #if ENABLE_IN_WIN

  // C51串口输出, 6个10位数据
#if ENABLE_IN_51 
  UART_SendString(display_buf, 6);
#endif  // #if ENABLE_IN_51  */

  return 0;
} // main().
