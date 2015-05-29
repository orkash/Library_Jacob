/**********************************************************************
 * Filename: "crc.h".
 * Description: A header file describing the various CRC standards.
 * Inclusion: CRC_8, CRC_8_ATM, CRC_8_CCITT, CRC_8_Dallas_Maxim, CRC_16_CCITT
 *            CRC_16_IBM, CRC_32_IEEE.
 *
 * Reversion: A0.1
 * Author:    Jacob Peng
 * Date:      30 October 2014  
 * Changed content:      
 *
 * Version: 
 * Author:   
 * Date:    
 *
 * Reference: 
 * [1] Barr, Michael. CRC Implementation Code in C[C]. Embedded
 *     Systems Programming. January 2000, pp. 37-46. Site:
 *     http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code.
 * [2] Ross N. Williams. A painless guide to crc error detection algorithms[C].
 *     http://www.repairfaq.org/filipg/LINK/F_crc_v31.html. 19 August 1993.
 *
 * Notes: Code style refers to "Google C++ Style Guide".
 **********************************************************************/

#ifndef CRC_H_
#define CRC_H_

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // end of #ifndef

// 是否使用位crc校验函数与字节crc校验函数,
// 可以消除编译器未使用某一个函数的警告。
#define ENABLE_BIT_CRC TRUE
#define ENABLE_BYTE_CRC TRUE

// Select the CRC standard from the following list.
#define CRC_8

#if defined(CRC_8)

typedef unsigned char Crc;

#define CRC_NAME "CRC-8"
#define POLYNOMIAL 0xD5
#define INITIAL_REMAINDER 0x00
#define FINAL_XOR_VALUE 0x00
#define REFLECT_DATA TRUE
#define REFLECT_REMAINDER TRUE
#define CHECK_VALUE 0x7C

#elif defined(CRC_8_ATM)

typedef unsigned char  Crc;

#define CRC_NAME "CRC-8-ATM"
#define POLYNOMIAL 0x7
#define INITIAL_REMAINDER 0x00
#define FINAL_XOR_VALUE 0x00
#define REFLECT_DATA TRUE
#define REFLECT_REMAINDER TRUE
#define CHECK_VALUE 0x20

#elif defined(CRC_8_CCITT)

typedef unsigned char  Crc;

#define CRC_NAME "CRC-8-CCITT"
#define POLYNOMIAL 0x8D
#define INITIAL_REMAINDER 0x00
#define FINAL_XOR_VALUE 0x00
#define REFLECT_DATA TRUE
#define REFLECT_REMAINDER TRUE
#define CHECK_VALUE 0xFC

#elif defined(CRC_8_Dallas_Maxim)

typedef unsigned char  Crc;

#define CRC_NAME "CRC-8-Dallas/Maxim"
#define POLYNOMIAL 0x31
#define INITIAL_REMAINDER 0x00
#define FINAL_XOR_VALUE 0x00
#define REFLECT_DATA TRUE
#define REFLECT_REMAINDER TRUE
#define CHECK_VALUE 0xA1

#elif defined(CRC_16_CCITT)

typedef unsigned short  Crc;

#define CRC_NAME "CRC-16-CCITT"
#define POLYNOMIAL 0x1021
#define INITIAL_REMAINDER 0xFFFF
#define FINAL_XOR_VALUE 0x0000
#define REFLECT_DATA FALSE
#define REFLECT_REMAINDER FALSE
#define CHECK_VALUE 0x29B1

#elif defined(CRC_16_IBM)

typedef unsigned short  Crc;

#define CRC_NAME "CRC-16-IBM"
#define POLYNOMIAL 0x8005
#define INITIAL_REMAINDER 0x0000
#define FINAL_XOR_VALUE 0x0000
#define REFLECT_DATA TRUE
#define REFLECT_REMAINDER TRUE
#define CHECK_VALUE 0xBB3D

#elif defined(CRC_32_IEEE)

typedef unsigned long  Crc;

#define CRC_NAME "CRC-32-IEEE"
#define POLYNOMIAL 0x04C11DB7
#define INITIAL_REMAINDER 0xFFFFFFFF
#define FINAL_XOR_VALUE 0xFFFFFFFF
#define REFLECT_DATA TRUE
#define REFLECT_REMAINDER TRUE
#define CHECK_VALUE 0xCBF43926

#else

#error "The CRC standard isn't in the above list!!"

#endif  // if defined().

// Declare all the functions that used in "crc.c".
#if ENABLE_BIT_CRC // 是否调用: CheckCrcSlowly()
Crc CheckCrcSlowly(const unsigned char message[], unsigned int message_bytes);
#endif  // #if ENABLE_BIT_CRC

#if ENABLE_BYTE_CRC  // 是否调用: CheckCrcFaster()
void InitCrc(void);
Crc CheckCrcFaster(const unsigned char message[], unsigned int message_bytes);
#endif	// #if ENABLE_BYTE_CRC 

#endif  // CRC_H_.
