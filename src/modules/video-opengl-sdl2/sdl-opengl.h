/*!
   @file
   \brief Contains definitions for everything needed to load external modules
*/

#ifndef WHEEL_MODULE_VIDEO_SDL_OPENGL_HEADER
#define WHEEL_MODULE_VIDEO_SDL_OPENGL_HEADER

#include "../../../include/wheel_core_debug.h"
#include "../../../include/wheel_core_resource.h"
#include "../../../include/wheel_module_video.h"
#include "../../../include/wheel_math_geometry.hpp"

#define GL3_PROTOTYPES 1
#include <GL/gl3.h>

typedef wmath::vector<float, 4> colour4;

struct shadowgl_t
{
   wmath::vec4f clearcolour;

   bool        drawn;

   shadowgl_t() {
      clearcolour[0] = 0.0f;
      clearcolour[1] = 0.0f;
      clearcolour[2] = 0.0f;
      clearcolour[3] = 0.0f;
      drawn = false;
   }
};

#define SDL_NO_COMPAT
#include <SDL2/SDL.h>

#define WHEEL_SDL_OPENGL_TERMINATE_SDL_AT_EXIT  0x01

namespace wheel
{
   namespace video
   {
      struct shader_info_t
      {
         GLuint   vertex;
         GLuint   fragment;
         GLuint   program;
      };

      class SDLRenderer : public wheel::interface::Video
      {
         private:
            void*       window;
            void*       renderer;

            uint32_t    int_flags;

            bool        window_alive;
            bool        shader_active;

            shadowgl_t  shadow;

            std::unordered_map<string, shader_info_t> shaderlist;

         public:
            // Module functions
            void        get_module_info(modinfo_t* info);

            uint32_t    GetEvents(EventList* events);

            // Renderer functions

            uint32_t    OpenWindow(const string& title, uint32_t width, uint32_t height);
            void        SwapBuffers();

            bool        WindowIsOpen();

            void        Clear(float r, float g, float b, float a);

            uint32_t    SetWindowHints(uint32_t target, uint32_t hint);

            void        Batch();
            void        Draw(uint32_t count, wheel::shapes::triangle_t* triangle_ptr);

            // Shader stuff

            uint32_t    AddShader(const string& name, const string& vert, const string& frag);
            uint32_t    UseShader(const string& name);

            SDLRenderer();
            ~SDLRenderer();
      };     
   }
}

#endif