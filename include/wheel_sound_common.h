/*!
   @file
   \brief Common sound declarations
*/

#ifndef WHEEL_SOUND_COMMON_HEADER
#define WHEEL_SOUND_COMMON_HEADER

#include "wheel_core_common.h"
#include "wheel_core_debug.h"
#include "wheel_module_audio.h"

#define WHEEL_SOUNDFORMAT_UNKNOWN   0x00
#define WHEEL_SOUNDFORMAT_OGG       0x01

namespace wheel
{
   namespace audio
   {
      struct sound_data_t
      {
         uint32_t format;
         buffer_t data;
      };

      /*!
         Core class for sound handling, derive from this for different file formats.
      */
      class Sound
      {
         protected:
            uint32_t    len_µs;
            uint32_t    bitrate;
            float       freqrate;

            uint32_t    size;
            uint32_t    channels;

            sound_data_t sound;

         public:
            Sound() : len_µs(0), bitrate(0), freqrate(0), size(0), channels(0)
            {
               sound.format = WHEEL_SOUNDFORMAT_UNKNOWN;
            }
            virtual ~Sound() {}
            
            virtual uint32_t  Load(const buffer_t& buffer);

            virtual buffer_t* GetBuffer() { return &sound.data; }
      };
   }
}

#endif