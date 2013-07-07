#ifndef WHEEL_MODULE_VIDEO_GLFW
#define WHEEL_MODULE_VIDEO_GLFW

#include "../../../include/wheel_core_debug.h"
#include "../../../include/wheel_core_resource.h"
#include "../../../include/wheel_module_video.h"

#include <glm/glm.hpp>

#define GL3_PROTOTYPES 1
#include <GL/gl3.h>

#include "GL/glfw3.h"

struct shadowgl_t
{
   glm::vec4 clearcolour;

   bool        drawn;

   shadowgl_t() {
      clearcolour[0] = 0.0f;
      clearcolour[1] = 0.0f;
      clearcolour[2] = 0.0f;
      clearcolour[3] = 0.0f;
      drawn = false;
   }
};

namespace wheel
{
   namespace video
   {
      struct view_t
      {
         GLFWwindow* window;

         uint32_t width, height;
      };

      struct shader_info_t
      {
         GLuint   vertex;
         GLuint   fragment;
         GLuint   program;
      };

      class GLFWRenderer : public wheel::interface::Video
      {
         private:
            view_t      screen;
            shadowgl_t  shadow;

            std::unordered_map<string, shader_info_t> shaderlist;

         public:
            // Module functions
            void get_module_info(modinfo_t* info);

            // Renderer functions
            uint32_t OpenWindow(const string& title, uint32_t w, uint32_t h);

            void SwapBuffers();

            bool WindowIsOpen();

            void Clear(float r, float g, float b, float a);

            void Draw(const interface::Renderable& object);

            // Shader stuff

            uint32_t    AddShader(const string& name, const string& vert, const string& frag);
            uint32_t    UseShader(const string& name);

            GLFWRenderer();
           ~GLFWRenderer();
      };
   }
}

#endif