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
#include "../../include/wheel_sound_library.h"

namespace wheel
{
   namespace audio
   {
      /*!
         \brief  Checks file format of an sound.

         \return WHEEL_SOUNDFORMAT_X, where X is either UNKNOWN or a known format.
      */
      uint32_t CheckFileFormat(const buffer_t& buffer)
      {
         if (buffer.size() > 25)
            if ((buffer[0] == 'O')
            && (buffer[1] == 'g')
            && (buffer[2] == 'g')
            && (buffer[3] == 'S'))
               return WHEEL_SOUNDFORMAT_OGG;

         if (buffer.size() > 12)
            if ((buffer[0] == 'R')
            && (buffer[1] == 'I')
            && (buffer[2] == 'F')
            && (buffer[3] == 'F')

            && (buffer[8] == 'W')
            && (buffer[9] == 'A')
            && (buffer[10] == 'V')
            && (buffer[11] == 'E'))

               return WHEEL_SOUNDFORMAT_WAV;

         return WHEEL_SOUNDFORMAT_UNKNOWN;
      }

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

            if (format == WHEEL_SOUNDFORMAT_WAV)
            {

            }
            if (format == WHEEL_SOUNDFORMAT_OGG)
            {
               log << "Trying to load ogg file\n";
            }

            if (format == WHEEL_SOUNDFORMAT_UNKNOWN)
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