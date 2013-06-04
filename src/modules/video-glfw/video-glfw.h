#ifndef WHEEL_MODULE_VIDEO_GLFW
#define WHEEL_MODULE_VIDEO_GLFW

#include "../../../include/wheel_core_module.h"
#include "../../../include/wheel_module_video.h"

#include "GL/glfw3.h"

namespace wheel
{
   namespace video
   {
      struct view_t
      {
         GLFWwindow* window;

         uint32_t width, height;
      };

      class GLFWRenderer : public wheel::interface::Video
      {
         private:
            view_t   screen;

         public:
            // Module functions
            void get_module_info(modinfo_t* info);

            // Renderer functions
            uint32_t OpenWindow(const string& title, uint32_t w, uint32_t h);

            void SwapBuffers();

            bool WindowIsOpen();

            void Clear(float r, float g, float b, float a);

            void Draw(uint32_t count, wheel::shapes::triangle_t* triangle_ptr);

            GLFWRenderer();
           ~GLFWRenderer();
      };
   }
}

#endif