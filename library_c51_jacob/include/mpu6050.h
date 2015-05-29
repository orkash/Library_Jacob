/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: mpu6050.h 
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
#ifndef MPU6050_H_
#define MPU6050_H_

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // end of #ifndef

// �Ƿ�ʹ��MPU6050_CalcResult()����,
// ��������������δʹ��ĳһ�������ľ��档
#define ENABLE_MPU6050_CalcResult TRUE

/* ����MPU6050�ڲ���ַ */
// �����ǲ����ʣ�����ֵ��0x07(125Hz)
#define SMPLRT_DIV 0x19 
// ��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define CONFIG 0x1A 
// �������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define GYRO_CONFIG 0x1B  
// ���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define ACCEL_CONFIG 0x1C 
// ���ٶ������ַ
#define ACCEL_XOUT_ADDR_H 0x3B
#define ACCEL_XOUT_ADDR_L 0x3C
#define ACCEL_YOUT_ADDR_H 0x3D
#define ACCEL_YOUT_ADDR_L 0x3E
#define ACCEL_ZOUT_ADDR_H 0x3F
#define ACCEL_ZOUT_ADDR_L 0x40
// ���ٶ������ַ
#define GYRO_XOUT_ADDR_H 0x43
#define GYRO_XOUT_ADDR_L 0x44 
#define GYRO_YOUT_ADDR_H 0x45
#define GYRO_YOUT_ADDR_L 0x46
#define GYRO_ZOUT_ADDR_H 0x47
#define GYRO_ZOUT_ADDR_L 0x48
// �¶������ַ
#define TEMP_OUT_ADDR_H 0x41
#define TEMP_OUT_ADDR_L 0x42
// �����Ĵ���
#define PWR_MGMT 0x6B // ��Դ��������ֵ��0x00(��������)
// ���ٶȡ����ٶ����
#define ACCEL_XOUT MPU6050_GetData(ACCEL_XOUT_ADDR_H)
#define ACCEL_YOUT MPU6050_GetData(ACCEL_YOUT_ADDR_H)
#define ACCEL_ZOUT MPU6050_GetData(ACCEL_ZOUT_ADDR_H)
#define GYRO_XOUT MPU6050_GetData(GYRO_XOUT_ADDR_H)
#define GYRO_YOUT MPU6050_GetData(GYRO_YOUT_ADDR_H)
#define GYRO_ZOUT MPU6050_GetData(GYRO_ZOUT_ADDR_H)
// �¶����
#define TEMP_OUT MPU6050_GetData(TEMP_OUT_ADDR_H)
// ����ʵ�ʽ��ͨ��
#define ACCEL_CHANNEL (0)
#define GYRO_CHANNEL (1)
#define TEMP_CHANNEL (2)
// MPU6050�����У׼ֵ
#define MPU6050_ZERO_ACCELL (-465.522)
#define MPU6050_ZERO_GYRO (-58.8427)

// ��ʼ��MPU6050
void MPU6050_Init(void);
// �ϳ�����
int MPU6050_GetData(unsigned char reg_address);
// ����ʵ�ʼ��ٶȡ����ٶȡ��¶Ƚ��
#if ENABLE_MPU6050_CalcResult
float MPU6050_CalcResult(int input_data, unsigned char channel);
#endif // #if ENABLE_MPU6050_CalcResult

#endif // end of #ifndef MPU6050_H_
