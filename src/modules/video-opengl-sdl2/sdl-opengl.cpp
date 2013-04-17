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

         uint16_t r = 0xbe1e;
         if (*(uint8_t*)&r == 0x1e)
            int_flags |= WHEEL_LITTLE_ENDIAN;
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

         renderer = (void*)SDL_CreateRenderer((SDL_Window*)window, -1, SDL_RENDERER_ACCELERATED );

         if (renderer == nullptr)
            return !WHEEL_OK;

         std::cout << "OpenGL version string: " << glGetString(GL_VERSION) << "\n";

         window_alive = true;

         return 0;

      }

      void SDLRenderer::Clear(float r, float g, float b, float a)
      {
         if (shadow.clearcolour != glm::vec4(r, g, b ,a))
            glClearColor(r, g, b, a);

         glClear(GL_COLOR_BUFFER_BIT);
      }

      bool SDLRenderer::WindowIsOpen()
      {
         return window_alive;
      }

      void SDLRenderer::SwapBuffers()
      {
         SDL_RenderPresent((SDL_Renderer*)renderer);
      }

      uint32_t SDLRenderer::SetWindowHints(uint32_t target, uint32_t hint)
      {
         return 0;
      }

      void SDLRenderer::get_module_info(modinfo_t* info)
      {
         info->type = "VIDEO";
         info->name = "SDL2_VIDEO";
         info->version = "0.1-git";
         info->description = "Wheel reference renderer based on SDL2.0 and OpenGL";

         info->wheel_required_version = "any";
      }

      uint32_t SDLRenderer::GetEvents(EventList* events)
      {
         SDL_Event sdlevent;
         while(SDL_PollEvent(&sdlevent))
         {
            Event newevent;

            if (sdlevent.type == SDL_WINDOWEVENT)
            {
               switch (sdlevent.window.event)
               {
                  case SDL_WINDOWEVENT_CLOSE:
                     window_alive = false;
                     continue;
               }
            } else if (sdlevent.type == SDL_KEYUP) {
               newevent.data.push_back(WHEEL_KEYEVENT);
               newevent.data.push_back(WHEEL_RELEASE);

               // SDL keys need no translation, since the scancodes are
               // from usb_hid spec, other input systems, such as GLFW
               // require additional scancode conversion.

               uint8_t* p = (uint8_t*)&sdlevent.key.keysym.scancode;

               if (int_flags & WHEEL_LITTLE_ENDIAN)
               {
                  newevent.data.push_back(*(p+1));
                  newevent.data.push_back(*(p+0));
               } else {
                  newevent.data.push_back(*(p+0));
                  newevent.data.push_back(*(p+1));
               }

            } else if (sdlevent.type == SDL_KEYDOWN) {
               newevent.data.push_back(WHEEL_KEYEVENT);
               newevent.data.push_back(WHEEL_PRESS);

               uint8_t* p = (uint8_t*)&sdlevent.key.keysym.scancode;

               if (int_flags & WHEEL_LITTLE_ENDIAN)
               {
                  newevent.data.push_back(*(p+1));
                  newevent.data.push_back(*(p+0));
               } else {
                  newevent.data.push_back(*(p+0));
                  newevent.data.push_back(*(p+1));
               }
            }

            if (events != nullptr)
               events->push_back(newevent);
         }
         return WHEEL_OK;
      }
   }
}