/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: stc12_eeprom.h  
* �ļ���ʶ: �����ù���ƻ���
* ժ    Ҫ: ʹ��STC12ϵ��Ƭ���Դ�eeprom��غ�����ʵ�֣��ο�stc�ٷ��ֲ�
* ��    ��: IdleIap() - Disable ISP/IAP/EEPROM function
*           ReadByteIap() - Read one byte from ISP/IAP/EEPROM area
*           ProgramByteIap() - Program one byte to ISP/IAP/EEPROM area
*           EraseSectorIap() - Erase one sector area
* 
* ��ǰ�汾: E0.0.1
* ��    ��: Jacob Peng
* �������: 2015��3��2��
* ��������: E0.0.1, �����stc12 eeprom�Ķ���д, 2015��3��2��
* 
* ȡ���汾: None
* ԭ����  : Unknown
* �������: Unknown
*/
#ifndef STC12_EEPROM_H_
#define STC12_EEPROM_H_

// Disable ISP/IAP/EEPROM function.
void IdleIap();
// Read one byte from ISP/IAP/EEPROM area.
unsigned char ReadByteIap(unsigned int addr);
// Program one byte to ISP/IAP/EEPROM area.
void ProgramByteIap(unsigned int addr, unsigned char dat);
// Erase all data of a selected sector area.
void EraseSectorIap(unsigned int addr);

#endif  // end of STC12_EEPROM_H_