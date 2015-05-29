/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: i2c.h 
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: stm32, i2c��Ӧ�ӿ��뺯������
* ��    ��: I2C_GPIO_Init() - ��ʼ��I2C��ӦIO��
*           I2C_Start() - I2C��ʼ�ź�
*           I2C_Stop() - I2Cֹͣ�ź�
*           I2C_Wait_Ack() - �ȴ�Ӧ���źŵ���
*           I2C_Ack() - ����ACKӦ��
*           I2C_NAck() - ������ACKӦ��
*           I2C_Send_Byte() - ��I2C���߷���һ���ֽ�
*           I2C_Read_Byte() - ��I2C���߶�1���ֽ�
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��4��23��
* ��������: E0.0.1, ʵ����I2C�������ú���, 2015��4��23��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#ifndef STM32_I2C_H_
#define STM32_I2C_H_

#include "sys.h"

// ѡ��ʹ��USE_I2C1 or USE_I2C2
#define USE_I2C1
#if defined(USE_I2C1)
// ����ʹ��I2C1����PB6: SCL��PB7: SDA��
// PB7�������롢���ģʽ
#define I2C_SDA_IN()  {GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= 0x80000000;} 
#define I2C_SDA_OUT() {GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= 0x30000000;}
// ����I2C��SCL, SDA��Ӧ����
#define GPIO_PIN_SCL (GPIO_Pin_6)
#define GPIO_PIN_SDA (GPIO_Pin_7)
#elif defined(USE_I2C2)
// ʹ��I2C2ʱ��PB10: SCL��PB11: SDA
#define I2C_SDA_IN()  {GPIOB->CRH &= 0XFFFF0FFF; GPIOB->CRH |= 8<<12;}
#define I2C_SDA_OUT() {GPIOB->CRH &= 0XFFFF0FFF; GPIOB->CRH |= 3<<12;}
// ����I2C��SCL, SDA��Ӧ����
#define GPIO_PIN_SCL (GPIO_Pin_10)
#define GPIO_PIN_SDA (GPIO_Pin_11)
#else 
#error "Neither USE_I2C1 nor USE_I2C2 was defined!!"
#endif  // if defined()

// IO����
#define I2C_SCL PBout(6)  // PB6, SCL
#define I2C_SDA PBout(7)  // PB7, SDA
#define I2C_READ_SDA PBin(7)  // SDA����

/*----- High layer function -----*/
// ��ʼ��I2c
void I2C_GPIO_Init(void);
// I2C��ʼ�ź�
void I2C_Start(void);
// I2Cֹͣ�ź�
void I2C_Stop(void);
// �ȴ�Ӧ���źŵ���
u8 I2C_Wait_Ack(void);
// ����ACKӦ��
void I2C_Ack(void);
// ������ACKӦ��        
void I2C_NAck(void);
// ��I2C���߷���һ���ֽ�      
void I2C_Send_Byte(u8 dat);
// ��I2C���߶�1���ֽ�  
u8 I2C_Read_Byte(u8 ack);

/*----- Medium layer function -----*/

/*----- Low layer function -----*/

#endif  // #ifndef STM32_I2C_H_
