/*!
   @file
   \brief Contains definitions for utility functions, such as hashing
*/

#ifndef WHEEL_UTILITY_HEADER
#define WHEEL_UTILITY_HEADER

#include "wheel_core_common.h"
#include "wheel_core_string.h"

#include <cstdint>
#include <algorithm>

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

   //! Split by delim
   template <class InputIt, class FwdIt, class BinOp>
   void for_each_token(InputIt first, InputIt last,
                       FwdIt sfirst, FwdIt slast,
                       BinOp binary_op)
   {
      while(first != last)
      {
         const auto pos = std::find_first_of(first, last, sfirst, slast);
         binary_op(first, pos);
         if (pos == last)
            break;

         first = std::next(pos);
      }
   }

   //! Reads a big-endian value from a buffer and increments pointer.
   /*!
      \param buffer     Buffer to be read
      \param location   Pointer to the data inside the buffer

      \return Data in system native endian format.
   */
   template <typename T>
   T buffer_read(const buffer_t& buffer, size_t& location)
   {
      //T rval = *(T*)(&buffer[0] + location);
      T rval = *(T*)(buffer.getptr() + location);
      location += sizeof(T);

      if (!IsBigEndian())
         return rval;

      if (sizeof(T) == 2)
      {
         uint16_t tmpval = *(uint16_t*)(&rval);

         tmpval = (tmpval & 0xff00) >> 8
                | (tmpval & 0x00ff) << 8;

         rval = *(T*)(&tmpval);
      }
      else if (sizeof(T) == 4)
      {
         uint32_t tmpval = *(uint32_t*)(&rval);

         tmpval = (tmpval & 0xff000000) >> 24
                | (tmpval & 0x00ff0000) >> 8
                | (tmpval & 0x0000ff00) << 8
                | (tmpval & 0x000000ff) << 24;

         rval = *(T*)(&tmpval);
      }
      else if (sizeof(T) == 8)
      {
         rval = (rval & 0xff00000000000000) >> 56
              | (rval & 0x00ff000000000000) >> 40
              | (rval & 0x0000ff0000000000) >> 24
              | (rval & 0x000000ff00000000) >> 8
              | (rval & 0x00000000ff000000) << 8
              | (rval & 0x0000000000ff0000) << 24
              | (rval & 0x000000000000ff00) << 40
              | (rval & 0x00000000000000ff) << 56;
      }

      return rval;
   }

   //! Reads a little-endian value from a buffer and increments pointer.
   /*!
      \param buffer     Buffer to be read
      \param location   Pointer to the data inside the buffer

      \return Data in system native endian format.
   */
   template <typename T>
   T buffer_read_le(const buffer_t& buffer, size_t& location)
   {
//      T rval = *(T*)(&buffer[0] + location);
      T rval = *(T*)(buffer.getptr() + location);

      location += sizeof(T);

      if (IsBigEndian())
         return rval;

      if (sizeof(T) == 2)
      {
         uint16_t tmpval = *(uint16_t*)(&rval);

         tmpval = (tmpval & 0xff00) >> 8
                | (tmpval & 0x00ff) << 8;

         rval = *(T*)(&tmpval);
      }
      else if (sizeof(T) == 4)
      {
         uint32_t tmpval = *(uint32_t*)(&rval);

         tmpval = (tmpval & 0xff000000) >> 24
                | (tmpval & 0x00ff0000) >> 8
                | (tmpval & 0x0000ff00) << 8
                | (tmpval & 0x000000ff) << 24;

         rval = *(T*)(&tmpval);
      }
      else if (sizeof(T) == 8)
      {
         rval = (rval & 0xff00000000000000) >> 56
              | (rval & 0x00ff000000000000) >> 40
              | (rval & 0x0000ff0000000000) >> 24
              | (rval & 0x000000ff00000000) >> 8
              | (rval & 0x00000000ff000000) << 8
              | (rval & 0x0000000000ff0000) << 24
              | (rval & 0x000000000000ff00) << 40
              | (rval & 0x00000000000000ff) << 56;
      }

      return rval;
   }

   //! Calculate crc32
   /*!
      \return CRC32 value of the buffer given
   */
   uint32_t crc32(uint8_t* buffer, size_t len);
   uint32_t update_crc(uint32_t crc, uint8_t* buf, size_t len);

   //! Timer
   /*!
      A timer that counts microseconds
   */
   class Timer
   {
      private:
         std::chrono::steady_clock::time_point  start;
         wheel::string                          id;

         uint64_t                               usec;

      public:
         Timer(wheel::string id, uint64_t usec, bool repeat);

         void Reset();
         bool Check();

         wcl::string getID() { return id; }

         bool                                   repeat;
   };
}

#endif //WHEEL_UTILITY_HEADER
