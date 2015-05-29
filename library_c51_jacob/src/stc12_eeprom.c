/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: stc12_eeprom.c  
* 文件标识: 见配置管理计划书
* 摘    要: 使用STC12系列片内自带eeprom相关函数的实现，参考stc官方手册
* 函    数: IdleIap() - Disable ISP/IAP/EEPROM function
*           ReadByteIap() - Read one byte from ISP/IAP/EEPROM area
*           ProgramByteIap() - Program one byte to ISP/IAP/EEPROM area
*           EraseSectorIap() - Erase one sector area
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年3月2日
* 更改内容: E0.0.1, 完成了stc12 eeprom的读、写, 2015年3月2日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
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
* 函  数: IdleIap(void)
* 功  能: Disable ISP/IAP/EEPROM function,
*         make MCU in a safe state.
*
* 参  数: 无
*
* 返回值: 无
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
* 函  数: ReadByteIap(unsigned int)
* 功  能: Read one byte from ISP/IAP/EEPROM area.
*
* 参  数: addr - ISP/IAP/EEPROM address
*
* 返回值: Flash data
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
* 函  数: ProgramByteIap(unsigned int addr, unsigned char dat)
* 功  能: Program one byte to ISP/IAP/EEPROM area.
*
* 参  数: addr - ISP/IAP/EEPROM address
*         dat - ISP/IAP/EEPROM data
*
* 返回值: 无
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
* 函  数: EraseSectorIap(unsigned int)
* 功  能: Erase one sector area.
*
* 参  数: addr - ISP/IAP/EEPROM address
*
* 返回值: 无
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
