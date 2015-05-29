/**********************************************************************
 * Filename: "crc.c".
 * Description: Slow and fast implementations of the CRC standards.
 * Inclusion: CheckCrcSlowly(), CheckCrcFaster().
 *
 * Reversion: A0.1
 * Author:    Jacob Peng 
 * Date:      31 October 2014  
 * Changed content:      
 *
 * Version: 
 * Author:   
 * Date:    
 *
 * Reference:  * [1] Barr, Michael. CRC Implementation Code in C[C]. Embedded
 *     Systems Programming. January 2000, pp. 37-46. Site:
 *     http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code.
 * [2] Ross N. Williams. A painless guide to crc error detection algorithms[C].
 *     http://www.repairfaq.org/filipg/LINK/F_crc_v31.html. 19 August 1993.
 * 
 * Notes: The parameters for each supported CRC standard are
 *        defined in the header file "crc.h".  The implementations
 *        here should stand up to further additions to that list.
 *        Code style refers to "Google C++ Style Guide".
 **********************************************************************/

#include "../include/crc.h"

#include <string.h> // Declare memset().

// Derive parameters from the standard-specific parameters in "crc.h".
#define CRC_WIDTH (8 * sizeof(Crc))
#define TOPBIT (1 << (CRC_WIDTH - 1))

#if (TRUE == REFLECT_DATA)
#undef REFLECT_DATA
#define REFLECT_DATA(X) ((unsigned char) ReflectData((X), 8))
#else
#undef REFLECT_DATA
#define REFLECT_DATA(X) (X)
#endif

#if (TRUE == REFLECT_REMAINDER)
#undef REFLECT_REMAINDER
#define REFLECT_REMAINDER(X) ((Crc) ReflectData((X), CRC_WIDTH))
#else 
#undef REFLECT_REMAINDER
#define REFLECT_REMAINDER(X) (X)
#endif

/*********************************************************************
 * Function: ReflectData(), it's accessible only from this file.
 * Description: Reorder the bits of a binary sequence, by reflecting
 *              them about the middle position.
 * Notes: No checking is done that nBits <= 32.
 * Returns: The reflection of the original data.
 *********************************************************************/
static unsigned long ReflectData(unsigned long data0, unsigned char data0_bits) {
  unsigned long reflection = 0x00000000;
  unsigned char bit = 0;
  // Reflect the data about the center bit.
  for (bit = 0; bit < data0_bits; ++bit) {
    // If the LSB bit is set, set the reflection of it.
    if (data0 & 0x01) {
      reflection |= (1 << ((data0_bits - 1) - bit));
    }
    data0 >>= 1;  // Reflect the next bit.
  }

  return reflection;
} // ReflectData()

/*********************************************************************
 * Function: CheckCrcSlowly().
 * Description: Compute the CRC of a given message bit by bit.
 * Notes: Used only for "CRC-8" because it's inefficient.
 * Returns: The CRC of the message.
 *********************************************************************/
#if ENABLE_BIT_CRC // 是否调用: CheckCrcSlowly()
Crc CheckCrcSlowly(const unsigned char message[], unsigned int message_bytes) {
  Crc remainder = INITIAL_REMAINDER;
  unsigned int byte = 0;
  unsigned char bit = 0;  // The number of unchecked bit.

  // Perform modulo-2 division, a byte at a time.
  for (byte = 0; byte < message_bytes; ++byte) {
    // Bring the next byte into the remainder.
    remainder ^= (REFLECT_DATA(message[byte]) << (CRC_WIDTH - 8));
    // Perform modulo-2 division, a byte at a time.
    for (bit = 8; bit > 0; --bit) {
      // Try to divide the current data bit.
      if (remainder & TOPBIT) { // The top bit is 1 or not.
        // '<<' for normal plynomial, '>>' for reversed polynomial.
        remainder = (remainder << 1) ^ POLYNOMIAL;
      } else {
        remainder = (remainder << 1);
      }
    }
  }
  // The final remainder is the CRC result.
  return (REFLECT_REMAINDER(remainder) ^ FINAL_XOR_VALUE);
} // CheckCrcSlowly().
#endif  // #if ENABLE_BIT_CRC

#if ENABLE_BYTE_CRC  // 是否调用: CheckCrcFaster()
// 可以利用InitCrc()函数将计算出的crc表存储在这里，
// 便于调用CheckCrcFaster()函数时直接查询，不必每次重复计算。
static Crc g_crc_table[256];

/*********************************************************************
 * Function: InitCrc()
 * Description: Populate the partial CRC lookup table.
 * Notes: This function must be rerun any time the CRC standard
 *        is changed. If desired, it can be run "offline" and
 *        the table results stored in an embedded system's ROM.
 * Returns: None defined.
 *********************************************************************/
void InitCrc(void) {
  Crc remainder = 0;
  unsigned char bit = 0;
  unsigned int dividend = 0;
  
  memset(g_crc_table, 0, sizeof(g_crc_table));
  // Compute the remainder of each possible dividend.
  for (dividend = 0; dividend < 256; ++dividend) {
    // Start with the dividend followed by zeros.
    remainder = dividend << (CRC_WIDTH - 8);
      // Perform modulo-2 division, a byte at a time.
    for (bit = 8; bit > 0; --bit) {
      // Try to divide the current data bit.
      if (remainder & TOPBIT) { // The top bit is 1 or not.
        // '<<' for normal plynomial, '>>' for reversed polynomial.
        remainder = (remainder << 1) ^ POLYNOMIAL;
      } else {
        remainder = (remainder << 1);
      }
    }
    g_crc_table[dividend] = remainder;
  }
} // InitCrc().

/*********************************************************************
 * Function: CheckCrcFaster().
 * Description: Compute the CRC of a given message.
 * Notes: InitCrc() must be called first.
 * Returns: The CRC of the message.
 *********************************************************************/
Crc CheckCrcFaster(const unsigned char message[], unsigned int message_bytes) {	                   	
  Crc remainder = INITIAL_REMAINDER;
  unsigned char data0 = 0;
  unsigned int byte = 0;
  
  InitCrc();	// Populate the partial CRC lookup table.
  // Divide the message by the polynomila, a byte at a time.
  for (byte = 0; byte < message_bytes; ++byte) {
    data0 = (REFLECT_DATA(message[byte]) ^ (remainder >> (CRC_WIDTH - 8)));
    remainder = g_crc_table[data0] ^ (remainder << 8);
  }
  // The final remainder is the CRC result.
  return (REFLECT_REMAINDER(remainder) ^ FINAL_XOR_VALUE);
} // CheckCrcFaster().
#endif  // #if ENABLE_BYTE_CRC 
