/*!
   @file
   \brief Contains implementations for utility functions, such as hashing.
   \author Jari Ronkainen
*/

#include "../include/wheel_core_utility.h"

namespace wheel
{
   namespace internal
   {
      uint32_t crc_table[256];
      bool     crc_table_ready = false;

      void     calculate_crc_table()
      {
         uint32_t c, n, k;

         for (n = 0; n < 256; ++n)
         {
            c = n;
            for (k = 0; k < 8; ++k)
            {
               if (c & 1)
                  c = 0xedb88320 ^ (c >> 1);
               else
                  c = c >> 1;
            }
            crc_table[n] = c;
         }

         crc_table_ready = true;
      }
   }

   uint32_t update_crc(uint32_t crc, uint8_t* buf, size_t len)
   {
      uint32_t c = crc;

      if (!internal::crc_table_ready)
         internal::calculate_crc_table();

      for (uint32_t n = 0; n < len; ++n)
      {
         c = internal::crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
      }

      return c;
   }

   uint32_t crc32(uint8_t* buf, size_t len)
   {
      return update_crc(0xffffffff, buf, len) ^ 0xffffffff;
   }

   //! Hash a string
   /*!
      If no flags are specified, this function returns the same value as std::hash would.
      
   */
}