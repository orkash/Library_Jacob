/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: filter.c  
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
#include <string.h> // Declare memcpy().

/************************************************ 
* 函  数: AvgFilterInt(int, unsigned int)
* 功  能: int型数据算术平均值滤波
*
* 参  数: unfiltered_data - 待滤波数据, 常用AD值
*         data_count - 求平均值个数
*
* 返回值: 滤波后的平均值, int型
*************************************************/
#if ENABLE_AvgFilterInt
int AvgFilterInt(int unfiltered_data, unsigned int data_count)
{
  static unsigned int s_avg_cnt = 0;
  static int s_ret_avg = 0;  
  static long int s_sum = 0;

  if (s_avg_cnt < data_count)
  {
    s_sum += unfiltered_data;
    ++s_avg_cnt;
  } else {
    s_ret_avg = (int)(s_sum/s_avg_cnt);
    s_sum = 0;
    s_avg_cnt = 0;
  } // else 

  return s_ret_avg;
} // AvgFilterInt()
#endif // #if ENABLE_AvgFilterInt

/************************************************ 
* 函  数: RecurAvgFilterFloat(float, unsigned int)
* 功  能: float型数据递推平均滤波, 递推即0 to (n-1), 1 to n,
*         每次向后推一个, 总共n个数据求平均值
*
* 参  数: unfiltered_data - 待滤波数据, 常用AD值
*         data_count - 求平均值个数
*
* 返回值: 滤波后的平均值, float型
*************************************************/
#if ENABLE_RecurAvgFilterFloat
float RecurAvgFilterFloat(float unfiltered_data, unsigned int data_count)
{
  unsigned int i = 0;
  double ret_sum = 0;
  static float s_data_buffer[32] = {0.0};

  if (data_count > 32) return 0;  // 输入参数有误，返回错误。
  // 等待滤波的数据赋给数组最后一个值
  s_data_buffer[data_count - 1] = unfiltered_data;  
  for (i = 0; i < data_count; ++i)
  {
    s_data_buffer[i] = s_data_buffer[i + 1];
    ret_sum += s_data_buffer[i];
  } // for
  
  return (float)(ret_sum/data_count);
} // RecurAvgFilter()
#endif // #if ENABLE_RecurAvgFilterFloat

/************************************************ 
* 函  数: RecurAvgFilterInt(int, unsigned int)
* 功  能: int型数据递推平均滤波, 递推即0 to (n-1), 1 to n,
*         每次向后推一个, 总共n个数据求平均值
*
* 参  数: unfiltered_data - 待滤波数据, 常用AD值
*         data_count - 求平均值个数
*
* 返回值: 滤波后的平均值, int型
*************************************************/
#if ENABLE_RecurAvgFilterInt
int RecurAvgFilterInt(int unfiltered_data, unsigned int data_count)
{
  unsigned int i = 0;
  long int ret_sum = 0;
  static int s_data_buffer[32] = {0};

  if (data_count > 32) return 0;  // 输入参数有误，返回错误。
  // 等待滤波的数据赋给数组最后一个值
  s_data_buffer[data_count - 1] = unfiltered_data;  
  for (i = 0; i < data_count; ++i)
  {
    s_data_buffer[i] = s_data_buffer[i + 1];
    ret_sum += s_data_buffer[i];
  } // for
  
  return (int)(ret_sum/data_count);
} // RecurAvgFilter()
#endif // #if ENABLE_RecurAvgFilterInt

/************************************************ 
* 函  数: MedianFilterInt(const unsigned int [], unsigned int)
* 功  能: int型数据中位值滤波，单通道, 16个数据
*
* 参  数: unfiltered_buf[] - 待滤波数据, 常用AD值
*         buf_cnt - 中位值滤波个数
*
* 返回值: 滤波后的中位值, int型
*************************************************/
#if ENABLE_MedianFilterInt
unsigned int MedianFilterInt(const unsigned int unfiltered_buf[], unsigned int buf_cnt)
{
	int i = 0, j = 0, k = 0;
	unsigned int temp = 0;
  static unsigned int s_data_buffer[16] = {0};  // 16为待求中位值数据总数
  
  if (buf_cnt > 16) return 0;  // 输入参数有误，返回错误。
  memcpy(s_data_buffer, unfiltered_buf, buf_cnt*sizeof(unsigned int));
  // 选择排序
	for (i = 0; i < buf_cnt - 1; ++i)
	{
		k = i;
		for(j = i + 1; j < buf_cnt; ++j)
		{
			if(s_data_buffer[k] > s_data_buffer[j])
      {
        k = j;
      }
		}
		if(i != k)
		{
			temp = s_data_buffer[i];
			s_data_buffer[i] = s_data_buffer[k];
			s_data_buffer[k] = temp;
		}
	} // end of for ().
  
	return s_data_buffer[buf_cnt/2];
} // end of MedianFilter().
#endif // #if ENABLE_MedianFilterInt

/************************************************ 
* 函  数: MedianFilterMultiChannelInt(const unsigned int [], 
*                                     unsigned int, 
*                                     unsigned int)
* 功  能: int型数据多通道中位值滤波, 5通道, 16个数据
*
* 参  数: unfiltered_buf[] - 待滤波数据, 常用AD值
*         channel_index - 通道索引, 0 to 4
*         buf_cnt - 中位值滤波个数
*
* 返回值: 滤波后的中位值, int型
*************************************************/
#if ENABLE_MedianFilterMultiChannelInt
unsigned int MedianFilterMultiChannelInt(const unsigned int unfiltered_buf[], 
                                         unsigned int channel_index, 
                                         unsigned int buf_cnt)
{
	int i = 0, j = 0, k = 0;
	unsigned int temp = 0;
  static unsigned int s_data_buffer[5][16] = {0}; // 16为待求中位值数据总数

  if (channel_index > 4 || buf_cnt > 16) return 0; // 输入参数有误，返回错误。
  memcpy(s_data_buffer[channel_index], unfiltered_buf, buf_cnt*sizeof(unsigned int));
  // 选择排序
	for(i = 0; i < buf_cnt - 1; ++i)
	{
		k = i;
		for(j = i + 1; j < buf_cnt; ++j)
		{
			if(s_data_buffer[channel_index][k] > s_data_buffer[channel_index][j])
      {
        k = j;
      }
		}
		if(i != k)
		{
			temp = s_data_buffer[channel_index][i];
			s_data_buffer[channel_index][i] = s_data_buffer[channel_index][k];
			s_data_buffer[channel_index][k] = temp;
		}
	}
  
	return s_data_buffer[channel_index][buf_cnt/2];
} // MedianFilterMultiChannel()
#endif // #if ENABLE_MedianFilterMultiChannelInt

/************************************************ 
* 函  数: FirstOrderLagFilter(int)
* 功  能: 一阶滞后滤波:
*         截止频率，fl = coefficient/2/3.14/t；
*         t为采样时间间隔，即采样周期，本系统t = 20ms，1/t = 50；
*         fl = 50*0.2/2/3.14 = 1.6Hz，截止频率为1.6Hz；
*         对于高于1/2采样频率(这里为25Hz)的干扰信号无法滤除。
*
* 参  数: unfiltered_data - 待滤波数据, 常用AD值
*
* 返回值: 滤波后的结果, int型
*************************************************/
#if ENABLE_FirstOrderLagFilter
int FirstOrderLagFilter(int unfiltered_data)
{
  float coefficient = 0.2;
  static int s_ret_value = 0;

  s_ret_value = (int)(coefficient*unfiltered_data + (1 - coefficient)*s_ret_value);

  return s_ret_value;
} // FirstOrderLagFilter()
#endif // #if ENABLE_FirstOrderLagFilter
