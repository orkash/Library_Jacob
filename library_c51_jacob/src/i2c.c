/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: i2c.c  
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
#include "../include/i2c.h"

#include "../include/delay.h"

/************************************************ 
* ��  ��: I2C_Start(void)
* ��  ��: ����I2C��ʼ�ź�
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
#if ENABLE_I2C_Start
void I2C_Start(void) 
{
  I2C_SCL = 1;  // ȷ��I2C_SCL, I2C_SDA��Ϊ�ߵ�ƽ
  I2C_SDA = 1;
  DelayUs();
  I2C_SDA = 0;  // ������I2C_SDA
  DelayUs();
  I2C_SCL = 0;  // ������I2C_SCL
} // end of I2C_Start().
#endif // #if ENABLE_I2C_Start

/************************************************ 
* ��  ��: I2C_Stop(void)
* ��  ��: ����I2Cֹͣ�ź�
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
#if ENABLE_I2C_Stop
void I2C_Stop(void) 
{
  I2C_SCL = 0;  // ȷ��I2C_SCL, I2C_SDA��Ϊ�͵�ƽ
  I2C_SDA = 0;
  DelayUs();
  I2C_SCL = 1;  // ������I2C_SDA
  DelayUs();
  I2C_SDA = 1;  // ������I2C_SCL
  DelayUs();
} // end of I2C_Stop().
#endif // #if ENABLE_I2C_Stop

/************************************************ 
* ��  ��: I2C_SendACK(bit)
* ��  ��: ����I2CӦ���ź�
*
* ��  ��: ack - Ӧ��λ
*
* ����ֵ: ��
*************************************************/
#if ENABLE_I2C_SendACK
void I2C_SendACK(bit ack)
{
  I2C_SDA = ack;  // дӦ���ź�
  I2C_SCL = 1;  // ����ʱ����
  DelayUs();  // ��ʱ
  I2C_SCL = 0;  // ����ʱ����
  DelayUs();  // ��ʱ
} // end of I2C_SendACK().
#endif // #if ENABLE_I2C_SendACK

/************************************************ 
* ��  ��: I2C_RecvACK(void)
* ��  ��: ����I2CӦ���ź�
*
* ��  ��: ack - Ӧ��λ
*
* ����ֵ: I2CӦ���ź�
*************************************************/
#if ENABLE_I2C_RecvACK
bit I2C_RecvACK(void)
{
  I2C_SCL = 1;  // ����ʱ����
  DelayUs();  // ��ʱ
  CY = I2C_SDA; // ��Ӧ���ź�
  I2C_SCL = 0;  // ����ʱ����
  DelayUs();  // ��ʱ
  return CY;
} // end of I2C_RecvACK().
#endif // #if ENABLE_I2C_RecvACK

/************************************************ 
* ��  ��: I2C_SendByte(unsigned char)
* ��  ��: ��I2C���߷���һ���ֽ�����
*
* ��  ��: dat - �������ֽ�����
*
* ����ֵ: ��
*************************************************/
#if ENABLE_I2C_SendByte
void I2C_SendByte(unsigned char dat)
{
  unsigned char i = 0;

  for (i = 0; i < 8; ++i) //8λ������
  {
    dat <<= 1;  // �Ƴ����ݵ����λ
    I2C_SDA = CY; // �����ݿ�
    I2C_SCL = 1;  // ����ʱ����
    DelayUs();  // ��ʱ
    I2C_SCL = 0;  // ����ʱ����
    DelayUs();  // ��ʱ
  } // end of for ()
  I2C_RecvACK();
} // end of I2C_SendByte().
#endif // #if ENABLE_I2C_SendByte

/************************************************ 
* ��  ��: I2C_RecvByte(void)
* ��  ��: ��I2C���߽���һ���ֽ�����
*
* ��  ��: ��
*
* ����ֵ: ���յ����ֽ�����
*************************************************/
#if ENABLE_I2C_RecvByte
unsigned char I2C_RecvByte(void)
{
  unsigned char i = 0;
  unsigned char dat = 0;

  I2C_SDA = 1;  // ʹ���ڲ�����,׼����ȡ����,
  for (i = 0; i < 8; ++i) // 8λ������
  {
    dat <<= 1;
    I2C_SCL = 1;  // ����ʱ����
    DelayUs(); // ��ʱ
    dat |= I2C_SDA; // ������               
    I2C_SCL = 0;  // ����ʱ����
    DelayUs();  // ��ʱ
  } // end of for ()

  return dat;
} // end of I2C_RecvByte().
#endif // #if ENABLE_I2C_RecvByte

/************************************************ 
* ��  ��: I2C_WriteByteToSlave(unsigned char, unsigned char)
* ��  ��: ��I2C�豸д��һ���ֽ�����
*
* ��  ��: reg_address - I2C�豸�ڲ��Ĵ�����ַ
*         reg_data - ��д���ֽ�����
*
* ����ֵ: ��
*************************************************/
#if ENABLE_I2C_WriteByteToSlave
void I2C_WriteByteToSlave(unsigned char reg_address, unsigned char reg_data)
{
  I2C_Start();  // ��ʼ�ź�
  I2C_SendByte(SLAVE_ADDRESS);  // �����豸��ַ+д�ź�
  I2C_SendByte(reg_address);  // �ڲ��Ĵ�����ַ
  I2C_SendByte(reg_data); // �ڲ��Ĵ�������
  I2C_Stop(); // ����ֹͣ�ź�
} // end of I2C_WriteByteToSlave().
#endif // #if ENABLE_I2C_WriteByteToSlave

/************************************************ 
* ��  ��: I2C_ReadByteFromSlave(unsigned char)
* ��  ��: ��I2C�豸��ȡһ���ֽ�����
*
* ��  ��: reg_address - I2C�豸�ڲ��Ĵ�����ַ
*
* ����ֵ: �������ֽ�����
*************************************************/
#if ENABLE_I2C_ReadByteFromSlave
unsigned char I2C_ReadByteFromSlave(unsigned char reg_address)
{
  unsigned char reg_data = 0;

  I2C_Start();  // ��ʼ�ź�
  I2C_SendByte(SLAVE_ADDRESS); // �����豸��ַ+д�ź�
  I2C_SendByte(reg_address);  // ���ʹ洢��Ԫ��ַ����0��ʼ  
  I2C_Start();  // ��ʼ�ź�
  I2C_SendByte(SLAVE_ADDRESS + 1);  // �����豸��ַ+���ź�
  reg_data = I2C_RecvByte();  // �����Ĵ�������
  I2C_SendACK(1); // ����Ӧ���ź�
  I2C_Stop(); // ֹͣ�ź�

  return reg_data;
} // end of I2C_ReadByteFromSlave().
#endif // #if ENABLE_I2C_ReadByteFromSlave
