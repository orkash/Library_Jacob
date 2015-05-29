/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: filter.h 
* 文件标识: 见配置管理计划书
* 摘    要: 常用滤波函数
* 函    数: AvgFilterInt() - int型数据算术平均值滤波
*           RecurAvgFilterFloat() - float型数据递推平均滤波
*           RecurAvgFilterInt() - int型数据递推平均滤波
*           MedianFilterInt() - int型数据中位值滤波，单通道
*           MedianFilterMultiChannelInt() - int型数据多通道中位值滤波
*           FirstOrderLagFilter() - 一阶滞后滤波
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年3月18日
* 更改内容: E0.0.1, 完成了以上6个常用滤波函数, 2015年3月18日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#ifndef FILTER_H_
#define FILTER_H_

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // end of #ifndef

// 是否使用其中一个滤波函数,
// 可以消除编译器未使用某一个函数的警告。
#define ENABLE_AvgFilterInt TRUE
#define ENABLE_RecurAvgFilterFloat TRUE
#define ENABLE_RecurAvgFilterInt TRUE
#define ENABLE_MedianFilterInt TRUE
#define ENABLE_MedianFilterMultiChannelInt TRUE
#define ENABLE_FirstOrderLagFilter TRUE

// int型数据算术平均值滤波
#if ENABLE_AvgFilterInt
int AvgFilterInt(int unfiltered_data, unsigned int data_count);
#endif // #if ENABLE_AvgFilterInt
// float型数据递推平均滤波
#if ENABLE_RecurAvgFilterFloat
float RecurAvgFilterFloat(float unfiltered_data, unsigned int data_count);
#endif // #if ENABLE_RecurAvgFilterFloat
// int型数据递推平均滤波
#if ENABLE_RecurAvgFilterInt
int RecurAvgFilterInt(int unfiltered_data, unsigned int data_count);
#endif // #if ENABLE_RecurAvgFilterInt
// int型数据中位值滤波，单通道
#if ENABLE_MedianFilterInt
unsigned int MedianFilterInt(const unsigned int unfiltered_buf[], 
                             unsigned int buf_cnt);
#endif // #if ENABLE_MedianFilterInt
// int型数据多通道中位值滤波
#if ENABLE_MedianFilterMultiChannelInt
unsigned int MedianFilterMultiChannelInt(const unsigned int unfiltered_buf[], 
                                         unsigned int channel_index, 
                                         unsigned int buf_cnt);
#endif // #if ENABLE_MedianFilterMultiChannelInt
// 一阶滞后滤波
#if ENABLE_FirstOrderLagFilter
int FirstOrderLagFilter(int unfiltered_data);
#endif // #if ENABLE_FirstOrderLagFilter

#endif  // #ifndef FILTER_H_
