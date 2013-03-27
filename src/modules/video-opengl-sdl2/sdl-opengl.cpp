/*!
   @file
   \brief Sample SDL2.0-based renderer for wheel
*/

#include "sdl-opengl.h"
#include <iostream>

#define GL3_PROTOTYPES 1
#include <GL/gl3.h>

// These are required for every module
extern "C" {
   wheel::Module* register_module()
   {
      return new wheel::video::SDLRenderer;
   }

   void remove_module(wheel::Module* mod)
   {
      delete mod;
   }
}

namespace wheel
{
   namespace video
   {

      SDLRenderer::SDLRenderer()
      {
         int_flags = 0;
         // This might be a stupid way to check if SDL has already been initialised...
         if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
         {
            int_flags |= WHEEL_SDL_OPENGL_TERMINATE_SDL_AT_EXIT;
            SDL_Init(SDL_INIT_VIDEO);
         }
      }

      SDLRenderer::~SDLRenderer()
      {
         if (int_flags & WHEEL_SDL_OPENGL_TERMINATE_SDL_AT_EXIT)
            SDL_Quit();
      }

      uint32_t SDLRenderer::OpenWindow(const string& title, uint32_t w, uint32_t h)
      {
         window = (void*)SDL_CreateWindow(title.std_str().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);

         if (window == nullptr)
            return !WHEEL_OK;

         std::cout << "Got window pointer\n";

         renderer = (void*)SDL_CreateRenderer((SDL_Window*)window, -1, SDL_RENDERER_ACCELERATED );

         if (renderer == nullptr)
            return !WHEEL_OK;

         std::cout << "OpenGL version string: " << glGetString(GL_VERSION) << "\n";

         return 0;

      }

      bool SDLRenderer::WindowIsOpen()
      {
         return true;
      }

      void SDLRenderer::SwapBuffers()
      {
         SDL_RenderPresent((SDL_Renderer*)renderer);
      }

      void SDLRenderer::get_module_info(modinfo_t* info)
      {
         info->type = "VIDEO";
         info->name = "SDL2_VIDEO";
         info->version = "0.1-git";
         info->description = "Wheel reference renderer based on SDL2.0 and OpenGL";

         info->wheel_required_version = "any";
      }
   }
}