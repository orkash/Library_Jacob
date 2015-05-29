/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: stc12_eeprom.h  
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