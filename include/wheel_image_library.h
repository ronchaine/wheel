/*!
   @file
   \brief Image atlasing and loading
*/

#ifndef WHEEL_IMAGE_LIBRARY_HEADER
#define WHEEL_IMAGE_LIBRARY_HEADER

#include "wheel_image_common.h"

#include <unordered_map>

namespace wheel
{
   namespace image
   {
      /*!
         \brief Library for image handling
      */
      class Library
      {
         private:
            std::unordered_map<string, Image*> imagecache;

         public:
            const uint8_t* GetRawData();

            uint32_t       AddBuffer(const string& name, const buffer_t* buffer);
            uint32_t       Add(const string& filename);

            uint32_t       Savetofile(const string& filename);

            Library();
           ~Library();
      };
   }
}

#endif