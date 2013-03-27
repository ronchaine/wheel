#ifndef WHEEL_MODULE_VIDEO_GLFW
#define WHEEL_MODULE_VIDEO_GLFW

#include "../../../include/wheel_core_module.h"
#include "../../../include/wheel_module_video.h"

namespace wheel
{
   namespace video
   {
      class GLFWRenderer : public wheel::interface::Video
      {
         private:
         public:
            // Module functions
            void get_module_info(modinfo_t* info);

            // Renderer functions
            uint32_t OpenWindow(const string& title, uint32_t w, uint32_t h);

            void SwapBuffers();

            bool WindowIsOpen();

            GLFWRenderer();
           ~GLFWRenderer();
      };
   }
}

#endif