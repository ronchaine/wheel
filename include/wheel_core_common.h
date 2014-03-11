/*!
   @file
   \brief Contains common definitions
   \author Jari Ronkainen
*/

#ifndef WHEEL_CORE_COMMON_HEADER
#define WHEEL_CORE_COMMON_HEADER

#include <cstdint>
#include <cstdlib>
#include <vector>

#ifdef NDEBUG
   #define WHEEL_NO_DEBUG
#endif

/// Values
#define WHEEL_TRUE                        1
#define WHEEL_FALSE                       0

#define WHEEL_EOF                         0xf100

#define WHEEL_READ_FILE                   ~0

/// Error codes
#define WHEEL_OK                          0x0000
#define WHEEL_RESOURCE_UNAVAILABLE        0x0001
#define WHEEL_INVALID_PATH                0x0002
#define WHEEL_UNKNOWN_FORMAT              0x0003
#define WHEEL_INVALID_VALUE               0x0004
#define WHEEL_UNEXPECTED_END_OF_FILE      0x0005

#define WHEEL_ERROR_INIT_FILESYSTEM       0x0100

#define WHEEL_UNABLE_TO_OPEN_MODULE       0xa000
#define WHEEL_UNABLE_TO_REGISTER_MODULE   0xa001
#define WHEEL_CONFLICTING_MODULE          0xa002
#define WHEEL_MISSING_DEPENDENCIES        0xa003

#define WHEEL_UNIMPLEMENTED_FEATURE       0xff00

#define WHEEL_OUT_OF_MEMORY               0x5500

/// Module error codes
#define WHEEL_MODULE_FAILED_REQUEST       0xef00
#define WHEEL_MODULE_SHADER_COMPILE_ERROR 0xef01
#define WHEEL_MODULE_SHADER_LINK_ERROR    0xef02
#define WHEEL_MODULE_UNSUPPORTED_FEATURE  0xef03
#define WHEEL_MODULE_DEVICE_UNAVAILABLE   0xef04
#define WHEEL_MODULE_CONTEXT_CREATION_ERROR 0xef05

/// Flags
#define WHEEL_INITIALISED                 0x0001
#define WHEEL_LITTLE_ENDIAN               0x0002

// TODO: Keyboard scancodes, follow USB HID, seperate names for different layouts
// Fill according to http://www.freebsddiary.org/APC/usb_hid_usages.php and https://wiki.libsdl.org/SDLScancodeLookup
enum class scancode_t
{
   WHEEL_SCANCODE_US_A = 0x0004,
   WHEEL_SCANCODE_FI_A = 0x0004
};

/// Typedefs, structs & stuff

namespace wheel
{
   typedef int32_t flags_t;

   typedef  std::vector<uint8_t> buffer_t;

   // Required for hash functions
   constexpr bool size_t_x64()
   { return sizeof(size_t) == 8 ? true : false; }

   //! Checks if the system is big-endian
   /*!
      Performs fast check if the system is big-endian
   */
   inline bool big_endian()
   {
      uint16_t t = 0xbe1e;
      if ((uint8_t) *(&t) == 0x1e)
         return false;

      return true;
   }

   /*!
      Read a big-endian stored uint32_t value independent of endianness
   */
   inline uint32_t read_nod_uint32(const buffer_t& buffer, size_t& bufptr)
   {
      uint32_t rval = *(uint32_t*)(&buffer[0] + bufptr);

      bufptr += sizeof(uint32_t);

      if (big_endian())
         return rval;

      rval = (rval & 0xff000000) >> 24
           | (rval & 0x00ff0000) >> 8
           | (rval & 0x0000ff00) << 8
           | (rval & 0x000000ff) << 24;

      return rval;
   }

   /*!
      Read a little-endian uint32_t value independent of endianness
   */
   inline uint32_t read_nod_uint32_le(const buffer_t& buffer, size_t& bufptr)
   {
      uint32_t rval = *(uint32_t*)(&buffer[0] + bufptr);

      bufptr += sizeof(uint32_t);

      if (!big_endian())
         return rval;

      rval = (rval & 0xff000000) >> 24
           | (rval & 0x00ff0000) >> 8
           | (rval & 0x0000ff00) << 8
           | (rval & 0x000000ff) << 24;

      return rval;
   }

}

#endif
