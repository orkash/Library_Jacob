/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: ili9325.c  
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
#include "../include/ili9325.h"

#include <stdio.h>  // Declare printf().
#include <math.h> // Declare pow().
#include <ctype.h>  // Declare isalpha().

#include "../include/ili9325_font.h"
#include "../include/delay.h"  // Declare DelayMs().

// LCD type definition.
#define LCD_ILI9325 0x9325
#define LCD_ILI9328 0x9328

/************************************************ 
* 函  数: LCD_Init(void)
* 功  能: LCD初始化
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void LCD_Init(void)
{ 
  u16 device_id = 0x0000;
  char id_buf[8] = {0};
  
  ILI9325_GPIO_Init();  // 配置ILI9325数据线与控制线IO口
  // 必须先复位ILI9325, 否则读取不了控制器型号。
  // 当ILI9325的复位引脚直接连接单片机复位引脚时，可以不使用这一句？
  LCD_RST_SET;
  LCD_WriteRegister(R0, 0x0001); // Start OCS
  //DelayMs(20);
  device_id = LCD_ReadRegister(R0); // 读控制器型号ILI9325
  // 该初始化只对ILI9325, ILI9328有效
  if (LCD_ILI9325 == device_id || LCD_ILI9328 == device_id)
  {
    LCD_WriteRegister(R1, 0x0100); // set SS and SM bit
    LCD_WriteRegister(R2, 0x0700); // set 1 line inversion
    LCD_WriteRegister(R3, 0x1030); // set GRAM write x_dir and BGR=1.
    LCD_WriteRegister(R4, 0x0000); // Resize register
    LCD_WriteRegister(R8, 0x0207); // set the back porch and front porch
    LCD_WriteRegister(R9, 0x0000); // set non-display area refresh cycle ISC[3:0]
    LCD_WriteRegister(R10, 0x0000); // FMARK function
    LCD_WriteRegister(R12, 0x0000); // RGB interface setting
    LCD_WriteRegister(R13, 0x0000); // Frame marker Position
    LCD_WriteRegister(R15, 0x0000); // RGB interface polarity
    // ----------- Power On sequence ----------//
    LCD_WriteRegister(R16, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_WriteRegister(R17, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
    LCD_WriteRegister(R18, 0x0000); // VREG1OUT voltage
    LCD_WriteRegister(R19, 0x0000); // VDV[4:0] for VCOM amplitude
    // DelayMs(20); // Dis-charge capacitor power voltage
    LCD_WriteRegister(R16, 0x1290); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_WriteRegister(R17, 0x0227); // Set DC1[2:0], DC0[2:0], VC[2:0]
    // DelayMs(50); // Delay 50ms
    LCD_WriteRegister(R18, 0x001D); // 0012
    // DelayMs(50); // Delay 50ms
    LCD_WriteRegister(R19, 0x1500); // VDV[4:0] for VCOM amplitude
    LCD_WriteRegister(R41, 0x0018); // 04  VCM[5:0] for VCOMH
    LCD_WriteRegister(R43, 0x000D); // Set Frame Rate
    // DelayMs(50); // Delay 50ms
    LCD_WriteRegister(R32, 0x0000); // GRAM horizontal Address
    LCD_WriteRegister(R33, 0x0000); // GRAM Vertical Address
    // ----------- Adjust the Gamma Curve ----------//
    LCD_WriteRegister(R48, 0x0004);
    LCD_WriteRegister(R49, 0x0307);
    LCD_WriteRegister(R50, 0x0002);
    LCD_WriteRegister(R53, 0x0206);
    LCD_WriteRegister(R54, 0x0408);
    LCD_WriteRegister(R55, 0x0507);
    LCD_WriteRegister(R56, 0x0204);
    LCD_WriteRegister(R57, 0x0707);
    LCD_WriteRegister(R60, 0x0405);
    LCD_WriteRegister(R61, 0x0F02);
    //------------------ Set GRAM area ---------------//
    LCD_WriteRegister(R80, 0x0000); // Horizontal GRAM Start Address, x_coordinate
    LCD_WriteRegister(R81, 0x00EF); // Horizontal GRAM End Address, 0-239
    LCD_WriteRegister(R82, 0x0000); // Vertical GRAM Start Address, y_coordinate
    LCD_WriteRegister(R83, 0x013F); // Vertical GRAM End Address, 0-319
    LCD_WriteRegister(R96, 0xA700); // Gate Scan Line
    LCD_WriteRegister(R97, 0x0001); // NDL,VLE, REV
    
    LCD_WriteRegister(R106, 0x0000); // set scrolling line
    //-------------- Partial Display Control ---------//
    LCD_WriteRegister(R128, 0x0000);
    LCD_WriteRegister(R129, 0x0000);
    LCD_WriteRegister(R130, 0x0000);
    LCD_WriteRegister(R131, 0x0000);
    LCD_WriteRegister(R132, 0x0000);
    LCD_WriteRegister(R133, 0x0000);
    //-------------- Panel Control -------------------//
    LCD_WriteRegister(R144, 0x0010); 
    LCD_WriteRegister(R146, 0x0600); 
    LCD_WriteRegister(R147, 0x0003); 
    LCD_WriteRegister(R149, 0x0110); 
    LCD_WriteRegister(R151, 0x0000); 
    LCD_WriteRegister(R152, 0x0000);
    LCD_DisplayOn(1); // Display on.
    //LCD_WriteRegister(R7, 0x0133);  // Display on.
    
    printf("The LCD controller is 0x%X.\r\n", device_id);
    sprintf(id_buf, "0x%X.", device_id);
    LCD_Clear(LCD_BACK_COLOR);  // 屏幕背景设置为黑色
    LCD_ClearSingleLine(LINE0); // 可消除刚上电第一行白条的问题
    LCD_DisplaySingleLine(LINE0, "Initialize successfully! ^_^");
    LCD_DisplaySingleLine(LINE1, "The LCD controller is "); 
    LCD_DisplayStringLine(176, LINE1, id_buf);
    LCD_DisplaySingleLine(LINE2, "==============================");
  } else {  // 不能识别的控制器型号
    printf("Sorry, the 0x%X LCD controller was unrecognized!\r\n", device_id);
  } // if else
  // DelayMs(20);
} // LCD_Init()

/************************************************ 
* 函  数: ILI9325_GPIO_Init(void)
* 功  能: ILI9325控制器对应IO口配置
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void ILI9325_GPIO_Init(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;

  //----------- 初始化数据引脚与控制引脚 ----------//
  RCC_APB2PeriphClockCmd(LCD_DB0_7_RCC | LCD_DB8_15_RCC, ENABLE);
  //----------- 低8位数据线 ----------//
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | 
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
  GPIO_Init(LCD_DB0_7_GPIO, &GPIO_InitStructure);
  //----------- 高8位数据线 ----------//
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | 
                                GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | 
                                GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(LCD_DB8_15_GPIO, &GPIO_InitStructure);
  //----------- 控制引脚 ----------//
  // 控制脚 CS
  RCC_APB2PeriphClockCmd(LCD_CS_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
  GPIO_Init(LCD_CS_GPIO, &GPIO_InitStructure);
  // 控制脚 RS
  RCC_APB2PeriphClockCmd(LCD_RS_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
  GPIO_Init(LCD_RS_GPIO, &GPIO_InitStructure);
  // 控制脚 WR 
  RCC_APB2PeriphClockCmd(LCD_WR_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = LCD_WR_PIN;
  GPIO_Init(LCD_WR_GPIO, &GPIO_InitStructure);
  // 控制脚 RD 
  RCC_APB2PeriphClockCmd(LCD_RD_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = LCD_RD_PIN;
  GPIO_Init(LCD_RD_GPIO, &GPIO_InitStructure);
  // 控制脚 RST
  RCC_APB2PeriphClockCmd(LCD_RST_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN;
  GPIO_Init(LCD_RST_GPIO, &GPIO_InitStructure);
} // ILI9325_GPIO_Init()

/************************************************ 
* 函  数: ILI9325_Write(u16)
* 功  能: 向ILI9325写指令或数据
*
* 参  数: dat - 待写入指令或数据
*
* 返回值: 无
*************************************************/
void ILI9325_Write(u16 dat)
{
  // 低8位数据写入CPIOB高8位
  LCD_DB0_7_GPIO->ODR = (LCD_DB0_7_GPIO->ODR & 0X00FF) | 
                        (((dat << 8) >> 8) << 8); 
  // 高8位数据写入CPIOC低8位
  LCD_DB8_15_GPIO->ODR = (LCD_DB8_15_GPIO->ODR & 0XFF00) | 
                         (((dat >> 8) << 8) >> 8); 
} // ILI9325_Write()

/************************************************ 
* 函  数: ILI9325_WriteCmd(u16)
* 功  能: LCD写指令
*
* 参  数: cmd - 待写入指令
*
* 返回值: 无
*************************************************/
void ILI9325_WriteCmd(u16 cmd)
{
  LCD_CS_CLR;
  LCD_RS_CLR;
  LCD_RD_SET;
  ILI9325_Write(cmd);
  LCD_WR_CLR;
  LCD_WR_SET;
  LCD_CS_SET;
} // ILI9325_WriteCmd()

/************************************************ 
* 函  数: ILI9325_WriteData(u16)
* 功  能: LCD写数据
*
* 参  数: dat - 待写入数据
*
* 返回值: 无
*************************************************/
void ILI9325_WriteData(u16 dat)
{
  LCD_CS_CLR;
  LCD_RS_SET;
  LCD_RD_SET;
  ILI9325_Write(dat);
  LCD_WR_CLR;
  LCD_WR_SET;
  LCD_CS_SET;
} // ILI9325_WriteData()

/************************************************ 
* 函  数: LCD_WriteRegister(u16, u16)
* 功  能: 向指定寄存器写入一个值
*
* 参  数: index - 寄存器地址
*         dat - 待写入数据
*
* 返回值: 无
*************************************************/
void LCD_WriteRegister(u16 index, u16 dat)
{
  ILI9325_WriteCmd(index);
  ILI9325_WriteData(dat);
} // LCD_WriteRegister()

/************************************************ 
* 函  数: ILI9325_Read(void)
* 功  能: 读取ILI9325数据线上的数据
*
* 参  数: 无
*
* 返回值: 读取到的数据
*************************************************/
u16 ILI9325_Read(void)
{
  u16 ret_data = 0x0000;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  // 因为要读取数据，所以将16个数据IO口设置为输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | 
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | 
                                GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | 
                                GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  // 读取数据
  ret_data = ((LCD_DB0_7_GPIO->IDR & 0XFF00) >> 8);  // 低8位数据读取CPIOB高8位
  ret_data |= ((LCD_DB8_15_GPIO->IDR & 0X00FF) << 8);  // 高8位数据读取CPIOC低8位
  // 重新将16个数据IO口设置为输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | 
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | 
                                GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | 
                                GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  return ret_data;
} // ILI9325_Read()

/************************************************ 
* 函  数: ILI9325_ReadData(void)
* 功  能: 读取LCD数据线上的数据
*
* 参  数: 无
*
* 返回值: 读取到的数据
*************************************************/
u16 ILI9325_ReadData(void)
{
  u16 ret_data = 0;

  LCD_RD_CLR;   
  LCD_CS_CLR;  
  ret_data = ILI9325_Read(); 
  LCD_RD_SET;        
  LCD_CS_SET; 

  return ret_data;
} // ILI9325_ReadData()

/************************************************ 
* 函  数: LCD_ReadRegister(u16)
* 功  能: 读取指定地址寄存器的值
*
* 参  数: index - 寄存器地址
*
* 返回值: 读取到的数据
*************************************************/
u16 LCD_ReadRegister(u16 index)
{ 
  ILI9325_WriteCmd(index); 

  return ILI9325_ReadData();
} // LCD_ReadRegister()

/************************************************ 
* 函  数: LCD_SetCursor(u16, u16)
* 功  能: 设定光标位置
*
* 参  数: x - 横坐标
*         y - 纵坐标
*
* 返回值: 无
*************************************************/
void LCD_SetCursor(u8 x, u16 y)
{
  if (x > LCD_WIDTH || y > LCD_HEIGHT) return;

  LCD_WriteRegister(R32, x);  // 行
  LCD_WriteRegister(R33, y);  // 列
} // LCD_SetCursor()

/************************************************ 
* 函  数: LCD_SetDisplayWindow(u8, u16, u16, u16)
* 功  能: 设定显示窗口大小
*
* 参  数: xpos - 第一个点横坐标
*         ypos - 第一个点纵坐标
*         width - 窗口宽度
*         height - 窗口高度
*
* 返回值: 无
*************************************************/
void LCD_SetDisplayWindow(u8 xpos, u16 ypos, u8 width, u16 height)
{
  LCD_SetCursor(xpos, ypos);
  LCD_WriteRegister(R80, xpos);
  LCD_WriteRegister(R82, ypos);
  LCD_WriteRegister(R81, xpos + width);
  LCD_WriteRegister(R83, ypos + height);
} // LCD_SetDisplayWindow()

/************************************************ 
* 函  数: LCD_Clear(u16)
* 功  能: 将屏幕填充为一种颜色
*
* 参  数: color - 填充颜色
*
* 返回值: 无
*************************************************/
void LCD_Clear(u16 color)
{
  u32 i = 0;
  
  LCD_SetCursor(0x0000, 0x0000);
  ILI9325_WriteCmd(R34); 
  for (i = 0; i < LCD_WIDTH*LCD_HEIGHT; ++i)
  {
    ILI9325_WriteData(color);
  } // for
} // LCD_Clear()

/************************************************ 
* 函  数: LCD_DisplayOn(u8)
* 功  能: 开启或关闭LCD显示
*
* 参  数: on_or_off - 开启或关闭
*
* 返回值: 无
*************************************************/
void LCD_DisplayOn(u8 on_or_off)
{
  if (1 == on_or_off) 
    LCD_WriteRegister(R7, 0x0133);
  else if (0 == on_or_off)
    LCD_WriteRegister(R7, 0x0000);
} // LCD_DisplayOn()

/************************************************ 
* 函  数: LCD_ReadPointColor(u8 x, u16 y)
* 功  能: 读取指定坐标颜色值
*
* 参  数: on_or_off - 开启或关闭
*
* 返回值: 获取到的颜色值
*************************************************/
u16 LCD_ReadPointColor(u8 x, u16 y)
{
  u16 ret_color = 0x0000;

  if (x > LCD_WIDTH || y > LCD_HEIGHT) return 0;
  LCD_SetCursor(x, y);
  LCD_CS_CLR;
  ILI9325_WriteCmd(R34);
  LCD_RD_CLR;  
  ret_color = ILI9325_Read(); // dummy
  LCD_RD_SET;   
  LCD_RD_CLR;  
  ret_color = ILI9325_Read(); 
  LCD_RD_SET; 
  LCD_CS_SET;
  
  return ret_color;
} // LCD_ReadPointColor()

/************************************************ 
* 函  数: LCD_DrawPoint(u8, u16, u16)
* 功  能: 绘制一个点
*
* 参  数: xpos - 该点横坐标
*         ypos - 该点纵坐标
*         color - 该点颜色
*
* 返回值: 无
*************************************************/
void LCD_DrawPoint(u8 xpos, u16 ypos, u16 color)
{
  if ((xpos > LCD_WIDTH) || (ypos > LCD_HEIGHT)) return;

  LCD_SetCursor(xpos, ypos);
  ILI9325_WriteCmd(R34);
  ILI9325_WriteData(color);
} // LCD_DrawPoint()

/************************************************ 
* 函  数: LCD_DisplayChar(u8, u16, char)
* 功  能: 在指定坐标显示一个8x16点阵的ascii字符
*
* 参  数: xpos - 该点横坐标
*         ypos - 该点纵坐标
*         ch - 待显示字符
*
* 返回值: 无
*************************************************/
void LCD_DisplayChar(u8 xpos, u16 ypos, char ch)
{
  u8 ascii_temp = 0;
  u16 i = 0, j = 0, ypos_old = ypos;
  
  if ((xpos > LCD_WIDTH) || (ypos > LCD_HEIGHT)) return;
  ch = ch - ' ';  // 得到偏移后的值
  for (i = 0; i < 16; ++i)
  {
    ascii_temp = g_ascii_1608[ch][i];
    for (j = 0; j < 8; ++j)
    {
      if (ascii_temp & 0x80)
      { 
        LCD_DrawPoint(xpos, ypos, LCD_TEXT_COLOR); // 字符颜色
      } else {
        LCD_DrawPoint(xpos, ypos, LCD_BACK_COLOR); // 背景颜色
      } // if else
      ascii_temp <<= 1;
      ++ypos;
      if (xpos >= LCD_WIDTH) return; // 超出显示范围
      if (16 == (ypos - ypos_old))
      {
        ypos = ypos_old;
        ++xpos;
        if (xpos >= LCD_WIDTH) return; // 超出显示范围
        break;
      } // if
    } // for
  } // for 
} // LCD_DisplayChar()

/************************************************ 
* 函  数: GetNumberBits(u32)
* 功  能: 判断数字的有效位数
*
* 参  数: num - 待判断的数字
*
* 返回值: 该数字的有效位数
*************************************************/
u8 GetNumberBits(u32 num)
{
  u8 ret_bits = 1;  // 数字至少有一位

  while (num)
  {
    num /= 10;
    // 只有当num除以10不为0时, 该次位数才能增加
    if (num) ++ret_bits;
  } // while

  return ret_bits;
} // GetNumberBits()

/************************************************ 
* 函  数: LCD_DisplayNumber(u8, u16, u32)
* 功  能: 在指定坐标显示数字
*
* 参  数: x - 该点横坐标
*         y - 该点纵坐标
*         num - 待显示数字, 有效范围0 ~ 4294967295
*
* 返回值: 无
*************************************************/
void LCD_DisplayNumber(u8 x, u16 y, u32 num)
{
  u8 i = 0, num_base = 0;
  u8 en_display = 0;  
  u8 num_bits = GetNumberBits(num);

  for(i = 0; i < num_bits; ++i)
  {
    num_base = (num/(int)pow(10, num_bits - i - 1)) % 10;
    if (0 == en_display && i < num_bits - 1)
    {
      if (0 == num_base)
      {
        LCD_DisplayChar(x + LINE_WIDTH*i, y, ' ');
        continue;
      } else {
        en_display = 1;
      } // if else
    } // if
    LCD_DisplayChar(x + LINE_WIDTH*i, y, num_base + '0'); 
  } // for
} // LCD_DisplayNumber()

/************************************************ 
* 函  数: LCD_DisplayStringLine(u8, u16, const char [])
* 功  能: 在指定座标显示单行字符串
*
* 参  数: xpos - 起点横坐标
*         ypos - 起点纵坐标
*         str - 待显示字符串
*
* 返回值: 无
*************************************************/
void LCD_DisplayStringLine(u8 xpos, u16 ypos, const char str[])
{
  u8 i = 0;
  
  if ((xpos > LCD_WIDTH) || (ypos > LCD_HEIGHT)) return;
  // 判断是不是非法字符!
  while((str[i] <= '~') && (str[i] >= ' '))
  {
    LCD_DisplayChar((xpos + LINE_WIDTH*i), ypos, str[i]);
    ++i;
  } // while
} // LCD_DisplayStringLine()

/************************************************ 
* 函  数: LCD_DisplayStringMulLines(u8, u16, u8, u16, const char [])
* 功  能: 在指定座标显示多行字符串
*
* 参  数: xpos - 起点横坐标
*         ypos - 起点纵坐标
*         width - 显示区域宽度
*         height - 显示区域高度
*         str - 待显示字符串
*
* 返回值: 无
*************************************************/
void LCD_DisplayStringMulLines(u8 xpos, u16 ypos, u8 width, u16 height, const char str[])
{
  u8 x_start = xpos;  // 保存段首位置
  u16 i = 0;

  if ((xpos + width > LCD_WIDTH) || (ypos > LCD_HEIGHT) ||
      (height > LCD_HEIGHT)) return;
  width += xpos;
  height += ypos;
  // 判断是不是非法字符!
  while((str[i] <= '~' && str[i] >= ' ') || '\n' == str[i]) 
  {
    if (ypos >= height) break;  // 显示行数已经超过限制, 退出
    if (str[i] != '\n') 
    {
      LCD_DisplayChar(xpos, ypos, str[i]);
      xpos += LINE_WIDTH;  // 更新x显示坐标, 增加一个字符宽度      
    } // if
    // 当出现换行符'\n', 换行
    // 如果下一次显示坐标已经到达最大显示范围宽度时, 自动换行
    if (('\n' == str[i] || xpos + LINE_WIDTH >= width) )
    {
      // 当出现某一个单词换行被拆分时, 使用连字符'-'将该词连接起来;
      // 只有当前字符和下一个字符均为字母时, 才认为是一整个单词被拆分;
      // 如果当前字符和下一个字符均为'\n'时, 不显示连字符'-'
      if ((isalpha(str[i]) && isalpha(str[i + 1])) && 
          !('\n' == str[i - 1] && '\n' == str[i]))
      {
        LCD_DisplayChar(xpos, ypos, '-');        
      } // if
      // 换行
      xpos = x_start; // x坐标移动到起始位置
      ypos += LINE_HEIGHT; // y坐标增加一个行高
    } // if
    ++i;  // 判断下一个字符
  } // while
} // LCD_DisplayStringMulLine()

/************************************************ 
* 函  数: LCD_DisplaySingleLine(u16, const char [])
* 功  能: 单行显示字符串, 超出屏幕宽度部分不会显示
*
* 参  数: line - 行数
*         str - 待显示字符串
*
* 返回值: 无
*************************************************/
void LCD_DisplaySingleLine(u16 line, const char str[])
{
  // LCD_DisplayStringLine(0, line, str);
  LCD_DisplayStringMulLines(0, line, LCD_WIDTH, LINE_HEIGHT, str);
} // LCD_DisplaySingleLine()

/************************************************ 
* 函  数: LCD_DisplayMultipleLines(u16, const char [])
* 功  能: 多行显示字符串, 超出屏幕宽度部分会自动换行显示
*
* 参  数: line - 行数
*         str - 待显示字符串
*
* 返回值: 无
*************************************************/
void LCD_DisplayMultipleLines(u16 line, const char str[])
{
  LCD_DisplayStringMulLines(0, line, LCD_WIDTH, LCD_HEIGHT, str);
} // LCD_DisplayMultipleLines()

/************************************************ 
* 函  数: LCD_ClearSingleLine(u16 line_num)
* 功  能: 清除单行内容
*
* 参  数: line_num - 行数
*
* 返回值: 无
*************************************************/
void LCD_ClearSingleLine(u16 line_num)
{
  // 一共30个空格, 240 = 30*8
  LCD_DisplaySingleLine(line_num, "                              ");
} // LCD_ClearSingleLine()

/************************************************ 
* 函  数: LCD_ClearSingleLine(u16 start_line)
* 功  能: 清除多行内容
*
* 参  数: start_line - 行号, 起始行
*         lines - 行数, 总共行数
*
* 返回值: 无
*************************************************/
void LCD_ClearMultipleLines(u16 start_line, u8 lines)
{
  u8 i = 0;

  for (i = 0; i < lines; ++i)
  {
    LCD_ClearSingleLine(start_line + i*LINE_HEIGHT);
  } // for
} // LCD_ClearMultipleLines()

/************************************************ 
* 函  数: LCD_Test(void)
* 功  能: 显示彩条, 测试液晶屏是否正常工作
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void LCD_Test(void)
{
  u8 r_data = 0, g_data = 0, b_data = 0, i = 0, j = 0;

  LCD_SetCursor(0x00, 0x0000);
  LCD_WriteRegister(R80, 0x0000);  // 水平GRAM起始位置
  LCD_WriteRegister(R81, LCD_WIDTH - 1); // 水平GRAM终止位置
  LCD_WriteRegister(R82, 0); // 垂直GRAM起始位置
  LCD_WriteRegister(R83, LCD_HEIGHT - 1); // 垂直GRAM终止位置  
  LCD_CS_CLR; 
  ILI9325_WriteCmd(R34);
  LCD_RS_SET;
  // 红色渐强条
  r_data = 0; g_data = 0; b_data = 0;     
  for (j = 0; j < 50; ++j)  
  {
    for (i = 0; i < LCD_WIDTH; ++i)
    {
      r_data = i/8;
      ILI9325_Write(r_data << 11 | g_data << 5 | b_data);
      LCD_WR_CLR;
      LCD_WR_SET;
    } // for
  } // for
  r_data = 0X1F; g_data = 0X3F; b_data = 0X1F;
  for (j = 0; j < 50; ++j)
  {
    for (i = 0; i < LCD_WIDTH; ++i)
    { 
      g_data = 0X3F - (i/4);
      b_data = 0X1F - (i/8);
      ILI9325_Write(r_data << 11 | g_data << 5 | b_data);
      LCD_WR_CLR;
      LCD_WR_SET;
    } // for
  } // for
  // 绿色渐强条
  r_data = 0; g_data = 0; b_data = 0;
  for (j = 0; j < 50; ++j)  
  {
    for (i = 0; i < LCD_WIDTH; ++i)
    { 
      g_data = i/4;
      ILI9325_Write(r_data << 11 | g_data << 5 | b_data);
      LCD_WR_CLR;
      LCD_WR_SET;
    } // for
  } // for
  r_data = 0X1F; g_data = 0X3F; b_data = 0X1F;
  for (j = 0; j < 50; ++j)  
  {
    for (i = 0; i < LCD_WIDTH; ++i)
    { 
      r_data = 0X1F - (i/8);
      b_data = 0X1F - (i/8);
      ILI9325_Write(r_data << 11 | g_data << 5 | b_data);
      LCD_WR_CLR;
      LCD_WR_SET;
    } // for
  } // for
  // 蓝色渐强条
  r_data = 0; g_data = 0; b_data = 0;
  for (j = 0; j < 60; ++j)  
  {
    for (i = 0; i < LCD_WIDTH; ++i)
    { 
      b_data = i/8;
      ILI9325_Write(r_data << 11 | g_data << 5 | b_data);
      LCD_WR_CLR;
      LCD_WR_SET;
    } // for
  } // for
  r_data = 0X1F; g_data = 0X3F; b_data = 0X1F;
  for (j = 0; j < 60; ++j)
  {
    for (i = 0; i < LCD_WIDTH; ++i)
    { 
      g_data = 0X3F - (i/4);
      r_data = 0X1F - (i/8);
      ILI9325_Write(r_data << 11 | g_data << 5 | b_data);
      LCD_WR_CLR;
      LCD_WR_SET;
    } // for
  } // for
  LCD_CS_SET;
} // LCD_Test()

/************************************************ 
* 函  数: LCD_BGR2RGB(u16)
* 功  能: 将BGR颜色转换成RGB
*
* 参  数: color - 待转换BGR颜色值
*
* 返回值: 转换结束后的RGB颜色值
*************************************************/
u16 LCD_BGR2RGB(u16 color)
{
  u16  r = 0, g = 0, b = 0, rgb = 0;
  
  b = (color >> 0) & 0x1f;
  g = (color >> 5) & 0x3f;
  r = (color >> 11) & 0x1f;
  rgb = (b << 11) + (g << 5) + (r << 0);
  
  return rgb;
} // LCD_BGR2RGB()

/************************************************ 
* 函  数: LCD_RGB2Color(u32)
* 功  能: 将RGB颜色转换为16位颜色
*
* 参  数: rgb_value - RGB颜色值
*
* 返回值: 16位颜色值
*************************************************/
u16 LCD_RGB2Color(u32 rgb_value)
{
  u8  r, g, b;

  r = (rgb_value >> (16 + 3)) & 0x1f; // 取R色的高5位
  g = (rgb_value >> (8 + 2)) & 0x3f;  // 取G色的高6位
  b = (rgb_value >> (0 + 3)) & 0x1f;  // 取B色的高5位

  return ((r << 11) + (g << 5) + (b << 0));   
} // LCD_RGB2Color()

/************************************************ 
* 函  数: LCD_DrawLine(u8, u16, u8, u16, u16 color)
* 功  能: 根据起止点坐标绘制线段
*
* 参  数: x0 - 起点横坐标
*         y0 - 起点纵坐标
*         x1 - 终点横坐标
*         y1 - 终点纵坐标
*         color - 线段颜色
*
* 返回值: 无
*************************************************/
void LCD_DrawLine(u8 x0, u16 y0, u8 x1, u16 y1, u16 color)
{
  u16 i = 0, row = 0, column = 0; 
  int x_err = 0, y_err = 0; 
  int x_dir = 0, y_dir = 0; 
  int x_incremt = 0, y_incremt = 0, distance = 0; 

  if (x0 > LCD_WIDTH || y0 > LCD_HEIGHT || 
      x1 > LCD_WIDTH || y1 > LCD_HEIGHT) return;
  x_incremt = x1 - x0; // 计算坐标增量 
  y_incremt = y1 - y0; 
  row = x0; 
  column = y0; 
  if (x_incremt > 0) {
    x_dir = 1; // 设置单步方向 
  } else if (0 == x_incremt) {
    x_dir = 0; // 垂直线 
  } else {
    x_dir = -1;
    x_incremt = -x_incremt;
  } // else
  if (y_incremt > 0) {
    y_dir = 1; 
  } else if (0 == y_incremt) {
    y_dir = 0;  // 水平线 
  } else {
    y_dir = -1;
    y_incremt = -y_incremt;
  } 
  if( x_incremt > y_incremt)
    distance = x_incremt; // 选取基本增量坐标轴 
  else 
    distance = y_incremt; 
  // 画线输出
  for (i = 0; i <= distance + 1; ++i) 
  {  
    LCD_DrawPoint(row, column, color); // 画点 
    x_err += x_incremt ; 
    y_err += y_incremt ; 
    if (x_err > distance) 
    { 
      x_err -= distance; 
      row += x_dir; 
    } 
    if(y_err > distance) 
    { 
      y_err -= distance; 
      column += y_dir; 
    } // if
  } // for
} // LCD_DrawLine()

/************************************************ 
* 函  数: LCD_DrawHorizontalLine(u8, u16, u8, u16)
* 功  能: 绘制水平线段
*
* 参  数: x0 - 起点横坐标
*         y0 - 起点纵坐标
*         width - 线段长度
*         color - 线段颜色
*
* 返回值: 无
*************************************************/
void LCD_DrawHorizontalLine(u8 x0, u16 y0, u8 width, u16 color)
{
  LCD_DrawLine(x0, y0, x0 + width, y0, color);
} // LCD_DrawHorizontalLine()

/************************************************ 
* 函  数: LCD_DrawVerticallLine(u8, u16, u8, u16)
* 功  能: 绘制垂直线段
*
* 参  数: x0 - 起点横坐标
*         y0 - 起点纵坐标
*         height - 线段长度
*         color - 线段颜色
*
* 返回值: 无
*************************************************/
void LCD_DrawVerticallLine(u8 x0, u16 y0, u16 height, u16 color)
{
  LCD_DrawLine(x0, y0, x0, y0 + height, color);
} // LCD_DrawVerticallLine()

/************************************************ 
* 函  数: LCD_DrawQuadrangle(u8, u16, u8, u16, u16)
* 功  能: 绘制四边形
*
* 参  数: x0 - 起点横坐标
*         y0 - 起点纵坐标
*         x1 - 终点横坐标
*         y1 - 终点纵坐标
*         color - 线段颜色
*
* 返回值: 无
*************************************************/
void LCD_DrawQuadrangle(u8 x0, u16 y0, u8 x1, u16 y1, u16 color)
{
  LCD_DrawLine(x0, y0, x1, y0, color); // 顺时针绘制
  LCD_DrawLine(x1, y0, x1, y1, color);
  LCD_DrawLine(x1, y1, x0, y1, color);
  LCD_DrawLine(x0, y1, x0, y0, color);
} // LCD_DrawQuadrangle()

/************************************************ 
* 函  数: LCD_DrawRectangle(u8, u16, u8, u16, u16, u8)
* 功  能: 绘制矩形, 可填充颜色
*
* 参  数: x0 - 起点横坐标
*         y0 - 起点纵坐标
*         width - 矩形宽度
*         height - 矩形高度
*         color - 绘制颜色
*         fill - 是否填充颜色
*
* 返回值: 无
*************************************************/
void LCD_DrawRectangle(u8 x0, u16 y0, u8 width, u16 height, u16 color, u8 fill)
{
  if (fill)
  {
    u16 i = 0, x1 = x0 + width, y1 = y0 + height;

    if (x0 > x1)
    {
      i = x1;
      x1 = x0;
    } else {
      i = x0;
    } // if else
    for(; i <= x1; ++i)
    {
      LCD_DrawLine(i, y0, i, y1, color);
    } // for
    return;
  } // if
  LCD_DrawQuadrangle(x0, y0, x0 + width, y0 + height, color);
} // LCD_DrawRectangle()

/************************************************ 
* 函  数: LCD_DrawSquare(u8, u16, u16, u16, u8)
* 功  能: 绘制正方形, 可填充颜色
*
* 参  数: x0 - 起点横坐标
*         y0 - 起点纵坐标
*         side_lenght - 正方形边长
*         color - 绘制颜色
*         fill - 是否填充颜色
*
* 返回值: 无
*************************************************/
void LCD_DrawSquare(u8 x0, u16 y0, u16 side_lenght, u16 color, u8 fill)
{
  LCD_DrawRectangle(x0, y0, side_lenght, side_lenght, color, fill);
} // LCD_DrawSquare()

/************************************************ 
* 函  数: LCD_DrawCircle(u8, u16, u8, u16, u8)
* 功  能: 绘制圆, 可填充颜色, 使用Bresenham算法
*
* 参  数: x_center - 圆心横坐标
*         y_center - 圆心纵坐标
*         r - 半径
*         color - 绘制颜色
*         fill - 是否填充颜色
*
* 返回值: 无
*************************************************/
void LCD_DrawCircle(u8 x_center, u16 y_center, u8 r, u16 color, u8 fill)
{
  u8 x_now = 0; // x现在的值
  u16 y_now = r;  // y现在的值
  int decision = 3 - 2*r;  // 决定值

  while (x_now <= y_now)
  {
    if (fill)
    {
      LCD_DrawLine(x_center + x_now, y_center + y_now, x_center - x_now, y_center + y_now, color);
      LCD_DrawLine(x_center + x_now, y_center - y_now, x_center - x_now, y_center - y_now, color);
      LCD_DrawLine(x_center + y_now, y_center + x_now, x_center - y_now, y_center + x_now, color);
      LCD_DrawLine(x_center + y_now, y_center - x_now, x_center - y_now, y_center - x_now, color);
    } else {
      LCD_DrawPoint(x_center + x_now, y_center + y_now, color);
      LCD_DrawPoint(x_center + x_now, y_center - y_now, color);
      LCD_DrawPoint(x_center - x_now, y_center + y_now, color);
      LCD_DrawPoint(x_center - x_now, y_center - y_now, color);
      LCD_DrawPoint(x_center + y_now, y_center + x_now, color);
      LCD_DrawPoint(x_center + y_now, y_center - x_now, color);
      LCD_DrawPoint(x_center - y_now, y_center + x_now, color);
      LCD_DrawPoint(x_center - y_now, y_center - x_now, color);      
    } // if else 
    // 使用Bresenham算法画圆 
    if (decision < 0) {
      decision += 4*x_now + 6;
    } else {
      decision += 4*(x_now - y_now) + 10;
      --y_now;
    } // if else
    ++x_now;
  } // while
} // LCD_DrawCircle()

/************************************************ 
* 函  数: LCD_DrawBMP(u8, u16, u8, u16, u16 [])
* 功  能: 在指定坐标范围显示一幅图片
*
* 参  数: start_x - 起点横座标
*         start_y - 起点纵座标
*         end_x - 终点横座标
*         end_y - 终点纵座标
*         pic - 待显示图片, 图片取模格式为水平扫描, 16位颜色模式
*
* 返回值: 无
*************************************************/
void LCD_DrawBMP(u8 start_x, u16 start_y, u8 end_x, u16 end_y, const u16 pic[])
{
  u32 i = 0;

  if (start_x > LCD_WIDTH || start_y > LCD_HEIGHT ||
      end_x > LCD_WIDTH || end_y > LCD_HEIGHT) return;
  LCD_SetDisplayWindow(start_x, start_y, end_x, end_y);
  LCD_SetCursor(start_x, start_y);
  ILI9325_WriteCmd(R34);
  for (i = 0; i < end_x*end_y; ++i)
  {
    ILI9325_WriteData(pic[i]);
  } // for
} // LCD_DrawBMP()
