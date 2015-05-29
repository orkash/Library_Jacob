/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: sort.c  
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
* ��������: E0.0.1, ʵ����5�ֳ��������㷨, 2015��3��26��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#include "../include/sort.h"

#if defined(ASCENDING_ORDER)
#define IF(a, b) if ((a) > (b))
#elif defined(DECREASING_ORDER)
#define IF(a, b) if ((a) < (b))
#else 
#error "Neither ASCENDING_ORDER nor DECREASING_ORDER was defined!!"
#endif	// #if defined

/************************************************ 
* ��  ��: SwapFloat(float *, float *)
* ��  ��: ��������Ԫ��, float��
*
* ��  ��: a - ��������һ��Ԫ��
*         b - ����������һ��Ԫ��
*
* ����ֵ: ��
*************************************************/
void SwapFloat(float *a, float *b)
{
  float temp = *a;

  if (a == b) return;
  *a = *b;
  *b =  temp;
} // SwapFloat()

/************************************************ 
* ��  ��: BubbleSortFloat(float [], unsigned int)
* ��  ��: ð������, float��, (������, ������);
*         ��������ͨ�������ҳ����Ԫ�طŵ�������ǰ�ˡ�
*
* ��  ��: arr[] - �����������
*         len - �������Ԫ�ظ���
*
* ����ֵ: ��
*************************************************/
void BubbleSortFloat(float arr[], unsigned int len)
{
  unsigned int i = 0, j = 0;
  // float temp = 0.0;
  
  for (i = 0; i < len - 1; ++i)
  {
    // �ҳ��������е����ֵ��
    // ��������������ǰ�ˡ�
    for (j = 0; j < len - 1 - i; ++j)

    {
    	// if (arr[min_index] > arr[j])
    	IF(arr[j], arr[j + 1])
      {
        // temp = arr[j + 1];
        // arr[j + 1] = arr[j];
        // arr[j] = temp;
        SwapFloat(&arr[j], &arr[j + 1]);
      } // if
    } // for
  } // for
} // BubbleSortFloat()

/************************************************ 
* ��  ��: SelectSortFloat(float [], unsigned int)
* ��  ��: ѡ������, float��, (������, ������);
*         ������������һ����С��Ԫ�ظ����������ĺ���;
*         �����飺�Ƚϵö࣬�����١���ð�������һ�����
*
* ��  ��: arr[] - �����������
*         len - �������Ԫ�ظ���
*
* ����ֵ: ��
*************************************************/
void SelectSortFloat(float arr[], unsigned int len)
{
  unsigned int i = 0, j = 0, min_index = 0;
  // float temp = 0.0;
  
  for (i = 0; i < len - 1; ++i)
  {
    min_index = i;
    for (j = i + 1; j < len; ++j)
    { // �ҳ��������е���Сֵ
    	// if (arr[min_index] > arr[j])
      IF(arr[min_index], arr[j])
      {
        min_index = j;
      } // if
    } // for
    // �����������ĵ�һ�����������ݲ�����С�ģ�
    // ����������������Сֵ������
    if (min_index != i)
    {
      // temp = arr[min_index];
      // arr[min_index] = arr[i];
      // arr[i] = temp;
      SwapFloat(&arr[min_index], &arr[i]);
    } // if
  } // for
} // SelectSortFloat()

/************************************************ 
* ��  ��: InsertSortFloat(float [], unsigned int)
* ��  ��: ��������, float��, (������, ������);
*         ���������ĵ�һ��Ԫ�ز��뵽�������ĺ��ʵ�λ��;
*         �����飺�Ƚϵ��٣����öࡣ�������������ݣ�����С��ǧ��������
*
* ��  ��: arr[] - �����������
*         len - �������Ԫ�ظ���
*
* ����ֵ: ��
*************************************************/
void InsertSortFloat(float arr[], unsigned int len)
{
  int i = 0, j = 0;
  int first = 0, last = len - 1;
  float temp = 0.0;

  for (i = first + 1; i <= last; ++i)
  {
    temp = arr[i];
    // ��������������һ�Ƚϣ�����tempʱ���������ơ�
    // ��first=0��jѭ����-1ʱ�����ڶ�·��ֵ����������arr[-1]��
    // for (j = i - 1; j >= first && arr[j] > temp; --j)
    for (j = i - 1; j >= first; --j)
    {
      IF(arr[j], temp) 
        arr[j + 1] = arr[j];
      else
        break;
    } // for
    // ���������ŵ���ȷ��λ�á�
    arr[j + 1] = temp;  
  } // for
} // InsertSortFloat()

/************************************************ 
* ��  ��: InsertSortFloat(float [], unsigned int)
* ��  ��: ��������, float��
*         ������Ŧֵ(����ȡ�������һ��Ԫ��)��ķ����ұ�,
*         �ٰѱ���Ŧֵ(����ȡ�������һ��Ԫ��)С�ķ�����߱ߡ�(����)
*
* ��  ��: arr[] - �����������
*         len - �������Ԫ�ظ���
*
* ����ֵ: ��
*************************************************/
// ��������, float��
void QuickSortFloat(float arr[], unsigned int len)
{
  unsigned int left = 0, right = len - 1;
  float pivot_value = arr[0]; // ָ�������һ��Ԫ��Ϊ�ο�ֵ

  if (len < 2) return;  // ������Ԫ������2��ʱ����������
  while (left < right)  // ѭ����������
  {
    // �Ӻ���ǰ����С��pivot_value��Ԫ�أ��ҵ�����䵽arr[left]�в�����ѭ����
    for (; right > left; --right)
    {
      // if (arr[right] < pivot_value)
      IF(pivot_value, arr[right])
      {
        arr[left++] = arr[right];
        break;
      } // if
    } // for
    // ��ǰ�����������pivot_value��Ԫ�أ��ҵ�����䵽arr[right]�в�����ѭ����
    for (; left < right; ++left)
    {
      // if (arr[left] > pivot_value)
      IF(arr[left], pivot_value)
      {
        arr[right--] = arr[left];
        break;
      } // if
    } // for
  } // while 
  // ��������pivot_value�����ָ���arr[left]��
  arr[left] = pivot_value;
  // ����Ԫ�أ�arr[0]~arr[left - 1], pivot_value, arr[left + 1]~arr[len - 1]
  // �ݹ飬��pivot_valueǰleft��Ԫ�ؽ�������
  QuickSortFloat(arr, left); 
  // �ݹ飬��pivot_value���pivot+1��len��β��len-left-1��Ԫ�ؽ�������
  QuickSortFloat(arr + left + 1, len - left - 1);  
} // QuickSortFloat()
