/*!
   @file
   \brief Common sound declarations
*/

#ifndef WHEEL_SOUND_COMMON_HEADER
#define WHEEL_SOUND_COMMON_HEADER

#include "wheel_core_common.h"
#include "wheel_core_debug.h"
#include "wheel_module_audio.h"

namespace wheel
{
   namespace audio
   {
      struct sound_data_t
      {
         uint32_t format;
         buffer_t data;
      };

      struct sound_info_t
      {
         uint32_t    len_µs;
         uint32_t    bitrate;
         float       freqrate;
         uint32_t    size;
         uint32_t    channels;

         sound_info_t(uint32_t l, uint32_t btr, float freqrate, uint32_t siz, uint32_t cha) :
            len_µs(l), bitrate(btr), freqrate(freqrate), size(siz), channels(cha) {}
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
               sound.format = WHEEL_FILE_FORMAT_UNKNOWN;
            }
            virtual ~Sound() {}
            
            virtual uint32_t  Load(const buffer_t& buffer);

            virtual buffer_t* GetBuffer() { return &sound.data; }
            sound_info_t GetInfo() { return sound_info_t(len_µs, bitrate, freqrate, size, channels); }
      };
   }
}

#endif