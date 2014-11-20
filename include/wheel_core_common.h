/*!
   @file
   \brief Contains common definitions
   \author Jari Ronkainen
*/

#ifndef WHEEL_CORE_COMMON_HEADER
#define WHEEL_CORE_COMMON_HEADER

#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <list>

#include <iostream>

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
#define WHEEL_RESOURCE_BUSY               0x0006

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

// Events
#define WHEEL_EVENT_WINDOW       0x00

// 0x0y -- input event
#define WHEEL_EVENT_KEYBOARD     0x01
#define WHEEL_EVENT_MOUSE        0x02
#define WHEEL_EVENT_CONTROLLER   0x03
#define WHEEL_EVENT_TOUCH        0x04

// specifiers
#define WHEEL_PRESS              0x00
#define WHEEL_RELEASE            0x01
#define WHEEL_AXIS_POSITION      0x02
#define WHEEL_HAT_POSITION       0x03
#define WHEEL_MOUSE_POSITION     0x04

#define WHEEL_ANY                0xfe

// 0x1y timer event
#define WHEEL_EVENT_TIMER        0x10


// These values correspond to OpenGL numbers

#define WHEEL_PIXEL_FMT_NONE        ~0
#define WHEEL_PIXEL_FMT_BGR         0x80e0
#define WHEEL_PIXEL_FMT_BGRA        0x80e1
#define WHEEL_PIXEL_FMT_RGBA        0x1908
#define WHEEL_PIXEL_FMT_RGB         0x1907
#define WHEEL_PIXEL_FMT_MONO        0x1903
#define WHEEL_PIXEL_FMT_RG          0x8227

#define WHEEL_BYTE                  0x1400
#define WHEEL_UNSIGNED_BYTE         0x1401
#define WHEEL_SHORT                 0x1402
#define WHEEL_UNSIGNED_SHORT        0x1403
#define WHEEL_INT                   0x1404
#define WHEEL_UNSIGNED_INT          0x8035
#define WHEEL_FLOAT                 0x1406
#define WHEEL_DOUBLE                0x140A

// TODO: Keyboard scancodes, follow USB HID, seperate names for different layouts
// Fill according to http://www.freebsddiary.org/APC/usb_hid_usages.php and https://wiki.libsdl.org/SDLScancodeLookup
enum scancode_t
{
   WHEEL_SCANCODE_US_A        = 0x0004,
   WHEEL_SCANCODE_FI_A        = 0x0004,
   WHEEL_SCANCODE_LEFTARROW   = 0x0050,
   WHEEL_SCANCODE_RIGHTARROW  = 0x004f,
   WHEEL_SCANCODE_UPARROW     = 0x0052,
   WHEEL_SCANCODE_DOWNARROW   = 0x0051,
};

/// Typedefs, structs & stuff

namespace wheel
{
   typedef int32_t flags_t;

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
   //! Swaps endianness of an value
   /*!
       Swaps endianness of an value
   */
   template <typename T>
   T endian_swap(T value)
   {
      union
      {
         T value;
         uint8_t value_u8[sizeof(T)];
      } src, dst;

      src.value = value;

      for (size_t i = 0; i < sizeof(T); ++i)
         dst.value_u8[i] = src.value_u8[sizeof(T)-1-i];

      return dst.value;
   }

   class buffer_t : public std::vector<uint8_t>
   {
      public:
         buffer_t()
         {
            read_ptr = 0;
         }

         size_t read_ptr;
         inline const uint8_t* getptr() const { return &this->at(0); }
         inline size_t hash() const noexcept;

         inline std::string to_stl_string()
         {
            return std::string(this->begin(), this->end());
         }

         inline void from_stl_string(const std::string& s)
         {
            this->clear();
            std::copy(s.begin(), s.end(), back_inserter(*this));
         }

         template <typename T>
         inline T read()
         {
            T rval = *(T*)&(this->at(read_ptr));

            if (big_endian())
               rval = endian_swap(rval);

            read_ptr += sizeof(T);

            return rval;
         }

         template <typename T>
         inline void write(const T& data)
         {
            this->reserve(this->size() + sizeof(T));

            uint8_t* dataptr = nullptr;
            T t_val;

            if (big_endian())
            {
               t_val = endian_swap(data);
               dataptr = (uint8_t*)&t_val;
            } else {
               dataptr = (uint8_t*)&data;
            }

            for (size_t it = 0; it < sizeof(T); ++it)
               this->push_back(*(dataptr+it));
         }

         inline size_t pos()
         {
            return read_ptr;
         }

         inline void seek(size_t target)
         {
            if (target < size()) read_ptr = target;
         }
   };

   //! Event
   /*!
   */
   class Event
   {
      public:
         buffer_t data;

         uint32_t GetType();
   };

   typedef std::list<Event> EventList;

   // FIXME:  unimplemented: describe_event()
   template<typename ...Args>
   inline wheel::Event describe_event(Args ... codes)
   {
      wheel::Event rval;
      uint8_t args[] { (uint8_t)codes ... };

      for (uint8_t c : args)
      {
         rval.data.push_back(c);
      }

      return rval;
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


namespace wheel
{
   // This voodoo is to find out whether size_t is 8 or 4 bytes long, it could be
   // easily extended to different sizes, but for now this is enough.
   template<typename T, bool is_x86_64 = size_t_x64()>
   class Hash
   {
      public:
         size_t operator()(const T&)
         {
            assert(0 && "unknown bit depth");
         }
   };

   // 64-bit buffer hash
   template<>
   class Hash<buffer_t, true>
   {
      public:
         size_t operator()(const buffer_t& s) const noexcept
         {
            if (s.size() == 0)
               return 0;

            size_t hash  = 0xCBF29CE484222325;
            const uint64_t prime = 0x100000001B3;

            uint8_t* bytep = (uint8_t*)s.getptr();

            for (size_t it = 0; it < s.size(); ++it)
            {
               hash = (hash ^ *(bytep+it)) * prime;
            }

            return hash;
         }
   };

   // 32-bit buffer hash
   template<>
   class Hash<buffer_t, false>
   {
      public:
         size_t operator()(const buffer_t& s) const noexcept
         {
            if (s.size() == 0)
               return 0;

            size_t hash = 0x811C9DC5;
            const uint32_t prime = 0x1000193;

            uint8_t* bytep = (uint8_t*)s.getptr();

            for (size_t it = 0; it < s.size(); ++it)
            {
               hash = (hash ^ *(bytep+it)) * prime;
            }

            return hash;
         }
   };

   /*!
      Return hash of the string, note that the hash is not currently cached.

      \return FNV-1a hash.
   */
   inline size_t buffer_t::hash() const noexcept
   {
      wheel::Hash<wheel::buffer_t> rval_hash;
      return rval_hash(*this);
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
