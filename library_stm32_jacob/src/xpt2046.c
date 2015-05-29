/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: xpt2046.c
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
#include "../include/xpt2046.h"

#include <stm32f10x.h>
#include <math.h> // Declare sqrt().
#include <stdlib.h> // Declare abs().
#include <stdio.h>  // Declare printf().

#include "../include/delay.h"  // Declare DelayUs().
#include "../include/ili9325.h"  // Use LCD.
#include "../include/24cxx.h"  // Use AT24C02.
#include "../include/stm32_flash.h"  // Use flash to save data.

TOUCH_TypeDef g_touch = {
  // TOUCH_Init,
  // TOUCH_Scan,
  // TOUCH_Calibrate,
  0x0000,   // x_origin
  0x0000, // y_origin
  0x0000,   // x_now
  0x0000, // y_now
  0x00,   // pen_status
  0.0,    // xfac
  0.0,    // yfac
  0x0000, // xoff
  0x0000  // yoff
};  // TOUCH_TypeDef g_touch

/************************************************ 
* 函  数: TOUCH_WriteByte(u8)
* 功  能: 向触摸屏IC写入1个字节的数据, 使用SPI写入
*
* 参  数: dat - 待写入的字节数据
*
* 返回值: 无
*************************************************/
void TOUCH_WriteByte(u8 dat)
{
  u8 i = 0;

  for (i = 0; i < 8; ++i)
  {
    if (dat & 0x80)
      TOUCH_IN_SET;
    else
      TOUCH_IN_CLR;
    dat <<= 1;
    TOUCH_CLK_CLR;  // 上升沿有效
    TOUCH_CLK_SET; 
  } // for
} // TOUCH_WriteByte()

/************************************************ 
* 函  数: TOUCH_ReadAD(u8)
* 功  能: 从触摸屏IC读取ADC数据, 使用SPI读取
*
* 参  数: cmd - 读取指令
*
* 返回值: 读取到的AD数据, 12为AD
*************************************************/
u16 TOUCH_ReadAD(u8 cmd)
{
  u8 i = 0;
  u16 ret_data = 0;

  TOUCH_CLK_CLR;  // 先拉低时钟
  TOUCH_IN_CLR; // 拉低数据线
  TOUCH_CS_CLR; // 选中触摸屏IC
  TOUCH_WriteByte(cmd); // 发送命令
  DelayUs(6); // 等待AD转换完成, 转换时间最长为6us
  TOUCH_CLK_CLR;
  DelayUs(1);
  TOUCH_CLK_SET;  // 一个时钟, 清除BUSY
  TOUCH_CLK_CLR;
  for (i = 0; i < 16; ++i)
  {
    ret_data <<= 1;
    TOUCH_CLK_SET;  // 下降沿有效
    TOUCH_CLK_CLR;
    if (TOUCH_OUT) ++ret_data;
  } // for
  ret_data >>= 4; // 只有高12位有效
  TOUCH_CS_SET; // 释放片选

  return ret_data;
} // TOUCH_ReadAD()

/************************************************ 
* 函  数: TOUCH_ReadXorY(u8)
* 功  能: 读取x or y坐标值AD值, 连续读取READ_TIMES次数据, 对这些数据升序排列,
*         然后去掉最低和最高LOST_VALUE个数, 再取平均值
*
* 参  数: cmd - 读取x, y的指令
*
* 返回值: 读取到的坐标值AD值
*************************************************/
#define READ_TIMES 5  // 读取次数
#define LOST_VALUE 1  // 丢弃值
u16 TOUCH_ReadXorY(u8 cmd)
{ 
  u8 i = 0, j = 0;
  u16 swap_temp = 0;
  u16 read_buf[READ_TIMES] = {0};
  u16 ret_pos = 0;
  u32 sum = 0;

  // 读取数据到缓冲区
  for (i = 0; i < READ_TIMES; ++i)
    read_buf[i] = TOUCH_ReadAD(cmd);
  // 将读取到的数据进行升序排序
  for (i = 0; i < READ_TIMES - 1; ++i)
  {
    for (j = i + 1; j < READ_TIMES; ++j)
    {
      // 升序排序
      if (read_buf[i] > read_buf[j])
      {
        swap_temp = read_buf[i];
        read_buf[i] = read_buf[j];
        read_buf[j] = swap_temp;
      } // if
    } // for
  } // for
  // 再丢弃最大值和最小值, 然后求和, 最好计算平均值
  for (i = LOST_VALUE; i < READ_TIMES - LOST_VALUE; ++i)
    sum += read_buf[i];
  ret_pos = sum/(READ_TIMES - 2*LOST_VALUE);

  return ret_pos;
} // TOUCH_ReadXorY()

/************************************************ 
* 函  数: TOUCH_ReadXandY(u16 *, u16 *)
* 功  能: 同时读取x, y坐标的值
*
* 参  数: x - 读取到的x坐标值AD值, 0 ~ 4095?
*         y - 读取到的y坐标值AD值, 0 ~ 4095
*
* 返回值: 读取到的坐标值AD值
*************************************************/
u8 TOUCH_ReadXandY(u16 *x, u16 *y)
{
  u16 x_temp = 0, y_temp = 0;

  // 横屏: x, 0x90; y, 0xD0
  // 竖屏: x, 0xD0; y, 0x90
  x_temp = TOUCH_ReadXorY(0xD0);
  y_temp = TOUCH_ReadXorY(0x90);
  // if (x_temp < 100 || y_temp < 100) return 0; // 读取失败
  *x = x_temp;
  *y = y_temp;

  return 1; // 读取成功
} // TOUCH_ReadXandY()

/************************************************ 
* 函  数: TOUCH_ReadXandY(u8 *, u16 *)
* 功  能: 连续读取2次触摸屏IC, 且这两次的偏差不能超过ERR_RANGE,
*         满足条件, 则认为读数正确, 否则读数错误。能够提高读取准确度
*
* 参  数: x - 读取到的x坐标值
*         y - 读取到的y坐标值
*
* 返回值: 读取成功与否标志: 0, 失败; 1, 成功
*************************************************/
#define ERR_RANGE 50  // 误差范围
u8 TOUCH_ReadXandYTwice(u16 *x, u16 *y)
{
  u16 x0 = 0, x1 = 0;
  u16 y0 = 0, y1 = 0;

  if (0 == TOUCH_ReadXandY(&x0, &y0)) return 0;
  if (0 == TOUCH_ReadXandY(&x1, &y1)) return 0;
  // 前后两次采样误差在+-50以内, 取两次采样平均值
  if (abs(x0 - x1) < ERR_RANGE && abs(y0 - y1) < ERR_RANGE)
  {
    *x = (x0 + x1)/2;
    *y = (y0 + y1)/2;
  } else {  // 两次采样误差太大
    return 0;
  } // if else

  return 1; // 读取成功
} // TOUCH_ReadXandYTwice()

/************************************************ 
* 函  数: TOUCH_DrawCalibratePoint(u8, u16, u16)
* 功  能: 绘制校准点
*
* 参  数: x - 校准点x坐标值
*         y - 校准点y坐标值
*         color - 校准点颜色
*
* 返回值: 无
*************************************************/
void TOUCH_DrawCalibratePoint(u8 x, u16 y, u16 color)
{
  // 绘制十字线
  LCD_DrawHorizontalLine(x - 12, y, 25, color);  // 横线
  LCD_DrawVerticallLine(x, y - 12, 25, color); // 竖线
  // 绘制中心点
  LCD_DrawPoint(x + 1, y + 1, color);
  LCD_DrawPoint(x - 1, y + 1, color);
  LCD_DrawPoint(x + 1, y - 1, color);
  LCD_DrawPoint(x - 1, y - 1, color);
  // 绘制中心圆
  // LCD_DrawCircle(x, y, 6, color, NOT_FILL);  
} // TOUCH_DrawCalibratePoint()

/************************************************ 
* 函  数: TOUCH_Paint(u8, u16, u16)
* 功  能: 2*2点大小绘图
*
* 参  数: x - 当前x坐标值
*         y - 当前y坐标值
*         color - 校准点颜色
*
* 返回值: 无
*************************************************/
void TOUCH_Paint(u8 x, u16 y, u16 color)
{
  LCD_DrawPoint(x, y, color);
  LCD_DrawPoint(x + 1, y, color);
  LCD_DrawPoint(x, y + 1, color);
  LCD_DrawPoint(x + 1, y + 1, color);
} // TOUCH_Paint()

/************************************************ 
* 函  数: TOUCH_Scan(u8)
* 功  能: 触摸屏是否有点击或按下事件扫描
*
* 参  数: mode - 0, 屏幕坐标; 1, 物理坐标(校准等特殊场合使用)
*
* 返回值: 当前触摸屏状态: 0, 无触摸; 1, 有触摸
*************************************************/
u8 TOUCH_Scan(u8 mode)
{
  // 触摸屏被点击
  if (0 == TOUCH_PEN) {        
    if (mode) { // 读取物理坐标 
      TOUCH_ReadXandYTwice(&g_touch.x_now, &g_touch.y_now);  
      printf("x_now: %3d, y_now: %3d\r\n", g_touch.x_now, g_touch.y_now);    
    } else if (TOUCH_ReadXandYTwice(&g_touch.x_now, &g_touch.y_now)) {
      // 读取屏幕坐标
      g_touch.x_now = g_touch.xfac*g_touch.x_now + g_touch.xoff;  // 将结果转换为屏幕坐标
      g_touch.y_now = g_touch.yfac*g_touch.y_now + g_touch.yoff;
      printf("x_now: %3d, y_now: %3d\r\n", g_touch.x_now, g_touch.y_now);
    } // if else
    // 之前屏幕没有被点击, 记录第一次被按下的坐标, 作为原点坐标
    if (0 == (g_touch.pen_status & TOUCH_PRESS_DOWN)) {
      g_touch.pen_status = TOUCH_PRESS_DOWN | TOUCH_BUTTON_DOWN;  // 强制校准按键按下
      g_touch.x_origin = g_touch.x_now; // 记录第一次点击的坐标
      g_touch.y_origin = g_touch.y_now;
    } // if
  } else {  // 当前触摸屏没有被点击  
    if (g_touch.pen_status & TOUCH_PRESS_DOWN) {  // 之前触摸屏被点击     
      // g_touch.pen_status &= ~(1 << 7);  // 标记手指松开
      printf("Touch screen was pressed.\r\n"); 
      g_touch.pen_status &= ~TOUCH_PRESS_DOWN;  // 标记手指松开
    } else {  // 之前触摸屏也没有被点击  
      g_touch.x_origin = 0x0000;
      g_touch.y_origin = 0x0000;
      g_touch.x_now = 0xFFFF;
      g_touch.y_now = 0xFFFF;
    } // if else
  } // if else

  // 返回当前的触摸状态
  return (g_touch.pen_status & TOUCH_PRESS_DOWN);
} // TOUCH_Scan()

/************************************************ 
* 函  数: TOUCH_SaveCalibrateData(void)
* 功  能: 保存触摸屏校准参数到AT24C02
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
// 保存在EEPROM里面的地址区间基址,
// 占用12个字节(TOUCH_SAVE_ADDR_BASE ~ TOUCH_SAVE_ADDR_BASE + 11)
// #define TOUCH_SAVE_ADDR_BASE 32
void TOUCH_SaveCalibrateData(void) 
{
  u16 save_buf[8] = {0};

  save_buf[0] = TOUCH_CALIBRATED;
  save_buf[1] = (short)(g_touch.xfac*10000.0);
  save_buf[2] = (short)(g_touch.yfac*10000.0);
  save_buf[3] = g_touch.xoff;
  save_buf[4] = g_touch.yoff;
/*  save_buf[1] = (u16)((u32)(g_touch.xfac*100000000.0) << 16);
  save_buf[2] = (u16)((u32)(g_touch.xfac*100000000.0) >> 16);
  save_buf[3] = (u16)((u32)(g_touch.yfac*100000000.0) << 16);
  save_buf[4] = (u16)((u32)(g_touch.yfac*100000000.0) >> 16);  
  save_buf[5] = g_touch.xoff;
  save_buf[6] = g_touch.yoff;*/
  if (StmFlash_WriteBuffer(USER_SAVE_BASE_ADDR, save_buf, 5))
    printf("Data were saved! ^_^\r\n");
  else 
    printf("Sorry, failed to save data!\r\n");
  /*  // 保存已经被校准标志
  AT24CXX_WriteByte(TOUCH_SAVE_ADDR_BASE, TOUCH_CALIBRATED);
  // 保存x校准因子
  AT24CXX_WriteLenByte(TOUCH_SAVE_ADDR_BASE + 1, g_touch.xfac*100000000.0, 4);
  // 保存y校准因子
  AT24CXX_WriteLenByte(TOUCH_SAVE_ADDR_BASE + 5, g_touch.yfac*100000000.0, 4);
  // 保存x偏移量
  AT24CXX_WriteByte(TOUCH_SAVE_ADDR_BASE + 9, g_touch.xoff);
  // 保存y偏移量
  AT24CXX_WriteLenByte(TOUCH_SAVE_ADDR_BASE + 10, g_touch.yoff, 2); */
} // TOUCH_SaveCalibrateData()

/************************************************ 
* 函  数: TOUCH_ReadCalibrateData(void)
* 功  能: 从AT24C02读取触摸屏校准参数
*
* 参  数: 无
*
* 返回值: 读取成功与否标志: 0, 读取失败, 要求重新校准; 
*                           1, 读取成功
*************************************************/
u8 TOUCH_ReadCalibrateData(void)
{
  u8 calibrated_flag = 0x00;
  short fac_temp = 0x0000;
  u16 read_buf[8] = {0};

  if (StmFlash_ReadBuffer(USER_SAVE_BASE_ADDR, read_buf, 5))
    printf("Read data successfully!\r\n");
  else
    printf("Failed to read data!\r\n");
  printf("calibrated_flag: 0x%02X.\r\n", read_buf[0]);
  // 获取校准标志, 判断是否已经校准
  calibrated_flag = read_buf[0];
  // 触摸屏已经校准过了
  if (TOUCH_CALIBRATED == calibrated_flag)
  {
    // 获取校准相关参数
    fac_temp = read_buf[1];
    printf("xfac*10000.0: %d, ", fac_temp);
    g_touch.xfac = fac_temp/10000.0;
    fac_temp = read_buf[2];
    printf("yfac*10000.0: %d.\r\n", fac_temp);
    g_touch.yfac = fac_temp/10000.0; 
    g_touch.xoff = read_buf[3];
    g_touch.yoff = read_buf[4];    
    printf("xfac: %3g, yfac: %3g\r\n", g_touch.xfac, g_touch.yfac);
    printf("xoff: %d, yoff: %d\r\n", g_touch.xoff, g_touch.yoff);

    return 1;
  } // if 
/*  // 读取校准标志, 判断是否已经校准
  calibrated_flag = AT24CXX_ReadByte(TOUCH_SAVE_ADDR_BASE);
  // 触摸屏已经校准过了
  if (TOUCH_CALIBRATED == calibrated_flag)
  {
    // 读取校准相关参数
    g_touch.xfac = (float)AT24CXX_ReadLenByte(TOUCH_SAVE_ADDR_BASE + 1, 4)/100000000.0;
    g_touch.yfac = (float)AT24CXX_ReadLenByte(TOUCH_SAVE_ADDR_BASE + 5, 4)/100000000.0;
    g_touch.xoff = AT24CXX_ReadByte(TOUCH_SAVE_ADDR_BASE + 9);
    g_touch.yoff = AT24CXX_ReadLenByte(TOUCH_SAVE_ADDR_BASE + 10, 2);

    return 1;
  } // if*/

  return 0;
} // TOUCH_ReadCalibrateData()

/************************************************ 
* 函  数: TOUCH_DisplayCaliInfo(u8, u16, u8, u16, u8, u16, u8, u16, u16)
* 功  能: 显示校准参数
*
* 参  数: x0 - 0点横坐标
*         y0 - 0点纵坐标
*         x1 - 1点横坐标
*         y1 - 1点纵坐标
*         x2 - 2点横坐标
*         y2 - 2点纵坐标
*         x3 - 3点横坐标
*         y3 - 3点纵坐标
*         fac - 待显示的校准后的因子
*
* 返回值: 无
*************************************************/
void TOUCH_DisplayCaliInfo(u8 x0, u16 y0, u8 x1, u16 y1, 
                           u8 x2, u16 y2, u8 x3, u16 y3, u16 fac)
{
  LCD_DisplaySingleLine(LINE11, "      x0:      y0: ");
  LCD_DisplaySingleLine(LINE12, "      x1:      y1: ");
  LCD_DisplaySingleLine(LINE13, "      x2:      y2: ");
  LCD_DisplaySingleLine(LINE14, "      x3:      y3: ");  
  LCD_DisplaySingleLine(LINE15, "      fac*100:     ");
  LCD_DisplayNumber(80,   LINE11, x0);
  LCD_DisplayNumber(152,  LINE11, y0);
  LCD_DisplayNumber(80,   LINE12, x1);
  LCD_DisplayNumber(152,  LINE12, y1);
  LCD_DisplayNumber(80,   LINE13, x2);
  LCD_DisplayNumber(152,  LINE13, y2);
  LCD_DisplayNumber(80,   LINE14, x3);
  LCD_DisplayNumber(152,  LINE14, y3);
  LCD_DisplayNumber(120,  LINE15, fac);
} // TOUCH_DisplayCaliInfo()

/************************************************ 
* 函  数: TOUCH_Calibrate(void)
* 功  能: 校准屏幕, 获取4个校准点的参数
*         0, 1
*         3, 2
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
#define FAC_ERR 0.2 // 校准误差
void TOUCH_Calibrate(void)
{  
  u8 point_index = 0; // 4个校准点的索引
  u16 out_times = 0;  // 超时次数
  u16 x_differ = 0, y_differ = 0;  // 两点之间x, y坐标的差值
  u16 distance0 = 0, distance1 = 0;  // 0, 1两点物理距离; 2, 3两点物理距离
  u32 temp0 = 0, temp1 = 0; // 保存平方和的值
  float fac = 0.0;
  // 坐标: x      y
  // 0: [0][0]  [0][1]
  // 1: [1][0]  [1][1] 
  // 2: [2][0]  [2][1]
  // 3: [3][0]  [3][1]
  u16 pos_temp[4][2] = {0};  // 坐标缓存值

  g_touch.pen_status = 0x00; // 消除触发信号 
  g_touch.xfac = 0.0; // xfac用来标记是否校准过, 所以校准之前必须清掉, 以免错误 
  LCD_Clear(LCD_BACK_COLOR);  // 清屏
  // 显示校准提示信息
  LCD_DisplayStringLine(64, LINE3, "Calibrate now!");
  LCD_DisplayStringMulLines(20, LINE5, 200, LCD_HEIGHT, "  Please use the stylus click the \
cross on the screen. The cross will always move until the screen adjustment is completed."); 
  TOUCH_DrawCalibratePoint(20, 20, LCD_TEXT_COLOR);  // 绘制校准点0  
  LCD_ClearSingleLine(LINE4);   
  LCD_DisplayStringLine(56, LINE4, "Click #0, please.");   
  // 如果连续10秒钟没有按下, 则自动退出
  while (1)
  {
    TOUCH_Scan(1);  // 扫描物理坐标
    // 按键按下了一次(此时按键松开了)
    // if (TOUCH_BUTTON_DOWN == (g_touch.pen_status & 0xC0))
    if (TOUCH_BUTTON_DOWN == (g_touch.pen_status & (TOUCH_PRESS_DOWN | TOUCH_BUTTON_DOWN)))
    {
      out_times = 0;  // 清空超时
      // g_touch.pen_status &= ~(1 << 6); // 标记按键已经被处理过了
      g_touch.pen_status &= ~TOUCH_BUTTON_DOWN; // 标记按键已经被处理过了
      pos_temp[point_index][0] = g_touch.x_now;
      pos_temp[point_index][1] = g_touch.y_now;
      
      // LCD_ClearSingleLine(LINE2);
      LCD_DisplayStringLine(44, LINE2, "Yeah, another click. ^_^");  
      switch (point_index) {
        case 0: 
          TOUCH_DrawCalibratePoint(20, 20, LCD_BACK_COLOR); // 清除校准点0
          TOUCH_DrawCalibratePoint(LCD_WIDTH - 20, 20, LCD_TEXT_COLOR);  // 绘制校准点1
          // 点击下一个点
          LCD_ClearSingleLine(LINE4);   
          LCD_DisplayStringLine(56, LINE4, "Click #1, please.");
          printf("Click #1, please.\r\n"); 
          break;
        case 1: 
          TOUCH_DrawCalibratePoint(LCD_WIDTH - 20, 20, LCD_BACK_COLOR); // 清除校准点1
          TOUCH_DrawCalibratePoint(LCD_WIDTH - 20, LCD_HEIGHT - 20, LCD_TEXT_COLOR);  // 绘制校准点2
          // 点击下一个点
          LCD_ClearSingleLine(LINE4);   
          LCD_DisplayStringLine(56, LINE4, "Click #2, please."); 
          printf("Click #2, please.\r\n");        
          break;
        case 2: 
          TOUCH_DrawCalibratePoint(LCD_WIDTH - 20, LCD_HEIGHT - 20, LCD_BACK_COLOR); // 清除校准点2
          TOUCH_DrawCalibratePoint(20, LCD_HEIGHT - 20, LCD_TEXT_COLOR);  // 绘制校准点3
          // 点击下一个点
          LCD_ClearSingleLine(LINE4);   
          LCD_DisplayStringLine(56, LINE4, "Click #3, please."); 
          printf("Click #3, please.\r\n");        
          break;
        case 3: // 全部4个校准点的坐标已经得到
          // 利用对边相等判断校准点是否准确
          // 计算0, 1两点之间的距离
          x_differ = abs(pos_temp[0][0] - pos_temp[1][0]);  // x0 - x1
          y_differ = abs(pos_temp[0][1] - pos_temp[1][1]);  // y0 - y1
          temp0 = x_differ*x_differ + y_differ*y_differ;
          distance0 = sqrt(temp0);
          // 计算3, 2两点之间的距离
          x_differ = abs(pos_temp[3][0] - pos_temp[2][0]);  // x3 - x2
          y_differ = abs(pos_temp[3][1] - pos_temp[2][1]);  // y3 - y2
          temp1 = x_differ*x_differ + y_differ*y_differ;
          distance1 = sqrt(temp1);    
          fac = (float)distance0/distance1;
          printf("distance0: %d, distance1: %d, fac: %g\r\n", distance0, distance1, fac);
          // 判断获取的校准点是否合格
          if (fabs(fac - 1.0) > FAC_ERR || 0 == distance0 || 0 == distance1)
          {
            point_index = 0;  // 需要重新校准
            TOUCH_DrawCalibratePoint(20, LCD_HEIGHT - 20, LCD_BACK_COLOR); // 清除校准点3
            TOUCH_DrawCalibratePoint(20, 20, LCD_TEXT_COLOR);  // 绘制校准点0 
            LCD_ClearSingleLine(LINE4);   
            LCD_DisplayStringLine(56, LINE4, "Click #0, please.");              
            // 显示上次校准数据
            printf("#0, #1与#2, #3横向对边不相等!!\n");
            // LCD_Clear(LCD_BACK_COLOR);
            TOUCH_DisplayCaliInfo(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], 
                                  pos_temp[1][1], pos_temp[2][0], pos_temp[2][1],
                                  pos_temp[3][0], pos_temp[3][1], (u16)(fac*100.0)); 

            continue;            
          } // if
          // 计算0, 3两点之间的距离
          x_differ = abs(pos_temp[0][0] - pos_temp[3][0]);  // x0 - x3
          y_differ = abs(pos_temp[0][1] - pos_temp[3][1]);  // y0 - y3
          temp0 = x_differ*x_differ + y_differ*y_differ;
          distance0 = sqrt(temp0); 
          // 计算1, 2两点之间的距离
          x_differ = abs(pos_temp[1][0] - pos_temp[2][0]);  // x1 - x2
          y_differ = abs(pos_temp[1][1] - pos_temp[2][1]);  // y1 - y2
          temp1 = x_differ*x_differ + y_differ*y_differ;
          distance1 = sqrt(temp1); 
          fac = (float)distance0/distance1;
          printf("distance0: %d, distance1: %d, fac: %g\r\n", distance0, distance1, fac);
          // 判断获取的校准点是否合格
          if (fabs(fac - 1.0) > FAC_ERR)
          {
            point_index = 0;  // 需要重新校准
            TOUCH_DrawCalibratePoint(20, LCD_HEIGHT - 20, LCD_BACK_COLOR); // 清除校准点3
            TOUCH_DrawCalibratePoint(20, 20, LCD_TEXT_COLOR);  // 绘制校准点0 
            LCD_ClearSingleLine(LINE4);   
            LCD_DisplayStringLine(56, LINE4, "Click #0, please.");               
            // 显示上次校准数据
            printf("#0, #3与#1, #2纵向对边不相等!!\n");
            // LCD_Clear(LCD_BACK_COLOR);
            TOUCH_DisplayCaliInfo(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], 
                                  pos_temp[1][1], pos_temp[2][0], pos_temp[2][1],
                                  pos_temp[3][0], pos_temp[3][1], (u16)(fac*100.0));                
            continue;
          } // if
          // 判断对角线是否相等
          // 计算0, 2两点之间的距离
          x_differ = abs(pos_temp[0][0] - pos_temp[2][0]);  // x0 - x2
          y_differ = abs(pos_temp[0][1] - pos_temp[2][1]);  // y0 - y2
          temp0 = x_differ*x_differ + y_differ*y_differ;
          distance0 = sqrt(temp0);   
          // 计算1, 3两点之间的距离
          x_differ = abs(pos_temp[1][0] - pos_temp[3][0]);  // x1 - x3
          y_differ = abs(pos_temp[1][1] - pos_temp[3][1]);  // y1 - y3
          temp1 = x_differ*x_differ + y_differ*y_differ;
          distance1 = sqrt(temp1);  
          fac = (float)distance0/distance1; 
          printf("distance0: %d, distance1: %d, fac: %g\r\n", distance0, distance1, fac);
          // 判断获取的校准点是否合格
          if (fabs(fac - 1.0) > FAC_ERR)
          {
            point_index = 0;  // 需要重新校准
            TOUCH_DrawCalibratePoint(20, LCD_HEIGHT - 20, LCD_BACK_COLOR); // 清除校准点3
            TOUCH_DrawCalibratePoint(20, 20, LCD_TEXT_COLOR);  // 绘制校准点0 
            LCD_ClearSingleLine(LINE4);   
            LCD_DisplayStringLine(56, LINE4, "Click #0, please.");               
            // 显示上次校准数据
            printf("对角线不相等!!\n");
            // LCD_Clear(LCD_BACK_COLOR);
            TOUCH_DisplayCaliInfo(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], 
                                  pos_temp[1][1], pos_temp[2][0], pos_temp[2][1],
                                  pos_temp[3][0], pos_temp[3][1], (u16)(fac*100.0));    
            continue;            
          } // if
          // 现在所有获取的校准点均合格, 计算结果
          g_touch.xfac = (float)(LCD_WIDTH - 40)/(pos_temp[1][0] - pos_temp[0][0]);  // 得到xfac
          g_touch.xoff = (LCD_WIDTH - g_touch.xfac*(pos_temp[1][0] + pos_temp[0][0]))/2;  // 得到xoff
          g_touch.yfac = (float)(LCD_HEIGHT - 40)/(pos_temp[3][1] - pos_temp[0][1]);  // 得到yfac
          g_touch.yoff = (LCD_HEIGHT - g_touch.yfac*(pos_temp[3][1] + pos_temp[0][1]))/2;  // 得到yoff
          printf("xfac: %g, yfac: %g.\r\n", g_touch.xfac, g_touch.yfac);  
          printf("xoff: %d, yoff: %d.\r\n", g_touch.xoff, g_touch.yoff);

          // 校准成功
          TOUCH_SaveCalibrateData();  // 保存校准数据 
          LCD_Clear(LCD_BACK_COLOR);  // 清屏
          LCD_DisplayStringLine(8, LINE5, "      Congratulation!");
          LCD_DisplayStringLine(8, LINE7, "Calibrated successfully! ^_^"); 
          printf("      Congratulation!\r\n");  
          printf("Calibrated successfully! ^_^\r\n"); 
          DelayMs(1000);
          LCD_Clear(LCD_BACK_COLOR);  // 清屏

          return; // 校准完成
         // break; 
          default: break;                             
      } // switch
      ++point_index;
    } else {  // 没有屏幕点击事件发生
      // LCD_ClearSingleLine(LINE2);
      // LCD_DisplayStringLine(44, LINE2, "Oops, no click! 0_#");
    } // if else
    DelayMs(10);
    ++out_times;
    // 校准时间到达10s, 无论是否校准成功都直接退出
    if (out_times >= 10000)
    {
      out_times = 0;
      LCD_Clear(LCD_BACK_COLOR);
      LCD_DisplayStringLine(16, LINE5, "No way! 10 seconds elapsed.");
      TOUCH_ReadCalibrateData();  // 获取屏幕校准参数
      break;
    } // if
  } // while
} // TOUCH_Calibrate()

/************************************************ 
* 函  数: TOUCH_Init(void)
* 功  能: 触摸屏初始化
*
* 参  数: 无
*
* 返回值: 无 
*************************************************/
void TOUCH_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 时钟使能
  RCC_APB2PeriphClockCmd(TOUCH_CS_RCC | TOUCH_CLK_RCC | TOUCH_PEN_RCC |
                         TOUCH_IN_RCC | TOUCH_OUT_RCC, ENABLE);
  // TOUCH_CS, TOUCH_CLK, TOUCH_IN为输出
  // TOUCH_CS
  GPIO_InitStructure.GPIO_Pin = TOUCH_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TOUCH_CS_GPIO, &GPIO_InitStructure);
  // TOUCH_CLK
  GPIO_InitStructure.GPIO_Pin = TOUCH_CLK_PIN;
  GPIO_Init(TOUCH_CLK_GPIO, &GPIO_InitStructure);
  // TOUCH_IN, 触摸屏输入, stm32输出, MOSI
  GPIO_InitStructure.GPIO_Pin = TOUCH_IN_PIN;
  GPIO_Init(TOUCH_IN_GPIO, &GPIO_InitStructure); 
  // TOUCH_PEN, TOUCH_OUT为输入
  // TOUCH_PEN
  GPIO_InitStructure.GPIO_Pin = TOUCH_PEN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
  GPIO_Init(TOUCH_PEN_GPIO, &GPIO_InitStructure);
  // TOUCH_OUT, 触摸屏输出, stm32输入, MISO
  GPIO_InitStructure.GPIO_Pin = TOUCH_OUT_PIN;
  GPIO_Init(TOUCH_OUT_GPIO, &GPIO_InitStructure);  

  // SPI start.
  // TOUCH_CLK_CLR;  // CLK, 0
  TOUCH_CS_SET;   // CS, 1
  TOUCH_IN_SET;   // IN, 1
  TOUCH_CLK_SET;  // CLK, 1
  TOUCH_PEN_SET;  // PEN, 1
  // TOUCH_CS_CLR;   // CS, 0
  // 判断屏幕是否已经校准过
  TOUCH_ReadXandY(&g_touch.x_now, &g_touch.y_now); // 第一次读取初始化
  // AT24CXX_Init(); // 初始化24C02, 从24C02中读取已校准标志;
  // 24C02使用了引脚PB6与PB7, 故使用触摸屏的同时不能使用24C02;
  // 这里就使用stm32片内flash存储
  // 未进行过校准
  if (!TOUCH_ReadCalibrateData())
  {
    TOUCH_Calibrate();  // 屏幕校准, 并保持校准相关参数
  } // if
  printf("Touch screen was initialized successfully!\r\n");
} // TOUCH_Init()
