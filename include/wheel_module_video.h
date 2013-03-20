/*!
   @file
   \brief General video/input interface
   \author Jari Ronkainen
*/

#ifndef WHEEL_MODULE_VIDEO_INTERFACE_H
#define WHEEL_MODULE_VIDEO_INTERFACE_H

#include "wheel_core_module.h"

namespace wheel
{
   namespace core
   {
      class Event
      {
         private:
         public:
      };

      typedef void(* wheel_event_callback_t)(Event&);

      enum wheel_input_t
      {
         WHEEL_KEYBOARD_EVENT,
         WHEEL_MOUSE_EVENT,
         WHEEL_JOYSTICK_EVENT,
         WHEEL_WINDOW_EVENT
      };
   }
   namespace video
   {
      //! Interface for renderer modules
      class Renderer : public core::Module
      {
         public:
            virtual  ~Renderer() {};

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
               they MUST be implemented in a way that they can be OR'd together to set multiple
               values at the same call.  The implementation does not need to support all hints, but it
               SHOULD leave a log message for each unsupported request.

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
            virtual  uint32_t OpenWindow(const core::string& title, uint32_t width, uint32_t height) = 0;

            //! Swap the front and back buffers of the window.
            virtual  void     SwapBuffers() = 0;

            //! Query if the window is still open
            /*!
               \return <code>true</code> if the window is open, otherwise <code>false</code>.
            */
            virtual  bool     WindowIsOpen() = 0;

            // Input-related

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
            virtual  uint32_t SetCallback(core::wheel_input_t type, core::wheel_event_callback_t event) { return WHEEL_UNIMPLEMENTED_FEATURE; }

            //! Process events
            /*!
               Processes input events.  A renderer SHOULD implement this feature, but it is not strictly required.
               
               \return <code>WHEEL_OK</code> on success \n
                       <code>WHEEL_UNIMPLEMENTED_FEATURE</code> if the implementation cannot be found from the module.

            */
            virtual  uint32_t Update() { return WHEEL_UNIMPLEMENTED_FEATURE; }
      };
   }
}

#endif