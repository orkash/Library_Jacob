/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: spi_hardware.h
* 文件标识: 见配置管理计划书
* 摘    要: 使用stm32硬件SPI, 配置成主机模式, 访问SD card. 包含硬件SPI相应IO口声明.
*           硬件SPI接口速度最大18Mbit/s, 实际速度5Mbit/s左右, 模拟SPI速度较慢.
*           访问flash/sd卡用硬件SPI, 读取屏幕信息使用模拟SPI即可.
* 函    数: SPI1_Init() - 硬件SPI1初始化
*           SPI1_SetSpeed() - 硬件SPI速度设置
*           SPI1_ReadWriteCmd() - 写入命令, 读取返回信息
* 
* 当前版本: A0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年5月19日
* 更改内容: A0.0.1, 完成硬件SPI初始化、速度设定等3个函数, 2015年5月19日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#ifndef STM32_SPI_HARDWARE_H_
#define STM32_SPI_HARDWARE_H_

#include "../include/sys.h"

// SD卡对应SPI1连接
#define SD_SPI1_CS  PAout(4)
#define SD_SPI1_SCK PAout(5)
#define SD_SPI1_OUT PAout(6)
#define SD_SPI1_IN  PAout(7)
// SPI1所有IO口, 具体根据芯片型号而定
#define SD_SPI1_CS_PIN  GPIO_Pin_4
#define SD_SPI1_SCK_PIN GPIO_Pin_5
#define SD_SPI1_OUT_PIN GPIO_Pin_6
#define SD_SPI1_IN_PIN  GPIO_Pin_7

/*----- High layer function -----*/
// 硬件SPI1初始化, 配置成主机模式, 访问SD card.
void SPI1_Init(void);
// 硬件SPI速度设置
void SPI1_SetSpeed(u8 spi_prescaler);
// 写入命令, 读取返回信息
u8 SPI1_ReadWriteCmd(u8 tx_data);

/*----- Medium layer function -----*/

/*----- Low layer function -----*/

#endif  // #ifndef STM32_SPI_HARDWARE_H_
