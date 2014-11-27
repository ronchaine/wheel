/*!
   @file
   \brief Image atlasing and loading
*/

#ifndef WHEEL_IMAGE_PNG_HEADER
#define WHEEL_IMAGE_PNG_HEADER

#include "wheel_image_common.h"

namespace wheel
{
  
   namespace image
   {
      struct PNGChunk;

      class PNG : public Image
      {
         private:
            std::vector<PNGChunk*> chunks;

            void decode_idat_data();

            void ReadChunks();

         public:
            uint32_t Decode(const buffer_t& buffer);
            void     DisplayInfo();

            uint32_t GetWidth();
            uint32_t GetHeight();

           ~PNG();
      };
   }
}

#endif