/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: i2c.h 
* 文件标识: 见配置管理计划书
* 摘    要: stm32, i2c对应接口与函数声明
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
#ifndef STM32_I2C_H_
#define STM32_I2C_H_

#include "sys.h"

// 选择使用USE_I2C1 or USE_I2C2
#define USE_I2C1
#if defined(USE_I2C1)
// 这里使用I2C1，即PB6: SCL和PB7: SDA口
// PB7设置输入、输出模式
#define I2C_SDA_IN()  {GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= 0x80000000;} 
#define I2C_SDA_OUT() {GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= 0x30000000;}
// 定义I2C的SCL, SDA对应引脚
#define GPIO_PIN_SCL (GPIO_Pin_6)
#define GPIO_PIN_SDA (GPIO_Pin_7)
#elif defined(USE_I2C2)
// 使用I2C2时，PB10: SCL和PB11: SDA
#define I2C_SDA_IN()  {GPIOB->CRH &= 0XFFFF0FFF; GPIOB->CRH |= 8<<12;}
#define I2C_SDA_OUT() {GPIOB->CRH &= 0XFFFF0FFF; GPIOB->CRH |= 3<<12;}
// 定义I2C的SCL, SDA对应引脚
#define GPIO_PIN_SCL (GPIO_Pin_10)
#define GPIO_PIN_SDA (GPIO_Pin_11)
#else 
#error "Neither USE_I2C1 nor USE_I2C2 was defined!!"
#endif  // if defined()

// IO操作
#define I2C_SCL PBout(6)  // PB6, SCL
#define I2C_SDA PBout(7)  // PB7, SDA
#define I2C_READ_SDA PBin(7)  // SDA输入

/*----- High layer function -----*/
// 初始化I2c
void I2C_GPIO_Init(void);
// I2C起始信号
void I2C_Start(void);
// I2C停止信号
void I2C_Stop(void);
// 等待应答信号到来
u8 I2C_Wait_Ack(void);
// 产生ACK应答
void I2C_Ack(void);
// 不产生ACK应答        
void I2C_NAck(void);
// 向I2C总线发送一个字节      
void I2C_Send_Byte(u8 dat);
// 从I2C总线读1个字节  
u8 I2C_Read_Byte(u8 ack);

/*----- Medium layer function -----*/

/*----- Low layer function -----*/

#endif  // #ifndef STM32_I2C_H_
