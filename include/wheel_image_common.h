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
      typedef std::vector<uint32_t> palette_t;

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

            /*
               WHEEL_PIXEL_FMT_BGR     [ ]
               WHEEL_PIXEL_FMT_BGRA    [ ]
               WHEEL_PIXEL_FMT_RGBA    [ ]
               WHEEL_PIXEL_FMT_RGB     [ ]
               WHEEL_PIXEL_FMT_MONO    [ ]
               WHEEL_PIXEL_FMT_RG      [ ]
            */
            uint32_t format;

            palette_t palette;

            void convert(const uint32_t format) {};

            // 
            uint32_t create_optimum_palette(palette_t& target);

            // Return ID to optimum palette of palette_t palettes.
            uint32_t find_optimum_palette(const std::vector<palette_t>& palette) {};

            Image() : width(0), height(0), channels(0) {}
            virtual ~Image() {}
      };
   }
}

#endif
