/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: spi_hardware.c
* 文件标识: 见配置管理计划书
* 摘    要: 使用stm32硬件SPI, 配置成主机模式, 访问SD card. 
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
#include "../include/spi_hardware.h"

#include <stdio.h>
#include <stm32f10x.h>
#include <stm32f10x_spi.h>

/************************************************ 
* 函  数: SPI1_Init(void)
* 功  能: 硬件SPI1初始化, 配置成主机模式, 访问SD card
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void SPI1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // 这里使用硬件SPI, 所以必须初始化SPI, 否则只需初始化IO口即可
  SPI_InitTypeDef SPI_InitStructure;

  // 使能SPI1相应时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
  
  // 这里SD_SPI1_OUT_PIN必须使用复用功能
  GPIO_InitStructure.GPIO_Pin = SD_SPI1_OUT_PIN | SD_SPI1_SCK_PIN | SD_SPI1_IN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SD_SPI1_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  // Start SPI1.
  GPIO_SetBits(GPIOA, SD_SPI1_CS_PIN | SD_SPI1_SCK_PIN | SD_SPI1_OUT_PIN | SD_SPI1_IN_PIN);
  
  // 双线双向全双工
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // 主机模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 8位数据帧
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; // 时钟悬空高
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  // 数据捕获于第2个时钟沿
  // CS片选型号由软件控制, 内部硬件有NSS信号SSI位控制
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
  // 256分频, 72M模式下, 为280KHz左右(对于某些SD卡, <400KHz即可), 对于某些SD卡初始化也会太快;
  // 故, 最好降低AHB总线速度, 进行2分频, 36M模式, 140KHz足够了, RCC_PCLK2Config(RCC_HCLK_Div2);
  // 如果仍然不行, 进行4分频, 18M模式, 70KHz总不快了吧?!
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;  // 256分频
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  // 数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7;  // CRC计算多项式
  SPI_Init(SPI1, &SPI_InitStructure);
  // 使能SPI外设
  SPI_Cmd(SPI1, ENABLE);
  // 启动传输
  SPI1_ReadWriteCmd(0xFF); 
} // SPI1_Init()

/************************************************ 
* 函  数: SPI1_SetSpeed(u8)
* 功  能: 硬件SPI速度设置
*
* 参  数: spi_prescaler - 预分频值
*         SPI_BaudRatePrescaler_2   2分频   (SPI 36MHz,     system 72MHz)
*         SPI_BaudRatePrescaler_8   8分频   (SPI 9MHz,      system 72MHz)
*         SPI_BaudRatePrescaler_16  16分频  (SPI 4.5MHz,    system 72MHz)
*         SPI_BaudRatePrescaler_256 256分频 (SPI 281.25KHz, system 72MHz)
*
* 返回值: 无
*************************************************/
void SPI1_SetSpeed(u8 spi_prescaler)
{
  SPI_InitTypeDef SPI_InitStructure;
  
  // 双线双向全双工
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // 主机模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 8位数据帧
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; // 时钟悬空高
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  // 数据捕获于第2个时钟沿
  // CS片选型号由软件控制, 内部硬件有NSS信号SSI位控制
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   
  SPI_InitStructure.SPI_BaudRatePrescaler = spi_prescaler;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  // 数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7;  // CRC计算多项式  
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_Cmd(SPI1, ENABLE);
} // SPI1_SetSpeed()

/************************************************ 
* 函  数: SPI1_ReadWriteCmd(u8)
* 功  能: 写入命令, 读取返回信息
*
* 参  数: cmd - 待写入的命令
*
* 返回值: 读取到的返回信息数据
*************************************************/
u8 SPI1_ReadWriteCmd(u8 cmd)
{
  u8 retry_times = 0;  // 重试等待次数
  
  // 检查发送缓冲空标志位
  while (RESET == SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
  {
    // 这里死循环了?不知道为什么
    ++retry_times;
    if (retry_times >= 200) 
    {
      //printf("cmd = %d, retry_times = %d.\r\n", cmd, retry_times);
      return 0;
    }
  } // while
  SPI_I2S_SendData(SPI1, cmd);  // 通过SPI1发送一个数据
  retry_times = 0;
  // 检查接收缓冲区非空标志位
  while (RESET == SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
  {
    ++retry_times;
    if (retry_times >= 200) return 0;
  } // while

  return SPI_I2S_ReceiveData(SPI1); // 返回SPI1最近接收的数据
} // SPI1_ReadWriteCmd()
