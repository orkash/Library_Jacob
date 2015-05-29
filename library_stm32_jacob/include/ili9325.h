/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: ili9325.h 
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
#ifndef ILI9325_H_
#define ILI9325_H_

#include <stm32f10x.h>

// ���ݽӿ�����RCCʱ��
#define LCD_DB0_7_RCC   RCC_APB2Periph_GPIOB
#define LCD_DB8_15_RCC  RCC_APB2Periph_GPIOC
// ���ݽӿ�GPIOx
#define LCD_DB0_7_GPIO  GPIOB
#define LCD_DB8_15_GPIO GPIOC
// CS, RS, WR, RD, RST��������RCCʱ��
#define LCD_CS_RCC  RCC_APB2Periph_GPIOC
#define LCD_RS_RCC  RCC_APB2Periph_GPIOC
#define LCD_WR_RCC  RCC_APB2Periph_GPIOC
#define LCD_RD_RCC  RCC_APB2Periph_GPIOC
#define LCD_RST_RCC RCC_APB2Periph_GPIOC
// CS, RS, WR, RD, RST��������GPIOx
#define LCD_CS_GPIO   GPIOC
#define LCD_RS_GPIO   GPIOC
#define LCD_WR_GPIO   GPIOC
#define LCD_RD_GPIO   GPIOC
#define LCD_RST_GPIO	GPIOC
// CS, RS, WR, RD, RST�������Ŷ�ӦIO�ڱ��
#define LCD_CS_PIN  GPIO_Pin_11 // LCDƬѡ�ź�
#define LCD_RS_PIN  GPIO_Pin_8  // ����/���ݱ�־(0, ��д����; 1, ��д����)
#define LCD_WR_PIN  GPIO_Pin_9  // ��LCDд������
#define LCD_RD_PIN  GPIO_Pin_10 // ��LCD��ȡ����
#define LCD_RST_PIN GPIO_Pin_12 // LCD��λ�ź�, ��λ�ź����ֱ������stm32��λ����
// CS, RS, WR, RD, RST��1������
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
// ��Ļʵ�ʿ��
#define LCD_WIDTH   240
#define LCD_HEIGHT  320
// �иߡ��п�
#define LINE_WIDTH  8   // �ַ����Ϊ8
#define LINE_HEIGHT 20  // �ַ��߶�Ϊ16
// ��Ļ����, 320�� = 16��*20�и�
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
// ��ɫ
#define WHITE   0xFFFF
#define BLACK   0x0000
#define GREY    0xF7DE
#define BLUE    0x001F
#define BLUE2   0x051F
#define RED     0xF800
#define MAGENTA 0xF81F  // Ʒ��, ���
#define GREEN   0x07E0  
#define CYAN    0x7FFF  // ��ɫ
#define YELLOW  0xFFE0
// LCD�ı���ɫ�ͱ���ɫ
#define LCD_TEXT_COLOR WHITE
#define LCD_BACK_COLOR BLACK
// �Ƿ����ͼ��
#define FILL 1
#define NOT_FILL 0

/*----- High layer function -----*/
// LCD��ʼ��
void LCD_Init(void);
// ��ʾ����, ����Һ�����Ƿ���������
void LCD_Test(void);
// ����Ļ���Ϊһ����ɫ
void LCD_Clear(u16 color);
// ��ȡָ��������ɫֵ
u16 LCD_ReadPointColor(u8 x, u16 y);
// ��ָ��������ʾһ��8x16�����ascii�ַ�
void LCD_DisplayChar(u8 xpos, u16 ypos, char ch);
// ��ָ��������ʾ����
void LCD_DisplayNumber(u8 x, u16 y, u32 num);
// ��ָ��������ʾ�ַ���
void LCD_DisplayStringLine(u8 xpos, u16 ypos, const char str[]);
// ��ָ��������ʾ�����ַ���
void LCD_DisplayStringMulLines(u8 xpos, u16 ypos, u8 width, u16 height, const char str[]);
// ������ʾ�ַ���, ������Ļ��Ȳ��ֲ�����ʾ
void LCD_DisplaySingleLine(u16 line, const char str[]);
// ������ʾ�ַ���, ������Ļ��Ȳ��ֻ��Զ�������ʾ
void LCD_DisplayMultipleLines(u16 line, const char str[]);
// �����������
void LCD_ClearSingleLine(u16 line);
// �����������
void LCD_ClearMultipleLines(u16 start_line, u8 lines);
// ����ˮƽ�߶�
void LCD_DrawHorizontalLine(u8 x0, u16 y0, u8 width, u16 color);
// ���ƴ�ֱ�߶�
void LCD_DrawVerticallLine(u8 x0, u16 y0, u16 height, u16 color);
// ���ƾ���, �������ɫ
void LCD_DrawRectangle(u8 x0, u16 y0, u8 width, u16 height, u16 color, u8 fill);
// ����������, �������ɫ
void LCD_DrawSquare(u8 x0, u16 y0, u16 side_lenght, u16 color, u8 fill);
// ����Բ, �������ɫ
void LCD_DrawCircle(u8 x_center, u16 y_center, u8 r, u16 color, u8 fill);
// ��ָ�����귶Χ��ʾһ��ͼƬ
void LCD_DrawBMP(u8 start_x, u16 start_y, u8 end_x, u16 end_y, const u16 pic[]);

/*----- Medium layer function -----*/
// ILI9325��������ӦIO������
void ILI9325_GPIO_Init(void);
// �趨���λ��
void LCD_SetCursor(u8 x, u16 y);
// �趨��ʾ���ڴ�С
void LCD_SetDisplayWindow(u8 xpos, u16 ypos, u8 width, u16 height);
// ������ر�LCD��ʾ
void LCD_DisplayOn(u8 on_or_off);
// ��BGR��ɫת����RGB
u16 LCD_BGR2RGB(u16 color);
// ��RGB��ɫת��Ϊ16λ��ɫ
u16 LCD_RGB2Color(u32 rgb_value);
// ��ָ���Ĵ���д��һ��ֵ
void LCD_WriteRegister(u16 index, u16 dat);
// ��ȡָ����ַ�Ĵ�����ֵ
u16 LCD_ReadRegister(u16 index);
// ����һ����
void LCD_DrawPoint(u8 xpos, u16 ypos, u16 color);
// ������ֹ����������߶�
void LCD_DrawLine(u8 x0, u16 y0, u8 x1, u16 y1, u16 color);
// �����ı���
void LCD_DrawQuadrangle(u8 x0, u16 y0, u8 x1, u16 y1, u16 color);

/*----- Low layer function -----*/
// ��ILI9325дָ�������
void ILI9325_Write(u16 dat);
// LCDдָ��
void ILI9325_WriteCmd(u16 cmd);
// LCDд����
void ILI9325_WriteData(u16 dat);
// ��ȡILI9325�������ϵ�����
u16 ILI9325_Read(void);
// ��ȡLCD�������ϵ�����
u16 ILI9325_ReadData(void);

//void Touch_Initializtion(void);
//u16  Touch_GetPhyX(void);
//u16  Touch_GetPhyY(void);
//u32  Touch_MeasurementX(void);
//u32  Touch_MeasurementY(void);

#endif  // #ifndef ILI9325_H_
