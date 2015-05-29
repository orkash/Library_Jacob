/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: filter.c  
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: �����˲�����
* ��    ��: AvgFilterInt() - int����������ƽ��ֵ�˲�
*           RecurAvgFilterFloat() - float�����ݵ���ƽ���˲�
*           RecurAvgFilterInt() - int�����ݵ���ƽ���˲�
*           MedianFilterInt() - int��������λֵ�˲�����ͨ��
*           MedianFilterMultiChannelInt() - int�����ݶ�ͨ����λֵ�˲�
*           FirstOrderLagFilter() - һ���ͺ��˲�
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��3��18��
* ��������: E0.0.1, ���������6�������˲�����, 2015��3��18��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#include <string.h> // Declare memcpy().

/************************************************ 
* ��  ��: AvgFilterInt(int, unsigned int)
* ��  ��: int����������ƽ��ֵ�˲�
*
* ��  ��: unfiltered_data - ���˲�����, ����ADֵ
*         data_count - ��ƽ��ֵ����
*
* ����ֵ: �˲����ƽ��ֵ, int��
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
* ��  ��: RecurAvgFilterFloat(float, unsigned int)
* ��  ��: float�����ݵ���ƽ���˲�, ���Ƽ�0 to (n-1), 1 to n,
*         ÿ�������һ��, �ܹ�n��������ƽ��ֵ
*
* ��  ��: unfiltered_data - ���˲�����, ����ADֵ
*         data_count - ��ƽ��ֵ����
*
* ����ֵ: �˲����ƽ��ֵ, float��
*************************************************/
#if ENABLE_RecurAvgFilterFloat
float RecurAvgFilterFloat(float unfiltered_data, unsigned int data_count)
{
  unsigned int i = 0;
  double ret_sum = 0;
  static float s_data_buffer[32] = {0.0};

  if (data_count > 32) return 0;  // ����������󣬷��ش���
  // �ȴ��˲������ݸ����������һ��ֵ
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
* ��  ��: RecurAvgFilterInt(int, unsigned int)
* ��  ��: int�����ݵ���ƽ���˲�, ���Ƽ�0 to (n-1), 1 to n,
*         ÿ�������һ��, �ܹ�n��������ƽ��ֵ
*
* ��  ��: unfiltered_data - ���˲�����, ����ADֵ
*         data_count - ��ƽ��ֵ����
*
* ����ֵ: �˲����ƽ��ֵ, int��
*************************************************/
#if ENABLE_RecurAvgFilterInt
int RecurAvgFilterInt(int unfiltered_data, unsigned int data_count)
{
  unsigned int i = 0;
  long int ret_sum = 0;
  static int s_data_buffer[32] = {0};

  if (data_count > 32) return 0;  // ����������󣬷��ش���
  // �ȴ��˲������ݸ����������һ��ֵ
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
* ��  ��: MedianFilterInt(const unsigned int [], unsigned int)
* ��  ��: int��������λֵ�˲�����ͨ��, 16������
*
* ��  ��: unfiltered_buf[] - ���˲�����, ����ADֵ
*         buf_cnt - ��λֵ�˲�����
*
* ����ֵ: �˲������λֵ, int��
*************************************************/
#if ENABLE_MedianFilterInt
unsigned int MedianFilterInt(const unsigned int unfiltered_buf[], unsigned int buf_cnt)
{
	int i = 0, j = 0, k = 0;
	unsigned int temp = 0;
  static unsigned int s_data_buffer[16] = {0};  // 16Ϊ������λֵ��������
  
  if (buf_cnt > 16) return 0;  // ����������󣬷��ش���
  memcpy(s_data_buffer, unfiltered_buf, buf_cnt*sizeof(unsigned int));
  // ѡ������
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
* ��  ��: MedianFilterMultiChannelInt(const unsigned int [], 
*                                     unsigned int, 
*                                     unsigned int)
* ��  ��: int�����ݶ�ͨ����λֵ�˲�, 5ͨ��, 16������
*
* ��  ��: unfiltered_buf[] - ���˲�����, ����ADֵ
*         channel_index - ͨ������, 0 to 4
*         buf_cnt - ��λֵ�˲�����
*
* ����ֵ: �˲������λֵ, int��
*************************************************/
#if ENABLE_MedianFilterMultiChannelInt
unsigned int MedianFilterMultiChannelInt(const unsigned int unfiltered_buf[], 
                                         unsigned int channel_index, 
                                         unsigned int buf_cnt)
{
	int i = 0, j = 0, k = 0;
	unsigned int temp = 0;
  static unsigned int s_data_buffer[5][16] = {0}; // 16Ϊ������λֵ��������

  if (channel_index > 4 || buf_cnt > 16) return 0; // ����������󣬷��ش���
  memcpy(s_data_buffer[channel_index], unfiltered_buf, buf_cnt*sizeof(unsigned int));
  // ѡ������
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
* ��  ��: FirstOrderLagFilter(int)
* ��  ��: һ���ͺ��˲�:
*         ��ֹƵ�ʣ�fl = coefficient/2/3.14/t��
*         tΪ����ʱ���������������ڣ���ϵͳt = 20ms��1/t = 50��
*         fl = 50*0.2/2/3.14 = 1.6Hz����ֹƵ��Ϊ1.6Hz��
*         ���ڸ���1/2����Ƶ��(����Ϊ25Hz)�ĸ����ź��޷��˳���
*
* ��  ��: unfiltered_data - ���˲�����, ����ADֵ
*
* ����ֵ: �˲���Ľ��, int��
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
