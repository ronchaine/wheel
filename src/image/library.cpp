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
      /*!
         \brief  Checks file format of an image.

         \return WHEEL_IMAGEFORMAT_X, where X is either UNKNOWN or a known format.
      */
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

      //! Add an image stored in a memory buffer to the texture library
      /*!
         \param name    Name used to later retrieve the buffer.
         \param 
         \returns <code>WHEEL_OK</code> on success, otherwise an unsigned integer describing the error.
      */
      uint32_t Library::AddBuffer(const string& name, uint32_t fmt, const buffer_t* buffer)
      {
         if (buffer == nullptr)
            return WHEEL_INVALID_VALUE;

         if (fmt == WHEEL_READ_FILE)
         {
            uint32_t format = CheckFileFormat(*buffer);

            if (format == WHEEL_IMAGEFORMAT_PNG)
            {
               PNG* img = new PNG();
               img->Load(*buffer);
               img->DisplayInfo();
            }

            if (format == WHEEL_IMAGEFORMAT_UNKNOWN)
               return WHEEL_UNKNOWN_FORMAT;
         }

         return WHEEL_OK;
      }

      uint32_t Library::Add(const string& file)
      {
         const wheel::buffer_t* buf = wheel::GetBuffer(file);
         return AddBuffer(file, WHEEL_READ_FILE, buf);
      }

   }
}