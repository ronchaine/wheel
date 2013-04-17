/*!
   @file
   \brief Contains definitions for everything needed to load external modules
*/

#ifndef WHEEL_MODULE_VIDEO_SDL_OPENGL_HEADER
#define WHEEL_MODULE_VIDEO_SDL_OPENGL_HEADER

#include "../../../include/wheel_module_video.h"
//#include "../../../include/wheel_module_event.h"

#include <glm/glm.hpp>

//#include "../../../include/wheel_module_opengl_utility.h"

struct shadowgl_t
{
   glm::vec4 clearcolour;
};

#define SDL_NO_COMPAT
#include <SDL2/SDL.h>

#define WHEEL_SDL_OPENGL_TERMINATE_SDL_AT_EXIT  0x01

namespace wheel
{
   namespace video
   {
      class SDLRenderer : public wheel::interface::Video
      {
         private:
            void*       window;
            void*       renderer;

            uint32_t    int_flags;

            bool        window_alive;

            shadowgl_t  shadow;

         public:
            // Module functions
            void get_module_info(modinfo_t* info);

            uint32_t    GetEvents(EventList* events);

            // Renderer functions

            uint32_t    OpenWindow(const string& title, uint32_t width, uint32_t height);
            void        SwapBuffers();

            bool        WindowIsOpen();

            void        Clear(float r, float g, float b, float a);

            uint32_t    SetWindowHints(uint32_t target, uint32_t hint);

            SDLRenderer();
            ~SDLRenderer();
      };     
   }
}

#endif