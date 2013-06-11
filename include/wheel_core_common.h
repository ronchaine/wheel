/*!
   @file
   \brief Contains common definitions
   \author Jari Ronkainen
*/

#ifndef WHEEL_CORE_COMMON_HEADER
#define WHEEL_CORE_COMMON_HEADER

#include <cstdint>
#include <cstdlib>

#ifdef NDEBUG
   #define WHEEL_NO_DEBUG
#endif

/// Values
#define WHEEL_TRUE                        1
#define WHEEL_FALSE                       0

/// Error codes
#define WHEEL_OK                          0x0000
#define WHEEL_RESOURCE_UNAVAILABLE        0x0001
#define WHEEL_INVALID_PATH                0x0002

#define WHEEL_UNABLE_TO_OPEN_MODULE       0xa000
#define WHEEL_UNABLE_TO_REGISTER_MODULE   0xa001

#define WHEEL_UNIMPLEMENTED_FEATURE       0xff00
                                       // 0xffffffff

/// Module error codes
#define WHEEL_MODULE_FAILED_REQUEST       0xefff

/// Flags
#define WHEEL_INITIALISED                 0x0001
#define WHEEL_LITTLE_ENDIAN               0x0002

/// Typedefs, structs & stuff

namespace wheel
{
   typedef int32_t flags_t;

   struct buffer_t
   {
      size_t   size;
      size_t   pos;

      uint8_t* data;

      buffer_t() {
         size = 0;
         data = nullptr;
         pos = 0;
      }

     ~buffer_t() {}
   };

   // Required for hash functions
   constexpr bool size_t_x64()
   { return sizeof(size_t) == 8 ? true : false; }
}

#endif
