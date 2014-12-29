/*!
   @file
   \brief Common image declarations
*/

#ifndef WHEEL_IMAGE_COMMON_HEADER
#define WHEEL_IMAGE_COMMON_HEADER

#include "wheel_core_common.h"
#include "wheel_core_debug.h"
#include "wheel_module_video.h"
#include "wheel_core_resource.h"

namespace wheel
{
   namespace image
   {
      /*!
         Image handling
      */
      class Image : public wheel::Resource
      {
         protected:
            uint32_t img_w;
            uint32_t img_h;
            uint32_t channels;
            uint32_t bpp;

            uint32_t type;

            buffer_t palette;

         public:
            Image() : img_w(0), img_h(0), channels(0) {}

            virtual ~Image() {}

            uint32_t width() { return img_w; };
            uint32_t height() { return img_h; };
      };
   }
}

#endif
