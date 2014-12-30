/*!
   @file
*/

#ifndef WHEEL_IMAGE_DECODERS_HEADER
#define WHEEL_IMAGE_DECODERS_HEADER

#include "wheel_image_common.h"

namespace wheel
{
   namespace image
   {
      uint32_t decode_png(const wheel::string& name, buffer_t& buffer);
   }
}

#endif