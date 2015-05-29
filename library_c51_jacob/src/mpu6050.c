/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: mpu6050.c  
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: MPU6050������������غ�����ʵ��
* ��    ��: MPU6050_Init() - ��ʼ��MPU6050
*           MPU6050_GetData() - �ϳɴ�mpu6050�ڲ��Ĵ�����ȡ��������
*           MPU6050_CalcResult() - ����ʵ�ʼ��ٶȡ����ٶȡ��¶Ƚ��
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��3��2��
* ��������: E0.0.1, �����I2C���8������, 2015��3��2��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#include "../include/mpu6050.h"

#include "../include/i2c.h"

/************************************************ 
* ��  ��: MPU6050_Init(void)
* ��  ��: ��ʼ��MPU6050
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void MPU6050_Init(void)
{
	I2C_WriteByteToSlave(PWR_MGMT, 0x00);	// �������״̬
	I2C_WriteByteToSlave(SMPLRT_DIV, 0x07);
	I2C_WriteByteToSlave(CONFIG, 0x06);
	I2C_WriteByteToSlave(GYRO_CONFIG, 0x18);
	I2C_WriteByteToSlave(ACCEL_CONFIG, 0x01);
} // end of MPU6050_Init().

/************************************************ 
* ��  ��: MPU6050_GetData(unsigned char)
* ��  ��: �ϳɴ�mpu6050�ڲ��Ĵ�����ȡ��������
*
* ��  ��: reg_address - mpu6050�ڲ��Ĵ�����ַ
*
* ����ֵ: �ϳ��Ժ������, int��
*************************************************/
int MPU6050_GetData(unsigned char reg_address)
{
	unsigned char high_8bits = 0, low_8bits = 0;
  
	high_8bits = I2C_ReadByteFromSlave(reg_address);
	low_8bits = I2C_ReadByteFromSlave(reg_address + 1);
  
	return (high_8bits << 8) + low_8bits; // �ϳ�����
} // end of MPU6050_GetData().

/************************************************ 
* ��  ��: MPU6050_CalcResult(int, unsigned char)
* ��  ��: ����ʵ�ʼ��ٶȡ����ٶȡ��¶Ƚ��
*
* ��  ��: input_data - mpu6050ԭʼ����
*         channel - ����ͨ��, 0: ���ٶ�, 1: ���ٶ�, 2: �¶�
*
* ����ֵ: ��������Ժ��ʵ�ʽ��, float��
*************************************************/
#if ENABLE_MPU6050_CalcResult

float MPU6050_CalcResult(int input_data, unsigned char channel)
{
  /* ���㹫ʽ
  // ���ٶȼ���
  �������ٶ�����/32768.0*16g(gΪ�������ٶȣ���ȡ9.8m/s2)
  // ���ٶȼ���
  �������ٶ�����/32768.0*2000(��/s)
  // �Ƕȼ���
  �����Ƕ�����/32768.0*180(��)
  // �¶ȼ���
  �����¶�����/340.0+36.25��
  */     
  float result = 0.0;
  
  if (channel != 0 && channel != 1 && channel != 2) return -1;  // ͨ�����Ϸ�
  switch (channel)
  {
    case ACCEL_CHANNEL: { // ���ü��ٶȼ���Ƕ�
      result = (input_data - MPU6050_ZERO_ACCELL)/16384.0;
      // result = result*180.0/PI/0.92; // ����ת��Ϊ�ȣ��ǶȽ�Сʱ�����������Ƽ���
      result = asin(result)*180.0/PI; // �����Һ�����������ȣ�Ȼ��ת��Ϊ��
      break;
    } // case
    case GYRO_CHANNEL: { // ���ٶ�
      // ȥ�����ƫ�ƣ�������ٶ�ֵ������Ϊ������  
      result = -(input_data - MPU6050_ZERO_GYRO)/16.4;    
      break;
    } // case
    case TEMP_CHANNEL: { // �¶�
      result = input_data/340.0+36.25;
      break;
    } // case
    default: {
      break;
    } // default
  } // end of switch ().
  
  return result;
} // end of MPU6050_CalcResult().

#endif // #if ENABLE_MPU6050_CalcResult
