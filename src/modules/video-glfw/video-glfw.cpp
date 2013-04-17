#include "video-glfw.h"

// These are required for every module
extern "C" {
   wheel::Module* register_module()
   {
      return new wheel::video::GLFWRenderer;
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
      GLFWRenderer::GLFWRenderer()
      {
         glfwInit();
      }

      GLFWRenderer::~GLFWRenderer()
      {
         glfwTerminate();
      }

      void GLFWRenderer::get_module_info(modinfo_t* info)
      {
         info->type = "VIDEO";
         info->name = "GLFW_VIDEO";
         info->version = "0.1-git";
         info->description = "Wheel reference renderer based on GLFW 3.0";

         info->wheel_required_version = "any";
      }

      uint32_t GLFWRenderer::OpenWindow(const string& title, uint32_t w, uint32_t h)
      {
         screen.window = glfwCreateWindow(w, h, title.std_str().c_str(), nullptr, nullptr);

         if (!screen.window)
            return WHEEL_MODULE_FAILED_REQUEST;

         glfwMakeContextCurrent(screen.window);

         screen.width = w;
         screen.height = h;

         return WHEEL_OK;
      }

      bool GLFWRenderer::WindowIsOpen()
      {
         return !glfwWindowShouldClose(screen.window);
      }

      void GLFWRenderer::SwapBuffers()
      {
         glfwSwapBuffers(screen.window);
      }

      void GLFWRenderer::Clear(float r, float g, float b, float a)
      {
/*
         if (shadow.clearcolour != vec4<float>(r, g, b ,a))
            glClearColor(r, g, b, a);
         glClear(GL_COLOR_BUFFER_BIT);
*/
      }

   }
}