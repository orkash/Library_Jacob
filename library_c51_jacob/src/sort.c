/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: sort.c  
* 文件标识: 见配置管理计划书
* 摘    要: 常见排序算法实现
* 函    数: SwapFloat() - 交换两个元素, float型
*           BubbleSortFloat() - 冒泡排序, float型
*           SelectSortFloat() - 选择排序, float型
*           InsertSortFloat() - 插入排序, float型
*           QuickSortFloat() - 快速排序, float型
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年3月26日
* 更改内容: E0.0.1, 实现了5种常见排序算法, 2015年3月26日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
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
* 函  数: SwapFloat(float *, float *)
* 功  能: 交换两个元素, float型
*
* 参  数: a - 待交换的一个元素
*         b - 待交换的另一个元素
*
* 返回值: 无
*************************************************/
void SwapFloat(float *a, float *b)
{
  float temp = *a;

  if (a == b) return;
  *a = *b;
  *b =  temp;
} // SwapFloat()

/************************************************ 
* 函  数: BubbleSortFloat(float [], unsigned int)
* 功  能: 冒泡排序, float型, (无序区, 有序区);
*         从无序区通过交换找出最大元素放到有序区前端。
*
* 参  数: arr[] - 待排序的数组
*         len - 待排序的元素个数
*
* 返回值: 无
*************************************************/
void BubbleSortFloat(float arr[], unsigned int len)
{
  unsigned int i = 0, j = 0;
  // float temp = 0.0;
  
  for (i = 0; i < len - 1; ++i)
  {
    // 找出无序区中的最大值，
    // 并放在有序区的前端。
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
* 函  数: SelectSortFloat(float [], unsigned int)
* 功  能: 选择排序, float型, (有序区, 无序区);
*         在无序区里找一个最小的元素跟在有序区的后面;
*         对数组：比较得多，换得少。比冒泡排序快一点儿。
*
* 参  数: arr[] - 待排序的数组
*         len - 待排序的元素个数
*
* 返回值: 无
*************************************************/
void SelectSortFloat(float arr[], unsigned int len)
{
  unsigned int i = 0, j = 0, min_index = 0;
  // float temp = 0.0;
  
  for (i = 0; i < len - 1; ++i)
  {
    min_index = i;
    for (j = i + 1; j < len; ++j)
    { // 找出无序区中的最小值
    	// if (arr[min_index] > arr[j])
      IF(arr[min_index], arr[j])
      {
        min_index = j;
      } // if
    } // for
    // 如果有序区后的第一个无序区数据不是最小的，
    // 则将它与无序区中最小值交换。
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
* 函  数: InsertSortFloat(float [], unsigned int)
* 功  能: 插入排序, float型, (有序区, 无序区);
*         把无序区的第一个元素插入到有序区的合适的位置;
*         对数组：比较得少，换得多。可用于少量数据（量级小于千）的排序。
*
* 参  数: arr[] - 待排序的数组
*         len - 待排序的元素个数
*
* 返回值: 无
*************************************************/
void InsertSortFloat(float arr[], unsigned int len)
{
  int i = 0, j = 0;
  int first = 0, last = len - 1;
  float temp = 0.0;

  for (i = first + 1; i <= last; ++i)
  {
    temp = arr[i];
    // 与有序区数据逐一比较，大于temp时，该数后移。
    // 当first=0，j循环到-1时，由于短路求值，不会运算arr[-1]。
    // for (j = i - 1; j >= first && arr[j] > temp; --j)
    for (j = i - 1; j >= first; --j)
    {
      IF(arr[j], temp) 
        arr[j + 1] = arr[j];
      else
        break;
    } // for
    // 被排序数放到正确的位置。
    arr[j + 1] = temp;  
  } // for
} // InsertSortFloat()

/************************************************ 
* 函  数: InsertSortFloat(float [], unsigned int)
* 功  能: 快速排序, float型
*         将比枢纽值(这里取的数组第一个元素)大的放在右边,
*         再把比枢纽值(这里取的数组第一个元素)小的放在左边边。(升序)
*
* 参  数: arr[] - 待排序的数组
*         len - 待排序的元素个数
*
* 返回值: 无
*************************************************/
// 快速排序, float型
void QuickSortFloat(float arr[], unsigned int len)
{
  unsigned int left = 0, right = len - 1;
  float pivot_value = arr[0]; // 指定数组第一个元素为参考值

  if (len < 2) return;  // 当数组元素少于2个时，结束排序
  while (left < right)  // 循环结束条件
  {
    // 从后向前搜索小于pivot_value的元素，找到后填充到arr[left]中并跳出循环。
    for (; right > left; --right)
    {
      // if (arr[right] < pivot_value)
      IF(pivot_value, arr[right])
      {
        arr[left++] = arr[right];
        break;
      } // if
    } // for
    // 从前向后搜索大于pivot_value的元素，找到后填充到arr[right]中并跳出循环。
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
  // 将保存在pivot_value的数恢复到arr[left]中
  arr[left] = pivot_value;
  // 所有元素：arr[0]~arr[left - 1], pivot_value, arr[left + 1]~arr[len - 1]
  // 递归，对pivot_value前left个元素进行排序
  QuickSortFloat(arr, left); 
  // 递归，对pivot_value后从pivot+1到len结尾的len-left-1个元素进行排序。
  QuickSortFloat(arr + left + 1, len - left - 1);  
} // QuickSortFloat()
