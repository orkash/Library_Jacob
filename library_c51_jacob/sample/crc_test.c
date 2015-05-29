/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: test_crc.c  
* 文件标识: 见配置管理计划书
* 摘    要: crc测试函数, 标准ANSI C环境即可, 比如dos
* 函    数: main() - crc调用例程
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月7日
* 更改内容: E0.0.1, 实现crc调用例程, 2015年4月7日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/

// 必须包含crc.h头文件, 包含路径可根据crc.h位置修改
#include "../include/crc.h" 

#include <stdio.h>
#include <string.h> // Declare strlen().

/************************************************ 
* 说  明: 调用crc说明
* 函  数: CheckCrcSlowly() - 按位进行crc校验, 不推荐
*         CheckCrcFaster() - 按字节进行crc校验, 速度更快，推荐
* Crc CheckCrcSlowly(const unsigned char message[], unsigned int message_bytes)
* Crc CheckCrcFaster(const unsigned char message[], unsigned int message_bytes)
*
* 参  数: message - 开始校验的数据地址, 一般从头开始
*         message_bytes - 需要校验的数据字节数, 需要自己指定
*
*返回值 : 计算完成的crc校验值
*************************************************/

int main(void)
{
  unsigned char  test_message[] = "123456789";
  
  // Compute the CRC of the test message, slowly.
  // 必须使能头文件里的USE_BIT_CRC
  printf("The CheckCrcSlowly() of \"123456789\" is 0x%X.\n", 
          CheckCrcSlowly(test_message, strlen(test_message)));
  // Compute the CRC of the test message, more efficiently.
  // 必须使能头文件里的USE_BYTE_CRC
  printf("The CheckCrcFaster() of \"123456789\" is 0x%X.\n", 
          CheckCrcFaster(test_message, strlen(test_message)));

  return 0;
} // main().
