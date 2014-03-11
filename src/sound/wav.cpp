#include "../../include/wheel_sound_wav.h"

namespace wheel
{
   namespace audio
   {
      /*!
         Struct to store WAV/RIFF chunk data
      */
      struct WAVChunk
      {
         uint32_t    len;
         char        type[4];
         uint8_t*    data;
         uint32_t    crc;

         WAVChunk()
         {
            data = nullptr;
         }
        ~WAVChunk()
         {
            if (data != nullptr)
               free(data);
         }
      };

      WAV::WAV()
      {}

      WAV::~WAV()
      {}

      uint32_t WAV::Load(const buffer_t& buffer)
      {
         return WHEEL_UNIMPLEMENTED_FEATURE;
      }
   }
}