/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: ili9325.h 
* 文件标识: 见配置管理计划书
* 摘    要: LCD显示屏ILI9325或者ILI9328驱动，这里使用了16位数据模式, 240*320
* 函    数: ILI9325_GPIO_Init() - ILI9325控制器对应IO口配置
*           LCD_Init() - LCD初始化
*           LCD_SetCursor() - 设定光标位置
*           LCD_SetDisplayWindow() - 设定窗口大小
*           LCD_Clear() - 填充屏幕为一种颜色
*           LCD_DisplayOn() - 开启或关闭LCD显示
*           LCD_ReadPointColor() - 读取指定坐标颜色值
*           LCD_DrawPoint() - 绘制一个点
*           LCD_DisplayChar() - 在指定坐标显示一个8x16点阵的ascii字符
*           GetNumberBits() - 判断数字的有效位数
*           LCD_DisplayNumber() - 在指定坐标显示数字
*           LCD_DisplayStringLine() - 在指定座标显示字符串
*           LCD_DisplayStringMulLines() - 在指定座标显示多行字符串
*           LCD_DisplayMultipleLines() - 多行显示字符串, 超出屏幕宽度部分会自动换行显示
*           LCD_DisplaySingleLine() - 显示单行字符串
*           LCD_ClearSingleLine() - 清除单行内容
*           LCD_ClearMultipleLines() - 清除多行内容
*           LCD_DrawBMP() - 在指定坐标范围显示一幅图片
*           LCD_Test() - 显示彩条, 测试液晶屏是否正常工作
*           LCD_BGR2RGB() - 将BGR颜色转换成RGB
*           ILI9325_Write() - ILI9325写指令或数据
*           ILI9325_WriteCmd() - 向LCD写指令
*           ILI9325_WriteData() - LCD写数据
*           LCD_WriteRegister() - 写指定地址寄存器的值
*           ILI9325_Read() - 读取ILI9325数据线上的数据
*           ILI9325_ReadData() - 读取LCD数据线上的数据
*           LCD_ReadRegister() - 读取指定地址寄存器的值
*           LCD_RGB2Color() - 将RGB颜色转换为16位颜色
*           LCD_DrawLine() - 根据起止点坐标绘制线段
*           LCD_DrawHorizontalLine() - 绘制水平线段
*           LCD_DrawVerticallLine() - 绘制垂直线段
*           LCD_DrawQuadrangle() - 绘制四边形
*           LCD_DrawRectangle() - 绘制矩形, 可填充颜色
*           LCD_DrawSquare() - 绘制正方形, 可填充颜色
*           LCD_DrawCircle() - 绘制圆, 可填充颜色
* 
* 当前版本: A0.0.5
* 作    者: Jacob Peng
* 完成日期: 2015年5月8日
* 更改内容: A0.0.1, 完成了基本ILI9325的初始化, 2015年4月29日
*           A0.0.2, 增加了ASCII字符和字符串显示, 2015年5月5日
*           A0.0.3, 增加了数字的显示以及线段、矩形、正方形、圆的绘制, 2015年5月6日
*           A0.0.4, 增加了自动换行显示字符串、清除多行内容函数, 2015年5月7日
*           A0.0.5, 多行显示函数增加'\n'换行和单词被拆分时连字符'-', 2015年5月8日
* 
* 取代版本: A0.0.4
* 原作者  : Jacob Peng
* 完成日期: 2015年5月7日
*/
#ifndef ILI9325_H_
#define ILI9325_H_

#include <stm32f10x.h>

// 数据接口引脚RCC时钟
#define LCD_DB0_7_RCC   RCC_APB2Periph_GPIOB
#define LCD_DB8_15_RCC  RCC_APB2Periph_GPIOC
// 数据接口GPIOx
#define LCD_DB0_7_GPIO  GPIOB
#define LCD_DB8_15_GPIO GPIOC
// CS, RS, WR, RD, RST控制引脚RCC时钟
#define LCD_CS_RCC  RCC_APB2Periph_GPIOC
#define LCD_RS_RCC  RCC_APB2Periph_GPIOC
#define LCD_WR_RCC  RCC_APB2Periph_GPIOC
#define LCD_RD_RCC  RCC_APB2Periph_GPIOC
#define LCD_RST_RCC RCC_APB2Periph_GPIOC
// CS, RS, WR, RD, RST控制引脚GPIOx
#define LCD_CS_GPIO   GPIOC
#define LCD_RS_GPIO   GPIOC
#define LCD_WR_GPIO   GPIOC
#define LCD_RD_GPIO   GPIOC
#define LCD_RST_GPIO	GPIOC
// CS, RS, WR, RD, RST控制引脚对应IO口编号
#define LCD_CS_PIN  GPIO_Pin_11 // LCD片选信号
#define LCD_RS_PIN  GPIO_Pin_8  // 命令/数据标志(0, 读写命令; 1, 读写数据)
#define LCD_WR_PIN  GPIO_Pin_9  // 向LCD写入数据
#define LCD_RD_PIN  GPIO_Pin_10 // 从LCD读取数据
#define LCD_RST_PIN GPIO_Pin_12 // LCD复位信号, 复位信号最好直接连接stm32复位引脚
// CS, RS, WR, RD, RST置1或清零
#define LCD_CS_SET  (LCD_CS_GPIO->BSRR = LCD_CS_PIN)    // GPIO_SetBits   (LCD_CS_GPIO, LCD_CS_PIN)
#define LCD_CS_CLR  (LCD_CS_GPIO->BRR = LCD_CS_PIN)     // GPIO_ResetBits (LCD_CS_GPIO, LCD_CS_PIN)
#define LCD_RS_SET  (LCD_RS_GPIO->BSRR = LCD_RS_PIN)    // GPIO_SetBits   (LCD_RS_GPIO, LCD_RS_PIN) 
#define LCD_RS_CLR  (LCD_RS_GPIO->BRR = LCD_RS_PIN)     // GPIO_ResetBits (LCD_RS_GPIO, LCD_RS_PIN)
#define LCD_WR_SET  (LCD_WR_GPIO->BSRR = LCD_WR_PIN)    // GPIO_SetBits   (LCD_WR_GPIO, LCD_WR_PIN)	
#define LCD_WR_CLR  (LCD_WR_GPIO->BRR = LCD_WR_PIN)     // GPIO_ResetBits (LCD_WR_GPIO, LCD_WR_PIN)
#define LCD_RD_SET  (LCD_RD_GPIO->BSRR = LCD_RD_PIN)    // GPIO_SetBits   (LCD_RD_GPIO, LCD_RD_PIN) 
#define LCD_RD_CLR  (LCD_RD_GPIO->BRR = LCD_RD_PIN)     // GPIO_ResetBits (LCD_RD_GPIO, LCD_RD_PIN)
#define LCD_RST_SET (LCD_RST_GPIO->BSRR = LCD_RST_PIN)  // GPIO_SetBits   (LCD_RST_GPIO, LCD_RST_PIN)
#define LCD_RST_CLR (LCD_RST_GPIO->BRR = LCD_RST_PIN)   // GPIO_ResetBits (LCD_RST_GPIO, LCD_RST_PIN)
// LCD registers.
#define R0    0x00
#define R1    0x01
#define R2    0x02
#define R3    0x03
#define R4    0x04
#define R5    0x05
#define R6    0x06
#define R7    0x07
#define R8    0x08
#define R9    0x09
#define R10   0x0A
#define R12   0x0C
#define R13   0x0D
#define R14   0x0E
#define R15   0x0F
#define R16   0x10
#define R17   0x11
#define R18   0x12
#define R19   0x13
#define R20   0x14
#define R21   0x15
#define R22   0x16
#define R23   0x17
#define R24   0x18
#define R25   0x19
#define R26   0x1A
#define R27   0x1B
#define R28   0x1C
#define R29   0x1D
#define R30   0x1E
#define R31   0x1F
#define R32   0x20
#define R33   0x21
#define R34   0x22
#define R36   0x24
#define R37   0x25
#define R40   0x28
#define R41   0x29
#define R43   0x2B
#define R45   0x2D
#define R48   0x30
#define R49   0x31
#define R50   0x32
#define R51   0x33
#define R52   0x34
#define R53   0x35
#define R54   0x36
#define R55   0x37
#define R56   0x38
#define R57   0x39
#define R59   0x3B
#define R60   0x3C
#define R61   0x3D
#define R62   0x3E
#define R63   0x3F
#define R64   0x40
#define R65   0x41
#define R66   0x42
#define R67   0x43
#define R68   0x44
#define R69   0x45
#define R70   0x46
#define R71   0x47
#define R72   0x48
#define R73   0x49
#define R74   0x4A
#define R75   0x4B
#define R76   0x4C
#define R77   0x4D
#define R78   0x4E
#define R79   0x4F
#define R80   0x50
#define R81   0x51
#define R82   0x52
#define R83   0x53
#define R96   0x60
#define R97   0x61
#define R106  0x6A
#define R118  0x76
#define R128  0x80
#define R129  0x81
#define R130  0x82
#define R131  0x83
#define R132  0x84
#define R133  0x85
#define R134  0x86
#define R135  0x87
#define R136  0x88
#define R137  0x89
#define R139  0x8B
#define R140  0x8C
#define R141  0x8D
#define R143  0x8F
#define R144  0x90
#define R145  0x91
#define R146  0x92
#define R147  0x93
#define R148  0x94
#define R149  0x95
#define R150  0x96
#define R151  0x97
#define R152  0x98
#define R153  0x99
#define R154  0x9A
#define R157  0x9D
#define R192  0xC0
#define R193  0xC1
#define R229  0xE5
// 屏幕实际宽高
#define LCD_WIDTH   240
#define LCD_HEIGHT  320
// 行高、行宽
#define LINE_WIDTH  8   // 字符宽度为8
#define LINE_HEIGHT 20  // 字符高度为16
// 屏幕行数, 320高 = 16行*20行高
#define LINE0   (0)
#define LINE1   (1*LINE_HEIGHT)
#define LINE2   (2*LINE_HEIGHT)
#define LINE3   (3*LINE_HEIGHT)
#define LINE4   (4*LINE_HEIGHT)
#define LINE5   (5*LINE_HEIGHT)
#define LINE6   (6*LINE_HEIGHT)
#define LINE7   (7*LINE_HEIGHT)
#define LINE8   (8*LINE_HEIGHT)
#define LINE9   (9*LINE_HEIGHT)
#define LINE10  (10*LINE_HEIGHT)
#define LINE11  (11*LINE_HEIGHT)
#define LINE12  (12*LINE_HEIGHT)
#define LINE13  (13*LINE_HEIGHT)
#define LINE14  (14*LINE_HEIGHT)
#define LINE15  (15*LINE_HEIGHT)
// 颜色
#define WHITE   0xFFFF
#define BLACK   0x0000
#define GREY    0xF7DE
#define BLUE    0x001F
#define BLUE2   0x051F
#define RED     0xF800
#define MAGENTA 0xF81F  // 品红, 洋红
#define GREEN   0x07E0  
#define CYAN    0x7FFF  // 青色
#define YELLOW  0xFFE0
// LCD文本颜色和背景色
#define LCD_TEXT_COLOR WHITE
#define LCD_BACK_COLOR BLACK
// 是否填充图形
#define FILL 1
#define NOT_FILL 0

/*----- High layer function -----*/
// LCD初始化
void LCD_Init(void);
// 显示彩条, 测试液晶屏是否正常工作
void LCD_Test(void);
// 将屏幕填充为一种颜色
void LCD_Clear(u16 color);
// 读取指定坐标颜色值
u16 LCD_ReadPointColor(u8 x, u16 y);
// 在指定坐标显示一个8x16点阵的ascii字符
void LCD_DisplayChar(u8 xpos, u16 ypos, char ch);
// 在指定坐标显示数字
void LCD_DisplayNumber(u8 x, u16 y, u32 num);
// 在指定座标显示字符串
void LCD_DisplayStringLine(u8 xpos, u16 ypos, const char str[]);
// 在指定座标显示多行字符串
void LCD_DisplayStringMulLines(u8 xpos, u16 ypos, u8 width, u16 height, const char str[]);
// 单行显示字符串, 超出屏幕宽度部分不会显示
void LCD_DisplaySingleLine(u16 line, const char str[]);
// 多行显示字符串, 超出屏幕宽度部分会自动换行显示
void LCD_DisplayMultipleLines(u16 line, const char str[]);
// 清除单行内容
void LCD_ClearSingleLine(u16 line);
// 清除多行内容
void LCD_ClearMultipleLines(u16 start_line, u8 lines);
// 绘制水平线段
void LCD_DrawHorizontalLine(u8 x0, u16 y0, u8 width, u16 color);
// 绘制垂直线段
void LCD_DrawVerticallLine(u8 x0, u16 y0, u16 height, u16 color);
// 绘制矩形, 可填充颜色
void LCD_DrawRectangle(u8 x0, u16 y0, u8 width, u16 height, u16 color, u8 fill);
// 绘制正方形, 可填充颜色
void LCD_DrawSquare(u8 x0, u16 y0, u16 side_lenght, u16 color, u8 fill);
// 绘制圆, 可填充颜色
void LCD_DrawCircle(u8 x_center, u16 y_center, u8 r, u16 color, u8 fill);
// 在指定坐标范围显示一幅图片
void LCD_DrawBMP(u8 start_x, u16 start_y, u8 end_x, u16 end_y, const u16 pic[]);

/*----- Medium layer function -----*/
// ILI9325控制器对应IO口配置
void ILI9325_GPIO_Init(void);
// 设定光标位置
void LCD_SetCursor(u8 x, u16 y);
// 设定显示窗口大小
void LCD_SetDisplayWindow(u8 xpos, u16 ypos, u8 width, u16 height);
// 开启或关闭LCD显示
void LCD_DisplayOn(u8 on_or_off);
// 将BGR颜色转换成RGB
u16 LCD_BGR2RGB(u16 color);
// 将RGB颜色转换为16位颜色
u16 LCD_RGB2Color(u32 rgb_value);
// 向指定寄存器写入一个值
void LCD_WriteRegister(u16 index, u16 dat);
// 读取指定地址寄存器的值
u16 LCD_ReadRegister(u16 index);
// 绘制一个点
void LCD_DrawPoint(u8 xpos, u16 ypos, u16 color);
// 根据起止点坐标绘制线段
void LCD_DrawLine(u8 x0, u16 y0, u8 x1, u16 y1, u16 color);
// 绘制四边形
void LCD_DrawQuadrangle(u8 x0, u16 y0, u8 x1, u16 y1, u16 color);

/*----- Low layer function -----*/
// 向ILI9325写指令或数据
void ILI9325_Write(u16 dat);
// LCD写指令
void ILI9325_WriteCmd(u16 cmd);
// LCD写数据
void ILI9325_WriteData(u16 dat);
// 读取ILI9325数据线上的数据
u16 ILI9325_Read(void);
// 读取LCD数据线上的数据
u16 ILI9325_ReadData(void);

//void Touch_Initializtion(void);
//u16  Touch_GetPhyX(void);
//u16  Touch_GetPhyY(void);
//u32  Touch_MeasurementX(void);
//u32  Touch_MeasurementY(void);

#endif  // #ifndef ILI9325_H_
