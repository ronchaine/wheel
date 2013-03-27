#include "video-glfw.h"
#include "GL/glfw3.h"

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
      }

      bool GLFWRenderer::WindowIsOpen()
      {
         return true;
      }

      void GLFWRenderer::SwapBuffers()
      {
         //glfwSwapbuffers();
      }

   }
}