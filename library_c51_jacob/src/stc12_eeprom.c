/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* �ļ�����: stc12_eeprom.c  
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
#include <reg51.h>
#include <intrins.h>  // Declare _nop_().

/*Define ISP/IAP/EEPROM command*/
#define CMD_IDLE 0 // Stand-By
#define CMD_READ 1 // Byte-Read
#define CMD_PROGRAM 2 // Byte-Program
#define CMD_ERASE 3 // Sector-Erase
/*Define ISP/IAP/EEPROM operation constant for IAP_CONTR*/
#define ENABLE_IAP 0x82 // if SYSCLK<20MHz

/*Declare SFR associated with the IAP */
sfr IAP_DATA = 0xE2; // Flash data register
sfr IAP_ADDRH = 0xE3; // Flash address HIGH
sfr IAP_ADDRL = 0xE4; // Flash address LOW
sfr IAP_CMD = 0xE5; // Flash command register
sfr IAP_TRIG = 0xE6; // Flash command trigger
sfr IAP_CONTR = 0xE7; // Flash control register

/************************************************ 
* ��  ��: IdleIap(void)
* ��  ��: Disable ISP/IAP/EEPROM function,
*         make MCU in a safe state.
*
* ��  ��: ��
*
* ����ֵ: ��
*************************************************/
void IdleIap(void)
{
 IAP_CONTR = 0;  // Close IAP function
 IAP_CMD = CMD_IDLE;  // Clear command to standby
 IAP_TRIG = 0;  // Clear trigger register
 IAP_ADDRH = 0x80; // Data ptr point to non-EEPROM area
 IAP_ADDRL = 0;  // Clear IAP address to prevent misuse
} // IdleIap()

/************************************************ 
* ��  ��: ReadByteIap(unsigned int)
* ��  ��: Read one byte from ISP/IAP/EEPROM area.
*
* ��  ��: addr - ISP/IAP/EEPROM address
*
* ����ֵ: Flash data
*************************************************/
unsigned char ReadByteIap(unsigned int addr)
{
  unsigned char dat;  // Data buffer
  
  IAP_CONTR = ENABLE_IAP; // Open IAP function, and set wait time
  IAP_CMD = CMD_READ; // Set ISP/IAP/EEPROM READ command
  IAP_ADDRL = addr; // Set ISP/IAP/EEPROM address low
  IAP_ADDRH = addr >> 8;  // Set ISP/IAP/EEPROM address high
  IAP_TRIG = 0x46;  // Send trigger command1 (0x46)
  IAP_TRIG = 0xB9;  // Send trigger command2 (0xB9)
  _nop_();  // MCU will hold here until ISP/IAP/EEPROM
  // operation complete
  dat = IAP_DATA; // Read ISP/IAP/EEPROM data
  IdleIap();  // Close ISP/IAP/EEPROM function
  
  return dat; // Return Flash data
} // ReadByteIap()

/************************************************ 
* ��  ��: ProgramByteIap(unsigned int addr, unsigned char dat)
* ��  ��: Program one byte to ISP/IAP/EEPROM area.
*
* ��  ��: addr - ISP/IAP/EEPROM address
*         dat - ISP/IAP/EEPROM data
*
* ����ֵ: ��
*************************************************/
void ProgramByteIap(unsigned int addr, unsigned char dat)
{
  IAP_CONTR = ENABLE_IAP; // Open IAP function, and set wait time
  IAP_CMD = CMD_PROGRAM; // Set ISP/IAP/EEPROM PROGRAM command
  IAP_ADDRL = addr; // Set ISP/IAP/EEPROM address low
  IAP_ADDRH = addr >> 8; // Set ISP/IAP/EEPROM address high
  IAP_DATA = dat;  // Write ISP/IAP/EEPROM data
  IAP_TRIG = 0x46; // Send trigger command1 (0x46)
  IAP_TRIG = 0xB9;  // Send trigger command2 (0xB9)
  _nop_();  // MCU will hold here until ISP/IAP/EEPROM
  // operation complete
  IdleIap();
} // ProgramByteIap()

/************************************************ 
* ��  ��: EraseSectorIap(unsigned int)
* ��  ��: Erase one sector area.
*
* ��  ��: addr - ISP/IAP/EEPROM address
*
* ����ֵ: ��
*************************************************/
void EraseSectorIap(unsigned int addr)
{
  IAP_CONTR = ENABLE_IAP; // Open IAP function, and set wait time
  IAP_CMD = CMD_ERASE; // Set ISP/IAP/EEPROM ERASE command
  IAP_ADDRL = addr; // Set ISP/IAP/EEPROM address low
  IAP_ADDRH = addr >> 8; // Set ISP/IAP/EEPROM address high
  IAP_TRIG = 0x46;  // Send trigger command1 (0x46)
  IAP_TRIG = 0xB9;  // Send trigger command2 (0xB9)
  _nop_();  // MCU will hold here until ISP/IAP/EEPROM
  // operation complete
  IdleIap();
} // EraseSectorIap()
