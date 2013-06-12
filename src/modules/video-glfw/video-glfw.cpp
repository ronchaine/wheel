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
         if (shadow.clearcolour != glm::vec4(r, g, b ,a))
            glClearColor(r, g, b, a);

         glClear(GL_COLOR_BUFFER_BIT);
      }

      void GLFWRenderer::Draw(uint32_t count, wheel::shapes::triangle_t* triangle_ptr)
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
      }

      uint32_t GLFWRenderer::AddShader(const string& name, const string& vert, const string& frag)
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

      uint32_t GLFWRenderer::UseShader(const string& name)
      {
         if (shaderlist.count(name) == 1)
         {
            glUseProgram(shaderlist[name].program);
            return WHEEL_OK;
         }

         log << "tried to use an invalid shader \"" << name << "\"\n";

         return WHEEL_RESOURCE_UNAVAILABLE;
      }

      void GLFWRenderer::get_module_info(modinfo_t* info)
      {
         info->type = "VIDEO";
         info->name = "GLFW_VIDEO";
         info->version = "0.1-git";
         info->description = "Wheel reference renderer based on GLFW 3.0";

         info->wheel_required_version = "any";
      }
   }
}