/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: ili9325.c  
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: LCD��ʾ��ILI9325����ILI9328����������ʹ����16λ����ģʽ, 240*320
* ��    ��: ILI9325_GPIO_Init() - ILI9325��������ӦIO������
*           LCD_Init() - LCD��ʼ��
*           LCD_SetCursor() - �趨���λ��
*           LCD_SetDisplayWindow() - �趨���ڴ�С
*           LCD_Clear() - �����ĻΪһ����ɫ
*           LCD_DisplayOn() - ������ر�LCD��ʾ
*           LCD_ReadPointColor() - ��ȡָ��������ɫֵ
*           LCD_DrawPoint() - ����һ����
*           LCD_DisplayChar() - ��ָ��������ʾһ��8x16�����ascii�ַ�
*           GetNumberBits() - �ж����ֵ���Чλ��
*           LCD_DisplayNumber() - ��ָ��������ʾ����
*           LCD_DisplayStringLine() - ��ָ��������ʾ�ַ���
*           LCD_DisplayStringMulLines() - ��ָ��������ʾ�����ַ���
*           LCD_DisplayMultipleLines() - ������ʾ�ַ���, ������Ļ��Ȳ��ֻ��Զ�������ʾ
*           LCD_DisplaySingleLine() - ��ʾ�����ַ���
*           LCD_ClearSingleLine() - �����������
*           LCD_ClearMultipleLines() - �����������
*           LCD_DrawBMP() - ��ָ�����귶Χ��ʾһ��ͼƬ
*           LCD_Test() - ��ʾ����, ����Һ�����Ƿ���������
*           LCD_BGR2RGB() - ��BGR��ɫת����RGB
*           ILI9325_Write() - ILI9325дָ�������
*           ILI9325_WriteCmd() - ��LCDдָ��
*           ILI9325_WriteData() - LCDд����
*           LCD_WriteRegister() - дָ����ַ�Ĵ�����ֵ
*           ILI9325_Read() - ��ȡILI9325�������ϵ�����
*           ILI9325_ReadData() - ��ȡLCD�������ϵ�����
*           LCD_ReadRegister() - ��ȡָ����ַ�Ĵ�����ֵ
*           LCD_RGB2Color() - ��RGB��ɫת��Ϊ16λ��ɫ
*           LCD_DrawLine() - ������ֹ����������߶�
*           LCD_DrawHorizontalLine() - ����ˮƽ�߶�
*           LCD_DrawVerticallLine() - ���ƴ�ֱ�߶�
*           LCD_DrawQuadrangle() - �����ı���
*           LCD_DrawRectangle() - ���ƾ���, �������ɫ
*           LCD_DrawSquare() - ����������, �������ɫ
*           LCD_DrawCircle() - ����Բ, �������ɫ
* 
* ��ǰ�汾: A0.0.5
* ��    ��: Jacob Peng
* �������: 2015��5��8��
* ��������: A0.0.1, ����˻���ILI9325�ĳ�ʼ��, 2015��4��29��
*           A0.0.2, ������ASCII�ַ����ַ�����ʾ, 2015��5��5��
*           A0.0.3, ���������ֵ���ʾ�Լ��߶Ρ����Ρ������Ρ�Բ�Ļ���, 2015��5��6��
*           A0.0.4, �������Զ�������ʾ�ַ���������������ݺ���, 2015��5��7��
*           A0.0.5, ������ʾ��������'\n'���к͵��ʱ����ʱ���ַ�'-', 2015��5��8��
* 
* ȡ���汾: A0.0.4
* ԭ����  : Jacob Peng
* �������: 2015��5��7��
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
* ��  ��: LCD_Init(void)
* ��  ��: LCD��ʼ��
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void LCD_Init(void)
{ 
  u16 device_id = 0x0000;
  char id_buf[8] = {0};
  
  ILI9325_GPIO_Init();  // ����ILI9325�������������IO��
  // �����ȸ�λILI9325, �����ȡ���˿������ͺš�
  // ��ILI9325�ĸ�λ����ֱ�����ӵ�Ƭ����λ����ʱ�����Բ�ʹ����һ�䣿
  LCD_RST_SET;
  LCD_WriteRegister(R0, 0x0001); // Start OCS
  //DelayMs(20);
  device_id = LCD_ReadRegister(R0); // ���������ͺ�ILI9325
  // �ó�ʼ��ֻ��ILI9325, ILI9328��Ч
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
    LCD_Clear(LCD_BACK_COLOR);  // ��Ļ��������Ϊ��ɫ
    LCD_ClearSingleLine(LINE0); // ���������ϵ��һ�а���������
    LCD_DisplaySingleLine(LINE0, "Initialize successfully! ^_^");
    LCD_DisplaySingleLine(LINE1, "The LCD controller is "); 
    LCD_DisplayStringLine(176, LINE1, id_buf);
    LCD_DisplaySingleLine(LINE2, "==============================");
  } else {  // ����ʶ��Ŀ������ͺ�
    printf("Sorry, the 0x%X LCD controller was unrecognized!\r\n", device_id);
  } // if else
  // DelayMs(20);
} // LCD_Init()

/************************************************ 
* ��  ��: ILI9325_GPIO_Init(void)
* ��  ��: ILI9325��������ӦIO������
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void ILI9325_GPIO_Init(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;

  //----------- ��ʼ������������������� ----------//
  RCC_APB2PeriphClockCmd(LCD_DB0_7_RCC | LCD_DB8_15_RCC, ENABLE);
  //----------- ��8λ������ ----------//
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | 
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������
  GPIO_Init(LCD_DB0_7_GPIO, &GPIO_InitStructure);
  //----------- ��8λ������ ----------//
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | 
                                GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | 
                                GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(LCD_DB8_15_GPIO, &GPIO_InitStructure);
  //----------- �������� ----------//
  // ���ƽ� CS
  RCC_APB2PeriphClockCmd(LCD_CS_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
  GPIO_Init(LCD_CS_GPIO, &GPIO_InitStructure);
  // ���ƽ� RS
  RCC_APB2PeriphClockCmd(LCD_RS_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
  GPIO_Init(LCD_RS_GPIO, &GPIO_InitStructure);
  // ���ƽ� WR 
  RCC_APB2PeriphClockCmd(LCD_WR_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = LCD_WR_PIN;
  GPIO_Init(LCD_WR_GPIO, &GPIO_InitStructure);
  // ���ƽ� RD 
  RCC_APB2PeriphClockCmd(LCD_RD_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = LCD_RD_PIN;
  GPIO_Init(LCD_RD_GPIO, &GPIO_InitStructure);
  // ���ƽ� RST
  RCC_APB2PeriphClockCmd(LCD_RST_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN;
  GPIO_Init(LCD_RST_GPIO, &GPIO_InitStructure);
} // ILI9325_GPIO_Init()

/************************************************ 
* ��  ��: ILI9325_Write(u16)
* ��  ��: ��ILI9325дָ�������
*
* ��  ��: dat - ��д��ָ�������
*
* ����ֵ: ��
*************************************************/
void ILI9325_Write(u16 dat)
{
  // ��8λ����д��CPIOB��8λ
  LCD_DB0_7_GPIO->ODR = (LCD_DB0_7_GPIO->ODR & 0X00FF) | 
                        (((dat << 8) >> 8) << 8); 
  // ��8λ����д��CPIOC��8λ
  LCD_DB8_15_GPIO->ODR = (LCD_DB8_15_GPIO->ODR & 0XFF00) | 
                         (((dat >> 8) << 8) >> 8); 
} // ILI9325_Write()

/************************************************ 
* ��  ��: ILI9325_WriteCmd(u16)
* ��  ��: LCDдָ��
*
* ��  ��: cmd - ��д��ָ��
*
* ����ֵ: ��
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
* ��  ��: ILI9325_WriteData(u16)
* ��  ��: LCDд����
*
* ��  ��: dat - ��д������
*
* ����ֵ: ��
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
* ��  ��: LCD_WriteRegister(u16, u16)
* ��  ��: ��ָ���Ĵ���д��һ��ֵ
*
* ��  ��: index - �Ĵ�����ַ
*         dat - ��д������
*
* ����ֵ: ��
*************************************************/
void LCD_WriteRegister(u16 index, u16 dat)
{
  ILI9325_WriteCmd(index);
  ILI9325_WriteData(dat);
} // LCD_WriteRegister()

/************************************************ 
* ��  ��: ILI9325_Read(void)
* ��  ��: ��ȡILI9325�������ϵ�����
*
* ��  ��: ��
*
* ����ֵ: ��ȡ��������
*************************************************/
u16 ILI9325_Read(void)
{
  u16 ret_data = 0x0000;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  // ��ΪҪ��ȡ���ݣ����Խ�16������IO������Ϊ����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | 
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | 
                                GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | 
                                GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  // ��ȡ����
  ret_data = ((LCD_DB0_7_GPIO->IDR & 0XFF00) >> 8);  // ��8λ���ݶ�ȡCPIOB��8λ
  ret_data |= ((LCD_DB8_15_GPIO->IDR & 0X00FF) << 8);  // ��8λ���ݶ�ȡCPIOC��8λ
  // ���½�16������IO������Ϊ���
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | 
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | 
                                GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | 
                                GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  return ret_data;
} // ILI9325_Read()

/************************************************ 
* ��  ��: ILI9325_ReadData(void)
* ��  ��: ��ȡLCD�������ϵ�����
*
* ��  ��: ��
*
* ����ֵ: ��ȡ��������
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
* ��  ��: LCD_ReadRegister(u16)
* ��  ��: ��ȡָ����ַ�Ĵ�����ֵ
*
* ��  ��: index - �Ĵ�����ַ
*
* ����ֵ: ��ȡ��������
*************************************************/
u16 LCD_ReadRegister(u16 index)
{ 
  ILI9325_WriteCmd(index); 

  return ILI9325_ReadData();
} // LCD_ReadRegister()

/************************************************ 
* ��  ��: LCD_SetCursor(u16, u16)
* ��  ��: �趨���λ��
*
* ��  ��: x - ������
*         y - ������
*
* ����ֵ: ��
*************************************************/
void LCD_SetCursor(u8 x, u16 y)
{
  if (x > LCD_WIDTH || y > LCD_HEIGHT) return;

  LCD_WriteRegister(R32, x);  // ��
  LCD_WriteRegister(R33, y);  // ��
} // LCD_SetCursor()

/************************************************ 
* ��  ��: LCD_SetDisplayWindow(u8, u16, u16, u16)
* ��  ��: �趨��ʾ���ڴ�С
*
* ��  ��: xpos - ��һ���������
*         ypos - ��һ����������
*         width - ���ڿ��
*         height - ���ڸ߶�
*
* ����ֵ: ��
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
* ��  ��: LCD_Clear(u16)
* ��  ��: ����Ļ���Ϊһ����ɫ
*
* ��  ��: color - �����ɫ
*
* ����ֵ: ��
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
* ��  ��: LCD_DisplayOn(u8)
* ��  ��: ������ر�LCD��ʾ
*
* ��  ��: on_or_off - ������ر�
*
* ����ֵ: ��
*************************************************/
void LCD_DisplayOn(u8 on_or_off)
{
  if (1 == on_or_off) 
    LCD_WriteRegister(R7, 0x0133);
  else if (0 == on_or_off)
    LCD_WriteRegister(R7, 0x0000);
} // LCD_DisplayOn()

/************************************************ 
* ��  ��: LCD_ReadPointColor(u8 x, u16 y)
* ��  ��: ��ȡָ��������ɫֵ
*
* ��  ��: on_or_off - ������ر�
*
* ����ֵ: ��ȡ������ɫֵ
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
* ��  ��: LCD_DrawPoint(u8, u16, u16)
* ��  ��: ����һ����
*
* ��  ��: xpos - �õ������
*         ypos - �õ�������
*         color - �õ���ɫ
*
* ����ֵ: ��
*************************************************/
void LCD_DrawPoint(u8 xpos, u16 ypos, u16 color)
{
  if ((xpos > LCD_WIDTH) || (ypos > LCD_HEIGHT)) return;

  LCD_SetCursor(xpos, ypos);
  ILI9325_WriteCmd(R34);
  ILI9325_WriteData(color);
} // LCD_DrawPoint()

/************************************************ 
* ��  ��: LCD_DisplayChar(u8, u16, char)
* ��  ��: ��ָ��������ʾһ��8x16�����ascii�ַ�
*
* ��  ��: xpos - �õ������
*         ypos - �õ�������
*         ch - ����ʾ�ַ�
*
* ����ֵ: ��
*************************************************/
void LCD_DisplayChar(u8 xpos, u16 ypos, char ch)
{
  u8 ascii_temp = 0;
  u16 i = 0, j = 0, ypos_old = ypos;
  
  if ((xpos > LCD_WIDTH) || (ypos > LCD_HEIGHT)) return;
  ch = ch - ' ';  // �õ�ƫ�ƺ��ֵ
  for (i = 0; i < 16; ++i)
  {
    ascii_temp = g_ascii_1608[ch][i];
    for (j = 0; j < 8; ++j)
    {
      if (ascii_temp & 0x80)
      { 
        LCD_DrawPoint(xpos, ypos, LCD_TEXT_COLOR); // �ַ���ɫ
      } else {
        LCD_DrawPoint(xpos, ypos, LCD_BACK_COLOR); // ������ɫ
      } // if else
      ascii_temp <<= 1;
      ++ypos;
      if (xpos >= LCD_WIDTH) return; // ������ʾ��Χ
      if (16 == (ypos - ypos_old))
      {
        ypos = ypos_old;
        ++xpos;
        if (xpos >= LCD_WIDTH) return; // ������ʾ��Χ
        break;
      } // if
    } // for
  } // for 
} // LCD_DisplayChar()

/************************************************ 
* ��  ��: GetNumberBits(u32)
* ��  ��: �ж����ֵ���Чλ��
*
* ��  ��: num - ���жϵ�����
*
* ����ֵ: �����ֵ���Чλ��
*************************************************/
u8 GetNumberBits(u32 num)
{
  u8 ret_bits = 1;  // ����������һλ

  while (num)
  {
    num /= 10;
    // ֻ�е�num����10��Ϊ0ʱ, �ô�λ����������
    if (num) ++ret_bits;
  } // while

  return ret_bits;
} // GetNumberBits()

/************************************************ 
* ��  ��: LCD_DisplayNumber(u8, u16, u32)
* ��  ��: ��ָ��������ʾ����
*
* ��  ��: x - �õ������
*         y - �õ�������
*         num - ����ʾ����, ��Ч��Χ0 ~ 4294967295
*
* ����ֵ: ��
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
* ��  ��: LCD_DisplayStringLine(u8, u16, const char [])
* ��  ��: ��ָ��������ʾ�����ַ���
*
* ��  ��: xpos - ��������
*         ypos - ���������
*         str - ����ʾ�ַ���
*
* ����ֵ: ��
*************************************************/
void LCD_DisplayStringLine(u8 xpos, u16 ypos, const char str[])
{
  u8 i = 0;
  
  if ((xpos > LCD_WIDTH) || (ypos > LCD_HEIGHT)) return;
  // �ж��ǲ��ǷǷ��ַ�!
  while((str[i] <= '~') && (str[i] >= ' '))
  {
    LCD_DisplayChar((xpos + LINE_WIDTH*i), ypos, str[i]);
    ++i;
  } // while
} // LCD_DisplayStringLine()

/************************************************ 
* ��  ��: LCD_DisplayStringMulLines(u8, u16, u8, u16, const char [])
* ��  ��: ��ָ��������ʾ�����ַ���
*
* ��  ��: xpos - ��������
*         ypos - ���������
*         width - ��ʾ������
*         height - ��ʾ����߶�
*         str - ����ʾ�ַ���
*
* ����ֵ: ��
*************************************************/
void LCD_DisplayStringMulLines(u8 xpos, u16 ypos, u8 width, u16 height, const char str[])
{
  u8 x_start = xpos;  // �������λ��
  u16 i = 0;

  if ((xpos + width > LCD_WIDTH) || (ypos > LCD_HEIGHT) ||
      (height > LCD_HEIGHT)) return;
  width += xpos;
  height += ypos;
  // �ж��ǲ��ǷǷ��ַ�!
  while((str[i] <= '~' && str[i] >= ' ') || '\n' == str[i]) 
  {
    if (ypos >= height) break;  // ��ʾ�����Ѿ���������, �˳�
    if (str[i] != '\n') 
    {
      LCD_DisplayChar(xpos, ypos, str[i]);
      xpos += LINE_WIDTH;  // ����x��ʾ����, ����һ���ַ����      
    } // if
    // �����ֻ��з�'\n', ����
    // �����һ����ʾ�����Ѿ����������ʾ��Χ���ʱ, �Զ�����
    if (('\n' == str[i] || xpos + LINE_WIDTH >= width) )
    {
      // ������ĳһ�����ʻ��б����ʱ, ʹ�����ַ�'-'���ô���������;
      // ֻ�е�ǰ�ַ�����һ���ַ���Ϊ��ĸʱ, ����Ϊ��һ�������ʱ����;
      // �����ǰ�ַ�����һ���ַ���Ϊ'\n'ʱ, ����ʾ���ַ�'-'
      if ((isalpha(str[i]) && isalpha(str[i + 1])) && 
          !('\n' == str[i - 1] && '\n' == str[i]))
      {
        LCD_DisplayChar(xpos, ypos, '-');        
      } // if
      // ����
      xpos = x_start; // x�����ƶ�����ʼλ��
      ypos += LINE_HEIGHT; // y��������һ���и�
    } // if
    ++i;  // �ж���һ���ַ�
  } // while
} // LCD_DisplayStringMulLine()

/************************************************ 
* ��  ��: LCD_DisplaySingleLine(u16, const char [])
* ��  ��: ������ʾ�ַ���, ������Ļ��Ȳ��ֲ�����ʾ
*
* ��  ��: line - ����
*         str - ����ʾ�ַ���
*
* ����ֵ: ��
*************************************************/
void LCD_DisplaySingleLine(u16 line, const char str[])
{
  // LCD_DisplayStringLine(0, line, str);
  LCD_DisplayStringMulLines(0, line, LCD_WIDTH, LINE_HEIGHT, str);
} // LCD_DisplaySingleLine()

/************************************************ 
* ��  ��: LCD_DisplayMultipleLines(u16, const char [])
* ��  ��: ������ʾ�ַ���, ������Ļ��Ȳ��ֻ��Զ�������ʾ
*
* ��  ��: line - ����
*         str - ����ʾ�ַ���
*
* ����ֵ: ��
*************************************************/
void LCD_DisplayMultipleLines(u16 line, const char str[])
{
  LCD_DisplayStringMulLines(0, line, LCD_WIDTH, LCD_HEIGHT, str);
} // LCD_DisplayMultipleLines()

/************************************************ 
* ��  ��: LCD_ClearSingleLine(u16 line_num)
* ��  ��: �����������
*
* ��  ��: line_num - ����
*
* ����ֵ: ��
*************************************************/
void LCD_ClearSingleLine(u16 line_num)
{
  // һ��30���ո�, 240 = 30*8
  LCD_DisplaySingleLine(line_num, "                              ");
} // LCD_ClearSingleLine()

/************************************************ 
* ��  ��: LCD_ClearSingleLine(u16 start_line)
* ��  ��: �����������
*
* ��  ��: start_line - �к�, ��ʼ��
*         lines - ����, �ܹ�����
*
* ����ֵ: ��
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
* ��  ��: LCD_Test(void)
* ��  ��: ��ʾ����, ����Һ�����Ƿ���������
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void LCD_Test(void)
{
  u8 r_data = 0, g_data = 0, b_data = 0, i = 0, j = 0;

  LCD_SetCursor(0x00, 0x0000);
  LCD_WriteRegister(R80, 0x0000);  // ˮƽGRAM��ʼλ��
  LCD_WriteRegister(R81, LCD_WIDTH - 1); // ˮƽGRAM��ֹλ��
  LCD_WriteRegister(R82, 0); // ��ֱGRAM��ʼλ��
  LCD_WriteRegister(R83, LCD_HEIGHT - 1); // ��ֱGRAM��ֹλ��  
  LCD_CS_CLR; 
  ILI9325_WriteCmd(R34);
  LCD_RS_SET;
  // ��ɫ��ǿ��
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
  // ��ɫ��ǿ��
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
  // ��ɫ��ǿ��
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
* ��  ��: LCD_BGR2RGB(u16)
* ��  ��: ��BGR��ɫת����RGB
*
* ��  ��: color - ��ת��BGR��ɫֵ
*
* ����ֵ: ת���������RGB��ɫֵ
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
* ��  ��: LCD_RGB2Color(u32)
* ��  ��: ��RGB��ɫת��Ϊ16λ��ɫ
*
* ��  ��: rgb_value - RGB��ɫֵ
*
* ����ֵ: 16λ��ɫֵ
*************************************************/
u16 LCD_RGB2Color(u32 rgb_value)
{
  u8  r, g, b;

  r = (rgb_value >> (16 + 3)) & 0x1f; // ȡRɫ�ĸ�5λ
  g = (rgb_value >> (8 + 2)) & 0x3f;  // ȡGɫ�ĸ�6λ
  b = (rgb_value >> (0 + 3)) & 0x1f;  // ȡBɫ�ĸ�5λ

  return ((r << 11) + (g << 5) + (b << 0));   
} // LCD_RGB2Color()

/************************************************ 
* ��  ��: LCD_DrawLine(u8, u16, u8, u16, u16 color)
* ��  ��: ������ֹ����������߶�
*
* ��  ��: x0 - ��������
*         y0 - ���������
*         x1 - �յ������
*         y1 - �յ�������
*         color - �߶���ɫ
*
* ����ֵ: ��
*************************************************/
void LCD_DrawLine(u8 x0, u16 y0, u8 x1, u16 y1, u16 color)
{
  u16 i = 0, row = 0, column = 0; 
  int x_err = 0, y_err = 0; 
  int x_dir = 0, y_dir = 0; 
  int x_incremt = 0, y_incremt = 0, distance = 0; 

  if (x0 > LCD_WIDTH || y0 > LCD_HEIGHT || 
      x1 > LCD_WIDTH || y1 > LCD_HEIGHT) return;
  x_incremt = x1 - x0; // ������������ 
  y_incremt = y1 - y0; 
  row = x0; 
  column = y0; 
  if (x_incremt > 0) {
    x_dir = 1; // ���õ������� 
  } else if (0 == x_incremt) {
    x_dir = 0; // ��ֱ�� 
  } else {
    x_dir = -1;
    x_incremt = -x_incremt;
  } // else
  if (y_incremt > 0) {
    y_dir = 1; 
  } else if (0 == y_incremt) {
    y_dir = 0;  // ˮƽ�� 
  } else {
    y_dir = -1;
    y_incremt = -y_incremt;
  } 
  if( x_incremt > y_incremt)
    distance = x_incremt; // ѡȡ�������������� 
  else 
    distance = y_incremt; 
  // �������
  for (i = 0; i <= distance + 1; ++i) 
  {  
    LCD_DrawPoint(row, column, color); // ���� 
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
* ��  ��: LCD_DrawHorizontalLine(u8, u16, u8, u16)
* ��  ��: ����ˮƽ�߶�
*
* ��  ��: x0 - ��������
*         y0 - ���������
*         width - �߶γ���
*         color - �߶���ɫ
*
* ����ֵ: ��
*************************************************/
void LCD_DrawHorizontalLine(u8 x0, u16 y0, u8 width, u16 color)
{
  LCD_DrawLine(x0, y0, x0 + width, y0, color);
} // LCD_DrawHorizontalLine()

/************************************************ 
* ��  ��: LCD_DrawVerticallLine(u8, u16, u8, u16)
* ��  ��: ���ƴ�ֱ�߶�
*
* ��  ��: x0 - ��������
*         y0 - ���������
*         height - �߶γ���
*         color - �߶���ɫ
*
* ����ֵ: ��
*************************************************/
void LCD_DrawVerticallLine(u8 x0, u16 y0, u16 height, u16 color)
{
  LCD_DrawLine(x0, y0, x0, y0 + height, color);
} // LCD_DrawVerticallLine()

/************************************************ 
* ��  ��: LCD_DrawQuadrangle(u8, u16, u8, u16, u16)
* ��  ��: �����ı���
*
* ��  ��: x0 - ��������
*         y0 - ���������
*         x1 - �յ������
*         y1 - �յ�������
*         color - �߶���ɫ
*
* ����ֵ: ��
*************************************************/
void LCD_DrawQuadrangle(u8 x0, u16 y0, u8 x1, u16 y1, u16 color)
{
  LCD_DrawLine(x0, y0, x1, y0, color); // ˳ʱ�����
  LCD_DrawLine(x1, y0, x1, y1, color);
  LCD_DrawLine(x1, y1, x0, y1, color);
  LCD_DrawLine(x0, y1, x0, y0, color);
} // LCD_DrawQuadrangle()

/************************************************ 
* ��  ��: LCD_DrawRectangle(u8, u16, u8, u16, u16, u8)
* ��  ��: ���ƾ���, �������ɫ
*
* ��  ��: x0 - ��������
*         y0 - ���������
*         width - ���ο��
*         height - ���θ߶�
*         color - ������ɫ
*         fill - �Ƿ������ɫ
*
* ����ֵ: ��
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
* ��  ��: LCD_DrawSquare(u8, u16, u16, u16, u8)
* ��  ��: ����������, �������ɫ
*
* ��  ��: x0 - ��������
*         y0 - ���������
*         side_lenght - �����α߳�
*         color - ������ɫ
*         fill - �Ƿ������ɫ
*
* ����ֵ: ��
*************************************************/
void LCD_DrawSquare(u8 x0, u16 y0, u16 side_lenght, u16 color, u8 fill)
{
  LCD_DrawRectangle(x0, y0, side_lenght, side_lenght, color, fill);
} // LCD_DrawSquare()

/************************************************ 
* ��  ��: LCD_DrawCircle(u8, u16, u8, u16, u8)
* ��  ��: ����Բ, �������ɫ, ʹ��Bresenham�㷨
*
* ��  ��: x_center - Բ�ĺ�����
*         y_center - Բ��������
*         r - �뾶
*         color - ������ɫ
*         fill - �Ƿ������ɫ
*
* ����ֵ: ��
*************************************************/
void LCD_DrawCircle(u8 x_center, u16 y_center, u8 r, u16 color, u8 fill)
{
  u8 x_now = 0; // x���ڵ�ֵ
  u16 y_now = r;  // y���ڵ�ֵ
  int decision = 3 - 2*r;  // ����ֵ

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
    // ʹ��Bresenham�㷨��Բ 
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
* ��  ��: LCD_DrawBMP(u8, u16, u8, u16, u16 [])
* ��  ��: ��ָ�����귶Χ��ʾһ��ͼƬ
*
* ��  ��: start_x - ��������
*         start_y - ���������
*         end_x - �յ������
*         end_y - �յ�������
*         pic - ����ʾͼƬ, ͼƬȡģ��ʽΪˮƽɨ��, 16λ��ɫģʽ
*
* ����ֵ: ��
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
