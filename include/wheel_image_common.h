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

            uint32_t width;
            uint32_t height;
            uint32_t channels;

            buffer_t image_data;

         public:
            Image() : format(WHEEL_FILE_FORMAT_UNKNOWN), width(0), height(0), channels(0) {}

            virtual ~Image() {}

            uint32_t Load(const wheel::string& file);

            uint32_t GetWidth() { return width; };
            uint32_t GetHeight() { return height; };

            buffer_t* GetBuffer() { return &image_data; }
      };
   }
}

#endif
