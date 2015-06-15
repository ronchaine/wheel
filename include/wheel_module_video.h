/*!
   @file
   \brief General video/input interface
   \author Jari Ronkainen
*/

#ifndef WHEEL_MODULE_VIDEO_INTERFACE_H
#define WHEEL_MODULE_VIDEO_INTERFACE_H

#include "wheel_core_debug.h"
#include "wheel_core_module.h"
#include "wheel_video.h"

#include <cstring>
#include <unordered_map>

namespace wheel
{
   namespace interface
   {
      //! Interface for shaders
      class Shader
      {
         private:
            wcl::string                shader_type;

         public:
            virtual uint32_t           Use() = 0;
            
            // Convert vertices to shader format
            void                       to_shader_format(const buffer_t& input, buffer_t& output);

            // Used to set uniforms
            virtual uint32_t           operator[](const wcl::string& idx) = 0;
   
            wcl::string                type() { return shader_type; }

            Shader(wcl::string type) : shader_type(type) {}
            virtual ~Shader()          {}
      };

      //! Interface for renderer modules
      class Video : public Module
      {
         public:
            virtual  ~Video() {};

            //! Creates a window and associated context.
            /*!
               Options specifying how the window and context should be created are controlled with \ref SetWindowHints function.

               The actual window properties may differ from what specified as the parameters or hints are NOT required
               to be hard constraints.

               \param title   The initial, unicode-encoded window title.
               \param width   Width requested for window
               \param height  Height requested for window

               \return <code>WHEEL_OK</code> on success, or a value depicting an error.

               \sa SetWindowHints
            */
            virtual  uint32_t OpenWindow(const string& title, uint32_t width, uint32_t height) = 0;

            //! Swap the front and back buffers of the window.
            virtual  void     SwapBuffers() = 0;

            //! Query if the window is still open
            /*!
               \return <code>true</code> if the window is open, otherwise <code>false</code>.
            */
            virtual  bool     WindowIsOpen() = 0;

            //! Clear the screen to a colour
            /*!
               \param r       Red value of clear colour.
               \param g       Green value of clear colour.
               \param b       Blue value of clear colour.
               \param a       Alpha value of clear colour.

               \warning       The function will fail silently if the renderer
                              does not support this functionality.
            */
            virtual  void     Clear(float r, float g, float b, float a) = 0;

            //! Add a shader to collection
            /*!
               Add a shader to material collection
            */
            virtual  uint32_t AddShader(Shader* shader) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            //! Add a single vertex to the buffer
            /*!
            */
            virtual  void     AddVertex(vertex_t v, buffer_t* buf = nullptr) = 0;

            //! Flush all the vertex buffers
            /*!
            */
            virtual  void     Flush();

            //! Load a texture
            /*!
            */
            virtual  uint32_t LoadTexture(const wcl::string& file, const wcl::string& name = "") { return WHEEL_UNIMPLEMENTED_FEATURE; }

            //! Load/modify a texture
            /*!
            */
            virtual  uint32_t Texture(const wcl::string& name, int32_t x_off, int32_t y_off, size_t w, size_t h, void* pixel_data, uint32_t flags = 0) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            //! Draw a sprite to the buffer
            /*!
            */
            virtual  uint32_t DrawSprite(const wcl::string& sprite, uint32_t x, uint32_t y, uint32_t w = ~0, uint32_t h = ~0, int32_t pivot_x = 0, int32_t pivot_y = 0, float angle = .0f) { return WHEEL_UNIMPLEMENTED_FEATURE; }
      };

   }
}

#endif
