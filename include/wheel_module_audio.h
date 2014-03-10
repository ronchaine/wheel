/*!
   @file
   \brief General audio interface
   \author Jari Ronkainen
*/

#ifndef WHEEL_MODULE_AUDIO_INTERFACE_H
#define WHEEL_MODULE_AUDIO_INTERFACE_H

#include "wheel_core_module.h"

namespace wheel
{
   namespace interface
   {
      //! Interface for audio modules
      class Audio : public Module
      {
         public:
            //! Opens an audio device and creates associated context.
            /*!
               \return <code>WHEEL_OK</code> on success, or a value depicting an error.
            */
            virtual  uint32_t OpenDevice(const string& s = "", size_t channels = 32) = 0;

            //! Plays a sound
            /*!
            */
            virtual  uint32_t Play(const string& sound) = 0;

            virtual  std::vector<string> ListDevices() = 0;
      };
   }
}

#endif