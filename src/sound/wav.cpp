#include "../../include/wheel_core_debug.h"
#include "../../include/wheel_core_string.h"
#include "../../include/wheel_core_utility.h"

#include "../../include/wheel_sound_wav.h"

#include <cstring>

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
      {
         for (size_t i = 0; i < chunks.size(); ++i)
            delete chunks[i];
      }

      void WAV::DisplayInfo()
      {
         log << "WAV data\n";
         for (auto chunk : chunks)
         {
            string s(chunk->type, 4);
            log << "   WAV Chunk: " << s << std::dec << ", length " << chunk->len << "\n";

            if (s == "fmt ")
            {
               log << "      Format:\n";
               log << "         Compression code: " << *(uint16_t*)chunk->data + 0 << "\n";
               log << "         Number of channels: " << *(uint16_t*)(chunk->data + 2) << "\n";
               log << "         Sampling rate: " << *(uint32_t*)(chunk->data + 4) << "\n";
               log << "         Avg. bytes per second: " << *(uint32_t*)(chunk->data + 8) << "\n";
               log << "         Block align: " << *(uint16_t*)(chunk->data + 12) << "\n";
               log << "         Sig. bits per sample: " << *(uint16_t*)(chunk->data + 14) << "\n";
            }
            if (s == "data")
            {
               log << "      " << chunk->len << " bytes of WAVE data\n";
            }
         }
      }

      uint32_t WAV::Load(const buffer_t& buffer)
      {
         size_t bufptr = 12;

         while(bufptr < buffer.size())
         {
            WAVChunk* chunk = new WAVChunk;

            if (buffer.size() < bufptr + sizeof(char) * 4)
            {
               return WHEEL_UNEXPECTED_END_OF_FILE;
            }
            strncpy(chunk->type, (const char*)(&buffer[0] + bufptr), 4);

            bufptr += sizeof(char) * 4;

            chunk->len = buffer_read_le<uint32_t>(buffer, bufptr);

            if (buffer.size() < bufptr + chunk->len)
            {
               delete chunk;
               return WHEEL_UNEXPECTED_END_OF_FILE;
            }

            chunk->data = (uint8_t*)malloc(chunk->len);
            memcpy(chunk->data, (&buffer[0] + bufptr), chunk->len);
            bufptr += chunk->len;

            chunks.push_back(chunk);

            // We reached the end of file
            if (bufptr == buffer.size() -1)
               break;
         }

         return WHEEL_OK;
      }
   }
}