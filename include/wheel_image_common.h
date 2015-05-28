/*!
   @file
   \brief Common image declarations
*/

#ifndef WHEEL_IMAGE_COMMON_HEADER
#define WHEEL_IMAGE_COMMON_HEADER

#include "wheel_core_common.h"
#include "wheel_core_debug.h"
#include "wheel_video.h"
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
         public:
            uint32_t width;
            uint32_t height;
            uint16_t channels;
            uint16_t bpp;

            std::vector<uint32_t> palette;

            Image() : width(0), height(0), channels(0) {}

            virtual ~Image() {}
      };
   }
}

#endif
