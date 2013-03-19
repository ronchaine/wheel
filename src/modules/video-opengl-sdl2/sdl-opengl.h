/*!
   @file
   \brief Contains definitions for everything needed to load external modules
*/

#ifndef WHEEL_MODULE_VIDEO_SDL_OPENGL_HEADER
#define WHEEL_MODULE_VIDEO_SDL_OPENGL_HEADER

#include "../../../include/wheel_core_module.h"
#include "../../../include/wheel_module_video.h"

namespace wheel
{
   namespace video
   {
      class SDLRenderer : public wheel::core::Module, public wheel::video::Renderer
      {
         public:
            void get_module_info(core::modinfo_t* info);

//            SDLRenderer();
            ~SDLRenderer();
      };     
   }
}

#endif