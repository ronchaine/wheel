/*!
   @file
   \brief General video/input interface
   \author Jari Ronkainen
*/

#ifndef WHEEL_MODULE_VIDEO_INTERFACE_H
#define WHEEL_MODULE_VIDEO_INTERFACE_H

// These values correspond to OpenGL numbers

#define WHEEL_PIXEL_FMT_NONE        ~0
#define WHEEL_PIXEL_FMT_BGR         0x80e0
#define WHEEL_PIXEL_FMT_BGRA        0x80e1
#define WHEEL_PIXEL_FMT_RGBA        0x1908
#define WHEEL_PIXEL_FMT_RGB         0x1907
#define WHEEL_PIXEL_FMT_MONO        0x1903
#define WHEEL_PIXEL_FMT_RG          0x8227

#define WHEEL_BYTE                  0x1400
#define WHEEL_UNSIGNED_BYTE         0x1401
#define WHEEL_SHORT                 0x1402
#define WHEEL_UNSIGNED_SHORT        0x1403
#define WHEEL_INT                   0x1404
#define WHEEL_UNSIGNED_INT          0x1405
#define WHEEL_FLOAT                 0x1406
#define WHEEL_DOUBLE                0x140A

#include "wheel_core_module.h"

#include <cstring>
#include <unordered_map>

namespace wheel
{
   enum shadertype_t
   {
      VERTEX,
      FRAGMENT
   };

   struct vertex_t
   {
      float    x0, y0, z0;    // position
      uint16_t s0, t0;        // diffuse     (normalised, from 0 to 0xffff)
      uint16_t s1, t1;        // normal      (normalised, from 0 to 0xffff)
      uint16_t s2, t2;        // palette     (normalised, from 0 to 0xffff)
   /*
      uint16_t s3, t3;        // padding 1
      uint16_t s4, t4;        // more padding
   */
      vertex_t() {}
      vertex_t(float x, float y, float z) : x0(x), y0(y), z0(z) {}
   };

   namespace interface
   {
      //! Interface for renderer modules
      class Video : public Module
      {
         public:
            virtual  ~Video() {};

            //! \brief Set hints for the following OpenWindow call.
            /*!
               Set hints for the following OpenWindow call, the values of hints SHOULD be retained until this function
               is called again or the renderer module is unloaded, and they MUST remain the same at least until the next
               OpenWindow call.  A renderer SHOULD implement this feature, but it is not strictly required.

               \param target  The window hint(s) to be set
               \param hint    The new value for the hint(s)
               \return <code>WHEEL_OK</code> on success \n
                       <code>WHEEL_UNIMPLEMENTED_FEATURE</code> if the implementation cannot be found from the module.

               Possible values for <code>hints</code> can be found in the following table,
               The implementation does not need to support all hints, but it SHOULD leave
               a log message for each unsupported request.

               These values MUST be given default values when the implementation class is created, the
               values themselves may vary by the implementation.

               <table>
               <tr>
               <td><b>hint</b></td>
               <td><b>possible values</b></td>
               </tr>
               <tr>
               <td><code>WHEEL_WINDOW_FULLSCREEN</code></td>
               <td><code>WHEEL_TRUE</code> or <code>WHEEL_FALSE</code></td>
               </tr>
               <tr>
               <td><code>WHEEL_WINDOW_VISIBLE</code></td>
               <td><code>WHEEL_TRUE</code> or <code>WHEEL_FALSE</code></td>
               </tr>
               <tr>
               <td><code>WHEEL_WINDOW_RESIZABLE</code></td>
               <td><code>WHEEL_TRUE</code> or <code>WHEEL_FALSE</code></td>
               </tr>
               <tr>
               <td><code>WHEEL_WINDOW_DECORATED</code></td>
               <td><code>WHEEL_TRUE</code> or <code>WHEEL_FALSE</code></td>
               </tr>
               <tr><td><code>WHEEL_WINDOW_RED_BITS</code></td>
               <td><code>0</code> to <code>INT_MAX</code></td></tr>
               <tr><td><code>WHEEL_WINDOW_GREEN_BITS</code></td>
               <td><code>0</code> to <code>INT_MAX</code></td></tr>
               <tr><td><code>WHEEL_WINDOW_BLUE_BITS</code></td>
               <td><code>0</code> to <code>INT_MAX</code></td></tr>
               <tr><td><code>WHEEL_WINDOW_ALPHA_BITS</code></td>
               <td><code>0</code> to <code>INT_MAX</code></td></tr>
               <tr><td><code>WHEEL_WINDOW_DEPTH_BITS</code></td>
               <td><code>0</code> to <code>INT_MAX</code></td></tr>
               <tr><td><code>WHEEL_WINDOW_STENCIL_BITS</code></td>
               <td><code>0</code> to <code>INT_MAX</code></td></tr>

               <tr><td><code>WHEEL_WINDOW_ACCUM_RED_BITS</code></td>
               <td><code>0</code> to <code>INT_MAX</code></td></tr>
               <tr><td><code>WHEEL_WINDOW_ACCUM_GREEN_BITS</code></td>
               <td><code>0</code> to <code>INT_MAX</code></td></tr>
               <tr><td><code>WHEEL_WINDOW_ACCUM_BLUE_BITS</code></td>
               <td><code>0</code> to <code>INT_MAX</code></td></tr>
               <tr><td><code>WHEEL_WINDOW_ACCUM_ALPHA_BITS</code></td>
               <td><code>0</code> to <code>INT_MAX</code></td></tr>

               <tr><td><code>WHEEL_WINDOW_AUX_BUFFERS</code></td>
               <td><code>0</code> to <code>INT_MAX</code></td></tr>
               <tr><td><code>WHEEL_WINDOW_SAMPLES</code></td>
               <td><code>0</code> to <code>INT_MAX</code></td></tr>

               </table>
            */
            virtual  uint32_t SetWindowHints(uint32_t target, uint32_t hint) { return WHEEL_UNIMPLEMENTED_FEATURE; }

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

            //! Get native window pointer.
            /*!
               \return void pointer to the native window handle or nullptr.
            */
            virtual  void*    GetWindowPtr() { return nullptr; }

            // Shader-related

            //! Add new shader
            /*!
               Compiles and links a shader program.  This function SHOULD be implemented if any
               shaders are to be used, but it is not required.  If the function is implemented,
               the class needs to be able to retrieve the shader program by the name given.
               Names that start with <code>WHEEL_RSVD_</code> are reserved.

               \param name    name that the shader program is identified by.
               \param vert    file to read vertex shader from.
               \param frag    file to read fragment shader from.

               \return <code>WHEEL_OK</code> on success, or a value depicting an error.
            */
            virtual  uint32_t AddShader(const string& name, const string& vert, const string& frag) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            //! Remove a shader
            /*!
               Removes previously added shader.  This function SHOULD be implemented if any
               shaders are to be used, but it is not required.

               \param name    name that the shader program is identified by.

               \return <code>WHEEL_OK</code> on success, or a value depicting an error.
            */
            virtual  uint32_t RemoveShader(const string& name) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            //! Return shader ID
            virtual  int32_t getProgram() { return WHEEL_UNIMPLEMENTED_FEATURE; }


            //! Use different shader
            /*!
               Choose shader program to use.  This function MUST be implemented if any shaders
               are to be used.  Names that start with <code>WHEEL_RSVD_</code> are reserved.

               <table>
               <tr>
               <td><b>Name value</b></td>
               <td><b>Meaning</b></td>
               </tr>
               <tr>
               <td>WHEEL_RSVD_DEFAULT</td>
               <td>Should be used for the shader used by default by the module, if there is one.</td>
               </tr>
               </table>

               \param name    name of the shader program to use. 

               \return <code>WHEEL_OK</code> on success, or a value depicting an error.
            */
            virtual  uint32_t UseShader(const string& name) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            //! Update the framebuffer
            /*!
            */
            virtual  void Update() = 0;

            // Rendering

            //! Rendering stuff
            /*!
            */
            virtual  void     AddVertex(vertex_t v, buffer_t* buf = nullptr);
            virtual  void     Flush() {}

            virtual  uint32_t DrawRect(int32_t x, int32_t y, uint32_t w, uint32_t h, float rot = 0.0f, int32_t x_pivot = 0, int32_t y_pivot = 0) = 0;

            virtual  uint32_t AddTexture(const string& name, uint32_t w, uint32_t h, uint32_t fmt) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            virtual  uint32_t Rotate(float x, float y, float z, float w) { return WHEEL_UNIMPLEMENTED_FEATURE; }            

            virtual  uint32_t Translate(float x, float y, float z) { return WHEEL_UNIMPLEMENTED_FEATURE; }
            virtual  uint32_t Translatei(int32_t x, int32_t y, int32_t z) { return WHEEL_UNIMPLEMENTED_FEATURE; }
      };

   }
}

#endif