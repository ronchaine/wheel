/*!
   @file
   \brief Contains definitions for everything needed to load external modules
*/

#ifndef WHEEL_MODULE_VIDEO_SDL_OPENGL_HEADER
#define WHEEL_MODULE_VIDEO_SDL_OPENGL_HEADER

#include "../../../include/wheel_core_module.h"

namespace wheel
{
   namespace video
   {
      class SDLRenderer : public wheel::core::Module, public Renderer
      {
         public:
            void get_module_info(core::modinfo_t* info);
      };     
   }
}

#endif