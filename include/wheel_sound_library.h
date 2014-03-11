/*!
   @file
   \brief Sound handling and loading
*/

#ifndef WHEEL_SOUND_LIBRARY_HEADER
#define WHEEL_SOUND_LIBRARY_HEADER

#include "wheel_sound_common.h"

#include <unordered_map>

namespace wheel
{
   namespace audio
   {
      /*
         \brief Library for audio handling
      */
      class Library
      {
         private:
            std::unordered_map<string, Sound*> soundcache;

         public:
            const uint8_t* GetRawData();

            uint32_t       AddBuffer(const string& name, uint32_t fmt, const buffer_t* buffer);
            uint32_t       Add(const string& filename);

            uint32_t       Savetofile(const string& filename);

            Library();
           ~Library();
      };
   }
}

#endif