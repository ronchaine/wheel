/*!
   @file
   \brief Common image declarations
*/

#ifndef WHEEL_IMAGE_COMMON_HEADER
#define WHEEL_IMAGE_COMMON_HEADER

#include "wheel_core_common.h"
#include "wheel_core_debug.h"
#include "wheel_module_video.h"

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
               format = WHEEL_FILE_FORMAT_UNKNOWN;
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
