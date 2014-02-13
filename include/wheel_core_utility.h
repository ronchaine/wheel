/*!
   @file
   \brief Contains definitions for utility functions, such as hashing
*/

#ifndef WHEEL_UTILITY_HEADER
#define WHEEL_UTILITY_HEADER

#include "wheel_core_common.h"

#include <cstdint>

namespace wheel
{
   //! Check for system endianness
   /*!
      \return <code>TRUE</code> if the system is big-endian, otherwise <code>FALSE</code>
   */
   inline bool IsBigEndian()
   {
      static const uint16_t endian_check_value        = 0xbe1e;
      static const uint8_t  endian_check_value_first  = *(uint8_t*)&endian_check_value;

      return (endian_check_value_first == 0x1e);
   }

   //! Calculate crc32
   /*!
      \return CRC32 value of the buffer given
   */
   uint32_t crc32(uint8_t* buffer, size_t len);
   uint32_t update_crc(uint32_t crc, uint8_t* buf, size_t len);
}

#endif //WHEEL_UTILITY_HEADER
