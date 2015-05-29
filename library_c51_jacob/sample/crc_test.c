/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: test_crc.c  
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: crc���Ժ���, ��׼ANSI C��������, ����dos
* ��    ��: main() - crc��������
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��4��7��
* ��������: E0.0.1, ʵ��crc��������, 2015��4��7��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/

// �������crc.hͷ�ļ�, ����·���ɸ���crc.hλ���޸�
#include "../include/crc.h" 

#include <stdio.h>
#include <string.h> // Declare strlen().

/************************************************ 
* ˵  ��: ����crc˵��
* ��  ��: CheckCrcSlowly() - ��λ����crcУ��, ���Ƽ�
*         CheckCrcFaster() - ���ֽڽ���crcУ��, �ٶȸ��죬�Ƽ�
* Crc CheckCrcSlowly(const unsigned char message[], unsigned int message_bytes)
* Crc CheckCrcFaster(const unsigned char message[], unsigned int message_bytes)
*
* ��  ��: message - ��ʼУ������ݵ�ַ, һ���ͷ��ʼ
*         message_bytes - ��ҪУ��������ֽ���, ��Ҫ�Լ�ָ��
*
*����ֵ : ������ɵ�crcУ��ֵ
*************************************************/

int main(void)
{
  unsigned char  test_message[] = "123456789";
  
  // Compute the CRC of the test message, slowly.
  // ����ʹ��ͷ�ļ����USE_BIT_CRC
  printf("The CheckCrcSlowly() of \"123456789\" is 0x%X.\n", 
          CheckCrcSlowly(test_message, strlen(test_message)));
  // Compute the CRC of the test message, more efficiently.
  // ����ʹ��ͷ�ļ����USE_BYTE_CRC
  printf("The CheckCrcFaster() of \"123456789\" is 0x%X.\n", 
          CheckCrcFaster(test_message, strlen(test_message)));

  return 0;
} // main().
