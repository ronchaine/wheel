/*!
   @file
   \brief Contains definitions for everything needed to load external modules
*/

#ifndef WHEEL_MODULE_VIDEO_SDL_OPENGL_HEADER
#define WHEEL_MODULE_VIDEO_SDL_OPENGL_HEADER

#include "../../../include/wheel_core_module.h"
#include "../../../include/wheel_module_video.h"

#define SDL_NO_COMPAT
#include <SDL2/SDL.h>

#define WHEEL_SDL_OPENGL_TERMINATE_SDL_AT_EXIT  0x01

namespace wheel
{
   namespace video
   {
      class SDLRenderer : public wheel::video::Renderer
      {
         private:
            void*    window;
            void*    renderer;

            uint32_t int_flags;

         public:
            // Module functions
            void get_module_info(core::modinfo_t* info);

            // Renderer functions

            uint32_t OpenWindow(const core::string& title, uint32_t width, uint32_t height);
            void     SwapBuffers();

            bool     WindowIsOpen();

            SDLRenderer();
            ~SDLRenderer();
      };     
   }
}

#endif