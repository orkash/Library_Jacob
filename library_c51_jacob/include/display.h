/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: display.h
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: ������ÿһλ���ΪASCII��ʾ
* ��    ��: ConvertDataTo10Bits() - �����ݲ��Ϊ6��ASCII��ʽ���ݣ����㲹0
*           Display10BitsData() - ͨ��������ʾ6��ASCII��ʽ����, ���λΪ����λ
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��3��2��
* ��������: E0.0.1, ��ɲ��Ϊ10����ASCII��ʾ, 2015��3��2��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#ifndef DISPLAY_H_
#define DISPLAY_H_

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // end of #ifndef

// �Ƿ�ʹ��Display10BitsData()����, 
// ��������������δʹ��ĳһ�������ľ��档
#define ENABLE_DISPLAY TRUE

// ��C51��ʹ�û���Windows��ʹ��
// USED_IN_C51, USED_IN_WIN
#define USED_IN_WIN

#if defined(USED_IN_C51)
#define ENABLE_IN_51 TRUE
#define ENABLE_IN_WIN FALSE
#elif defined(USED_IN_WIN)
#define ENABLE_IN_51 FALSE
#define ENABLE_IN_WIN TRUE
#else
#error "Neither USED_IN_C51 nor USED_IN_WIN was defined!!"
#endif  // #if defined()

// �����ݲ��Ϊ6��10λ��ʽ��ʾ
void ConvertDataTo10Bits(unsigned char *display_ptr, int displayed_data);
#if ENABLE_DISPLAY  // �Ƿ�ʹ��Display10BitsData()����
// ��ʾ10λ����
void Display10BitsData(int displayed_data);
#endif // #if ENABLE_DISPLAY

#endif  // end of #ifndef DISPLAY_H_
