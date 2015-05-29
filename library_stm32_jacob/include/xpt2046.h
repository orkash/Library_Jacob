/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: xpt2046.h
* 文件标识: 见配置管理计划书
* 摘    要: 2.4寸TFT触摸屏控制器XPT2046驱动, 使用模拟SPI控制
* 函    数: TOUCH_WriteByte() - 向触摸屏IC写入1个字节的数据, 使用SPI写入
*           TOUCH_ReadAD() - 从触摸屏IC读取ADC数据, 使用SPI读取
*           TOUCH_ReadXorY() - 读取x or y坐标值
*           TOUCH_ReadXandY() - 同时读取x, y坐标的值
*           TOUCH_ReadXandYTwice() - 连续读取2次触摸屏IC
*           TOUCH_DrawCalibratePoint() - 绘制校准点
*           TOUCH_Scan() - 触摸屏是否有点击或按下事件扫描
*           TOUCH_SaveCalibrateData() - 保存触摸屏校准参数到AT24C02
*           TOUCH_DisplayCaliInfo() - 显示校准参数
*           TOUCH_ReadCalibrateData() - 从AT24C02读取触摸屏校准参数
*           TOUCH_Calibrate() - 校准屏幕, 获取4个校准点的参数, 并保存校准参数
*           TOUCH_Init() - 触摸屏初始化
*           TOUCH_Paint() - 2*2点大小绘图
* 
* 当前版本: A0.0.3
* 作    者: Jacob Peng
* 完成日期: 2015年5月14日
* 更改内容: A0.0.1, 完成了基本初始化功能, 2015年5月7日
*           A0.0.2, 发现24C02占用XPT2046两个控制端口, 故改用stm32内部flash进行
*                   存储, 2015年5月13日
*           A0.0.3, 触摸屏校准后存在偏移情况, 暂时无解, 2015年5月14日
* 
* 取代版本: A0.0.2
* 原作者  : Jacob Peng
* 完成日期: 2015年5月13日
*/
#ifndef TOUCH_H_
#define TOUCH_H_

#include <stm32f10x.h>

// 触摸屏结构体
typedef struct tagTOUCH_TypeDef
{
  // // 触摸屏相关函数的指针
  // u8 (*Init)(void); // 初始化触摸屏控制器
  // u8 (*Scan)(u8); // 扫描触摸屏
  // void (*Calibrate)(void);  // 触摸屏校准
  // 原点坐标(第一次按下时的坐标)
  u16 x_origin; // 可能是物理坐标, 0 ~ 4095, 12位AD 
  u16 y_origin; 
  // 当前坐标(当前扫描时, 触摸屏的坐标)
  u16 x_now;  // 可能是物理坐标, 0 ~ 4095, 12位AD   
  u16 y_now;
  // 触摸笔的状态: 0, 松开; 1, 按下
  u8 pen_status;  
  // 触摸屏校准参数: fac, 系数; off, 余数
  float xfac; 
  float yfac;
  short xoff;
  short yoff;
} TOUCH_TypeDef;  // typedef struct tagTOUCH_TypeDef

// CS, CLK, PEN, IN, OUT控制引脚RCC时钟
#define TOUCH_CS_RCC    RCC_APB2Periph_GPIOB
#define TOUCH_CLK_RCC   RCC_APB2Periph_GPIOA
#define TOUCH_PEN_RCC   RCC_APB2Periph_GPIOB
#define TOUCH_IN_RCC    RCC_APB2Periph_GPIOB
#define TOUCH_OUT_RCC   RCC_APB2Periph_GPIOB
// CS, CLK, PEN, IN, OUT控制引脚GPIOx
#define TOUCH_CS_GPIO   GPIOB
#define TOUCH_CLK_GPIO  GPIOA
#define TOUCH_PEN_GPIO  GPIOB
#define TOUCH_IN_GPIO   GPIOB
#define TOUCH_OUT_GPIO  GPIOB
// CS, CLK, PEN, IN, OUT控制引脚对应IO口编号
#define TOUCH_CS_PIN  GPIO_Pin_1
#define TOUCH_CLK_PIN GPIO_Pin_8 
#define TOUCH_PEN_PIN GPIO_Pin_7
#define TOUCH_IN_PIN  GPIO_Pin_5
#define TOUCH_OUT_PIN GPIO_Pin_6
// 触摸屏芯片连接引脚
#define TOUCH_CS_SET  (TOUCH_CS_GPIO->BSRR = TOUCH_CS_PIN)    // 触摸屏片选
#define TOUCH_CS_CLR  (TOUCH_CS_GPIO->BRR = TOUCH_CS_PIN)   
#define TOUCH_CLK_SET (TOUCH_CLK_GPIO->BSRR = TOUCH_CLK_PIN)  // 触摸屏时钟
#define TOUCH_CLK_CLR (TOUCH_CLK_GPIO->BRR = TOUCH_CLK_PIN)
#define TOUCH_PEN_SET (TOUCH_PEN_GPIO->BSRR = TOUCH_PEN_PIN)  // 触摸屏中断, 触摸笔
#define TOUCH_PEN_CLR (TOUCH_PEN_GPIO->BRR = TOUCH_PEN_PIN)
#define TOUCH_IN_SET  (TOUCH_IN_GPIO->BSRR = TOUCH_IN_PIN)    // 触摸屏数据输入
#define TOUCH_IN_CLR  (TOUCH_IN_GPIO->BRR = TOUCH_IN_PIN)
#define TOUCH_OUT_SET (TOUCH_OUT_GPIO->BSRR = TOUCH_OUT_PIN)  // 触摸屏数据输出
#define TOUCH_OUT_CLR (TOUCH_OUT_GPIO->BRR = TOUCH_OUT_PIN) 
#define TOUCH_OUT     (GPIO_ReadInputDataBit(TOUCH_OUT_GPIO, TOUCH_OUT_PIN))
#define TOUCH_PEN     (GPIO_ReadInputDataBit(TOUCH_PEN_GPIO, TOUCH_PEN_PIN))
// 触摸屏校准相关标志
#define TOUCH_PRESS_DOWN  0x80  // 触摸屏被按下
#define TOUCH_BUTTON_DOWN 0x40  // 有屏幕点击事件发生
#define TOUCH_CALIBRATED  0x0A  // 屏幕已经被校准标志

/*----- High layer function -----*/
// 从AT24C02读取触摸屏校准参数
u8 TOUCH_ReadCalibrateData(void);
// 校准屏幕, 获取4个校准点的参数, 并保存校准参数
void TOUCH_Calibrate(void);
// 触摸屏初始化
void TOUCH_Init(void);
// 2*2点大小绘图
void TOUCH_Paint(u8 x, u16 y, u16 color);

/*----- Medium layer function -----*/
// 读取x or y坐标值
u16 TOUCH_ReadXorY(u8 cmd);
// 同时读取x, y坐标的值
u8 TOUCH_ReadXandY(u16 *x, u16 *y);
// 连续读取2次触摸屏IC
u8 TOUCH_ReadXandYTwice(u16 *x, u16 *y);
// 绘制校准点
void TOUCH_DrawCalibratePoint(u8 x, u16 y, u16 color);
// 触摸屏是否有点击或按下事件扫描
u8 TOUCH_Scan(u8 mode);
// 保存触摸屏校准参数到AT24C02
void TOUCH_SaveCalibrateData(void);
// 显示校准参数
void TOUCH_DisplayCaliInfo(u8 x0, u16 y0, u8 x1, u16 y1, 
                           u8 x2, u16 y2, u8 x3, u16 y3, u16 fac);

/*----- Low layer function -----*/
// 向触摸屏IC写入1个字节的数据, 使用SPI写入
void TOUCH_WriteByte(u8 dat);
// 从触摸屏IC读取ADC数据, 使用SPI读取
u16 TOUCH_ReadAD(u8 cmd);

#endif  // #ifndef TOUCH_H_
