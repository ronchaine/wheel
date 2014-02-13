/*!
   @file
   \brief Image atlasing and loading
*/

#ifndef WHEEL_IMAGE_COMMON_HEADER
#define WHEEL_IMAGE_COMMON_HEADER

#include "wheel_core_common.h"
#include "wheel_core_debug.h"

#define WHEEL_IMAGEFORMAT_UNKNOWN   0x00
#define WHEEL_IMAGEFORMAT_PNG       0x01

#define WHEEL_PIXEL_FMT_NONE        ~0
#define WHEEL_PIXEL_FMT_BGR         0x80e0
#define WHEEL_PIXEL_FMT_BGRA        0x80e1
#define WHEEL_PIXEL_FMT_RGBA        0x1908
#define WHEEL_PIXEL_FMT_RGB         0x1907
#define WHEEL_PIXEL_FMT_MONO        0x1903

namespace wheel
{
   namespace image
   {
      /*!
         Core class for image handling, derive from this for different file formats.
      */
      class Image
      {
         protected:
            uint32_t format;
            buffer_t image_data;

         public:
            Image()
            {
               format = WHEEL_IMAGEFORMAT_UNKNOWN;
            }

            virtual ~Image() {}

            virtual uint32_t Load(const buffer_t& buffer) = 0;

            virtual uint32_t GetWidth() = 0;
            virtual uint32_t GetHeight() = 0;

            virtual buffer_t* GetBuffer() { return &image_data; }
      };
   }
}

#endif
