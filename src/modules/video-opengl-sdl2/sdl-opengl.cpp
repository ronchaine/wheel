/*!
   @file
   \brief Sample SDL2.0-based renderer for wheel
*/

#include "sdl-opengl.h"

#include <iostream>
#include <cstdio>

// These are required for every module
extern "C" {
   wheel::Module* register_module()
   {
      wmath::vec4f v4(0.0f, 1.0f, 2.0f, 3.0f);

      return new wheel::video::SDLRenderer;
   }

   void remove_module(wheel::Module* mod)
   {
      delete mod;
   }
}

static const std::string wsdlgl_vertexshader_default =
"\n\
   #version 140\n\
\n\
   in vec3 in_pos;\n\
   in vec4 in_col;\n\
\n\
   out vec3 frag_col;\n\
\n\
   void main(void)\n\
   {\n\
      gl_Position = vec4(in_pos, 1.0);\n\
      frag_col = in_col;\n\
   }\n\
";

static const std::string wsdlgl_fragshader_default =
"\n\
   #version 140\n\
   precision highp float;\n\
\n\
   in vec4 frag_col;\n\
   out vec4 out_col;\n\
\n\
   void main(void)\n\
   {\n\
      out_col = frag_col;\n\
   }\n\
";

namespace wheel
{
   namespace video
   {

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

         shader_active = false;

         window = nullptr;
         renderer = nullptr;

         window_alive = false;
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

         /* Create VAO, bind it, and forget it */
         GLuint VertexArrayID;
         glGenVertexArrays(1, &VertexArrayID);
         glBindVertexArray(VertexArrayID);

         window_alive = true;

         return 0;

      }

      void SDLRenderer::Clear(float r, float g, float b, float a)
      {

         if (shadow.clearcolour != wmath::vec4f(r, g, b ,a))
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

      void SDLRenderer::Draw(uint32_t count, wheel::shapes::triangle_t* triangle_ptr)
      {

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

      uint32_t SDLRenderer::AddShader(const string& name, const string& vert, const string& frag)
      {

         buffer_t vert_shader = *GetBuffer(vert);
         buffer_t frag_shader = *GetBuffer(frag);

         vert_shader.push_back('\0');
         frag_shader.push_back('\0');

         shader_info_t shader;

         shader.vertex = glCreateShader(GL_VERTEX_SHADER);
         glShaderSource(shader.vertex, 1, (const GLchar**)&vert_shader[0], 0);
         glCompileShader(shader.vertex);

         // No longer required, do not cache.
         DeleteBuffer(vert);

         int status = GL_TRUE;

         glGetShaderiv(shader.vertex, GL_COMPILE_STATUS, &status);
         if (status == GL_FALSE)
         {
            int loglen;
            char* infomsg;
            glGetShaderiv(shader.vertex, GL_INFO_LOG_LENGTH, &loglen);
            infomsg = (char*) malloc (loglen);
            glGetShaderInfoLog(shader.vertex, loglen, &loglen, infomsg);
            log << infomsg << "\n";
            free(infomsg);

            glDeleteShader(shader.vertex);

            return WHEEL_MODULE_SHADER_COMPILE_ERROR;
         }

         shader.fragment = glCreateShader(GL_FRAGMENT_SHADER);
         glShaderSource(shader.fragment, 1, (const GLchar**)&frag_shader[0], 0);
         glCompileShader(shader.fragment);

         // No longer required, do not cache.
         DeleteBuffer(frag);

         glGetShaderiv(shader.fragment, GL_COMPILE_STATUS, &status);
         if (status == GL_FALSE)
         {
            int loglen;
            char* infomsg;
            glGetShaderiv(shader.fragment, GL_INFO_LOG_LENGTH, &loglen);
            infomsg = (char*) malloc (loglen);
            glGetShaderInfoLog(shader.fragment, loglen, &loglen, infomsg);
            log << infomsg << "\n";
            free(infomsg);

            glDeleteShader(shader.fragment);
            glDeleteShader(shader.vertex);

            return WHEEL_MODULE_SHADER_COMPILE_ERROR;
         }

         shader.program = glCreateProgram();

         // TFIXME:  This the place for attriblocations and stuff.  The reference renderer
         //          doesn't yet give a method for handling this.
         /*
            This was old trick that I used, it's pretty unclean, so I didn't want to do
            that here.  Maybe use a initialiser list or custom file instead?

            if (prelink_ptr != nullptr)
               prelink_ptr(shader);
            else
            {
               glBindAttribLocation(shader.program, 0, "position");
               glBindAttribLocation(shader.program, 1, "colour");
               glBindAttribLocation(shader.program, 2, "texture");
               glBindAttribLocation(shader.program, 3, "indices");
            }
         */

         glLinkProgram(shader.program);
         glGetProgramiv(shader.program, GL_LINK_STATUS, &status);
         if (status == GL_FALSE)
         {
            log << "failed to link shader\n";
            glDeleteShader(shader.fragment);
            glDeleteShader(shader.vertex);
            glDeleteProgram(shader.program);

            return WHEEL_MODULE_SHADER_LINK_ERROR;
         }

         shaderlist[name] = shader;

         return WHEEL_OK;
      }

      uint32_t SDLRenderer::UseShader(const string& name)
      {
         if (shaderlist.count(name) == 1)
         {
            glUseProgram(shaderlist[name].program);
            return WHEEL_OK;
         }

         log << "tried to use an invalid shader \"" << name << "\"\n";

         return WHEEL_RESOURCE_UNAVAILABLE;
      }
   }
}