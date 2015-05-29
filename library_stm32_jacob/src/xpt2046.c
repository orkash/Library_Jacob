/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: xpt2046.c
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: 2.4��TFT������������XPT2046����, ʹ��ģ��SPI����
* ��    ��: TOUCH_WriteByte() - ������ICд��1���ֽڵ�����, ʹ��SPIд��
*           TOUCH_ReadAD() - �Ӵ�����IC��ȡADC����, ʹ��SPI��ȡ
*           TOUCH_ReadXorY() - ��ȡx or y����ֵ
*           TOUCH_ReadXandY() - ͬʱ��ȡx, y�����ֵ
*           TOUCH_ReadXandYTwice() - ������ȡ2�δ�����IC
*           TOUCH_DrawCalibratePoint() - ����У׼��
*           TOUCH_Scan() - �������Ƿ��е�������¼�ɨ��
*           TOUCH_SaveCalibrateData() - ���津����У׼������AT24C02
*           TOUCH_DisplayCaliInfo() - ��ʾУ׼����
*           TOUCH_ReadCalibrateData() - ��AT24C02��ȡ������У׼����
*           TOUCH_Calibrate() - У׼��Ļ, ��ȡ4��У׼��Ĳ���, ������У׼����
*           TOUCH_Init() - ��������ʼ��
*           TOUCH_Paint() - 2*2���С��ͼ
* 
* ��ǰ�汾: A0.0.3
* ��    ��: Jacob Peng
* �������: 2015��5��14��
* ��������: A0.0.1, ����˻�����ʼ������, 2015��5��7��
*           A0.0.2, ����24C02ռ��XPT2046�������ƶ˿�, �ʸ���stm32�ڲ�flash����
*                   �洢, 2015��5��13��
*           A0.0.3, ������У׼�����ƫ�����, ��ʱ�޽�, 2015��5��14��
* 
* ȡ���汾: A0.0.2
* ԭ����  : Jacob Peng
* �������: 2015��5��13��
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
* ��  ��: TOUCH_WriteByte(u8)
* ��  ��: ������ICд��1���ֽڵ�����, ʹ��SPIд��
*
* ��  ��: dat - ��д����ֽ�����
*
* ����ֵ: ��
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
    TOUCH_CLK_CLR;  // ��������Ч
    TOUCH_CLK_SET; 
  } // for
} // TOUCH_WriteByte()

/************************************************ 
* ��  ��: TOUCH_ReadAD(u8)
* ��  ��: �Ӵ�����IC��ȡADC����, ʹ��SPI��ȡ
*
* ��  ��: cmd - ��ȡָ��
*
* ����ֵ: ��ȡ����AD����, 12ΪAD
*************************************************/
u16 TOUCH_ReadAD(u8 cmd)
{
  u8 i = 0;
  u16 ret_data = 0;

  TOUCH_CLK_CLR;  // ������ʱ��
  TOUCH_IN_CLR; // ����������
  TOUCH_CS_CLR; // ѡ�д�����IC
  TOUCH_WriteByte(cmd); // ��������
  DelayUs(6); // �ȴ�ADת�����, ת��ʱ���Ϊ6us
  TOUCH_CLK_CLR;
  DelayUs(1);
  TOUCH_CLK_SET;  // һ��ʱ��, ���BUSY
  TOUCH_CLK_CLR;
  for (i = 0; i < 16; ++i)
  {
    ret_data <<= 1;
    TOUCH_CLK_SET;  // �½�����Ч
    TOUCH_CLK_CLR;
    if (TOUCH_OUT) ++ret_data;
  } // for
  ret_data >>= 4; // ֻ�и�12λ��Ч
  TOUCH_CS_SET; // �ͷ�Ƭѡ

  return ret_data;
} // TOUCH_ReadAD()

/************************************************ 
* ��  ��: TOUCH_ReadXorY(u8)
* ��  ��: ��ȡx or y����ֵADֵ, ������ȡREAD_TIMES������, ����Щ������������,
*         Ȼ��ȥ����ͺ����LOST_VALUE����, ��ȡƽ��ֵ
*
* ��  ��: cmd - ��ȡx, y��ָ��
*
* ����ֵ: ��ȡ��������ֵADֵ
*************************************************/
#define READ_TIMES 5  // ��ȡ����
#define LOST_VALUE 1  // ����ֵ
u16 TOUCH_ReadXorY(u8 cmd)
{ 
  u8 i = 0, j = 0;
  u16 swap_temp = 0;
  u16 read_buf[READ_TIMES] = {0};
  u16 ret_pos = 0;
  u32 sum = 0;

  // ��ȡ���ݵ�������
  for (i = 0; i < READ_TIMES; ++i)
    read_buf[i] = TOUCH_ReadAD(cmd);
  // ����ȡ�������ݽ�����������
  for (i = 0; i < READ_TIMES - 1; ++i)
  {
    for (j = i + 1; j < READ_TIMES; ++j)
    {
      // ��������
      if (read_buf[i] > read_buf[j])
      {
        swap_temp = read_buf[i];
        read_buf[i] = read_buf[j];
        read_buf[j] = swap_temp;
      } // if
    } // for
  } // for
  // �ٶ������ֵ����Сֵ, Ȼ�����, ��ü���ƽ��ֵ
  for (i = LOST_VALUE; i < READ_TIMES - LOST_VALUE; ++i)
    sum += read_buf[i];
  ret_pos = sum/(READ_TIMES - 2*LOST_VALUE);

  return ret_pos;
} // TOUCH_ReadXorY()

/************************************************ 
* ��  ��: TOUCH_ReadXandY(u16 *, u16 *)
* ��  ��: ͬʱ��ȡx, y�����ֵ
*
* ��  ��: x - ��ȡ����x����ֵADֵ, 0 ~ 4095?
*         y - ��ȡ����y����ֵADֵ, 0 ~ 4095
*
* ����ֵ: ��ȡ��������ֵADֵ
*************************************************/
u8 TOUCH_ReadXandY(u16 *x, u16 *y)
{
  u16 x_temp = 0, y_temp = 0;

  // ����: x, 0x90; y, 0xD0
  // ����: x, 0xD0; y, 0x90
  x_temp = TOUCH_ReadXorY(0xD0);
  y_temp = TOUCH_ReadXorY(0x90);
  // if (x_temp < 100 || y_temp < 100) return 0; // ��ȡʧ��
  *x = x_temp;
  *y = y_temp;

  return 1; // ��ȡ�ɹ�
} // TOUCH_ReadXandY()

/************************************************ 
* ��  ��: TOUCH_ReadXandY(u8 *, u16 *)
* ��  ��: ������ȡ2�δ�����IC, �������ε�ƫ��ܳ���ERR_RANGE,
*         ��������, ����Ϊ������ȷ, ������������ܹ���߶�ȡ׼ȷ��
*
* ��  ��: x - ��ȡ����x����ֵ
*         y - ��ȡ����y����ֵ
*
* ����ֵ: ��ȡ�ɹ�����־: 0, ʧ��; 1, �ɹ�
*************************************************/
#define ERR_RANGE 50  // ��Χ
u8 TOUCH_ReadXandYTwice(u16 *x, u16 *y)
{
  u16 x0 = 0, x1 = 0;
  u16 y0 = 0, y1 = 0;

  if (0 == TOUCH_ReadXandY(&x0, &y0)) return 0;
  if (0 == TOUCH_ReadXandY(&x1, &y1)) return 0;
  // ǰ�����β��������+-50����, ȡ���β���ƽ��ֵ
  if (abs(x0 - x1) < ERR_RANGE && abs(y0 - y1) < ERR_RANGE)
  {
    *x = (x0 + x1)/2;
    *y = (y0 + y1)/2;
  } else {  // ���β������̫��
    return 0;
  } // if else

  return 1; // ��ȡ�ɹ�
} // TOUCH_ReadXandYTwice()

/************************************************ 
* ��  ��: TOUCH_DrawCalibratePoint(u8, u16, u16)
* ��  ��: ����У׼��
*
* ��  ��: x - У׼��x����ֵ
*         y - У׼��y����ֵ
*         color - У׼����ɫ
*
* ����ֵ: ��
*************************************************/
void TOUCH_DrawCalibratePoint(u8 x, u16 y, u16 color)
{
  // ����ʮ����
  LCD_DrawHorizontalLine(x - 12, y, 25, color);  // ����
  LCD_DrawVerticallLine(x, y - 12, 25, color); // ����
  // �������ĵ�
  LCD_DrawPoint(x + 1, y + 1, color);
  LCD_DrawPoint(x - 1, y + 1, color);
  LCD_DrawPoint(x + 1, y - 1, color);
  LCD_DrawPoint(x - 1, y - 1, color);
  // ��������Բ
  // LCD_DrawCircle(x, y, 6, color, NOT_FILL);  
} // TOUCH_DrawCalibratePoint()

/************************************************ 
* ��  ��: TOUCH_Paint(u8, u16, u16)
* ��  ��: 2*2���С��ͼ
*
* ��  ��: x - ��ǰx����ֵ
*         y - ��ǰy����ֵ
*         color - У׼����ɫ
*
* ����ֵ: ��
*************************************************/
void TOUCH_Paint(u8 x, u16 y, u16 color)
{
  LCD_DrawPoint(x, y, color);
  LCD_DrawPoint(x + 1, y, color);
  LCD_DrawPoint(x, y + 1, color);
  LCD_DrawPoint(x + 1, y + 1, color);
} // TOUCH_Paint()

/************************************************ 
* ��  ��: TOUCH_Scan(u8)
* ��  ��: �������Ƿ��е�������¼�ɨ��
*
* ��  ��: mode - 0, ��Ļ����; 1, ��������(У׼�����ⳡ��ʹ��)
*
* ����ֵ: ��ǰ������״̬: 0, �޴���; 1, �д���
*************************************************/
u8 TOUCH_Scan(u8 mode)
{
  // �����������
  if (0 == TOUCH_PEN) {        
    if (mode) { // ��ȡ�������� 
      TOUCH_ReadXandYTwice(&g_touch.x_now, &g_touch.y_now);  
      printf("x_now: %3d, y_now: %3d\r\n", g_touch.x_now, g_touch.y_now);    
    } else if (TOUCH_ReadXandYTwice(&g_touch.x_now, &g_touch.y_now)) {
      // ��ȡ��Ļ����
      g_touch.x_now = g_touch.xfac*g_touch.x_now + g_touch.xoff;  // �����ת��Ϊ��Ļ����
      g_touch.y_now = g_touch.yfac*g_touch.y_now + g_touch.yoff;
      printf("x_now: %3d, y_now: %3d\r\n", g_touch.x_now, g_touch.y_now);
    } // if else
    // ֮ǰ��Ļû�б����, ��¼��һ�α����µ�����, ��Ϊԭ������
    if (0 == (g_touch.pen_status & TOUCH_PRESS_DOWN)) {
      g_touch.pen_status = TOUCH_PRESS_DOWN | TOUCH_BUTTON_DOWN;  // ǿ��У׼��������
      g_touch.x_origin = g_touch.x_now; // ��¼��һ�ε��������
      g_touch.y_origin = g_touch.y_now;
    } // if
  } else {  // ��ǰ������û�б����  
    if (g_touch.pen_status & TOUCH_PRESS_DOWN) {  // ֮ǰ�����������     
      // g_touch.pen_status &= ~(1 << 7);  // �����ָ�ɿ�
      printf("Touch screen was pressed.\r\n"); 
      g_touch.pen_status &= ~TOUCH_PRESS_DOWN;  // �����ָ�ɿ�
    } else {  // ֮ǰ������Ҳû�б����  
      g_touch.x_origin = 0x0000;
      g_touch.y_origin = 0x0000;
      g_touch.x_now = 0xFFFF;
      g_touch.y_now = 0xFFFF;
    } // if else
  } // if else

  // ���ص�ǰ�Ĵ���״̬
  return (g_touch.pen_status & TOUCH_PRESS_DOWN);
} // TOUCH_Scan()

/************************************************ 
* ��  ��: TOUCH_SaveCalibrateData(void)
* ��  ��: ���津����У׼������AT24C02
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
// ������EEPROM����ĵ�ַ�����ַ,
// ռ��12���ֽ�(TOUCH_SAVE_ADDR_BASE ~ TOUCH_SAVE_ADDR_BASE + 11)
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
  /*  // �����Ѿ���У׼��־
  AT24CXX_WriteByte(TOUCH_SAVE_ADDR_BASE, TOUCH_CALIBRATED);
  // ����xУ׼����
  AT24CXX_WriteLenByte(TOUCH_SAVE_ADDR_BASE + 1, g_touch.xfac*100000000.0, 4);
  // ����yУ׼����
  AT24CXX_WriteLenByte(TOUCH_SAVE_ADDR_BASE + 5, g_touch.yfac*100000000.0, 4);
  // ����xƫ����
  AT24CXX_WriteByte(TOUCH_SAVE_ADDR_BASE + 9, g_touch.xoff);
  // ����yƫ����
  AT24CXX_WriteLenByte(TOUCH_SAVE_ADDR_BASE + 10, g_touch.yoff, 2); */
} // TOUCH_SaveCalibrateData()

/************************************************ 
* ��  ��: TOUCH_ReadCalibrateData(void)
* ��  ��: ��AT24C02��ȡ������У׼����
*
* ��  ��: ��
*
* ����ֵ: ��ȡ�ɹ�����־: 0, ��ȡʧ��, Ҫ������У׼; 
*                           1, ��ȡ�ɹ�
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
  // ��ȡУ׼��־, �ж��Ƿ��Ѿ�У׼
  calibrated_flag = read_buf[0];
  // �������Ѿ�У׼����
  if (TOUCH_CALIBRATED == calibrated_flag)
  {
    // ��ȡУ׼��ز���
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
/*  // ��ȡУ׼��־, �ж��Ƿ��Ѿ�У׼
  calibrated_flag = AT24CXX_ReadByte(TOUCH_SAVE_ADDR_BASE);
  // �������Ѿ�У׼����
  if (TOUCH_CALIBRATED == calibrated_flag)
  {
    // ��ȡУ׼��ز���
    g_touch.xfac = (float)AT24CXX_ReadLenByte(TOUCH_SAVE_ADDR_BASE + 1, 4)/100000000.0;
    g_touch.yfac = (float)AT24CXX_ReadLenByte(TOUCH_SAVE_ADDR_BASE + 5, 4)/100000000.0;
    g_touch.xoff = AT24CXX_ReadByte(TOUCH_SAVE_ADDR_BASE + 9);
    g_touch.yoff = AT24CXX_ReadLenByte(TOUCH_SAVE_ADDR_BASE + 10, 2);

    return 1;
  } // if*/

  return 0;
} // TOUCH_ReadCalibrateData()

/************************************************ 
* ��  ��: TOUCH_DisplayCaliInfo(u8, u16, u8, u16, u8, u16, u8, u16, u16)
* ��  ��: ��ʾУ׼����
*
* ��  ��: x0 - 0�������
*         y0 - 0��������
*         x1 - 1�������
*         y1 - 1��������
*         x2 - 2�������
*         y2 - 2��������
*         x3 - 3�������
*         y3 - 3��������
*         fac - ����ʾ��У׼�������
*
* ����ֵ: ��
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
* ��  ��: TOUCH_Calibrate(void)
* ��  ��: У׼��Ļ, ��ȡ4��У׼��Ĳ���
*         0, 1
*         3, 2
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
#define FAC_ERR 0.2 // У׼���
void TOUCH_Calibrate(void)
{  
  u8 point_index = 0; // 4��У׼�������
  u16 out_times = 0;  // ��ʱ����
  u16 x_differ = 0, y_differ = 0;  // ����֮��x, y����Ĳ�ֵ
  u16 distance0 = 0, distance1 = 0;  // 0, 1�����������; 2, 3�����������
  u32 temp0 = 0, temp1 = 0; // ����ƽ���͵�ֵ
  float fac = 0.0;
  // ����: x      y
  // 0: [0][0]  [0][1]
  // 1: [1][0]  [1][1] 
  // 2: [2][0]  [2][1]
  // 3: [3][0]  [3][1]
  u16 pos_temp[4][2] = {0};  // ���껺��ֵ

  g_touch.pen_status = 0x00; // ���������ź� 
  g_touch.xfac = 0.0; // xfac��������Ƿ�У׼��, ����У׼֮ǰ�������, ������� 
  LCD_Clear(LCD_BACK_COLOR);  // ����
  // ��ʾУ׼��ʾ��Ϣ
  LCD_DisplayStringLine(64, LINE3, "Calibrate now!");
  LCD_DisplayStringMulLines(20, LINE5, 200, LCD_HEIGHT, "  Please use the stylus click the \
cross on the screen. The cross will always move until the screen adjustment is completed."); 
  TOUCH_DrawCalibratePoint(20, 20, LCD_TEXT_COLOR);  // ����У׼��0  
  LCD_ClearSingleLine(LINE4);   
  LCD_DisplayStringLine(56, LINE4, "Click #0, please.");   
  // �������10����û�а���, ���Զ��˳�
  while (1)
  {
    TOUCH_Scan(1);  // ɨ����������
    // ����������һ��(��ʱ�����ɿ���)
    // if (TOUCH_BUTTON_DOWN == (g_touch.pen_status & 0xC0))
    if (TOUCH_BUTTON_DOWN == (g_touch.pen_status & (TOUCH_PRESS_DOWN | TOUCH_BUTTON_DOWN)))
    {
      out_times = 0;  // ��ճ�ʱ
      // g_touch.pen_status &= ~(1 << 6); // ��ǰ����Ѿ����������
      g_touch.pen_status &= ~TOUCH_BUTTON_DOWN; // ��ǰ����Ѿ����������
      pos_temp[point_index][0] = g_touch.x_now;
      pos_temp[point_index][1] = g_touch.y_now;
      
      // LCD_ClearSingleLine(LINE2);
      LCD_DisplayStringLine(44, LINE2, "Yeah, another click. ^_^");  
      switch (point_index) {
        case 0: 
          TOUCH_DrawCalibratePoint(20, 20, LCD_BACK_COLOR); // ���У׼��0
          TOUCH_DrawCalibratePoint(LCD_WIDTH - 20, 20, LCD_TEXT_COLOR);  // ����У׼��1
          // �����һ����
          LCD_ClearSingleLine(LINE4);   
          LCD_DisplayStringLine(56, LINE4, "Click #1, please.");
          printf("Click #1, please.\r\n"); 
          break;
        case 1: 
          TOUCH_DrawCalibratePoint(LCD_WIDTH - 20, 20, LCD_BACK_COLOR); // ���У׼��1
          TOUCH_DrawCalibratePoint(LCD_WIDTH - 20, LCD_HEIGHT - 20, LCD_TEXT_COLOR);  // ����У׼��2
          // �����һ����
          LCD_ClearSingleLine(LINE4);   
          LCD_DisplayStringLine(56, LINE4, "Click #2, please."); 
          printf("Click #2, please.\r\n");        
          break;
        case 2: 
          TOUCH_DrawCalibratePoint(LCD_WIDTH - 20, LCD_HEIGHT - 20, LCD_BACK_COLOR); // ���У׼��2
          TOUCH_DrawCalibratePoint(20, LCD_HEIGHT - 20, LCD_TEXT_COLOR);  // ����У׼��3
          // �����һ����
          LCD_ClearSingleLine(LINE4);   
          LCD_DisplayStringLine(56, LINE4, "Click #3, please."); 
          printf("Click #3, please.\r\n");        
          break;
        case 3: // ȫ��4��У׼��������Ѿ��õ�
          // ���öԱ�����ж�У׼���Ƿ�׼ȷ
          // ����0, 1����֮��ľ���
          x_differ = abs(pos_temp[0][0] - pos_temp[1][0]);  // x0 - x1
          y_differ = abs(pos_temp[0][1] - pos_temp[1][1]);  // y0 - y1
          temp0 = x_differ*x_differ + y_differ*y_differ;
          distance0 = sqrt(temp0);
          // ����3, 2����֮��ľ���
          x_differ = abs(pos_temp[3][0] - pos_temp[2][0]);  // x3 - x2
          y_differ = abs(pos_temp[3][1] - pos_temp[2][1]);  // y3 - y2
          temp1 = x_differ*x_differ + y_differ*y_differ;
          distance1 = sqrt(temp1);    
          fac = (float)distance0/distance1;
          printf("distance0: %d, distance1: %d, fac: %g\r\n", distance0, distance1, fac);
          // �жϻ�ȡ��У׼���Ƿ�ϸ�
          if (fabs(fac - 1.0) > FAC_ERR || 0 == distance0 || 0 == distance1)
          {
            point_index = 0;  // ��Ҫ����У׼
            TOUCH_DrawCalibratePoint(20, LCD_HEIGHT - 20, LCD_BACK_COLOR); // ���У׼��3
            TOUCH_DrawCalibratePoint(20, 20, LCD_TEXT_COLOR);  // ����У׼��0 
            LCD_ClearSingleLine(LINE4);   
            LCD_DisplayStringLine(56, LINE4, "Click #0, please.");              
            // ��ʾ�ϴ�У׼����
            printf("#0, #1��#2, #3����Ա߲����!!\n");
            // LCD_Clear(LCD_BACK_COLOR);
            TOUCH_DisplayCaliInfo(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], 
                                  pos_temp[1][1], pos_temp[2][0], pos_temp[2][1],
                                  pos_temp[3][0], pos_temp[3][1], (u16)(fac*100.0)); 

            continue;            
          } // if
          // ����0, 3����֮��ľ���
          x_differ = abs(pos_temp[0][0] - pos_temp[3][0]);  // x0 - x3
          y_differ = abs(pos_temp[0][1] - pos_temp[3][1]);  // y0 - y3
          temp0 = x_differ*x_differ + y_differ*y_differ;
          distance0 = sqrt(temp0); 
          // ����1, 2����֮��ľ���
          x_differ = abs(pos_temp[1][0] - pos_temp[2][0]);  // x1 - x2
          y_differ = abs(pos_temp[1][1] - pos_temp[2][1]);  // y1 - y2
          temp1 = x_differ*x_differ + y_differ*y_differ;
          distance1 = sqrt(temp1); 
          fac = (float)distance0/distance1;
          printf("distance0: %d, distance1: %d, fac: %g\r\n", distance0, distance1, fac);
          // �жϻ�ȡ��У׼���Ƿ�ϸ�
          if (fabs(fac - 1.0) > FAC_ERR)
          {
            point_index = 0;  // ��Ҫ����У׼
            TOUCH_DrawCalibratePoint(20, LCD_HEIGHT - 20, LCD_BACK_COLOR); // ���У׼��3
            TOUCH_DrawCalibratePoint(20, 20, LCD_TEXT_COLOR);  // ����У׼��0 
            LCD_ClearSingleLine(LINE4);   
            LCD_DisplayStringLine(56, LINE4, "Click #0, please.");               
            // ��ʾ�ϴ�У׼����
            printf("#0, #3��#1, #2����Ա߲����!!\n");
            // LCD_Clear(LCD_BACK_COLOR);
            TOUCH_DisplayCaliInfo(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], 
                                  pos_temp[1][1], pos_temp[2][0], pos_temp[2][1],
                                  pos_temp[3][0], pos_temp[3][1], (u16)(fac*100.0));                
            continue;
          } // if
          // �ж϶Խ����Ƿ����
          // ����0, 2����֮��ľ���
          x_differ = abs(pos_temp[0][0] - pos_temp[2][0]);  // x0 - x2
          y_differ = abs(pos_temp[0][1] - pos_temp[2][1]);  // y0 - y2
          temp0 = x_differ*x_differ + y_differ*y_differ;
          distance0 = sqrt(temp0);   
          // ����1, 3����֮��ľ���
          x_differ = abs(pos_temp[1][0] - pos_temp[3][0]);  // x1 - x3
          y_differ = abs(pos_temp[1][1] - pos_temp[3][1]);  // y1 - y3
          temp1 = x_differ*x_differ + y_differ*y_differ;
          distance1 = sqrt(temp1);  
          fac = (float)distance0/distance1; 
          printf("distance0: %d, distance1: %d, fac: %g\r\n", distance0, distance1, fac);
          // �жϻ�ȡ��У׼���Ƿ�ϸ�
          if (fabs(fac - 1.0) > FAC_ERR)
          {
            point_index = 0;  // ��Ҫ����У׼
            TOUCH_DrawCalibratePoint(20, LCD_HEIGHT - 20, LCD_BACK_COLOR); // ���У׼��3
            TOUCH_DrawCalibratePoint(20, 20, LCD_TEXT_COLOR);  // ����У׼��0 
            LCD_ClearSingleLine(LINE4);   
            LCD_DisplayStringLine(56, LINE4, "Click #0, please.");               
            // ��ʾ�ϴ�У׼����
            printf("�Խ��߲����!!\n");
            // LCD_Clear(LCD_BACK_COLOR);
            TOUCH_DisplayCaliInfo(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], 
                                  pos_temp[1][1], pos_temp[2][0], pos_temp[2][1],
                                  pos_temp[3][0], pos_temp[3][1], (u16)(fac*100.0));    
            continue;            
          } // if
          // �������л�ȡ��У׼����ϸ�, ������
          g_touch.xfac = (float)(LCD_WIDTH - 40)/(pos_temp[1][0] - pos_temp[0][0]);  // �õ�xfac
          g_touch.xoff = (LCD_WIDTH - g_touch.xfac*(pos_temp[1][0] + pos_temp[0][0]))/2;  // �õ�xoff
          g_touch.yfac = (float)(LCD_HEIGHT - 40)/(pos_temp[3][1] - pos_temp[0][1]);  // �õ�yfac
          g_touch.yoff = (LCD_HEIGHT - g_touch.yfac*(pos_temp[3][1] + pos_temp[0][1]))/2;  // �õ�yoff
          printf("xfac: %g, yfac: %g.\r\n", g_touch.xfac, g_touch.yfac);  
          printf("xoff: %d, yoff: %d.\r\n", g_touch.xoff, g_touch.yoff);

          // У׼�ɹ�
          TOUCH_SaveCalibrateData();  // ����У׼���� 
          LCD_Clear(LCD_BACK_COLOR);  // ����
          LCD_DisplayStringLine(8, LINE5, "      Congratulation!");
          LCD_DisplayStringLine(8, LINE7, "Calibrated successfully! ^_^"); 
          printf("      Congratulation!\r\n");  
          printf("Calibrated successfully! ^_^\r\n"); 
          DelayMs(1000);
          LCD_Clear(LCD_BACK_COLOR);  // ����

          return; // У׼���
         // break; 
          default: break;                             
      } // switch
      ++point_index;
    } else {  // û����Ļ����¼�����
      // LCD_ClearSingleLine(LINE2);
      // LCD_DisplayStringLine(44, LINE2, "Oops, no click! 0_#");
    } // if else
    DelayMs(10);
    ++out_times;
    // У׼ʱ�䵽��10s, �����Ƿ�У׼�ɹ���ֱ���˳�
    if (out_times >= 10000)
    {
      out_times = 0;
      LCD_Clear(LCD_BACK_COLOR);
      LCD_DisplayStringLine(16, LINE5, "No way! 10 seconds elapsed.");
      TOUCH_ReadCalibrateData();  // ��ȡ��ĻУ׼����
      break;
    } // if
  } // while
} // TOUCH_Calibrate()

/************************************************ 
* ��  ��: TOUCH_Init(void)
* ��  ��: ��������ʼ��
*
* ��  ��: ��
*
* ����ֵ: �� 
*************************************************/
void TOUCH_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ʱ��ʹ��
  RCC_APB2PeriphClockCmd(TOUCH_CS_RCC | TOUCH_CLK_RCC | TOUCH_PEN_RCC |
                         TOUCH_IN_RCC | TOUCH_OUT_RCC, ENABLE);
  // TOUCH_CS, TOUCH_CLK, TOUCH_INΪ���
  // TOUCH_CS
  GPIO_InitStructure.GPIO_Pin = TOUCH_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TOUCH_CS_GPIO, &GPIO_InitStructure);
  // TOUCH_CLK
  GPIO_InitStructure.GPIO_Pin = TOUCH_CLK_PIN;
  GPIO_Init(TOUCH_CLK_GPIO, &GPIO_InitStructure);
  // TOUCH_IN, ����������, stm32���, MOSI
  GPIO_InitStructure.GPIO_Pin = TOUCH_IN_PIN;
  GPIO_Init(TOUCH_IN_GPIO, &GPIO_InitStructure); 
  // TOUCH_PEN, TOUCH_OUTΪ����
  // TOUCH_PEN
  GPIO_InitStructure.GPIO_Pin = TOUCH_PEN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // ��������
  GPIO_Init(TOUCH_PEN_GPIO, &GPIO_InitStructure);
  // TOUCH_OUT, ���������, stm32����, MISO
  GPIO_InitStructure.GPIO_Pin = TOUCH_OUT_PIN;
  GPIO_Init(TOUCH_OUT_GPIO, &GPIO_InitStructure);  

  // SPI start.
  // TOUCH_CLK_CLR;  // CLK, 0
  TOUCH_CS_SET;   // CS, 1
  TOUCH_IN_SET;   // IN, 1
  TOUCH_CLK_SET;  // CLK, 1
  TOUCH_PEN_SET;  // PEN, 1
  // TOUCH_CS_CLR;   // CS, 0
  // �ж���Ļ�Ƿ��Ѿ�У׼��
  TOUCH_ReadXandY(&g_touch.x_now, &g_touch.y_now); // ��һ�ζ�ȡ��ʼ��
  // AT24CXX_Init(); // ��ʼ��24C02, ��24C02�ж�ȡ��У׼��־;
  // 24C02ʹ��������PB6��PB7, ��ʹ�ô�������ͬʱ����ʹ��24C02;
  // �����ʹ��stm32Ƭ��flash�洢
  // δ���й�У׼
  if (!TOUCH_ReadCalibrateData())
  {
    TOUCH_Calibrate();  // ��ĻУ׼, ������У׼��ز���
  } // if
  printf("Touch screen was initialized successfully!\r\n");
} // TOUCH_Init()
