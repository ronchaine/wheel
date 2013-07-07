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

char* filetobuf(const char *file)
{
   FILE *fptr;
   long length;
   char *buf;
 
   fptr = fopen(file, "rb"); /* Open file for reading */
   if (!fptr) /* Return NULL on failure */
      return NULL;
   fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
   length = ftell(fptr); /* Find out how many bytes into the file we are */
   buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
   fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
   fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
   fclose(fptr); /* Close the file */
   buf[length] = 0; /* Null terminator */
 
   return buf; /* Return the buffer */
}

namespace wheel
{
   namespace video
   {
      SDLRenderable::SDLRenderable()
      {
         glGenBuffers(1, &vertexbuffer);
      }

      SDLRenderable::~SDLRenderable()
      {
         glDeleteBuffers(1, &vertexbuffer);
      }


      SDLRenderer::SDLRenderer()
      {
         int_flags = 0;
         // This might be a stupid way to check if SDL has already been initialised...
         if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
         {
            int_flags |= WHEEL_SDL_OPENGL_TERMINATE_SDL_AT_EXIT;
            SDL_Init(SDL_INIT_VIDEO);

            SDL_version ver;
            SDL_GetVersion(&ver);
            log << "Using SDL version " << (uint32_t)ver.major << "." << (uint32_t)ver.minor << "." << (uint32_t)ver.patch << "\n";
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

         context = SDL_GL_CreateContext((SDL_Window*)window);

         if (context == nullptr)
            return !WHEEL_OK;

         std::cout << "OpenGL version string: " << glGetString(GL_VERSION) << "\n";

         glViewport(0.0, 0.0, w, h);

         /* Create VAO, bind it, and forget it */
         GLuint VertexArrayID;
         glGenVertexArrays(1, &VertexArrayID);
         glBindVertexArray(VertexArrayID);

         int32_t GLMaj, GLMin;
         glGetIntegerv(GL_MAJOR_VERSION, &GLMaj);
         glGetIntegerv(GL_MINOR_VERSION, &GLMin);

         log << "OpenGL version: " << GLMaj << "." << GLMin << "\n";

         window_alive = true;

         log << "SDL-OpenGL: Opened window\n";

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
         SDL_GL_SwapWindow((SDL_Window*)window);
      }

      void SDLRenderer::Draw(const interface::Renderable& object)
      {
/*
         // An array of 3 vectors which represents 3 vertices
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

      uint32_t p = 0;
      uint32_t SDLRenderer::AddShader(const string& name, const string& vert, const string& frag)
      {
         GLuint vertexshader, fragmentshader, program;

         int vertexcompiled, fragmentcompiled;
         int linked;

         const wheel::buffer_t* raw_vs = wheel::GetBuffer(vert);
         const wheel::buffer_t* raw_fs = wheel::GetBuffer(frag);

         wheel::buffer_t vs_buf = *raw_vs;
         vs_buf.push_back('\0');

         wheel::buffer_t fs_buf = *raw_fs;
         fs_buf.push_back('\0');

         const char* vert_ptr = (const char*)&vs_buf[0];
         const char* frag_ptr = (const char*)&fs_buf[0];

         vertexshader = glCreateShader(GL_VERTEX_SHADER);
         glShaderSource(vertexshader, 1, (const GLchar**) &vert_ptr, 0);

         glCompileShader(vertexshader);
         glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &vertexcompiled);

         if(!vertexcompiled)
            return WHEEL_MODULE_SHADER_COMPILE_ERROR;

         fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
         glShaderSource(fragmentshader, 1, (const GLchar**) &frag_ptr, 0);

         glCompileShader(fragmentshader);
         glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &fragmentcompiled);

         if(!fragmentcompiled)
            return WHEEL_MODULE_SHADER_COMPILE_ERROR;

         program = glCreateProgram();

         glAttachShader(program, vertexshader);
         glAttachShader(program, fragmentshader);

         glLinkProgram(program);

         glGetProgramiv(program, GL_LINK_STATUS, (int*)&linked);
         if (!linked)
            return WHEEL_MODULE_SHADER_COMPILE_ERROR;

         shaderlist[name] = program;

         return 0;
/*
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

         std::cout << "compiled program:" << shader.program << "\n";

         if (glIsProgram(shader.program) != GL_TRUE)
            std::cout << "but it isn't really a program!\n";

         shaderlist[name] = shader;

         return WHEEL_OK;
*/
      }

      uint32_t SDLRenderer::UseShader(const string& name)
      {
         if (shaderlist.count(name) == 1)
         {
            glUseProgram(shaderlist[name]);
            return WHEEL_OK;
         }

         log << "tried to use an invalid shader \"" << name << "\"\n";
         return WHEEL_RESOURCE_UNAVAILABLE;
      }
   }
}