/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: i2c.h
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: I2C��غ���
* ��    ��: I2C_Start() - I2C��ʼ�ź�
*           I2C_Stop() - I2C��ʼ�ź�
*           I2C_SendACK() - I2C����Ӧ���ź�
*           I2C_RecvACK() - I2C����Ӧ���ź�
*           I2C_SendByte() - ��I2C���߷���һ���ֽ�����
*           I2C_RecvByte() - ��I2C���߽���һ���ֽ�����
*           I2C_WriteByteToSlave() - ��I2C�豸д��һ���ֽ�����
*           I2C_ReadByteFromSlave() - ��I2C�豸��ȡһ���ֽ�����
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��2��28��
* ��������: E0.0.1, �����I2C���8������, 2015��2��28��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/

#ifndef I2C_H_
#define I2C_H_

#include <reg51.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // end of #ifndef

// �Ƿ�ʹ������һ��I2C����,
// ��������������δʹ��ĳһ�������ľ��档
#define ENABLE_I2C_Start TRUE
#define ENABLE_I2C_Stop TRUE
#define ENABLE_I2C_SendACK TRUE
#define ENABLE_I2C_RecvACK TRUE
#define ENABLE_I2C_SendByte TRUE
#define ENABLE_I2C_RecvByte TRUE
#define ENABLE_I2C_WriteByteToSlave TRUE
#define ENABLE_I2C_ReadByteFromSlave TRUE

#define SLAVE_ADDRESS 0xD0  // IICд��ʱ�ĵ�ַ��+1Ϊ��ȡ

sbit I2C_SDA = P3^4;  // IIC�������Ŷ���
sbit I2C_SCL = P3^5;  // IICʱ�����Ŷ���

// I2C��ʼ�ź�
#if ENABLE_I2C_Start
static void I2C_Start(void);
#endif // #if ENABLE_I2C_Start
// I2Cֹͣ�ź�
#if ENABLE_I2C_Stop
static void I2C_Stop(void);
#endif // #if ENABLE_I2C_Stop
// I2C����Ӧ���ź�
#if ENABLE_I2C_SendACK
static void I2C_SendACK(bit ack);
#endif // #if ENABLE_I2C_SendACK
// I2C����Ӧ���ź�
#if ENABLE_I2C_RecvACK
static bit I2C_RecvACK(void);
#endif // #if ENABLE_I2C_RecvACK
// ��I2C���߷���һ���ֽ�����
#if ENABLE_I2C_SendByte
void I2C_SendByte(unsigned char dat);
#endif // #if ENABLE_I2C_SendByte
// ��I2C���߽���һ���ֽ�����
#if ENABLE_I2C_RecvByte
unsigned char I2C_RecvByte(void);
#endif // #if ENABLE_I2C_RecvByte
// ��I2C�豸д��һ���ֽ�����
#if ENABLE_I2C_WriteByteToSlave
void I2C_WriteByteToSlave(unsigned char reg_address, unsigned char reg_data);
#endif // #if ENABLE_I2C_WriteByteToSlave
// ��I2C�豸��ȡһ���ֽ�����
#if ENABLE_I2C_ReadByteFromSlave
unsigned char I2C_ReadByteFromSlave(unsigned char reg_address);
#endif // #if ENABLE_I2C_ReadByteFromSlave

#endif  // end of #ifndef I2C_H_
