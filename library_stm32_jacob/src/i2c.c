/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: i2c.c 
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: stm32, i2c��Ӧ����
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
#include "../include/i2c.h"

#include "../include/delay.h"

/************************************************ 
* ��  ��: I2C_GPIO_Init(void)
* ��  ��: ��ʼ��I2C��ӦIO��
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void I2C_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ����GPIOBʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SCL | GPIO_PIN_SDA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB, GPIO_PIN_SCL | GPIO_PIN_SDA); // SCL��SDA������ߵ�ƽ
} // I2C_Init()

/************************************************ 
* ��  ��: I2C_Start(void)
* ��  ��: ����I2C��ʼ�ź�
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void I2C_Start(void)
{
  I2C_SDA_OUT();  // SDA�����
  I2C_SDA = 1;  // ȷ��I2C_SCL, I2C_SDA��Ϊ�ߵ�ƽ
  I2C_SCL = 1;
  DelayUs(2);
  I2C_SDA = 0;  // ������I2C_SDA
  DelayUs(2);
  I2C_SCL = 0;  // ������I2C_SCL
} // I2C_Start()

/************************************************ 
* ��  ��: I2C_Stop(void)
* ��  ��: ����I2Cֹͣ�ź�
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void I2C_Stop(void)
{
  I2C_SDA_OUT();  // SDA�����
  I2C_SCL = 0;  // ȷ��I2C_SCL, I2C_SDA��Ϊ�͵�ƽ
  I2C_SDA = 0;
  DelayUs(2);
  I2C_SCL = 1;  // ������I2C_SDA
  DelayUs(2);
  I2C_SDA = 1;  // ������I2C_SCL
  DelayUs(2);  
} // I2C_Stop()

/************************************************ 
* ��  ��: I2C_Wait_Ack(void)
* ��  ��: �ȴ�Ӧ���źŵ���
*
* ��  ��: ��
*
* ����ֵ: 1, ����Ӧ��ʧ��; 0, ����Ӧ��ɹ�
*************************************************/
u8 I2C_Wait_Ack(void)
{
  u8 error_times = 0;
  
  I2C_SDA_IN(); // SDA����Ϊ����  
  I2C_SDA = 1;
  DelayUs(2);     
  I2C_SCL = 1;
  DelayUs(2);  
  while (I2C_READ_SDA)
  {
    ++error_times;
    if (error_times > 250)
    {
      I2C_Stop();

      return 1;
    } // if
  } // while
  I2C_SCL = 0;  // ʱ�����0  

  return 0;  
} // I2C_Wait_Ack()

/************************************************ 
* ��  ��: I2C_Ack(void)
* ��  ��: ����ACKӦ��
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void I2C_Ack(void)
{
  I2C_SDA_OUT();
  I2C_SCL = 0;
  I2C_SDA = 0;
  DelayUs(2);
  I2C_SCL = 1;
  DelayUs(2);
  I2C_SCL = 0;
} // I2C_Ack()

/************************************************ 
* ��  ��: I2C_NAck(void)
* ��  ��: ������ACKӦ��
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/      
void I2C_NAck(void)
{
  I2C_SDA_OUT();
  I2C_SCL = 0;
  I2C_SDA = 1;
  DelayUs(2);
  I2C_SCL = 1;
  DelayUs(2);
  I2C_SCL = 0;
} // I2C_NAck() 

/************************************************ 
* ��  ��: I2C_Send_Byte(u8)
* ��  ��: ��I2C���߷���һ���ֽ�
*
* ��  ��: dat - �����͵��ֽ�����
*
* ����ֵ: ��
*************************************************/    
void I2C_Send_Byte(u8 dat)
{                        
  u8 i = 0;   

  I2C_SDA_OUT();      
  I2C_SCL = 0;  // ����ʱ�ӿ�ʼ���ݴ���
  for (i = 0; i < 8; ++i)
  {              
    // I2C_SDA = (dat & 0x80) >> 7;
    if ((dat & 0x80) >> 7)
      I2C_SDA = 1;
    else
      I2C_SDA = 0;
    dat <<= 1;    
    I2C_SCL = 1;
    I2C_SCL = 0;
  } // for
} // I2C_Send_Byte()

/************************************************ 
* ��  ��: I2C_Send_Byte(u8)
* ��  ��: ��I2C���߶�1���ֽ�
*
* ��  ��: ack - �Ƿ���Ҫ����ackӦ��: 1, ����Ack; 0, ����NAck
*
* ����ֵ: �����߶�ȡ�������ֽ�����
*************************************************/ 
u8 I2C_Read_Byte(u8 ack)
{
  unsigned char i = 0, ret_data = 0x00;

  I2C_SDA_IN(); // SDA����Ϊ����
  for (i = 0; i < 8; ++i)
  {
    I2C_SCL=0; 
    DelayUs(2);
    I2C_SCL=1;
    ret_data<<=1;
    if (I2C_READ_SDA) ret_data++;   
    DelayUs(2); 
  } // for     
  if (!ack)
      I2C_NAck(); // ����noACK
  else
      I2C_Ack();  // ����ACK   
  return ret_data;
} // I2C_Read_Byte()
