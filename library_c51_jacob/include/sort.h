/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: sort.h 
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
* 更改内容: E0.0.1, 完成了I2C相关8个函数, 2015年3月26日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#ifndef SORT_H_
#define SORT_H_

// 选择升序、降序
// #define ASCENDING_ORDER  // 升序
#define DECREASING_ORDER // 降序

// 交换两个元素, float型
static void SwapFloat(float *a, float *b);
// 冒泡排序, float型
void BubbleSortFloat(float arr[], unsigned int len);
// 选择排序, float型
void SelectSortFloat(float arr[], unsigned int len);
// 插入排序, float型
void InsertSortFloat(float arr[], unsigned int len);
// 快速排序, float型
void QuickSortFloat(float arr[], unsigned int len);

#endif  // #ifndef SORT_H_
