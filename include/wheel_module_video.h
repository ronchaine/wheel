/*!
   @file
   \brief General video/input interface
   \author Jari Ronkainen
*/

#ifndef WHEEL_MODULE_VIDEO_INTERFACE_H
#define WHEEL_MODULE_VIDEO_INTERFACE_H

#include "wheel_core_module.h"
//#include "wheel_math_geometry.hpp"

namespace wheel
{
   enum shadertype_t
   {
      VERTEX,
      FRAGMENT
   };

   enum vertexformat_t
   {
      WHEEL_VERTEXFORMAT_CUSTOM  =  0b001,
      WHEEL_VERTEXFORMAT_VT      =  0b010,
      WHEEL_VERTEXFORMAT_VNT     =  0b100
   };

   struct vertex_vt_t
   {
   };

   struct vertex_vnt_t
   {
   };

   namespace interface
   {
      // Forward declaration
      class Renderable;

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

            // Drawing 
            /*!
               \param Renderable Object of type renderable to be drawn.
            */
            virtual  void     Draw(const Renderable& object) = 0;

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

            // Renderer objects

            //! Create new renderable object of proper type
            /*!
               This should probably just be <code>return new renderable_class</code> for most objects.
            */
            virtual  Renderable* CreateObject() = 0;

            // Event-related

            //! Set event callbacks
            /*!
               Sets callback functions for events.  The function MUST be implemented so that it returns
               <code>WHEEL_UNIMPLEMENTED_FEATURE</code> in case of unsupported event type.

               \param type Event type of the event
               \param func Pointer to the callback function for the chosen type of an event.

               \return <code>WHEEL_OK</code> on success \n
                       <code>WHEEL_UNIMPLEMENTED_FEATURE</code> if the implementation cannot be found from the module or the
                                                                event type is unsupported.
            */
//            virtual  uint32_t SetCallback(wheel_input_t type, wheel_event_callback_t event) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            //! Get list of events
            /*!
               Retrieves events.  A renderer SHOULD implement this feature, but it is not strictly required.
               
               \return <code>WHEEL_OK</code> on success\n
                       <code>WHEEL_UNIMPLEMENTED_FEATURE</code> if the implementation cannot be found from the module.
            */
      };

      //! Base class for everything that can be rendered, derive from this.
      class Renderable
      {
         protected:
            string            custom_vertex_format;
            flags_t           vertex_format;

         public:
            Renderable() {}
            virtual ~Renderable() {}

            virtual uint32_t SetCustomVertexFormatString(string newformat) = 0;
            inline string GetCustomVertexFormatString() { return custom_vertex_format; }

            inline void SetVertexFormat(vertexformat_t format) { vertex_format = format; }
            inline flags_t GetVertexFormat() { return vertex_format; }

            inline void Draw(Video* renderer) { return renderer->Draw(*this); }
      };
   }
}

#endif