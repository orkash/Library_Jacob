/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: filter.h 
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
#ifndef FILTER_H_
#define FILTER_H_

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // end of #ifndef

// �Ƿ�ʹ������һ���˲�����,
// ��������������δʹ��ĳһ�������ľ��档
#define ENABLE_AvgFilterInt TRUE
#define ENABLE_RecurAvgFilterFloat TRUE
#define ENABLE_RecurAvgFilterInt TRUE
#define ENABLE_MedianFilterInt TRUE
#define ENABLE_MedianFilterMultiChannelInt TRUE
#define ENABLE_FirstOrderLagFilter TRUE

// int����������ƽ��ֵ�˲�
#if ENABLE_AvgFilterInt
int AvgFilterInt(int unfiltered_data, unsigned int data_count);
#endif // #if ENABLE_AvgFilterInt
// float�����ݵ���ƽ���˲�
#if ENABLE_RecurAvgFilterFloat
float RecurAvgFilterFloat(float unfiltered_data, unsigned int data_count);
#endif // #if ENABLE_RecurAvgFilterFloat
// int�����ݵ���ƽ���˲�
#if ENABLE_RecurAvgFilterInt
int RecurAvgFilterInt(int unfiltered_data, unsigned int data_count);
#endif // #if ENABLE_RecurAvgFilterInt
// int��������λֵ�˲�����ͨ��
#if ENABLE_MedianFilterInt
unsigned int MedianFilterInt(const unsigned int unfiltered_buf[], 
                             unsigned int buf_cnt);
#endif // #if ENABLE_MedianFilterInt
// int�����ݶ�ͨ����λֵ�˲�
#if ENABLE_MedianFilterMultiChannelInt
unsigned int MedianFilterMultiChannelInt(const unsigned int unfiltered_buf[], 
                                         unsigned int channel_index, 
                                         unsigned int buf_cnt);
#endif // #if ENABLE_MedianFilterMultiChannelInt
// һ���ͺ��˲�
#if ENABLE_FirstOrderLagFilter
int FirstOrderLagFilter(int unfiltered_data);
#endif // #if ENABLE_FirstOrderLagFilter

#endif  // #ifndef FILTER_H_
