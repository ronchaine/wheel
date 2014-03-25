/*!
   @file
   \brief Contains implementations
   \author Jari Ronkainen
*/

#include "../../include/wheel_core_debug.h"
#include "../../include/wheel_core_string.h"
#include "../../include/wheel_core_utility.h"
#include "../../include/wheel_core_resource.h"

#include "../../include/wheel_sound.h"
#include "../../include/wheel_sound_wav.h"
#include "../../include/wheel_sound_library.h"

namespace wheel
{
   namespace audio
   {
      Library::Library()
      {}

      Library::~Library()
      {
         for (auto img : soundcache)
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

            if (format == WHEEL_FILE_FORMAT_WAV)
            {
               WAV* snd = new WAV();
               snd->Load(*buffer);
               snd->DisplayInfo();
            }
            if (format == WHEEL_FILE_FORMAT_OGG)
            {
               log << "Trying to load ogg file\n";
            }

            if (format == WHEEL_FILE_FORMAT_UNKNOWN)
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