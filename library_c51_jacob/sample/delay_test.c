/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: delay_test.c  
* 文件标识: 见配置管理计划书
* 摘    要: delay测试函数, 标准ANSI C环境即可, 比如C51
* 函    数: main() - delay调用例程
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月7日
* 更改内容: E0.0.1, 实现delay调用例程, 2015年4月7日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/

// 必须包含delay.h头文件, 包含路径可根据delay.h位置修改
#include "../include/delay.h" 

#include <reg51.h> 

/************************************************ 
* 说  明: 调用crc说明
* 函  数: DelayUs() - us延时
*         DelayMs() - ms延时
* void DelayUs(void)
* void DelayMs(unsigned int ms)
*
* 参  数: ms - 延时ms数
*
*返回值 : 无
*************************************************/

int main(void)
{
  DelayUs();
  
  while (1)
  {
    DelayMs(100);
    /*code here*/
  }; // while 

  return 0;
} // main().
