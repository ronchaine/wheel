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

// File formats
#define WHEEL_FILE_FORMAT_UNKNOWN   0x00
#define WHEEL_FILE_FORMAT_PNG       0x01
#define WHEEL_FILE_FORMAT_OGG       0x02
#define WHEEL_FILE_FORMAT_WAV       0x03

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

//   typedef  std::vector<uint8_t> buffer_t;
/*
   class buffer_t
   {
      class _buf_proxy
      {
         private:
            buffer_t&   buf;
            int         idx;
         public:
            _buf_proxy(buffer_t& buf, int idx) : buf(buf), idx(idx) {}
            uint8_t& operator=(uint8_t x) { buf.data[idx] = x; return buf.data[x]; }
      };

      private:
         std::vector<uint8_t> data;
      public:
         _buf_proxy operator[](int index) { return _buf_proxy(*this, index); }
         uint8_t operator[](int index) const { return data[index]; }

         inline const uint8_t* data_ptr() const { return &data[0]; }

         inline size_t size() const { return data.size(); }
         inline void resize(size_t size) { data.resize(size); }

         inline size_t hash() const { return 0; }

         inline uint8_t at(int index) { return data.at(index); }

         inline 
   };
*/
   class buffer_t : public std::vector<uint8_t>
   {
      public:
         inline const uint8_t* data_ptr() const { return &this->at(0); }
   };

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
      \brief  Checks file format of a file.

      \return WHEEL_FILE_FORMAT_X, where X is either UNKNOWN or a known format.
   */
   inline uint32_t CheckFileFormat(const buffer_t& buffer)
      {
         if (buffer.size() > 8)
            if ((buffer[0] == 137)
            && (buffer[1] == 80)
            && (buffer[2] == 78)
            && (buffer[3] == 71)
            && (buffer[4] == 13)
            && (buffer[5] == 10)
            && (buffer[6] == 26)
            && (buffer[7] == 10))
               return WHEEL_FILE_FORMAT_PNG;

         if (buffer.size() > 25)
            if ((buffer[0] == 'O')
            && (buffer[1] == 'g')
            && (buffer[2] == 'g')
            && (buffer[3] == 'S'))
               return WHEEL_FILE_FORMAT_OGG;

         if (buffer.size() > 44)
            if ((buffer[0] == 'R')
            && (buffer[1] == 'I')
            && (buffer[2] == 'F')
            && (buffer[3] == 'F')

            && (buffer[8] == 'W')
            && (buffer[9] == 'A')
            && (buffer[10] == 'V')
            && (buffer[11] == 'E'))
               return WHEEL_FILE_FORMAT_WAV;

         return WHEEL_FILE_FORMAT_UNKNOWN;
      }
}


namespace std
{
   template<>
   struct hash<wheel::buffer_t>
   {
      size_t operator()(const wheel::buffer_t& __s) const noexcept
      {
         return __s.hash();
      }
   };
}

#endif
