/*!
   @file
   \brief Contains implementations
   \author Jari Ronkainen
*/

#include "../../include/wheel_core_debug.h"
#include "../../include/wheel_core_string.h"
#include "../../include/wheel_core_utility.h"
#include "../../include/wheel_core_resource.h"

#include "../../include/wheel_image_png.h"
#include "../../include/wheel_image_library.h"

namespace wheel
{
   namespace image
   {
      uint32_t CheckFileFormat(const buffer_t& buffer)
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
               return WHEEL_IMAGEFORMAT_PNG;

         return WHEEL_IMAGEFORMAT_UNKNOWN;
      }

      Library::Library()
      {}

      Library::~Library()
      {
         for (auto img : imagecache)
         {
            log << "deleting " << img.second << "\n";
            delete img.second;
         }
      }

      uint32_t Library::AddBuffer(const string& name, const buffer_t* buffer)
      {
         if (buffer == nullptr)
            return WHEEL_INVALID_VALUE;

         uint32_t format = CheckFileFormat(*buffer);

         if (format == WHEEL_IMAGEFORMAT_PNG)
         {
            PNG* img = new PNG();
            img->Load(*buffer);
            img->DisplayInfo();
         }

         return WHEEL_OK;
      }

      uint32_t Library::Add(const string& file)
      {
         const wheel::buffer_t* buf = wheel::GetBuffer(file);
         return AddBuffer(file, buf);
      }
   }
}