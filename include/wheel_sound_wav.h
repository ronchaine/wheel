/*!
   @file
   \brief WAV file loading
*/

#ifndef WHEEL_SOUND_WAV_HEADER
#define WHEEL_SOUND_WAV_HEADER

#include "wheel_sound_common.h"

namespace wheel
{
   namespace audio
   {
      struct WAVChunk;

      class WAV : public Sound
      {
         private:
            std::vector<WAVChunk*> chunks;

         public:
            uint32_t Load(const buffer_t& buffer);
            void     DisplayInfo();

            WAV();
           ~WAV();
      };
   }
}

#endif