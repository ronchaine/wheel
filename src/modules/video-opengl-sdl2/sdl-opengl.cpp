/*!
   @file
   \brief Sample SDL2.0-based renderer for wheel
*/

#include "sdl-opengl.h"
#include <iostream>

extern "C" {
   wheel::core::Module* register_module()
   {
      return new wheel::video::SDLRenderer;
   }

   void remove_module(wheel::core::Module* mod)
   {
      delete mod;
   }
}

namespace wheel
{
   namespace video
   {
/*
      SDLRenderer::SDLRenderer()
      {
      }
*/
      SDLRenderer::~SDLRenderer()
      {
      }

      void SDLRenderer::get_module_info(core::modinfo_t* info)
      {
         info->type = "VIDEO";
         info->name = "SDL2_VIDEO";
         info->version = "0.1-git";
         info->description = "Wheel reference renderer based on SDL2.0 and OpenGL";

         info->wheel_required_version = "any";
      }
   }
}