/*!
   @file
   \brief General video/input interface
   \author Jari Ronkainen
*/

#ifndef WHEEL_MODULE_VIDEO_INTERFACE_H
#define WHEEL_MODULE_VIDEO_INTERFACE_H

#include "wheel_core_module.h"
#include "wheel_video.h"

#include <cstring>
#include <unordered_map>

namespace wheel
{
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

            //! Create shader
            /*!
               Creates a shader from files given.  Takes shader name, fragment shader filename
               and vertex shader filename as parameters.  This function is optional to
               implement.

               \param name    name to give the new shader
               \param vert    vertex shader filename
               \param frag    fragment shader filename

               \return pointer to a new shader object or nullptr.
            */
            virtual  Shader*  CreateShader(const string& name, const string& vert, const string& frag) { return nullptr; }

            //! Create shader
            /*!
               Creates a shader from files given.  Takes a shader definition file as an input.
               This function is optional to implement.

               \param info    file to read shader info from

               \return pointer to a new shader object or nullptr.
            */
            virtual  Shader*  CreateShader(const string& info) { return nullptr; }

            //! Add a shader to collection
            /*!
               Add a shader to material collection
            */
            virtual  uint32_t AddShader(Shader* shader) { return WHEEL_UNIMPLEMENTED_FEATURE; }

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

               \deprecated
            */
            virtual  uint32_t AddShader(const string& name, const string& vert, const string& frag) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            //! Remove a shader
            /*!
               Removes previously added shader.  This function SHOULD be implemented if any
               shaders are to be used, but it is not required.

               \param name    name that the shader program is identified by.

               \return <code>WHEEL_OK</code> on success, or a value depicting an error.

               \deprecated
            */
            virtual  uint32_t RemoveShader(const string& name) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            //! Return shader ID
            /*!
               \deprecated
            */
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
               \warning       DEPRECATED
            */
            virtual  uint32_t UseShader(const string& name) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            //! Update the framebuffer
            /*!
            */
            virtual  void Update() = 0;

            // Retrieve stuff
            virtual  uint32_t getScrWidth() = 0;
            virtual  uint32_t getScrHeight() = 0;

            // Rendering

            //! Rendering stuff
            /*!
            */
            virtual  void     AddVertex(vertex_t v, buffer_t* buf = nullptr);
            virtual  void     Flush(int32_t array_type = 4) {}

            virtual  uint32_t DrawRect(int32_t x, int32_t y, uint32_t w, uint32_t h, float rot = 0.0f, int32_t x_pivot = 0, int32_t y_pivot = 0) = 0;

            virtual  uint32_t Rotate(float x, float y, float z, float w) { return WHEEL_UNIMPLEMENTED_FEATURE; }            

            virtual  uint32_t Translate(float x, float y, float z) { return WHEEL_UNIMPLEMENTED_FEATURE; }
            virtual  uint32_t Translatei(int32_t x, int32_t y, int32_t z) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            // Atlasing
            virtual  uint32_t CreateAtlas(const wcl::string& atlas, const wcl::string& texture) { return WHEEL_UNIMPLEMENTED_FEATURE; }
            virtual  uint32_t AtlasBuffer(const wcl::string& atlasname, const wcl::string& name, uint32_t w, uint32_t h, void* data) { return WHEEL_UNIMPLEMENTED_FEATURE; }
            virtual  uint32_t GetAtlasPos(const wcl::string& atlasname, const wcl::string& buffername, rect_t* result);

            // Texturing
            virtual  void     CreateTexture(const wcl::string& name, uint32_t w, uint32_t h, uint32_t components, uint32_t format);
            virtual  void     DeleteTexture(const wcl::string& name);
            virtual  uint32_t UpdateTextureData(const wcl::string& name, int32_t x_off, int32_t y_off, size_t w, size_t h, void* pixel_data);

            virtual  void     SetTexture(uint32_t tu, const wcl::string& name);
            virtual  void     SetTexture(const wcl::string& uniform, uint32_t tu, const wcl::string& name);

            virtual  uint32_t AssignTU(const wcl::string& uniform, uint32_t tu) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            // Surfaces
            virtual  uint32_t CreateSurface(const wcl::string& name, uint32_t type = 0) { return WHEEL_UNIMPLEMENTED_FEATURE; }
            virtual  uint32_t AttachToSurface(const wcl::string& surface, const wcl::string& texture) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            virtual  void     SetSurface(const wcl::string& name);
      };

   }
}

#endif
