/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: sort.h 
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: ���������㷨ʵ��
* ��    ��: SwapFloat() - ��������Ԫ��, float��
*           BubbleSortFloat() - ð������, float��
*           SelectSortFloat() - ѡ������, float��
*           InsertSortFloat() - ��������, float��
*           QuickSortFloat() - ��������, float��
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��3��26��
* ��������: E0.0.1, �����I2C���8������, 2015��3��26��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#ifndef SORT_H_
#define SORT_H_

// ѡ�����򡢽���
// #define ASCENDING_ORDER  // ����
#define DECREASING_ORDER // ����

// ��������Ԫ��, float��
static void SwapFloat(float *a, float *b);
// ð������, float��
void BubbleSortFloat(float arr[], unsigned int len);
// ѡ������, float��
void SelectSortFloat(float arr[], unsigned int len);
// ��������, float��
void InsertSortFloat(float arr[], unsigned int len);
// ��������, float��
void QuickSortFloat(float arr[], unsigned int len);

#endif  // #ifndef SORT_H_
