/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: i2c.c 
* 文件标识: 见配置管理计划书
* 摘    要: stm32, i2c相应操作
* 函    数: I2C_GPIO_Init() - 初始化I2C对应IO口
*           I2C_Start() - I2C起始信号
*           I2C_Stop() - I2C停止信号
*           I2C_Wait_Ack() - 等待应答信号到来
*           I2C_Ack() - 产生ACK应答
*           I2C_NAck() - 不产生ACK应答
*           I2C_Send_Byte() - 向I2C总线发送一个字节
*           I2C_Read_Byte() - 从I2C总线读1个字节
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年4月23日
* 更改内容: E0.0.1, 实现了I2C几个常用函数, 2015年4月23日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include "../include/i2c.h"

#include "../include/delay.h"

/************************************************ 
* 函  数: I2C_GPIO_Init(void)
* 功  能: 初始化I2C对应IO口
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void I2C_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 开启GPIOB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SCL | GPIO_PIN_SDA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB, GPIO_PIN_SCL | GPIO_PIN_SDA); // SCL与SDA均输出高电平
} // I2C_Init()

/************************************************ 
* 函  数: I2C_Start(void)
* 功  能: 产生I2C起始信号
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void I2C_Start(void)
{
  I2C_SDA_OUT();  // SDA线输出
  I2C_SDA = 1;  // 确保I2C_SCL, I2C_SDA均为高电平
  I2C_SCL = 1;
  DelayUs(2);
  I2C_SDA = 0;  // 先拉低I2C_SDA
  DelayUs(2);
  I2C_SCL = 0;  // 再拉低I2C_SCL
} // I2C_Start()

/************************************************ 
* 函  数: I2C_Stop(void)
* 功  能: 产生I2C停止信号
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void I2C_Stop(void)
{
  I2C_SDA_OUT();  // SDA线输出
  I2C_SCL = 0;  // 确保I2C_SCL, I2C_SDA均为低电平
  I2C_SDA = 0;
  DelayUs(2);
  I2C_SCL = 1;  // 先拉高I2C_SDA
  DelayUs(2);
  I2C_SDA = 1;  // 再拉高I2C_SCL
  DelayUs(2);  
} // I2C_Stop()

/************************************************ 
* 函  数: I2C_Wait_Ack(void)
* 功  能: 等待应答信号到来
*
* 参  数: 无
*
* 返回值: 1, 接收应答失败; 0, 接收应答成功
*************************************************/
u8 I2C_Wait_Ack(void)
{
  u8 error_times = 0;
  
  I2C_SDA_IN(); // SDA设置为输入  
  I2C_SDA = 1;
  DelayUs(2);     
  I2C_SCL = 1;
  DelayUs(2);  
  while (I2C_READ_SDA)
  {
    ++error_times;
    if (error_times > 250)
    {
      I2C_Stop();

      return 1;
    } // if
  } // while
  I2C_SCL = 0;  // 时钟输出0  

  return 0;  
} // I2C_Wait_Ack()

/************************************************ 
* 函  数: I2C_Ack(void)
* 功  能: 产生ACK应答
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void I2C_Ack(void)
{
  I2C_SDA_OUT();
  I2C_SCL = 0;
  I2C_SDA = 0;
  DelayUs(2);
  I2C_SCL = 1;
  DelayUs(2);
  I2C_SCL = 0;
} // I2C_Ack()

/************************************************ 
* 函  数: I2C_NAck(void)
* 功  能: 不产生ACK应答
*
* 参  数: 无
*
* 返回值: 无
*************************************************/      
void I2C_NAck(void)
{
  I2C_SDA_OUT();
  I2C_SCL = 0;
  I2C_SDA = 1;
  DelayUs(2);
  I2C_SCL = 1;
  DelayUs(2);
  I2C_SCL = 0;
} // I2C_NAck() 

/************************************************ 
* 函  数: I2C_Send_Byte(u8)
* 功  能: 向I2C总线发送一个字节
*
* 参  数: dat - 待发送的字节数据
*
* 返回值: 无
*************************************************/    
void I2C_Send_Byte(u8 dat)
{                        
  u8 i = 0;   

  I2C_SDA_OUT();      
  I2C_SCL = 0;  // 拉低时钟开始数据传输
  for (i = 0; i < 8; ++i)
  {              
    // I2C_SDA = (dat & 0x80) >> 7;
    if ((dat & 0x80) >> 7)
      I2C_SDA = 1;
    else
      I2C_SDA = 0;
    dat <<= 1;    
    I2C_SCL = 1;
    I2C_SCL = 0;
  } // for
} // I2C_Send_Byte()

/************************************************ 
* 函  数: I2C_Send_Byte(u8)
* 功  能: 从I2C总线读1个字节
*
* 参  数: ack - 是否需要发送ack应答: 1, 发送Ack; 0, 发送NAck
*
* 返回值: 从总线读取回来的字节数据
*************************************************/ 
u8 I2C_Read_Byte(u8 ack)
{
  unsigned char i = 0, ret_data = 0x00;

  I2C_SDA_IN(); // SDA设置为输入
  for (i = 0; i < 8; ++i)
  {
    I2C_SCL=0; 
    DelayUs(2);
    I2C_SCL=1;
    ret_data<<=1;
    if (I2C_READ_SDA) ret_data++;   
    DelayUs(2); 
  } // for     
  if (!ack)
      I2C_NAck(); // 发送noACK
  else
      I2C_Ack();  // 发送ACK   
  return ret_data;
} // I2C_Read_Byte()
