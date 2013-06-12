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
      glm::vec4 v4(0.0f, 1.0f, 2.0f, 3.0f);

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
         SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
         SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
         SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
         SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
         SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

         window = (void*)SDL_CreateWindow(title.std_str().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);

         if (window == nullptr)
            return !WHEEL_OK;

         renderer = (void*)SDL_CreateRenderer((SDL_Window*)window, -1, SDL_RENDERER_ACCELERATED );

         if (renderer == nullptr)
            return !WHEEL_OK;

         std::cout << "OpenGL version string: " << glGetString(GL_VERSION) << "\n";

         glViewport(0.0, 0.0, w, h);

         /* Create VAO, bind it, and forget it */
         GLuint VertexArrayID;
         glGenVertexArrays(1, &VertexArrayID);
         glBindVertexArray(VertexArrayID);

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

      void SDLRenderer::Draw(uint32_t count, wheel::shapes::triangle_t* triangle_ptr)
      {
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         // An array of 3 vectors which represents 3 vertices
         static const GLfloat g_vertex_buffer_data[] = {
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
         };
         static GLuint vertexbuffer;
         static bool genbuffer = false;

         if (genbuffer == false)
         {
            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
            genbuffer = true;
         }
         glEnableVertexAttribArray(0);
         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

         glDrawArrays(GL_TRIANGLES, 0, 3);

         glDisableVertexAttribArray(0);
/*

         static GLuint vertexbuffer;
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, triangle_ptr);

         glDrawArrays(GL_TRIANGLES, 0, 3 * count);
*/

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
         const buffer_t* dummy = GetBuffer(vert);
         if (dummy == nullptr)
         {
            log << "ERROR: couldn't find vertex shader input file.\n";
            return WHEEL_RESOURCE_UNAVAILABLE;
         }

         buffer_t vert_shader = *dummy;

         dummy = GetBuffer(frag);
         if (dummy == nullptr)
         {
            log << "ERROR: couldn't find fragment shader input file.\n";
            return WHEEL_RESOURCE_UNAVAILABLE;
         }

         buffer_t frag_shader = *dummy;

         vert_shader.push_back('\0');
         frag_shader.push_back('\0');

         shader_info_t shader;

         shader.vertex = glCreateShader(GL_VERTEX_SHADER);

         char const* vert_ptr = (char const*)&vert_shader[0];

         glShaderSource(shader.vertex, 1, &vert_ptr, 0);
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
            log << "vert:" << infomsg << "\n";
            free(infomsg);

            glDeleteShader(shader.vertex);

            return WHEEL_MODULE_SHADER_COMPILE_ERROR;
         }

         char const* frag_ptr = (char const*)&frag_shader[0];

         shader.fragment = glCreateShader(GL_FRAGMENT_SHADER);
         glShaderSource(shader.fragment, 1, &frag_ptr, 0);
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
            log << "frag:" << infomsg << "\n";
            free(infomsg);

            glDeleteShader(shader.fragment);
            glDeleteShader(shader.vertex);

            return WHEEL_MODULE_SHADER_COMPILE_ERROR;
         }

         shader.program = glCreateProgram();

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